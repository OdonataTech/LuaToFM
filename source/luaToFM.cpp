/*
 Copyright 2023 OdonataTech LLS. All rights reserved.
 */

#include <luaToFM.h>
#include <lua_funcs.h>

// Protype functions of needed additional packages
// Use extern "C" if becouse library make in C usualy
extern "C" {
	// CJSON package
	int luaopen_cjson(lua_State *l);
	int luaopen_cjson_safe(lua_State *l);

}

// ********************************************************************************************
// LuaToFM classs methods
// ********************************************************************************************

// newLuaState
lua_State* LuaToFM::newLuaState(fmx::ptrtype sessionID){

	string res;
	string luasocket;
	int error ;
	
	//Create Lua Enviroment and open standart library
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	
	//open additional packages
	luaL_requiref(L, "JSON", luaopen_cjson, 1);
	lua_pop(L, 1);  /* remove lib from stack */
	luaL_requiref(L, "JSON.SAFE", luaopen_cjson_safe, 1);
	lua_pop(L, 1);  /* remove lib from stack */
	
		
	//create BASE64 table with C function
	luaL_newlib(L, BASE64_reg);
	lua_setglobal(L, "BASE64");

	//create LuaToFM table with C function
	luaL_newlib(L, LuaToFM_reg);
	lua_setglobal(L, "LuaToFM");
	
	//	"LuaToFM = {} "
	//	" "
	//add lua functions and varibale to LuaToFM
	string luaString = ""
		"LuaToFM.GetToFM = function(var) "
			"if (var == nil) then return nil end "
			"if (type(var)=='table' ) then return JSON.encode(var) end "
			"return var "
		"end "
	    " "
		"LuaToFM.SetGlobalEnableStatus = true "
		"LuaToFM.LastGlobalValue = nil "
		" "
		"LuaToFM.SetGlobal = function( varname, value ) "
			"while LuaToFM.SetGlobalEnableStatus ~= true do "
			"end "
			"LuaToFM.SetGlobalEnableStatus = false "
			"LuaToFM.LastGlobalValue = value "
			"local str = varname .. ' = LuaToFM.LastGlobalValue ' "
			"local chunk = load( str ) "
			"local res, err = pcall(chunk) "
			"LuaToFM.SetGlobalEnableStatus = true "
			"if (err) then return false, err "
			"else return true end "
		"end "
	    " "
		"LuaToFM.sleep = function(pause) "
	        "local st = os.time() "
			"while os.difftime ( os.time() , st ) < pause do end "
			"return pause "
	     "end "
	;
				
	error = luaL_loadstring(L, luaString.c_str() ) || lua_pcall(L, 0, 0, 0);
	if (error){
		res = lua_tostring(L, -1);
		lua_pop(L, -1);
	}
	
	
	if (sessionID == 0 ){
		// if sessionID = 0 it's start plugin and main Lua state (or restart)
		mainLuaState = L;
	}else {
		// add sessionID to global map of LuaState.
		auto itr = fmSessionMap.find(sessionID);
		if (itr != fmSessionMap.cend() ){
			itr->second.luaState.push_back(L);
		}else{
			fmSession fms;
			fms.luaState.push_back(L);
			fmSessionMap.insert(pair<fmx::ptrtype , fmSession>(sessionID, fms) );
		}
	}
	luaStateVector.push_back(L);
	
	return L;

}

// closeLuaState
lua_State* LuaToFM::closeLuaState( string address, fmx::ExprEnv& enviroment  ){
	
	auto luaState_itr = std::find_if(luaStateVector.begin(), luaStateVector.end(), [&address](lua_State* ptr) {
		ostringstream ost;
		ost << ptr  ;
		return ost.str() == address;
	});
	
	if (luaState_itr == luaStateVector.cend() ){
		// address not found
		return nullptr;
	}
		
	// delete pointer to Lua state for all fm session
	auto fms_itr = fmSessionMap.begin();
	for ( size_t i=0 ; i<fmSessionMap.size(); ++i, fms_itr ++){
		auto fms_lua_itr = std::find_if( fms_itr->second.luaState.begin(), fms_itr->second.luaState.end(), [&luaState_itr](lua_State* ptr) {
			return ptr == *luaState_itr;
		});
		if ( fms_lua_itr != fms_itr->second.luaState.cend()){
			fms_itr->second.luaState.erase(fms_lua_itr);
		}
	}
	
	lua_close(*luaState_itr);
	luaStateVector.erase(luaState_itr);
			
	return initSession(enviroment);
		
	
}

