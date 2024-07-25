/*
 Copyright 2023 OdonataTech LLS. All rights reserved.
 */

#include <init.h>

extern LuaToFM *LTFM;
extern FMX_ExternCallPtr gFMX_ExternCallPtr;

#pragma mark -
#pragma mark FMX
#pragma mark -

/* *************************************************************************************************************
LuaToFM PACKAGE FUNCTIONS
*****************************************************************************************************************/

// LuaToFM_fmEvaluate
// fmEvaluate( fm_expression )
int LuaToFM_fmEvaluate( lua_State *L){
	
	string expression = luaL_checkstring(L, 1);
	ostringstream ost;
	
	auto thread_itr = LTFM->threadMap.find(L);
	fmx::ptrtype sessionID = thread_itr->second;

//	lua_getfield(L, LUA_REGISTRYINDEX, "KeyInLuaRegister_SessionID");
//	ost << "lua_gettop = " << lua_gettop(L) << "\n";
//	ost << "type pos 2 = " << lua_type(L, -1) << "\n";
//	expression = lua_tostring(L, -1);
//	ost << "Register Value  = " << expression << "\n";
	
	int t = lua_type(L, -1);
	if ( t != LUA_TSTRING){
		lua_pushstring(L, "ERROR");
		lua_pushstring(L, "Incorrect expression");
		return 2;
	}
		
	fmx::ExprEnv* enviroment = LTFM->getEnviroment(sessionID);
			
	if (enviroment == nullptr ){
		lua_pushstring(L, "ERROR");
		ost << sessionID << "SessionID is not known" ;
		lua_pushstring(L, ost.str().c_str() );
		return 2;
	}
	
	fmx::TextUniquePtr expressionPtr;
	fmx::DataUniquePtr resultPtr;
	expressionPtr->Assign( expression.c_str(), fmx::Text::kEncoding_UTF8 ) ;
	int error = enviroment->Evaluate ( *expressionPtr, *resultPtr );
	string res =  TextAsUTF8String (resultPtr->GetAsText() );
	
	lua_pushstring(L, res.c_str() );
	return 1;
}


// LuaToFM_setFMvariable
// setFMvariable( var_name , var_value)
int LuaToFM_setFMvariable( lua_State *L){
	
	string var_name = luaL_checkstring(L, 1);
	string var_value = luaL_checkstring(L, 2);
	string expression = "";
	ostringstream ost;
	
	auto thread_itr = LTFM->threadMap.find(L);
	fmx::ptrtype sessionID = thread_itr->second;
	
	int t = lua_type(L, 1);
	if ( t != LUA_TSTRING){
		lua_pushstring(L, "ERROR");
		lua_pushstring(L, "Incorrect name");
		return 2;
	}
	
	t = lua_type(L, 2);
	if ( t == LUA_TNUMBER ){
		expression = "Let ( $$" + var_name + " = " + var_value + "; 1 )" ;
	}else if (t == LUA_TSTRING){
		substitute(var_value, "\"", "\\\"");
		expression = "Let ( $$" + var_name + " = \"" + var_value + "\"; 1 )" ;
	}else{
		lua_pushstring(L, "ERROR");
		lua_pushstring(L, "Incorrect value");
		return 2;
	}
		
	fmx::ExprEnv* enviroment = LTFM->getEnviroment(sessionID);
	if (enviroment == nullptr ){
		lua_pushstring(L, "ERROR");
		ost << sessionID << "SessionID is not known" ;
		lua_pushstring(L, ost.str().c_str() );
		return 2;
	}
	
	fmx::TextUniquePtr expressionPtr;
	fmx::DataUniquePtr resultPtr;
	expressionPtr->Assign( expression.c_str(), fmx::Text::kEncoding_UTF8 ) ;
	int error = enviroment->Evaluate ( *expressionPtr, *resultPtr );
	string res =  TextAsUTF8String (resultPtr->GetAsText() );
	
	lua_pushstring(L, res.c_str() );
	lua_pushstring(L, expression.c_str() );
	return 2;
}


