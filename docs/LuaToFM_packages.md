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



