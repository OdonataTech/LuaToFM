/*
 Copyright 2023 OdonataTech LLS. All rights reserved.
 */


#define FMX_USE_UNIQUE_PTR 1


#include <init.h>



using namespace std;

int initCounter = 1;

// Globals
short g_next_function;	// used to keep track of the funciton id number
FMX_ExternCallPtr gFMX_ExternCallPtr; // needed for the FMX API

#pragma mark -
#pragma mark Functions
#pragma mark -


/* *************************************************************************************************************
MAIN POINTER TO LuaToFM ENVIROMENT (Lua states and other info ), will be inicialisated in the init()
**************************************************************************************************************** */
LuaToFM *LTFM;


/* *************************************************************************************************************
SPECIAL FUNCTION FOR INIT AND REGISTRATE PLUGIN FUNCTIONS
*****************************************************************************************************************/

#pragma mark -
#pragma mark Plugin
#pragma mark -


/*
 initalise the plug-in
 perform any setup and register functions
 */


const fmx::ptrtype Init ( FMX_ExternCallPtr /* pb */ )
{
	// perform any initialisation and set-up globals

	LTFM = new LuaToFM;
	LTFM->newLuaState(0);
			
	fmx::errcode error = kSPNoError;
	fmx::ptrtype enable = kCurrentExtnVersion; // kDoNotEnable to prevent the plugin loading

	/*
	 register plug-in functions
	 functions must always be registered in the same order (to avoid breaking existing calculation in FM).
	 */
	
	error = RegisterFunction ( LUA_Evaluate,
							  "Evaluate ( luacode )" ,
							  Evaluate,
							  "Evaluate any lua code"
							  );
	if ( kSPNoError != error ) { enable = (fmx::ptrtype)kDoNotEnable; }

	
	error = RegisterFunction ( LUA_Get,
							  "Get ( lua_variable_name ) " ,
							  Get,
							  "Get value of the global lua varibale"
							  );
	if ( kSPNoError != error ) { enable = (fmx::ptrtype)kDoNotEnable; }

	
	error = RegisterFunction ( LUA_Restart,
							  "Restart" ,
							  Restart,
							  "Restart Lua machine. All variables in memory will be cleared"
							  );
	if ( kSPNoError != error ) { enable = (fmx::ptrtype)kDoNotEnable; }

	
	error = RegisterFunction ( LUA_RegisterLuaFunction,
							  "RegisterLuaFunction( function_prototype ; function_description )" ,
							  RegisterLuaFunction,
							  "Registrate any Lua function as FileMaker's function"
							  );
	if ( kSPNoError != error ) { enable = (fmx::ptrtype)kDoNotEnable; }


	error = RegisterFunction ( LUA_DoFile,
							  "DoFile( filepath { ; convert_path_flag } )" ,
							  DoFile,
							  "Evaluate any lua code from file. Use FileMaker format path."
							  );
	if ( kSPNoError != error ) { enable = (fmx::ptrtype)kDoNotEnable; }
	
	error = RegisterFunction ( LUA_LuaStateNew,
							  "LuaStateNew" ,
							  LuaStateNew,
							  "Create new LuaState, set as current and return lua_state_id"
							  );
	if ( kSPNoError != error ) { enable = (fmx::ptrtype)kDoNotEnable; }
	
	error = RegisterFunction ( LUA_LuaStateClose,
							  "LuaStateClose( lua_state_id )" ,
							  LuaStateClose,
							  "Close LuaState by lua_state_id"
							  );
	if ( kSPNoError != error ) { enable = (fmx::ptrtype)kDoNotEnable; }
	
	error = RegisterFunction ( LUA_GetLuaStateID,
							  "GetLuaStateID" ,
							  GetLuaStateID,
							  "Return id of active lua state"
							  );
	if ( kSPNoError != error ) { enable = (fmx::ptrtype)kDoNotEnable; }
	
	error = RegisterFunction ( LUA_GetFromState,
							  "GetFromState ( lua_variable_name ; lua_state_id )" ,
							  Get,
							  "Get value of the global lua varibale from the particlular lua state"
							  );
	if ( kSPNoError != error ) { enable = (fmx::ptrtype)kDoNotEnable; }
	
	error = RegisterFunction ( LUA_SetToState,
							  "SetToState ( luacode ; lua_state_id )" ,
							  Evaluate,
							  "Evaluate any lua code in the particlular lua state"
							  );
	if ( kSPNoError != error ) { enable = (fmx::ptrtype)kDoNotEnable; }
	
	
	error = RegisterFunction ( LUA_SetLuaStateActive,
							  "SetLuaStateActive ( lua_state_id )" ,
							  SetLuaStateActive,
							  "Set specified Lua state as active"
							  );
	if ( kSPNoError != error ) { enable = (fmx::ptrtype)kDoNotEnable; }

	
	error = RegisterFunction ( LUA_GetAsContainer,
							"GetAsContainer ( lua_variable_name {; filename; filetype ; pixelsWidth; pixelsHeight } )" ,
							GetAsContainer,
							"Get binary string from Lua and return as file to FM"
							);
	if ( kSPNoError != error ) { enable = (fmx::ptrtype)kDoNotEnable; }

	error = RegisterFunction ( LUA_GetAsContainerFromState,
							"GetAsContainerFromState ( lua_variable_name ; lua_state_id {; filename; filetype ; pixelsWidth; pixelsHeight } )" ,
							GetAsContainer,
							"Get binary string from Lua and return as file to FM from the particlular lua state"
							);
	if ( kSPNoError != error ) { enable = (fmx::ptrtype)kDoNotEnable; }	


	
	
	
	error = RegisterFunction ( LUA_Status,
							  "Status" ,
							  Status,
							  "Return return info about current state of LuaToFM plugin. Use for debug"
							  );
	if ( kSPNoError != error ) { enable = (fmx::ptrtype)kDoNotEnable; }
	
	
	
	error = RegisterFunction ( LUA_Test,
							  "Test ( sometext )" ,
							  Test,
							  "for testing"
							  );
	if ( kSPNoError != error ) { enable = (fmx::ptrtype)kDoNotEnable; }


	return enable;

} // Init


