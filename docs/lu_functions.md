
## lu_Evaluate ( luacode )
Evaluate any valid text as lua code

```
lu_Evaluate ( " 
    a=2
" ) 
```

Returns:
- OK on succsses
- two string, whrer first string is "ERROR" and second string is error description

```
lu_Evaluate ( " smth wrong" )

returns:
ERROR
[string " smth wrong"]:1: syntax error near 'wrong'

```

## lu_Get ( lua_variable_name )
Get global lua variable. Also can be used for geting lua function result or simle lua sentence

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
The type of results:
- Returns 'nil' if returned lua variable is not exist
- Returns variable as FileMaker number if returned lua variable is number
- Returns variable as Fileamker text if returned lua variable is string
- Returns variable as JSON string if returned lua variable is lua table
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

  



