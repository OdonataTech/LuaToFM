/*
 Copyright 2023 OdonataTech LLS. All rights reserved.
 */

#include <init.h>

extern LuaToFM *LTFM;

// *************************************************************************************************************
// Evaluate ( luacode ) and SetToState ( luacode ; lua_state_id ) -  Evaluate any lua code
// *************************************************************************************************************
fmx::errcode Evaluate ( short function_id,  fmx::ExprEnv&  environment , const fmx::DataVect& parameters, fmx::Data& reply){
	
	fmx::TextUniquePtr    resultString;
	fmx::LocaleUniquePtr default_locale;
	int error;
	string res, address;

	string luaString = TextAsUTF8String( parameters.AtAsText(0) );
	
	lua_State * LuaEnv = nullptr ;
	
	if ( function_id == LUA_Evaluate ){
		//get active Lua state for current FM session
		LuaEnv = LTFM->initSession( environment );
	}else if (function_id == LUA_SetToState ){
		//check address and get specified Lua state
		address = TextAsUTF8String(parameters.AtAsText(1)).c_str();
		
		if (address.empty()){
			res = "ERROR\n" + address + " is empty";
			resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
			reply.SetAsText ( *resultString, *default_locale );
			return 0;
		}
		
		LuaEnv = LTFM->checkLuaStateAddress(address);
	}
	
	if ( LuaEnv == nullptr){
		res = "ERROR\n Lua state address " + address + " is incorrect";
		resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
		reply.SetAsText ( *resultString, *default_locale );
		return 0;
	}
	
	
	// create new thread and push sesionID to first steck value
	lua_State *thread = lua_newthread (LuaEnv);
	int position = lua_gettop(LuaEnv);
	fmx::ptrtype sesionID = environment.SessionID();
	LTFM->threadMap.insert(pair<lua_State * , fmx::ptrtype>(thread, sesionID) );

//	lua_pushstring(thread, "KeyInLuaRegister_SessionID");
//	ostringstream ost;
//	ost << thread;
//	lua_pushstring(thread, ost.str().c_str() );
//	lua_settable(thread, LUA_REGISTRYINDEX);
	
	error = luaL_loadstring(thread, luaString.c_str() ) || lua_pcall(thread, 0, 0, 0);
	if (error){
		res = lua_tostring(thread, -1);
		res = "ERROR\n" + res;
		lua_pop(thread, 1);
	}else
		res = "OK";
		
	resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
	reply.SetAsText ( *resultString, *default_locale );
	
	//clear session id and delete new thread
	LTFM->closeThread(thread);
	LTFM->closeSession(environment.SessionID());
	lua_remove(LuaEnv, position);
	

	return 0;
		
}

// *************************************************************************************************************
// DoFile( filepath { ; convertpathflag } )Evaluate any lua code from file. Use FileMaker format path.
// *************************************************************************************************************
fmx::errcode DoFile ( short /* function_id */,  fmx::ExprEnv&  environment, const fmx::DataVect& parameters, fmx::Data& reply) {

	//get active Lua state for current FM session and create new thread
	lua_State * LuaEnv = LTFM->initSession( environment );
		
	// create new thread and push sesionID to first steck value
	lua_State *thread = lua_newthread (LuaEnv);
	int position = lua_gettop(LuaEnv);
	fmx::ptrtype sesionID = environment.SessionID();
	LTFM->threadMap.insert(pair<lua_State * , fmx::ptrtype>(thread, sesionID) );
	
	fmx::TextUniquePtr    resultString;
	fmx::LocaleUniquePtr default_locale;
	fmx::DataUniquePtr resultPtr;
	
	int error;
	string res, file_path;
	
	int convert_path = 0;
	if ( parameters.Size()>1 ) convert_path = parameters.AtAsNumber(1).AsLong();
		
						
	if ( LTFM->fmVersion >= 1900 && convert_path ){
		// for FM 19 and later convert path in FM format to system's format if convert_path is true
		fmx::TextUniquePtr  fm_file_path_ptr;
		if ( LTFM->fmSystemPlatform == fmPlatformWINDOWS ){
			error = environment.EvaluateConvertFromFileMakerPath ( parameters.AtAsText(0) , environment.kConvert_WinPath, *fm_file_path_ptr );
		}else{
			error = environment.EvaluateConvertFromFileMakerPath ( parameters.AtAsText(0) , environment.kConvert_PosixPath, *fm_file_path_ptr );
		}
		file_path = TextAsUTF8String( *fm_file_path_ptr );
		
	}else{
		// for FM before 19.0 use path as is
		file_path = TextAsUTF8String( parameters.AtAsText(0) );
	}

	error = luaL_loadfile(thread, file_path.c_str() ) || lua_pcall(thread, 0, 0, 0);
	if (error){
		res = lua_tostring(thread, -1);
		res = "ERROR\n" + res;
		lua_pop(thread, 1);
	}else
		res = "OK. " + file_path;

	resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
	reply.SetAsText ( *resultString, *default_locale );
		
	//clear session id and delete new thread
	LTFM->closeThread(thread);
	LTFM->closeSession(environment.SessionID());
	lua_remove(LuaEnv, position);

	
	return 0;
}