/*
 clean up anything set up or allocated in Init
 plug-in functions are un-registered automatically before this function is called
 */

void Shutdown ( void )
{
	
	LTFM->closeAllState();
	delete LTFM;
		
}


/*
 the main entry point for the plug-in

 calls from fmp go either here or directly to registered plugin function
 see also the options for FMX_ExternCallSwitch in FMXExtern.h

 only edit to add additonal call handlers
 */

void FMX_ENTRYPT FMExternCallProc ( FMX_ExternCallPtr pb )
{
	gFMX_ExternCallPtr = pb;
	
	switch ( pb->whichCall )
	{
		case kFMXT_GetString:
			GetString ( pb );
			break;

		case kFMXT_Init:
			g_next_function = kSPFirstFunction;
			pb->result = Init ( pb );
			break;

		case kFMXT_Shutdown:
			UnregisterFunctions ( );
			Shutdown ( );
			break;

	}

}	// FMExternCallProc


/* ***************************************************************************
 You should not need to edit anything in this section.
 *************************************************************************** */

#pragma mark -
#pragma mark Private Functions
#pragma mark -


// get the plug-in name or options string and hand back to FileMaker

void GetString ( FMX_ExternCallPtr pb )
{
	fmx::TextUniquePtr string;

	switch ( pb->parm1 )
	{
		case kSPOptionsString:
		case kFMXT_OptionsStr:
			string->SetText ( *PluginOptionsString() );
			break;

		case kFMXT_NameStr:
			string->Assign ( PLUGIN_NAME );
			break;
		case kFMXT_AppConfigStr:
			string->Assign ( PLUGIN_DESCRIPTION_STRING_ID );
			break;

//		default:

	}

	string->GetUnicode ( (fmx::unichar16*)(pb->result), 0, fmx::Text::kSize_End );

} // GetString



/*
 register plug-in functions

 RegisterFunction takes three parameters:
 1. the external function signature as it should appear in the calcuation dialogs but
 without the prefix i.e. Explode ( timer ) rather than Bomb_Explode ( timer )
 2.	the plug-in function to call when the function is used in FileMaker
 3. (optional) a description of the function ... default: ""
 */

const fmx::errcode RegisterFunction ( const short functionID, const std::string prototype, const fmx::ExtPluginType function, const std::string description )
{
	fmx::TextUniquePtr underscore;
	underscore->Assign ( "_" );

	fmx::TextUniquePtr function_protoype;
	function_protoype->Assign ( prototype.c_str() );
	function_protoype->InsertText ( *PluginPrefix(), 0 );
	function_protoype->InsertText ( *underscore, kSPPrefixLength );

	fmx::TextUniquePtr function_description;
	function_description->Assign ( description.c_str() );

	fmx::TextUniquePtr name;
	name->SetText ( *FunctionName ( function_protoype ) );

	short required_parameters = 0;
	short optional_rarameters = 0;
	NumberOfParameters ( function_protoype, required_parameters, optional_rarameters );

	const fmx::uint32 function_flags = fmx::ExprEnv::kDisplayInAllDialogs;
	
	const fmx::errcode error = fmx::ExprEnv::RegisterExternalFunctionEx ( *PluginID(),
																		 functionID,
																		 *name,
																		 *function_protoype,
																		 *function_description,
																		 required_parameters,
																		 required_parameters + optional_rarameters,
																		 function_flags,
																		 function
																 );

	if ( error != kSPNoError ) {
		std::cerr << "Error registering: " << prototype << "! Error #: " << error << std::endl;
	}

	// ++g_next_function;

	return error;

} // RegisterFunction



