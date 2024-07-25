/*
 Copyright 2023 OdonataTech LLS. All rights reserved.
 */


#ifndef lua_funcs_h
#define lua_funcs_h

#include "lua.hpp"


/* *************************************************************************************************************
LuaToFM PACKAGE FUNCTIONS
*****************************************************************************************************************/


int LuaToFM_fmEvaluate( lua_State *L);

int LuaToFM_setFMvariable( lua_State *L);

int LuaToFM_executeFMSQL( lua_State *L);

int LuaToFM_fmPerfomScript( lua_State *L);

const luaL_Reg LuaToFM_reg[] = {
    { "fmEvaluate" , LuaToFM_fmEvaluate },
    { "setFMvariable" , LuaToFM_setFMvariable },
    { "executeFMSQL" , LuaToFM_executeFMSQL },
    { "fmPerfomScript" , LuaToFM_fmPerfomScript },
    { NULL, NULL}
};




#endif /* lua_funcs_h */

