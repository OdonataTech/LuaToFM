# LuaToFM
#### Enhance Your FileMaker Experience with Lua Integration

LuaToFM is a free powerful plugin that seamlessly integrates Lua scripting into your FileMaker environment. With LuaToFM, you can unlock new possibilities for your FileMaker applications, enhancing functionality and performance.

Execute Lua Code in FileMaker
```
lu_Evaluate ( "
   function my_sum( a, ... ) 
      local sum = a
      for i, x in ipairs{...} do
        sum = sum + x
      end     
      return sum
   end 
" ) & ¶ & 
lu_RegisterLuaFunction( "my_sum( a { ; ... } )" ; "my sum lua function" )
```