// unregister all registered functions

void UnregisterFunctions ( void )
{
	for ( short i = kSPFirstFunction ; i < g_next_function ; i++ ) {
		fmx::ExprEnv::UnRegisterExternalFunction ( *PluginID(), i );
	}
}


// automaticlly generate the PluginID from the prefix

const fmx::QuadCharUniquePtr PluginID ( void )
{
	/*
	fmx::TextUniquePtr prefix;
	prefix->SetText ( *PluginPrefix() );
	char buffer[kSPPluginIDLength];
	prefix->GetBytes ( buffer, kSPPluginIDLength );
	fmx::QuadCharUniquePtr id ( buffer[0], buffer[1], buffer[2], buffer[3] );
	*/

	std::string plgID = PLUGIN_ID;
	fmx::QuadCharUniquePtr id ( plgID[0], plgID[1], plgID[2], plgID[3] );
	return id;

}


// use the defined prefix if it exists otherwise use the first four characters of the name

const fmx::TextUniquePtr PluginPrefix ( void )
{
	fmx::TextUniquePtr prefix;

#ifdef PLUGIN_PREFIX
	prefix->Assign ( PLUGIN_PREFIX );
#else
	prefix->Assign ( PLUGIN_NAME );
	prefix->DeleteText ( kSPPrefixLength );
#endif

	return prefix;
}


// use the options string defined above otherwise turn everything on

const fmx::TextUniquePtr PluginOptionsString ( void )
{
	fmx::TextUniquePtr optionsString;

#ifdef PLUGIN_OPTIONS_STRING
	optionsString->Assign ( PLUGIN_OPTIONS_STRING );
#else
	optionsString->Assign ( "1YnYYnn" );
#endif

	//optionsString->InsertText ( *PluginPrefix(), 0 );

	fmx::TextUniquePtr plgID;
	plgID->Assign (  PLUGIN_ID );

	optionsString->InsertText ( *plgID, 0 );

	return optionsString;
}


// extract the function name from a function signature/prototype

const fmx::TextUniquePtr FunctionName ( const fmx::TextUniquePtr& signature )
{

	fmx::TextUniquePtr separator;
	separator->Assign ( "(" );

	fmx::uint32 parameters_start = signature->Find ( *separator, 0 );
	if ( parameters_start == fmx::Text::kSize_Invalid ) {
		parameters_start = fmx::Text::kSize_End;
	} else {

		// there may or may not be spaces between the function name and the bracket

		fmx::TextUniquePtr space;
		space->Assign ( " " );

		fmx::uint32 last = parameters_start - 1;
		while ( signature->Find ( *space, last ) == last ) {
			--last;
		}
		parameters_start = last + 1;
	}

	fmx::TextUniquePtr name;
	name->SetText ( *signature, 0, parameters_start );

	return name;

} // FunctionName


// calculate the number of required and optional parameters from a function signature/prototye

void NumberOfParameters ( const fmx::TextUniquePtr& signature, short& required, short& optional )
{
	required = 0;
	optional = 0;

	fmx::TextUniquePtr separator;
	separator->Assign ( "(" );

	const fmx::uint32 parameters_start = signature->Find ( *separator, 0 );
	if ( parameters_start == fmx::Text::kSize_Invalid ) {
		return;
	}

	// we have parameters

	fmx::TextUniquePtr semi_colon;
	semi_colon->Assign ( ";" );

	fmx::TextUniquePtr curly_bracket;
	curly_bracket->Assign ( "{" );

	bool has_optional_parameters = false;
	fmx::uint32 next = parameters_start;

	while ( next != fmx::Text::kSize_Invalid ) {

		++next;
		const fmx::uint32 next_semi_colon = signature->Find ( *semi_colon, next );
		const fmx::uint32 next_curly_bracket = signature->Find ( *curly_bracket, next );

		if ( next_curly_bracket < next_semi_colon && has_optional_parameters == false ) {

			next = signature->Find ( *semi_colon, next_curly_bracket + 1 );
			++required;
			has_optional_parameters = true;

			fmx::TextUniquePtr elipsis;
			elipsis->Assign ( "…" );

			if ( signature->Find ( *elipsis, next_curly_bracket + 1 ) != fmx::Text::kSize_Invalid ) {
				optional = 2000; //-1;
				next = fmx::Text::kSize_Invalid;
			} else {

				fmx::TextUniquePtr faux_elipsis;
				faux_elipsis->Assign ( "..." );

				if ( signature->Find ( *faux_elipsis, next_curly_bracket + 1 ) != fmx::Text::kSize_Invalid ) {
					optional = 2000; //kSPManyParameters;
					next = fmx::Text::kSize_Invalid;
				}
			}


		} else {
			next = next_semi_colon;

			if ( has_optional_parameters == true ) {
				++optional;
			} else {
				++required;
			}
		}

	}

} // NumberOfParameters



