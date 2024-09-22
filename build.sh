#!/bin/bash

arg1="none";
debugmode="false";

# Check if an argument is provided
if [ $# -ge 1 ]; then
    arg1=$1;
fi

# Check if Emscripten is installed
emcc -v
if [ $? -ne 0 ]; then
    clear;
    echo "emscripten SDK not found install https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)#1-install-emscripten-toolchain";
    exit;
fi

# Check if libraylib.a exists
if [ ! -f ./lib/libraylib.a ]; then
    echo "libraylib.a not found! you need compile https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)#2-compile-raylib-library";
    exit;
fi

clear;

# Compilation parameters
out="-o src/engine.js";   # Output file, the .html extension determines the files that need to be generated: `.wasm`, `.js` (glue code) and `.html` (optional: `.data`). All files are already configured to just work.
file="src/rayjsweb.c";    # The input files for compilation, in this case just one but it could be multiple code files: `rayjsweb.c`
flags="-Os -Wall";    # Some config parameters for the compiler, optimize code for small size and show all warnings generated
dll="./lib/libraylib.a";    # This is the libraylib.a generated, it's recommended to provide it directly, with the path to it: i.e. `./lib/libraylib.a`
include="-I include";    # Include path to look for additional #include .h files 
lib="-L lib";    # Library path to look for additional library .a files
shell="--shell-file include/shell.html";    # default emscripten has a `shell.html`
glfw="-sUSE_GLFW=3";    # We tell the linker that the game/library uses GLFW3 library internally, it must be linked automatically (emscripten provides the implementation)
async="-s ASYNCIFY";    # Add this flag ONLY in case we are using ASYNCIFY code
platform="-D PLATFORM_WEB";    # raylib macro compile for web it is necessary 
filesystem="-s FORCE_FILESYSTEM=1";    # Force filesystem creation to load/save files data
methods=-sEXPORTED_RUNTIME_METHODS='["cwrap","ccall","getValue","setValue","FS"]';    # for wrapper 
function=-sEXPORTED_FUNCTIONS='["_malloc","_free","_main"]';    # for wrapper

# Verify debug mode is active
if [ "$arg1" = "debug" ]; then
    debug_mode="true";
    flags="-O0 -g4 -Wall";  
    echo "Debug mode ON: Flags set to $flags";
fi


# Compile command
command="emcc  ${out} ${file} ${flags} ${dll} ${include} ${lib} ${glfw} ${async} ${shell}  ${platform} ${filesystem} ${methods} ${function} -s MODULARIZE=1 -s EXPORT_ES6=1"; 

echo "Start compiling";

$command;
statuscompile=$?;

if [ $statuscompile -eq 0 ]; then
    clear
    echo "Compilation successfully";
else
    echo "Compilation failure";
    exit;
fi

# Build process
if [ "$arg1" = "build" ]; then

    if [ ! -d ./build ]; then
        mkdir build;
    fi 
    
    aq0="./src/engine.js";
    aq1="./src/engine.wasm";
    if [ -f "$aq0" ] && [ -f "$aq1" ]; then
        cp "$aq0" ./build
        cp "$aq1" ./build
        cp "./src/core.js" ./build
        cp "./index.html" ./build
        echo "Build process completed successfully.";
    else
        echo "One or more files do not exist."
    fi

fi