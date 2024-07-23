
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
#### Format
#### Parameters
#### Result
#### Examples

## lu_Restart
#### Format
#### Parameters
#### Result
#### Examples

## lu_LuaStateNew
#### Format
#### Parameters
#### Result
#### Examples

## lu_LuaStateClose
#### Format
#### Parameters
#### Result
#### Examples

## lu_GetLuaStateID
#### Format
#### Parameters
#### Result
#### Examples

## lu_SetLuaStateActive
#### Format
#### Parameters
#### Result
#### Examples

## lu_GetFromState
#### Format
#### Parameters
#### Result
#### Examples

## lu_SetToState
#### Format
#### Parameters
#### Result
#### Examples

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





  



