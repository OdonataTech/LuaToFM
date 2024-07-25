/*
 Copyright 2020 OdonataTech LLS. All rights reserved.
 */

#ifndef init_h
#define init_h

#include <lua.hpp>

#include <utilites.h>
#include <func_def.h>
#include <luaToFM.h>
#include <lua_funcs.h>
#include <base64.h>


using namespace std;

#define PLUGIN_NAME "LuaToFM"        //    default is the project name (Xcode only)
//        in Visual Studio set $(TargetName)

#define PLUGIN_PREFIX "lu"        //    default is the first characters of the function name
#define PLUGIN_ID "LUFM"        //   always must be four characters Plugin ID

#define VERSION_STRING "1.0.4001" // also in xcode project build settings

#define PLUGIN_DESCRIPTION_STRING_ID PLUGIN_NAME " Version: " VERSION_STRING "\n\nEvaluate Lua language"


#pragma mark -
#pragma mark Prototypes
#pragma mark -

#define MAIN_CONTAINER_TYPE "MAIN"
#define SOUND_CONTAINER_TYPE "snd "
#define JPEG_CONTAINER_TYPE "JPEG"
#define GIF_CONTAINER_TYPE "GIFf"
#define EPS_CONTAINER_TYPE "EPS "
#define META_CONTAINER_TYPE "META"
#define PNG_CONTAINER_TYPE "PNGf"
#define BMP_CONTAINER_TYPE "BMPf"
#define PDF_CONTAINER_TYPE "PDF "


// pragma mark generates a warning C4068 in Visual Studio so that warning is disabled in VS

#pragma mark -
#pragma mark Enums, Defines & Globals
#pragma mark -

enum {
	kSPOptionsString = 1
};


enum {
	kSPManyParameters = -1,
	kSPPrefixLength = 2,
	kSPPluginIDLength = 5,    //    kSPPrefixLength = 4 + 1 | not use now
	kSPFirstFunction = 1000
};


enum {
	kSPFirstParameter = 0,
	kSPSecondParameter = 1,
	kSPThirdParameter = 2,
	kSPFourthParameter = 3,
	kSPFifthParameter = 4
};


enum errors {
	kSPNoError = 0,
};


enum fmPlatform{
	fmPlatformMACOS= 1,
	fmPlatformWINDOWS = - 2,
	fmPlatformIOS = 3,
	fmPlatformWEBDIRECT = 4,
	fmPlatformCENTOS = 5,
	fmPlatformUBUNTU = 8
};


const fmx::ptrtype Init ( FMX_ExternCallPtr /* pb */ );
void Shutdown ( void );

const fmx::QuadCharUniquePtr PluginID ( void );
const fmx::TextUniquePtr PluginPrefix ( void );
const fmx::TextUniquePtr PluginOptionsString ( void );


void GetString ( FMX_ExternCallPtr pb );
const fmx::errcode RegisterFunction ( const short functionID, const std::string prototype, const fmx::ExtPluginType function, const std::string description = "" );
void UnregisterFunctions ( void );
const fmx::TextUniquePtr FunctionName ( const fmx::TextUniquePtr& signature );
void NumberOfParameters ( const fmx::TextUniquePtr& signature, short& required, short& optional );



#endif /* init_h */