// **********************************************************************************************************
// Get ( lua_variable_name ) and GetFromState ( lua_variable_name ; lua_state_id ) - get value of the global lua varibale
// *********************************************************************************************************
fmx::errcode Get ( short  function_id ,  fmx::ExprEnv&  environment , const fmx::DataVect& parameters, fmx::Data& reply){

	fmx::TextUniquePtr    resultString;
	fmx::FixPtUniquePtr   resultDouble;
	fmx::LocaleUniquePtr default_locale;
	
	string varibaleName = TextAsUTF8String( parameters.AtAsText(0) );
	string res, address;
	
	double resNumber;
	int error;

	lua_State * LuaEnv = nullptr ;
	
	if ( function_id == LUA_Get ){
		//get active Lua state for current FM session
		LuaEnv = LTFM->initSession( environment );
	}else if (function_id == LUA_GetFromState ){
		//check address and get specified Lua state
		address = TextAsUTF8String(parameters.AtAsText(1)).c_str();
		
		if (address.empty()){
			res = "ERROR\n" + address + " is empty";
			resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
			reply.SetAsText ( *resultString, *default_locale );
			return 0;
		}
		
		LuaEnv = LTFM->checkLuaStateAddress(address);
	}
	
	if ( LuaEnv == nullptr){
		res = "ERROR\n Lua state address " + address + " is incorrect";
		resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
		reply.SetAsText ( *resultString, *default_locale );
		return 0;
	}
	
	// create new thread and push sesionID to first steck value
	lua_State *thread = lua_newthread (LuaEnv);
	int position = lua_gettop(LuaEnv);
	fmx::ptrtype sesionID = environment.SessionID();
	LTFM->threadMap.insert(pair<lua_State * , fmx::ptrtype>(thread, sesionID) );
	
//	string luaGetFunc = "_getLuaToFMLastResult = ";
//	luaGetFunc += "LuaToFM.GetToFM(" + varibaleName + ")";
//
//	error = luaL_loadstring(LuaEnv, luaGetFunc.c_str() ) || lua_pcall(LuaEnv, 0, 0, 0);
//	if (error){
//		res = lua_tostring(LuaEnv, -1);
//		lua_pop(LuaEnv, -1);
//		resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
//		reply.SetAsText ( *resultString, *default_locale );
//		return 0;
//	}
//
//	lua_getglobal(LuaEnv, "_getLuaToFMLastResult" );
	
	string luaGetFunc = "return LuaToFM.GetToFM(" + varibaleName + ")" ;
	error = luaL_loadstring(thread, luaGetFunc.c_str() ) || lua_pcall(thread, 0, 1, 0);
	if (error){
		res = lua_tostring(thread, -1);
		res = "ERROR\n" + res;
		lua_pop(thread, 1);
		resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
		reply.SetAsText ( *resultString, *default_locale );
		return 0;
	}
	
			
	int t = lua_type(thread, -1);
	switch (t) {
		case LUA_TNIL: {
			res = "nil";
			resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
			reply.SetAsText ( *resultString, *default_locale );
			break;
		}
		case LUA_TNUMBER: {
			resNumber = lua_tonumber(thread, -1);
			resultDouble ->AssignDouble( resNumber );
			reply.SetAsNumber( *resultDouble );
			break;
		}
		case LUA_TBOOLEAN: {
			resNumber = lua_tonumber(thread, -1);
			resultDouble ->AssignDouble( resNumber );
			reply.SetAsNumber( *resultDouble );
			break;
		}
		case LUA_TSTRING: {
			res = lua_tostring(thread, -1);
			resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
			reply.SetAsText ( *resultString, *default_locale );
			break;
		}
		default: {
			res = "Unsupported type";
			resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
			reply.SetAsText ( *resultString, *default_locale );
			break;
		}
	}
	lua_pop(thread, 1);
	
	//clear session id and delete new thread
	LTFM->closeThread(thread);
	LTFM->closeSession(environment.SessionID());
	lua_remove(LuaEnv, position);

	return 0;
		
}




