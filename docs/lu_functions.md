
## lu_Evaluate
Evaluate any valid text as Lua code
#### Format
lu_Evaluate ( luacode )
#### Parameters
luacode - any valid Lua code
#### Result
- OK on succsses
- two string, where first string is "ERROR" and second string is error description
#### Examples
```
lu_Evaluate ( " 
    a = 2
    b = 3
    c = a + b 
" )

returns:
OK
```
```
lu_Evaluate ( " smth wrong" )

returns:
ERROR
[string " smth wrong"]:1: syntax error near 'wrong'

```

## lu_Get
Get global lua variable. Also can be used for geting Lua function result or simle Lua expression
#### Format
lu_Get ( lua_variable_name )
#### Parameters
lua_variable_name - the name of any Lua global variable, function or Lua expression
### Examples
```
lu_Get ( "a" ) 
returns value of variable 'a' if 'a' a is exist or 'nil'
```
```
lu_Get ( "2+2" ) 
// returns 4
```
```
lu_Get ( "a*2" ) 
// returns ERROR if varibale 'a' is not exist or 'a' is not number 
// returns  a*2 if 'a' is number
```
```
lu_Get ( "string.upper ( 'Hello FileMaker' )" ) 
// returns 'HELLO FILEMAKER'
```
#### The type of results:
- Returns 'nil' if returned lua variable is not exist
- Returns variable as FileMaker number if returned Lua variable is number
- Returns variable as Fileamker text if returned Lua variable is string
- Returns variable as JSON string if returned Lua variable is lua table
- Returns variable as Fileamker text for all other valid case

``` 
lu_Evaluate ( "
  tbl = {
    a = 1,
    b = 'smth text',
    c = { 6, 3, 5 , 1 },
    d = { 
        e = { 'aa', 'bb', 'cc' },
        f =  { 1, 'dddd' , 3 }
        }
   }
"
 ) & ¶ & 
 lu_Get ( "tbl" )

 returns
 {"c":[1,2,3,4],"b":"smth text","a":1,"d":{"e":["aa","bb","cc"],"f":[1,"dddd",3]}}
```
You can get any element of lua table:

```
lu_Get ( "tbl.a" )
returns 1

lu_Get ( "tbl.c[3]" )
returns 5

lu_Get ( "tbl.d.f" )
reurns json string [1,"dddd",3]
```

## lu_DoFile
Load and execute .lua file
#### Format
lu_DoFile( filepath { ; convert_path_flag } )
#### Parameters
filepath - full path to .lua file
convert_path_flag - set flag to convert path from Filemaker format to format of operating system (work only FileMaker 19.0 and above). If flag don't seated the function will be wait file path in current OS format
#### Result
- OK and filepath in OS format on succsses
- two string, where first string is "ERROR" and second string is error description
#### Examples
```
lu_DoFile( Get(DocumentsPath) & "test.lua"  ; 1 )
returns
OK. /Users/arcady/Documents/test.lua
```
```
lu_DoFile( "/Users/arcady/Documents/test.lua" )
returns
OK. /Users/arcady/Documents/test.lua
```
```
lu_DoFile( "/Users/arcady/Documents/testTTT.lua" )
returns
ERROR
cannot open /Users/arcady/Documents/testTTT.lua: No such file or directory
```

## lu_RegisterLuaFunction
Registrates Lua function as FileMaker function. The function does not have to exist in memory at the moment and can be declared later and changed at any time.
#### Format
lu_RegisterLuaFunction( function_prototype ; function_description )
#### Parameters
function_prototype - the prototype of the function including variable names. Optional variables may be specified. The function may also be specified as containing an unspecified number of variables, but not less than one.
function_description - text description of the function. Will be displayed in Description field of 'Edit Expression'' Filemaker window
#### Result
func_name registred, where func_name is name of registrated function, or error
#### Examples
```
lu_RegisterLuaFunction( "my_func( a ; b )" ; "my first register lua function" )
returns
my_func registred.
```
```
lu_Evaluate ( "
   function my_sum( a, ... ) 
      local sum = a
      for i, x in ipairs{...} do
        sum = sum + x
      end     
      return sum
   end 
" )
lu_RegisterLuaFunction( "my_sum( a { ; ... } )" ; "my sum lua function" )

```

