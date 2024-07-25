
CC= gcc -std=gnu99
CFLAGS= -fPIC -O2 -Wall -Wextra $(SYSCFLAGS) $(MYCFLAGS)

CXX = g++ -std=c++11
CXXFLAGS = -fPIC -I ./headers -I ./libb64  -I ./cjson -I ./lua -I ./headers/FMWrapper



ifdef OS
#windows
VPATH = lua source libb64 cjson headers Libraries/Win/x64

MYCFLAGS=-DLUA_BUILD_AS_DLL

TARGET = LuaToFM.fmx64

FMWRAP = ./Libraries/Win/x64/FMWrapper.lib

RESOURCE = version.txt
-DRESOURCE_FILE=\"$(RESOURCE)\"

else
#macos
VPATH = lua source libb64 cjson headers Libraries/Mac/FMWrapper.framework/Versions/A/FMWrapper

MYCFLAGS=-DLUA_USE_MACOSX

FMWRAP = ./Libraries/Mac/FMWrapper.framework/Versions/A/FMWrapper

TARGET = LuaToFM
endif


all: $(TARGET)

SOURCE_OBJ = base64.o fm_funcs.o lua_funcs.o luaToFM.o utilites.o init.o

LUA_OBJ =  lapi.o lcode.o lctype.o ldebug.o ldo.o ldump.o lfunc.o lgc.o llex.o lmem.o lobject.o lopcodes.o lparser.o lstate.o lstring.o ltable.o ltm.o lundump.o lvm.o lzio.o lauxlib.o lbaselib.o lcorolib.o ldblib.o liolib.o lmathlib.o loadlib.o loslib.o lstrlib.o ltablib.o lutf8lib.o linit.o

B64_OBJ = cdecode.o cencode.o

CJSON_OBJ = lua_cjson.o fpconv.o strbuf.o

$(TARGET): $(SOURCE_OBJ) $(LUA_OBJ) $(B64_OBJ) $(CJSON_OBJ)
	$(CXX) -shared -o $(TARGET) $(SOURCE_OBJ) $(LUA_OBJ) $(B64_OBJ) $(CJSON_OBJ) $(FMWRAP)
ifdef OS
	strip --strip-unneeded $(TARGET)
endif

.PHONY: clean
clean:
	rm -rf *.o



# DO NOT DELETE
#

lua_cjson.o: lua_cjson.c
	$(CC) -c $< -o $@


#base64.o: base64.cpp base64.h cencode.h cdecode.h
#	gcc -c $(INCFLAGS) $< -o $@
#fm_funcs.o: fm_funcs.cpp
#	g++ $(FMFLAGS) $(INCFLAGS) -c $< -o $@
#init.o: init.cpp
#	g++ $(FMFLAGS) $(INCFLAGS) -c $< -o $@
#lua_funcs.o: lua_funcs.cpp
#	g++ $(FMFLAGS) $(INCFLAGS) -c $< -o $@
#luaToFM.o: luaToFM.cpp
#	g++ $(FMFLAGS) $(INCFLAGS) -c $< -o $@
#utilites.o: utilites.cpp
#	g++ $(FMFLAGS) $(INCFLAGS) -c $< -o $@

#
#lapi.o: lapi.c
#lauxlib.o: lauxlib.c
#lbaselib.o: lbaselib.c
#lcode.o: lcode.c
#lcorolib.o: lcorolib.c
#lctype.o: lctype.c
#ldblib.o: ldblib.c
#ldebug.o: ldebug.c
#ldo.o: ldo.c
#ldump.o: ldump.c
#lfunc.o: lfunc.c
#lgc.o: lgc.c
#linit.o: linit.c
#liolib.o: liolib.c
#llex.o: llex.c
#lmathlib.o: lmathlib.c
#lmem.o: lmem.c
#loadlib.o: loadlib.c
#lobject.o: lobject.c
#lopcodes.o: lopcodes.c
#loslib.o: loslib.c
#lstate.o: lstate.c
#lstring.o: lstring.c
#lstrlib.o: lstrlib.c
#ltable.o: ltable.c
#ltablib.o: ltablib.c
#ltm.o: ltm.c
#lundump.o: lundump.c
#lutf8lib.o: lutf8lib.c
#lzio.o: lzio.c

# (end of Makefile)


