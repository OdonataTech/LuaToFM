# LuaToFM
#### Enhance Your FileMaker Experience with Lua Integration

LuaToFM is a free powerful plugin that seamlessly integrates Lua scripting into your FileMaker environment. With LuaToFM, you can unlock new possibilities for your FileMaker applications, enhancing functionality and performance.

### Execute Lua Code in FileMaker
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
### Persistent Lua Environment

With LuaToFM, you can store data in the Lua environment's memory independently of FileMaker files. This feature ensures that your data remains accessible across different sessions and FileMaker files, providing a robust solution for data management.

### Cross-File Access

Access your Lua variables from multiple FileMaker files, both in FileMaker Pro and on the server. This cross-file functionality allows for seamless data sharing and enhances the collaborative capabilities of your applications.

### Multiple Lua States

LuaToFM supports working with multiple Lua states (environments) simultaneously. This feature ensures safe operation for multiple FileMaker applications or processes running on the same server, preventing conflicts and enhancing stability.

### JSON Data Return

Easily return Lua table data to FileMaker in JSON format. This capability simplifies data handling and integration, making it easier to work with complex data structures within your FileMaker solutions.

### Special Lua Functions for FileMaker

The plugin includes a dedicated package of Lua functions designed specifically for FileMaker. With these functions, you can:
- Perform direct SQL queries to the FileMaker database without relying on ODBC or APIs.
- Execute FileMaker scripts directly from Lua.
- Store data in FileMaker global variables.
- Call FileMaker functions from Lua code, streamlining your workflow.

### Unleash the Power of External Lua Packages

```
lu_Evaluate ( " 
  http = require('socket.http') 
  h = http.request('https://odonata.tech')
" )
```

One of the most exciting aspects of LuaToFM is its ability to leverage the vast ecosystem of existing Lua packages. Lua has a rich history as an embedded language, leading to the development of numerous high-quality, open-source packages that address a wide range of needs. With LuaToFM, you can now seamlessly integrate these packages into your FileMaker solutions, unlocking new possibilities for functionality and efficiency.

By integrating these and other Lua packages with LuaToFM, developers gain access to a vast array of functionality that can be seamlessly incorporated into their FileMaker solutions. This opens up new possibilities for data processing, automation, integration, and more, ultimately enhancing the capabilities and efficiency of FileMaker applications.

#### Please read [documentations](https://github.com/OdonataTech/LuaToFM/tree/main/docs) for details information