## lu_GetAsContainer
Gets binary data from Lua variable and stores in FileMaker container as file
#### Format
lu_GetAsContainer ( lua_variable_name {; filename; filetype ; pixelsWidth; pixelsHeight } )
#### Parameters
lua_variable_name - the global Lua variable, should contain binary representation of file
filename - the name of file
filetype - type of file. Supported 'png', 'jpg', 'eps', 'gif', 'bmp', 'pdf'. For all other case leave this parameters empty.
pixelsWidth - with in pixels of image 
pixelsHeight - height in pixels of image 
#### Result
File to store in FileMaker container
#### Examples
```
lu_GetAsContainer ( "png_qr_data" ; "qrcodeTEST.png"; "png" )

Here png_qr_data is Lua variable with binary data of png file
```

## lu_Restart
Complete restart Lua environment. All Lua states and data will be delete, the memory will be cleared
#### Format
lu_Restart
#### Parameters
No any parameters
#### Result
Lua restarted. All state and variables was clear.
#### Examples
```
lu_Restart
```

## lu_LuaStateNew
Creates and make as active for current user new Lua state. Notes, the previous states don't deleted and stay in memory
#### Format
lu_LuaStateNew
#### Parameters
No any parameters
#### Result
The ID of created state. It is recommended to save this ID for future use.
#### Examples
```
lu_LuaStateNew
returns
0x382e36c08
```
## lu_LuaStateClose
Close (delete) Lua state specified by ID
#### Format
lu_LuaStateClose( lua_state_id )
#### Parameters
lua_state_id - valid ID of Lua state
#### Result
on success returns three rows
OK
lua_state_id_closed of closed
lua_state_id_current current state

on error
ERROR
lua_state_id not found
#### Examples
```
lu_LuaStateClose( "0x382e36c08" )
returns
OK
0x382e36c08 closed
0x355d69408 current state
```

## lu_GetLuaStateID
Gets ID of the current Lua state
#### Format
lu_GetLuaStateID
#### Parameters
No any parameters
#### Result
ID of current Lua state
#### Examples
```
lu_GetLuaStateID
rerurns
0x355d69408
```

## lu_SetLuaStateActive
Sets Lua state specified in lua_state_id as current 
#### Format
lu_SetLuaStateActive ( lua_state_id )
#### Parameters
lua_state_id - ID of Lua state. Lua state with specified ID must be exist
#### Result
on success
OK
 Lua state address 0x355d69408 is active

on error
ERROR
 Lua state address 111 is incorrect
#### Examples
```
lu_SetLuaStateActive ( "0x355d69408" )
returns
OK
 Lua state address 0x355d69408 is active
```

## lu_GetFromState
The full analog from lu_Get, but gets value from any Lua state, which specified by ID
#### Format
lu_GetFromState ( lua_variable_name ; lua_state_id )
#### Parameters
lua_variable_name - the name of any Lua global variable, function or Lua expression
lua_state_id - ID of one from existed Lua states
#### Result
see lu_Get description
#### Examples
```
lu_GetFromState ( "a" ; "0x355d69408" ) 
returns value of variable 'a' if 'a' a is exist or 'nil' from Lua state with ID 0x355d69408
```

## lu_SetToState
The full anaog from lu_Evaluate but run Lua code in Lua state, which specified by ID
#### Format
lu_SetToState ( luacode ; lua_state_id )
#### Parameters
luacode - any valid Lua code
lua_state_id - ID of one from existed Lua states
#### Result
- OK on succsses
- two string, where first string is "ERROR" and second string is error description
#### Examples
```
lu_SetToState ( " 
    a = 2
    b = 3
    c = a + b 
" ;
"0x355d69408"
)
& ¶ & 
lu_GetFromState ( "c" ; "0x355d69408" )

returns
OK
5
```

## lu_GetAsContainerFromState
#### Format
#### Parameters
#### Result
#### Examples

## lu_Status
#### Format
#### Parameters
#### Result
#### Examples





  



