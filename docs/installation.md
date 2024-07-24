# Install LuaToFM plugin

Install plugin in a standard way. On some modern operating system, you can have an error on the launch plugin. Use this recipe:

## On the macOS:

Open terminal and type command like this (type correct path to DataBox.fmplugin file on your system)

Example:
``` 
sudo xattr -rd com.apple.quarantine /Users/YourUserName/Library/Application\ Support/FileMaker/FileMaker\ Pro/19.0/Extensions/LuaToFM.fmplugin
```

## On the Windows:

- Click the right button and open file properties
- Found the certificate and import into The certificates to Trusted Root Certification folder