// LuaToFM_ExecuteFMSQL
// ExecuteFMSQL ( SQL_parameters_table )
int LuaToFM_executeFMSQL( lua_State *L){
	
	ostringstream ost;
	
	auto thread_itr = LTFM->threadMap.find(L);
	fmx::ptrtype sessionID = thread_itr->second;
	fmx::ExprEnv* enviroment = LTFM->getEnviroment(sessionID);
	if (enviroment == nullptr ){
		lua_pushstring(L, "ERROR");
		ost << sessionID << "SessionID is not known" ;
		lua_pushstring(L, ost.str().c_str() );
		return 2;
	}
		
	if ( lua_type(L, 1) != LUA_TTABLE ){
		lua_pushstring(L, "ERROR");
		lua_pushstring(L, "All functions paramaters must be the one table");
		return 2;
	}
	
	lua_getfield(L, 1, "file");
	if ( lua_type(L, -1) != LUA_TSTRING  ){
		lua_pushstring(L, "ERROR");
		lua_pushstring(L, "File name is not string");
		return 2;
	}
	string file_name = lua_tostring(L, -1);
	lua_pop(L, 1);
	fmx::TextUniquePtr fileNamePtr;
	fileNamePtr->Assign( file_name.c_str(), fmx::Text::kEncoding_UTF8 ) ;
	
	lua_getfield(L, 1, "query");
	if ( lua_type(L, -1) != LUA_TSTRING ){
		lua_pushstring(L, "ERROR");
		lua_pushstring(L, "Query expression is not string");
		return 2;
	}
	string expression = lua_tostring(L, -1);
	lua_pop(L, 1);
	fmx::TextUniquePtr expressionPtr;
	expressionPtr->Assign( expression.c_str(), fmx::Text::kEncoding_UTF8 ) ;
	
	
	lua_getfield(L, 1, "params");
	fmx::DataVectUniquePtr parameters;
	if ( lua_type(L, -1 ) == LUA_TTABLE ){
		int size_params = (int)lua_rawlen(L, -1);
		string prm_str;
		fmx::FixPtUniquePtr   paramDouble;
		fmx::TextUniquePtr paramText;
		fmx::DataUniquePtr paramData;
		fmx::LocaleUniquePtr default_locale;
		for ( int i = 1; i<= size_params; ++i ){
			lua_geti(L, -1, i);
			int t = lua_type(L, -1);
			switch (t) {
				// process only number or text
				case LUA_TNUMBER: {
					paramDouble ->AssignDouble( lua_tonumber(L, -1) );
					paramData->SetAsNumber( *paramDouble );
					parameters->PushBack(*paramData);
					break;
				}
				case LUA_TSTRING: {
					prm_str = lua_tostring(L, -1);
					paramText->Assign ( prm_str.c_str(),  fmx::Text::kEncoding_UTF8 );
					paramData->SetAsText ( *paramText, *default_locale );
					parameters->PushBack(*paramData);
					break;
				}
			}
			lua_pop(L, 1); // delete value from stack
		}
		lua_pop(L, 1);
	}
	
	// call ExecuteFileSQL
	fmx::RowVectUniquePtr result;
	enviroment->ExecuteFileSQL ( *expressionPtr, *fileNamePtr,  *parameters, *result );
	
	long row_nums =result->Size();
	
	if(row_nums == 0){
		lua_pushnumber(L, row_nums );
		lua_pushstring(L, "" );
		return 2;
	}
	
	// define additional parameters for result
	size_t col_nums = result->At(0).Size();
	
	string types = "";
	lua_getfield(L, 1, "types");
	if ( lua_type(L, -1) == LUA_TSTRING  ){
		types = lua_tostring(L, -1);
		lua_pop(L, 1);
	}
	// if types not specified or not specify for all cols then all tail cols set as string
	if ( col_nums > types.size() )
		types += string ( col_nums - types.size() , 's');
	
	string keys_type = "numeric";
	lua_getfield(L, 1, "keys_type");
	if ( lua_type(L, -1) == LUA_TSTRING  ){
		keys_type = lua_tostring(L, -1);
		lua_pop(L, 1);
	}
	
	vector<string> fields_names ;
	lua_getfield(L, 1, "fields_names");
	if ( lua_type(L, -1) == LUA_TTABLE  ){
		int size_fields_names = (int)lua_rawlen(L, -1);
		for ( int i = 1; i<= size_fields_names; ++i ){
			lua_geti(L, -1, i);
			fields_names.push_back(lua_tostring(L, -1) );
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
	}
	
	string table_type = "rows";
	lua_getfield(L, 1, "table_type");
	if ( lua_type(L, -1) == LUA_TSTRING  ){
		table_type = lua_tostring(L, -1);
		lua_pop(L, 1);
	}
	
		
	lua_pushnumber(L, row_nums );
	string col_str;
	double col_double;
	
	if ( table_type != "columns" ){
		// table is array of rows
		// create and fill table of result
		lua_createtable(L, (int)row_nums, 0);
		for ( unsigned int i = 0; i<row_nums ; ++i){
			//create row
			lua_createtable(L, (int)col_nums, 0);
			for ( unsigned int j = 0; j< col_nums ; ++j){
				if (keys_type == "names" && (j+1)<=fields_names.size() ){
					lua_pushstring(L, fields_names[j].c_str());
				}else{
					lua_pushnumber(L, j+1);
				}
				if ( types.substr(j, 1) == "d" ){
					col_double = result->At(i).AtAsNumber(j).AsFloat();
					lua_pushnumber(L, col_double);
				}else{
					col_str = TextAsUTF8String( result->At(i).AtAsText(j) );
					lua_pushstring(L, col_str.c_str());
				}
				lua_settable (L, -3);
	//			lua_rawseti(L, -2, j+1);
			}
			lua_rawseti(L, -2, i+1);
		}

		
	}else{
		// table is array of columns
		// create and fill table of result
		lua_createtable(L, (int)col_nums, 0);
				
		//create table for each columns
		unsigned int pos_col = (unsigned int)lua_gettop(L) + 1;
		for ( unsigned int j = 0; j< col_nums ; ++j){
			lua_createtable(L, (int)row_nums, 0);
		}
		
		for ( unsigned int i = 0; i<row_nums ; ++i){
			//write each value in row to his columns
			for ( unsigned int j = 0; j< col_nums ; ++j){
				if ( types.substr(j, 1) == "d" ){
					col_double = result->At(i).AtAsNumber(j).AsFloat();
					lua_pushnumber(L, col_double);
				}else{
					col_str = TextAsUTF8String( result->At(i).AtAsText(j) );
					lua_pushstring(L, col_str.c_str());
				}
				lua_rawseti(L, (int)(pos_col+j), i+1);
			}
		}
		
		// add columns to main table
		for ( unsigned int j = 0; j< col_nums ; ++j){
			if (keys_type == "names" && (j+1)<=fields_names.size() ){
				lua_pushstring(L, fields_names[j].c_str());
			}else{
				lua_pushnumber(L, j+1);
			}
			lua_pushvalue(L, (int)(pos_col+j) );
			lua_settable (L, (int)(pos_col-1) );
		}
		// clear unneeded values in stack. The main table must be in top
		lua_settop(L, (int)(pos_col-1));
	}
	
	return 2;
	
}

// LuaToFM_fmPerfomScript
// fmPerfomScript( fm_file_name, script_name, script_parameter, script_control )
int LuaToFM_fmPerfomScript( lua_State *L){
	
	if ( lua_type(L, 1) != LUA_TSTRING){
		lua_pushstring(L, "ERROR");
		lua_pushstring(L, "Incorrect file name");
		return 2;
	}
	string file_name = lua_tostring(L, 1);
	
	if ( lua_type(L, 2) != LUA_TSTRING){
		lua_pushstring(L, "ERROR");
		lua_pushstring(L, "Incorrect script name");
		return 2;
	}
	string script_name = lua_tostring(L, 2);
	
	string script_parameter = "";
	if ( lua_type(L, 3) == LUA_TSTRING){
		script_parameter = lua_tostring(L, 3);
	}
	
	string script_control = "Pause";
	if ( lua_type(L, 4) == LUA_TSTRING){
		script_parameter = lua_tostring(L, 4);
	}
	
	unsigned char script_control_char ;
	if (script_control == "Halt" )
		script_control_char = kFMXT_Halt;
	else if (script_control == "Exit" )
		script_control_char = kFMXT_Exit;
	else if (script_control == "Resume" )
		script_control_char = kFMXT_Resume;
	else
		script_control_char = kFMXT_Pause;
	
	
	ostringstream ost;
	
	fmx::TextUniquePtr fileNamePtr;
	fileNamePtr->Assign( file_name.c_str(), fmx::Text::kEncoding_UTF8 ) ;
		
	fmx::TextUniquePtr scriptNamePtr;
	scriptNamePtr->Assign( script_name.c_str(), fmx::Text::kEncoding_UTF8 ) ;
	
	fmx::TextUniquePtr scriptParameterPtr;
	scriptParameterPtr->Assign( script_parameter.c_str(), fmx::Text::kEncoding_UTF8 ) ;
	
	fmx::DataUniquePtr scriptParameterDataPtr;
	fmx::LocaleUniquePtr default_locale;
	scriptParameterDataPtr->SetAsText (  *scriptParameterPtr, *default_locale );
	
	
	int error = FMX_StartScript(&*fileNamePtr, &*scriptNamePtr, script_control_char, &*scriptParameterDataPtr );
	
	lua_pushnumber(L, error );
	
	return 1;
}




