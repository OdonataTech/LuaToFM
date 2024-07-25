/*
 Copyright 2020 OdonataTech LLS. All rights reserved.
 */

#ifndef base64_h
#define base64_h

extern "C" {
#include <cencode.h>
#include <cdecode.h>
}

#include "lua.hpp"
#include <string>


using namespace std;

int BASE64_decode ( lua_State *L);
int BASE64_encode ( lua_State *L);


const luaL_Reg BASE64_reg[] = {
    { "encode" , BASE64_encode },
    { "decode" , BASE64_decode },
    { NULL, NULL}
};



#endif /* base64_h */