// ******************************************************************************************************************
// GetAsContainer ( lua_variable_name {; filename; filetype ; pixelsWidth, pixelsHeight } )
// GetAsContainerFromState ( lua_variable_name , lua_state_id {; filename; filetype ; pixelsWidth, pixelsHeight } )
// Get binary string from Lua and return as file to FM
// ********************************************************************************************************************/
fmx::errcode GetAsContainer ( short function_id ,   fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& reply){

	fmx::TextUniquePtr    resultString;
	fmx::FixPtUniquePtr   resultDouble;
	fmx::LocaleUniquePtr default_locale;
	
	string varibaleName = TextAsUTF8String( parameters.AtAsText(0) );
	string res, address;
		
	double resNumber;
	int error;	
	int paramCnt = 1;

	lua_State * LuaEnv = nullptr ;
	
	if ( function_id == LUA_GetAsContainer ){
		//get active Lua state for current FM session
		LuaEnv = LTFM->initSession( environment );

	}else if (function_id == LUA_GetAsContainerFromState ){
		//check address and get specified Lua state
		address = TextAsUTF8String(parameters.AtAsText(1)).c_str();
		
		if (address.empty()){
			res = "ERROR\n" + address + " is empty";
			resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
			reply.SetAsText ( *resultString, *default_locale );
			return 0;
		}
		
		LuaEnv = LTFM->checkLuaStateAddress(address);		
		paramCnt = 2;
	}
	
	if ( LuaEnv == nullptr){
		res = "ERROR\n Lua state address " + address + " is incorrect";
		resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
		reply.SetAsText ( *resultString, *default_locale );
		return 0;
	}

	// create new thread and push sesionID to first steck value
	lua_State *thread = lua_newthread (LuaEnv);
	int position = lua_gettop(LuaEnv);
	fmx::ptrtype sesionID = environment.SessionID();
	LTFM->threadMap.insert(pair<lua_State * , fmx::ptrtype>(thread, sesionID) );

	string fname ;
	if ( parameters.Size()>paramCnt ) 
		fname = TextAsUTF8String( parameters.AtAsText(paramCnt) );
	else 
		fname = "file.data";
	paramCnt++;

	fmx::QuadCharUniquePtr  filetype;  
	if ( parameters.Size()>paramCnt ) 
		filetype = GetFileQuadType ( TextAsUTF8String( parameters.AtAsText(paramCnt) ) );
	else 
		filetype = GetFileQuadType("");
	paramCnt++;

	long pixelsWidth; 
	if ( parameters.Size()>paramCnt ) 		
		pixelsWidth= parameters.AtAsNumber(paramCnt).AsLong();
	else 
		pixelsWidth = 0;
	paramCnt++;

	long pixelsHeight; 
	if ( parameters.Size()>paramCnt ) 		
		pixelsHeight= parameters.AtAsNumber(paramCnt).AsLong();
	else 
		pixelsHeight = 0;
	paramCnt++;


	lua_getglobal(thread, varibaleName.c_str());

	if ( lua_type(thread, -1) != LUA_TSTRING){
		res = "ERROR\n incorrect varaibale or data";
		resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
		reply.SetAsText ( *resultString, *default_locale );
		return 0;
	}

	size_t len;
	const char *data_string = lua_tolstring(thread, -1, &len);

	fmx::errcode    err = 0;
	fmx::BinaryDataUniquePtr    outImage;

	const fmx::TextUniquePtr fileName ;
    fileName->Assign ( fname.c_str(), fmx::Text::kEncoding_UTF8 );
	    
    outImage->AddFNAMData( *fileName );       // so FileMaker has a name for our image if the user tries to save it or external storage is used

	if ( pixelsWidth >0 && pixelsHeight > 0)
		outImage->AddSIZEData( (short)pixelsWidth, (short)pixelsHeight );  // all images must have a size specified

	err = outImage->Add ( *filetype, (FMX_UInt32)len, (void *)data_string );    // Let FM know the image type
    if (err == 0) {
        reply.SetBinaryData ( *outImage, true );            // Add the image data
    }


	//clear session id and delete new thread
	LTFM->closeThread(thread);
	LTFM->closeSession(environment.SessionID());
	lua_remove(LuaEnv, position);

	return 0;
}



