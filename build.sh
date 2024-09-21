#!/bin/bash


if [ $# -ge 0 ]; then
    arg1=$1;
fi


emcc -v
if [ $? -ne 0 ]; then
    clear;
    echo "emscripten SDK not found install https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)#1-install-emscripten-toolchain";
    exit;
fi

if [ ! -f ./lib/libraylib.a ]; then
    echo "libraylib.a not found! you need compile https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)#2-compile-raylib-library";
    exit;
fi

clear;

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
methods=-sEXPORTED_RUNTIME_METHODS='["cwrap","ccall","getValue","setValue"]';    # for wrapper 
function=-sEXPORTED_FUNCTIONS='["_malloc","_free","_main"]';    # for wrapper

command="emcc  ${out} ${file} ${flags} ${dll} ${include} ${lib} ${glfw} ${async} ${shell}  ${platform} ${filesystem} ${methods} ${function}"; 

echo "Start compiling";

$command;
statuscompile=$?;

clear
if [ $statuscompile == 0 ]; then
    echo "Compilation successfully";
else
    echo "Compilation failure";
    exit;
fi


if [ $arg1 == "release" ]; then

    if [ ! -d ./build ]; then
        mkdir release;
    fi 

    aq="-f ./src/engine.js";
    aq2="-f ./src/engine.wasm";
    if [  $aq -a $aq2 ]; then
        cp ./src/engine.js  ./release;
        cp ./src/engine.wasm  ./release;
        cp ./src/index.html  ./release;
    fi

fi