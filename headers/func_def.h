/*
 Copyright 2020 OdonataTech LLS. All rights reserved.
 */

#ifndef func_def_h
#define func_def_h

#include <utilites.h>


/* *************************************************************************************************************
 PLUGIN FUNCTIONS DEFENITIONS
 *****************************************************************************************************************/


#define LUA_Evaluate 101
fmx::errcode Evaluate ( short /* function_id */,  fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply);

#define LUA_Get 102
fmx::errcode Get ( short /* function_id */,  fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply);

#define LUA_Restart 103
fmx::errcode Restart ( short /* function_id */,  fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply);

#define LUA_RegisterLuaFunction 104
fmx::errcode RegisterLuaFunction ( short /* function_id */,  fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply);

fmx::errcode runLuaFunction ( short function_id,  fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply);

#define LUA_DoFile 105
fmx::errcode DoFile ( short /* function_id */,  fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply);

#define LUA_LuaStateNew 106
fmx::errcode LuaStateNew ( short /* function_id */,  fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply);

#define LUA_LuaStateClose 107
fmx::errcode LuaStateClose ( short /* function_id */,  fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply);

#define LUA_GetLuaStateID 108
fmx::errcode GetLuaStateID ( short /* function_id */,  fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply);

#define LUA_GetFromState 109
// LUA_GetFromState uses Get function

#define LUA_SetToState 110
// LUA_SetToState uses Evaluate function

#define LUA_SetLuaStateActive 111
fmx::errcode SetLuaStateActive ( short /* function_id */,  fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply);

#define LUA_GetAsContainer 112
fmx::errcode GetAsContainer ( short /* function_id */,  fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply);

#define LUA_GetAsContainerFromState 113



#define LUA_Status 501
fmx::errcode Status ( short /* function_id */,   fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply);


#define LUA_Test 500
fmx::errcode Test ( short /* function_id */,   fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply);



#endif /* func_def_h */
