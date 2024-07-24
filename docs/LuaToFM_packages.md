# Inline LuaToFM Packages

LuaToFM already has some Lua packages. These packages are loaded when LuaToFM starts and when you create a new Lua state. You don't need to load them and use the require function to activate their functions.

## Work with JSON

### JSON.encode( table )
Converts Lua table to JSON string
```
lu_Evaluate ( "
    tbl = { a = 1 ; b = {1,2,3} }  
    str = JSON.encode(tbl)
 ") & ¶ & 
lu_Get ( "str" ) 
// {"b":[1,2,3],"a":1}
```
Here the result is the same as if we wrote just lu_Get("tbl"), but we may need to convert the table to JSON inside the lua code, for example when sending an http request

### JSON.decode( json )
Converts JSON string to Lua table
```
lu_Evaluate ( "
    str = ' {\"b\":[1,2,3],\"a\":1 } '
    tbl2 = JSON.decode(str)
 ") & ¶ & 
lu_Get("tbl2.b[2]")
// returns 2
```

## Work with BASE64

### BASE64.encode( text )
Converts data to the BASE64 string
```
lu_Evaluate ( "
    str = 'FileMaker!'
    b64 = BASE64.encode(str)
 ") & ¶ & 
lu_Get("b64")
```

### BASE64.decode( b64text )
Converts BASE64 string to the data
```
lu_Evaluate ( "
    str = BASE64.decode('RmlsZU1ha2VyIQ==')
 ") & ¶ & 
lu_Get("str")
```

## LuaToFM package to interact with FileMaker from Lua code

### LuaToFM.setFMvariable( var_name , var_value)
Stores a value in the FileMaker global variable like $$

```
lu_Evaluate ( "
    LuaToFM.setFMvariable( 'luaVar' , package.path )
 ")

//in FileMaker a variable $$luaVar will appear with the value package.path
```

### fmEvaluate( fm_expression )
Actually calls FileMaker's Evaluate function and returns its calculated value to a lua variable
```
lu_Evaluate ( "
   str = LuaToFM.fmEvaluate( 'Get(TemporaryPath)' )
" ) 
```

## fmPerfomScript( fm_file_name, script_name, script_parameter, script_control )
Call FileMaker script from Lua code

Returns 0 on success. The result of script is not returned.

script_control can be seated to the 0,1,2,3 (Halt, Exit, Resume, Pause )
```
lu_Evaluate ( " 
   run  = LuaToFM.fmPerfomScript( 'LTest' , 'test_from_lua', '1' ,  3  ) 
" ) 
```

## ExecuteFMSQL ( SQL_parameters_table )
Calls a SQL query on a FileMaker file from lua code

For a SELECT query, it returns two values ​​- the number of records and the generated Lua table as the query result
```
cnt, res = LuaToFM.executeFMSQL ( {
      file = 'Ltest',
      query = 'SELECT id, test_text FROM LTest WHERE id > 26 '
})
```

Required parameters:
- file - FM file name
- query - query text
    
Extra parameters:
- params - table of transmitted parameters, fills in the ? signs in the request
- types - a string of characters corresponding to the number of fields returned from the query, defining the type of the returned values. Two values ​​are supported: d and s. By default, the type is always s
- keys type - defines the indexing type of the returned table, can take two values:
  - numeric - by default, the resulting table has a numeric index
  - names - the resulting table is indexed by the field name. The fields_names parameter must be specified
- fields_names - a table of field names used for indexing by name.
- table_type - defines the structure of the returned table
  - rows - by default. The returned table is an array of rows
  - columns  - Возвращаемая таблица - массив колонок
 
Examples

```
lu_Evaluate ( "
   cnt, res = LuaToFM.executeFMSQL ( {
      file = 'Ltest',
      query = 'SELECT id, test_text FROM LTest WHERE id > ? AND id < ? ',
      params = { 39,  42 }
})
" ) &  ¶ & 
lu_Get ( "cnt" )  & ¶ & 
JSONFormatElements ( lu_Get ( "res" ) )

returns
OK
2
[
	[ "40", "Test 1" ],
	[ "41", "Test 2" ]
]
```

```
lu_Evaluate ( "
   cnt, res = LuaToFM.executeFMSQL ( {
      file = 'Ltest',
      query = 'SELECT id, test_text, timestamp_create FROM LTest WHERE id > ? ',
      params = {20},
      types = 'dsd',
      keys_type = 'names' ,
      fields_names = { 'id', 'test_text', 'timestamp_create'},
      table_type = 'columns'
})
" ) &  ¶ & 
JSONFormatElements ( lu_Get ( "res" ) )

returns
OK
{
	"id" : [ 39, 40, 41, 42, 43 ],
	"test_text" : 
	[
		"Some text",
		"Test 1",
		"Test 2",
		"Test 3",
		""
	],
	"timestamp_create" : [ 63853813298, 63853813305, 63853904090, 63853904094, 63853904429 ]
}
```

LuaToFM.executeFMSQL can be used not only for SELECT but also for other types of queries (INSERT, UPDATE)

```
lu_Evaluate ( "
   err = LuaToFM.executeFMSQL ( {
      file = 'Ltest',
      query = 'INSERT INTO LTest (test_text, test_num) VALUES ( ? , ? ) ',
      params = {'Тест FM', 42},
})
" )

err = 0 if FileMaker completed the request successfully
```

```
lu_Evaluate ( "
   err = LuaToFM.executeFMSQL ( {
      file = 'Ltest',
      query = 'UPDATE LTest SET test_text = ? , test_num = ? WHERE id = 38 ',
      params = {'I changed it', 29},
})
" )
```

