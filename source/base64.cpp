/*
 Copyright 2023 OdonataTech LLS. All rights reserved.
 */

#include "base64.h"

// BASE64_encode
// BASE64.encode( data )

int BASE64_encode ( lua_State *L){
	
	if ( lua_type(L, 1) != LUA_TSTRING){
		lua_pushstring(L, "ERROR");
		lua_pushstring(L, "Incorrect data");
		return 2;
	}
	
	size_t len;
	const char *data_string = lua_tolstring(L, 1, &len);
	
	char* result = new char[ len *2 ];
		
	base64_encodestate s;
	base64_init_encodestate(&s);
	size_t cnt = base64_encode_block(data_string, len , result, &s);
	cnt += base64_encode_blockend(result + cnt , &s);
	base64_init_encodestate(&s);
	
	//null-terminate string
	result[cnt] = 0;
	lua_pushlstring(L, result , cnt);
	
	delete[] result;
	
	return 1;
	
}


// BASE64_decode
// BASE64.decode ( text )
int BASE64_decode ( lua_State *L){
	
	if ( lua_type(L, 1) != LUA_TSTRING){
		lua_pushstring(L, "ERROR");
		lua_pushstring(L, "Incorrect data");
		return 2;
	}
	string data_string = lua_tostring(L, 1);
	
	char* result = new char[ data_string.size() ];
	
	base64_decodestate s;
	base64_init_decodestate(&s);
	size_t cnt = base64_decode_block(data_string.c_str(), data_string.size() , result, &s);
	base64_init_decodestate(&s);
	
	//null-terminate string
	result[cnt] = 0;
	lua_pushlstring(L, result , cnt);
	
	delete[] result;
	
	return 1;
}