// *************************************************************************************************************
// Restart  Restart Lua machine
// *************************************************************************************************************
fmx::errcode Restart ( short /* function_id */,  fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply){
		
	LTFM->closeAllState();
	LTFM->newLuaState(0);
	
	fmx::TextUniquePtr    resultString;
	fmx::LocaleUniquePtr default_locale;
	string res = "Lua restarted. All state and variables was clear.";
	resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
	reply.SetAsText ( *resultString, *default_locale );
	
	return 0;
}


// *************************************************************************************************************
// RegisterLuaFunction( function_prototype ; function_description ) Registrate any Lua function as FileMaker's function
// *************************************************************************************************************
fmx::errcode RegisterLuaFunction ( short /* function_id */,  fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply){

	fmx::errcode error = kSPNoError;
	
	fmx::TextUniquePtr    resultString;
	fmx::LocaleUniquePtr default_locale;
	
	string function_prototype = TextAsUTF8String( parameters.AtAsText(0) );
	string function_descr = TextAsUTF8String( parameters.AtAsText(1) );
	string res;
	

	if (function_prototype.empty()) {
		res = "Error";
		resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
		reply.SetAsText ( *resultString, *default_locale );
		return 0;
	}
	
	string function_name = "";
	string currentSymbol = "";
	unsigned long i = 0;
	while (i < function_prototype.length() && currentSymbol != "(" ) {
		currentSymbol = function_prototype.substr(i, 1 );
		if ( currentSymbol != " " && currentSymbol != "(" )
			function_name += currentSymbol;
		++i;
	}
		
	error = RegisterFunction ( LTFM->nextLuaFuctionsID,
							  function_prototype ,
							  runLuaFunction,
							  function_descr
							  );
	
	if ( kSPNoError != error ) {
		res = "Error";
	}else{
		LTFM->addLuaFunction(function_name);
		ostringstream ost;
		ost << function_name << " registred."; // << " Function ID: " << nextLuaFuctionsID << " required_parameters " << required_parameters << " optional_rarameters " << optional_rarameters ;
		res = ost.str();
	}
		
	resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
	reply.SetAsText ( *resultString, *default_locale );

	return 0;
}


// *************************************************************************************************************
// runLuaFunction( .. )  Called in FM when calling custom Lua functions
// *************************************************************************************************************
fmx::errcode runLuaFunction ( short function_id,  fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& reply){
		
	fmx::FixPtUniquePtr   resultDouble;
	fmx::TextUniquePtr    resultString;
	fmx::LocaleUniquePtr default_locale;
	string res;
	double resNumber;
		
	string function_name = LTFM->getLuaFunctionName(function_id);
	if ( function_name == "0"){
		res = "ERROR\n Function not found ";
		resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
		reply.SetAsText ( *resultString, *default_locale );
		return 0;
	}
	
	//get active Lua state for current FM session and create new thread
	lua_State * LuaEnv = LTFM->initSession( environment );
		
	// create new thread and push sesionID to first steck value
	lua_State *thread = lua_newthread (LuaEnv);
	int position = lua_gettop(LuaEnv);
	fmx::ptrtype sesionID = environment.SessionID();
	LTFM->threadMap.insert(pair<lua_State * , fmx::ptrtype>(thread, sesionID) );
	
	// push function name to steck
	lua_getglobal(thread, function_name.c_str() );
	// push parameters to steck as string
	if ( parameters.Size() >= LUA_MINSTACK ) luaL_checkstack (thread, (int)parameters.Size() , "");
	uint32_t n = 0;
	while (n < parameters.Size() ) {
		lua_pushstring(thread, TextAsUTF8String(parameters.AtAsText(n)).c_str() );
		++n;
	}
		
	// call function and check error
	int error = lua_pcall(thread, (int)parameters.Size(), 1, 0);
	if (error){
		res = lua_tostring(thread, -1);
		res = "ERROR\n" + res;
		lua_pop(thread, 1);
		resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
		reply.SetAsText ( *resultString, *default_locale );
		LTFM->closeSession(environment.SessionID());
		lua_remove(LuaEnv, position);
		return 0;
	}
	
	
	int t = lua_type(thread, -1);
	switch (t) {
		case LUA_TNIL: {
			res = "nil";
			resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
			reply.SetAsText ( *resultString, *default_locale );
			break;
		}
		case LUA_TNUMBER: {
			resNumber = lua_tonumber(thread, -1);
			resultDouble ->AssignDouble( resNumber );
			reply.SetAsNumber( *resultDouble );
			break;
		}
		case LUA_TBOOLEAN: {
			resNumber = lua_tonumber(thread, -1);
			resultDouble ->AssignDouble( resNumber );
			reply.SetAsNumber( *resultDouble );
			break;
		}
		case LUA_TSTRING: {
			res = lua_tostring(thread, -1);
			resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
			reply.SetAsText ( *resultString, *default_locale );
			break;
		}
		case LUA_TTABLE: {
			
			// set as temp global table in Lua enviroment with random name and get as json
			ostringstream ost;
			ost << "_getLuaToFMLastResultTable_" << mersenne_rand_int();
			string random_tablename = ost.str();
			
			lua_setglobal(thread, random_tablename.c_str() );
			string luaGetFunc = "return JSON.encode( " + random_tablename + " )" ;
			error = luaL_loadstring(thread, luaGetFunc.c_str() ) || lua_pcall(thread, 0, 1, 0);
			
			res = lua_tostring(thread, -1);
			if (error){
				res = "ERROR\n" + res;
				resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
				reply.SetAsText ( *resultString, *default_locale );
			}else{
				resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
				reply.SetAsText ( *resultString, *default_locale );
			}
						
			// delete temp table
			string luaString = random_tablename + " = nil" ;
			luaL_loadstring(thread, luaString.c_str() ) || lua_pcall(thread, 0, 0, 0);
			
			break;
		}
		default: {
			res = "Unsupported type";
			resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
			reply.SetAsText ( *resultString, *default_locale );
			break;
		}
	}
		
	lua_pop(thread, 1);
	
	//clear session id and delete new thread
	LTFM->closeThread(thread);
	LTFM->closeSession(environment.SessionID());
	lua_remove(LuaEnv, position);

	return 0;
}