// checkLuaStateAddress
lua_State* LuaToFM::checkLuaStateAddress ( string address ){
	
	auto luaState_itr = std::find_if(luaStateVector.begin(), luaStateVector.end(), [&address](lua_State* ptr) {
		ostringstream ost;
		ost << ptr  ;
		return ost.str() == address;
	});
	
	if (luaState_itr == luaStateVector.cend() ){
		// address not found
		return nullptr;
	} else {
		return *luaState_itr;
	}
	

}

// setLuaStateActive
lua_State* LuaToFM::setLuaStateActive ( string address, fmx::ExprEnv& enviroment  ){
	
	auto luaState_itr = std::find_if(luaStateVector.begin(), luaStateVector.end(), [&address](lua_State* ptr) {
		ostringstream ost;
		ost << ptr  ;
		return ost.str() == address;
	});
	
	if (luaState_itr == luaStateVector.cend() ){
		// address not found
		return nullptr;
		
	} else {
		fmx::ptrtype sessionID = enviroment.SessionID();
		
		//find session in fmSessionMap
		auto fms_itr = fmSessionMap.find(sessionID);
		if (fms_itr != fmSessionMap.cend() ){
			fms_itr->second.enviroment = enviroment.getPtr();
						
			//add lua_State pointer to vector. if pointer exist delete it in began because pointer must be last in vector now
			auto fms_lua_itr = std::find_if( fms_itr->second.luaState.begin(), fms_itr->second.luaState.end(), [&luaState_itr](lua_State* ptr) {
				return ptr == *luaState_itr;
			});
			if ( fms_lua_itr != fms_itr->second.luaState.cend()){
				fms_itr->second.luaState.erase(fms_lua_itr);
			}
			fms_itr->second.luaState.push_back(*luaState_itr);
			
		}else{
			// create session if session nod found
			fmSession fms;
			fms.enviroment = enviroment.getPtr();
			fms.luaState.push_back(*luaState_itr);
			fmSessionMap.insert(pair<fmx::ptrtype , fmSession>(sessionID, fms) );
		}
		
		return *luaState_itr;
	}
	
}



// initSession
lua_State*  LuaToFM::initSession( fmx::ExprEnv& enviroment ){
	int error = 0;
	lua_State* L;
	fmx::ptrtype sessionID = enviroment.SessionID();
	
	auto itr = fmSessionMap.find(sessionID);
	if (itr != fmSessionMap.cend() ){
		itr->second.enviroment = enviroment.getPtr();
		if (itr->second.luaState.size()==0){
			L = mainLuaState;
			itr->second.luaState.push_back(L);
		}else{
			L = itr->second.luaState.back();
		}
	}else{
		L = mainLuaState;
		fmSession fms;
		fms.enviroment = enviroment.getPtr();
		fms.luaState.push_back(L);
		fmSessionMap.insert(pair<fmx::ptrtype , fmSession>(sessionID, fms) );
	}
	
	
		
	if (fmVersion == 0) {
		fmx::DataUniquePtr resultPtr;
		
		// define version of FM
		fmx::TextUniquePtr expressionPtr;
		expressionPtr->Assign( "Get ( ApplicationVersion )", fmx::Text::kEncoding_UTF8 ) ;
		error = enviroment.Evaluate ( *expressionPtr, *resultPtr );
		fmVersion = resultPtr->GetAsNumber().AsLong();
		
		// define system platform
		error = enviroment.EvaluateGetFunction ( enviroment.kGet_SystemPlatform, *resultPtr );
		fmSystemPlatform  = resultPtr->GetAsNumber().AsLong();
	}
		
	return L;
}


