/*
 Copyright 2023 OdonataTech LLS. All rights reserved.
 */


#ifndef luaToFM_h
#define luaToFM_h

#include <utilites.h>
#include <base64.h>
#include <lua.hpp>

using namespace std;

struct fmSession {
    fmx::ExprEnv* enviroment;
    std::vector<lua_State *> luaState;
};

struct LuaToFM{
	
    int fmVersion  = 0;
    int fmSystemPlatform = 0 ;
    short nextLuaFuctionsID = 5000;
    
    lua_State* mainLuaState;
    std::vector<lua_State *> luaStateVector;
    
    std::map<fmx::ptrtype, fmSession> fmSessionMap;
    std::map<lua_State * , fmx::ptrtype> threadMap;
    std::map<short , string> luaFunctionsMap;
    
    
    // methods
    lua_State* newLuaState( fmx::ptrtype sessionID );
    lua_State* closeLuaState( string address, fmx::ExprEnv& enviroment  );
    short closeAllState();
    lua_State* checkLuaStateAddress ( string address );
    lua_State* setLuaStateActive ( string address, fmx::ExprEnv& enviroment  );
    
    lua_State* initSession( fmx::ExprEnv& env );
    unsigned long closeSession( fmx::ptrtype sessionID );
    fmx::ExprEnv* getEnviroment(fmx::ptrtype sessionID);
    
    void addLuaFunction(string function_name);
    string getLuaFunctionName (short function_id);
    
    unsigned long closeThread( lua_State *thread);
    
    string statusDebug();
                
};




#endif /* luaToFM_h */