// ******************************************************************************************************************
// LuaStateNew  Create new LuaState and set as current"
// ********************************************************************************************************************/
fmx::errcode LuaStateNew ( short /* function_id */,   fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& reply){
	
	fmx::TextUniquePtr    resultString;
	fmx::LocaleUniquePtr default_locale;

	lua_State * LuaEnv = LTFM->newLuaState(environment.SessionID());
		
	ostringstream ost;
	ost << LuaEnv  ;
	string str = ost.str();
			
	resultString ->Assign ( str.c_str(),  fmx::Text::kEncoding_UTF8 );
	reply.SetAsText ( *resultString, *default_locale );
	
	return 0;
}

// ******************************************************************************************************************
// LuaStateClose  Create new LuaState and set as current
// ********************************************************************************************************************/
fmx::errcode LuaStateClose ( short /* function_id */,   fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& reply){

	fmx::TextUniquePtr    resultString;
	fmx::LocaleUniquePtr default_locale;
	string str;
	
	string address = TextAsUTF8String(parameters.AtAsText(0)).c_str();
	
	if (address.empty()){
		str = "ERROR\n" + address + " is empty";
		resultString ->Assign ( str.c_str(),  fmx::Text::kEncoding_UTF8 );
		reply.SetAsText ( *resultString, *default_locale );
		return 0;
	}
	
	ostringstream ost_state;
	ost_state << LTFM->mainLuaState;
	if ( ost_state.str() == address){
		str = "ERROR\n" + address + " is main Lua state";
		resultString ->Assign ( str.c_str(),  fmx::Text::kEncoding_UTF8 );
		reply.SetAsText ( *resultString, *default_locale );
		return 0;
	}
	
	lua_State * LuaEnv = LTFM->closeLuaState(address, environment);
				
	if (LuaEnv == nullptr){
		str = "ERROR\n" + address + " not found";
	}else{
		ostringstream ost;
		ost << "OK\n" << address  << " closed\n" << LuaEnv << " current state";
		str = ost.str();
	}
	
	resultString ->Assign ( str.c_str(),  fmx::Text::kEncoding_UTF8 );
	reply.SetAsText ( *resultString, *default_locale );
	
	return 0;
}

// ******************************************************************************************************************
// GetLuaStateID  Return id of active lua state
// ********************************************************************************************************************/
fmx::errcode GetLuaStateID ( short /* function_id */,   fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& reply){
	
	fmx::TextUniquePtr    resultString;
	fmx::LocaleUniquePtr default_locale;
	
	lua_State * LuaEnv = LTFM->initSession( environment );

	ostringstream ost;
	ost << LuaEnv ;
	string str = ost.str();
	
	resultString ->Assign ( str.c_str(),  fmx::Text::kEncoding_UTF8 );
	reply.SetAsText ( *resultString, *default_locale );
	
	return 0;
}