// closeSession
unsigned long LuaToFM::closeSession( fmx::ptrtype sessionID ){
	
//  we can't delete fmSession becouse we'll lost ptr to active lua state for FM session
	
//	auto itr = fmSessionMap.find(sessionID);
//	if (itr == fmSessionMap.cend() )
//			return 0;
//	else {
//		fmSessionMap.erase(itr);
//		return fmSessionMap.size();
//	}
	return 0;
}


// closeThread
unsigned long LuaToFM::closeThread( lua_State *thread){
		
	auto itr = threadMap.find(thread);
	if (itr == threadMap.cend() )
			return 0;
	else {
		threadMap.erase(itr);
		return threadMap.size();
	}
	return 0;
}


// getEnviroment
fmx::ExprEnv* LuaToFM::getEnviroment(fmx::ptrtype sessionID){
		
	auto itr = fmSessionMap.find(sessionID);
	if (itr != fmSessionMap.cend() ){
		return itr->second.enviroment;
	}
	
	return nullptr;
}


// closeAllState
short LuaToFM::closeAllState(){
	
	for(size_t i=0; i<luaStateVector.size(); ++i){
		lua_close(luaStateVector[i]);
	}
	luaStateVector.clear();
	fmSessionMap.clear();
	
	return 1;
}


// addLuaFunction
void LuaToFM::addLuaFunction(string function_name){
	
	luaFunctionsMap.insert(pair<short, string>(nextLuaFuctionsID, function_name));
	nextLuaFuctionsID += 1;
}


// getLuaFunctionName
string LuaToFM::getLuaFunctionName (short function_id){
	
	auto itr = luaFunctionsMap.find(function_id);
	if (itr != luaFunctionsMap.cend() ){
		return itr->second;
	}else{
		return "0";
	}

}


//statusDebug
string LuaToFM::statusDebug(){
	ostringstream res;
	
	res << "{ ";
	res << "\"fmVersion\" : " << fmVersion << ", ";
	res << "\"fmSystemPlatform\" : " << fmSystemPlatform << ", ";
	res << "\"nextLuaFuctionsID\" : " << nextLuaFuctionsID << ", ";
	res << "\"mainLuaState\": \"" << mainLuaState << "\", ";
	
	res << "\"luaStateVector\" : [ " ;
	for ( size_t i=0 ; i<luaStateVector.size(); ++i){
		res << "\"" << luaStateVector[i] << "\"" ;
		if ( i < luaStateVector.size()-1) res << ", " ;
	}
	res << " ], " ;
	
	auto fms_itr = fmSessionMap.begin();
	res << "\"fmSessionMap\" : [ " ;
	for ( size_t i=0 ; i<fmSessionMap.size(); ++i, fms_itr ++){
		res << "{ \"sessionID\" : " << fms_itr->first << ", ";
		res << "\"enviroment\" : \"" << fms_itr->second.enviroment << "\", ";
		res << "\"luaState\" : [ " ;
		for ( size_t j=0 ; j<fms_itr->second.luaState.size()  ; ++j){
			res << "\"" <<  fms_itr->second.luaState[j] << "\"" ;
			if ( j < fms_itr->second.luaState.size()-1) res << ", " ;
		}
		res << " ] }" ;
		if ( i < fmSessionMap.size()-1) res << ", " ;
	}
	res << " ], " ;
	
	auto func_itr = luaFunctionsMap.begin();
	res << "\"luaFunctionsMap\" : [ " ;
	for ( size_t i=0 ; i<luaFunctionsMap.size(); ++i, func_itr++){
		res << "{ \"num\": " << func_itr->first << ", \"name\": \"" << func_itr->second << "\" }";
		if ( i < luaFunctionsMap.size()-1) res << ", " ;
	}
	res << " ] " ;
	res << "}";
	
	
	return res.str();
}