// ******************************************************************************************************************
// SetLuaStateActive ( lua_state_id )  Return id of active lua state
// ********************************************************************************************************************/
fmx::errcode SetLuaStateActive ( short /* function_id */,   fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& reply){
	
	fmx::TextUniquePtr    resultString;
	fmx::LocaleUniquePtr default_locale;
	
	string res, address;
	
	address = TextAsUTF8String(parameters.AtAsText(0)).c_str();
	
	if (address.empty()){
		res = "ERROR\n" + address + " is empty";
		resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
		reply.SetAsText ( *resultString, *default_locale );
		return 0;
	}
		
	lua_State * LuaEnv = LTFM->setLuaStateActive(address, environment );

	if ( LuaEnv == nullptr){
		res = "ERROR\n Lua state address " + address + " is incorrect";
		resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
		reply.SetAsText ( *resultString, *default_locale );
		return 0;
	}
	
	res = "OK\n Lua state address " + address + " is active";
	resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
	reply.SetAsText ( *resultString, *default_locale );

	return 0;
}




// ******************************************************************************************************************
// Status  Return return info about current state of LuaToFM plugin. Use for debug
// ********************************************************************************************************************/
fmx::errcode Status ( short /* function_id */,   fmx::ExprEnv& /*environment */, const fmx::DataVect& parameters, fmx::Data& reply){
	
	fmx::TextUniquePtr    resultString;
	fmx::LocaleUniquePtr default_locale;
	
	string res = LTFM->statusDebug();
	
	resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
	reply.SetAsText ( *resultString, *default_locale );
	
	return 0;
	
}

// ******************************************************************************************************************
// Test( testdata )  FOR TESTING
// ********************************************************************************************************************/
fmx::errcode Test ( short /* function_id */,   fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& reply){
	
	fmx::TextUniquePtr    resultString;
	fmx::LocaleUniquePtr default_locale;
	
	string res = LTFM->statusDebug();
	
	resultString ->Assign ( res.c_str(),  fmx::Text::kEncoding_UTF8 );
	reply.SetAsText ( *resultString, *default_locale );
		
	
//	//get active Lua state for current FM session and create new thread
//	lua_State * LuaEnv = LTFM->initSession( environment );
//	lua_State *thread = lua_newthread (LuaEnv);
//	int position = lua_gettop(LuaEnv);
		
	
//	std::string param1 = TextAsUTF8String( parameters.AtAsText(0) );
//
//	string luaString = "testLua = 'Your send text to Lua: " +  param1 + ". Lua is perfect!'";
//	luaL_loadstring(LuaEnv, luaString.c_str() );
//	lua_pcall(LuaEnv, 0, 0, 0);
//
//	lua_getglobal(LuaEnv, "testLua");
//	string res = lua_tostring(LuaEnv, -1);
	
//	fmx::ptrtype sessionID = environment.SessionID();
//	fmx::ExprEnv *envPtr = environment.getPtr() ;
//	sessionEnviromentMap->insert(pair<fmx::ptrtype , fmx::ExprEnv*>(sessionID, envPtr ));
	
//	sessionEnviromentMap->insert(pair<fmx::ptrtype , fmx::ExprEnv*>(environment.SessionID(), environment.getPtr() ));
//				
//	auto itr = sessionEnviromentMap->find(environment.SessionID());
//	fmx::ExprEnv *envFound = itr->second;
	
//	fmx::ExprEnv *envFound = LTFM->getEnviroment( environment.SessionID());
	
//	ostringstream ost;
//	ost << "fmVersion: " << LTFM->fmVersion << " fmSystem: " << LTFM->fmSystemPlatform << " SessionID: " << environment.SessionID() << " SessionID from LTFM: " << envFound->SessionID() << " fmSessionMap size: " <<  LTFM->fmSessionMap.size() ;
//	
//	//<< " SessionID from map: " << envFound->SessionID() << " size: " << sessionEnviromentMap->size() ;
//	string str = ost.str();
//
//	resultString ->Assign ( str.c_str(),  fmx::Text::kEncoding_UTF8 );
//	reply.SetAsText ( *resultString, *default_locale );
//	
//	
//	//clear session id and delete new thread
//	LTFM->closeSession(environment.SessionID());
//	lua_remove(LuaEnv, position);

	return 0;
		
}
