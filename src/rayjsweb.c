#include <emscripten/emscripten.h>

#if defined(PLATFORM_WEB)
#endif

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

#include "raylib.h"
#include "utils.h"
#include <stdio.h>

void Update(void); // Update and Draw one frame

//static variables
Texture texture;

EXTERN EMSCRIPTEN_KEEPALIVE Texture* load_texture(const char* name)
{
    const char* finalPath = TextFormat("working/%s",name);
    EM_ASM({ 
        console.log(UTF8ToString($0));
    },name);

    texture = LoadTexture(finalPath);

    printf("textura carregada com sucesso\n");
    return &texture;
}

EXTERN EMSCRIPTEN_KEEPALIVE void load_asset_memory(const int* data, int dataLength, const char* name)
{
    EM_ASM({ 
            
        FS.writeFile('working/' + UTF8ToString($2) , HEAPU8.subarray($0, $0 + $1)); // Escreva o arquivo no sistema de arquivos em memória
        console.log(UTF8ToString($2));

    },data, dataLength, name);
} 

EXTERN EMSCRIPTEN_KEEPALIVE void init_window(int width, int height, const char* title)
{
    InitWindow(width, height, title);
}

EXTERN EMSCRIPTEN_KEEPALIVE void begin_drawing()
{
    BeginDrawing();
}

EXTERN EMSCRIPTEN_KEEPALIVE void clear_background()
{
    ClearBackground(RAYWHITE);
}

EXTERN EMSCRIPTEN_KEEPALIVE void draw_rectangle(int posX, int posY, int width, int height)
{
    DrawRectangle(posX, posY, width, height, RED);
}

EXTERN EMSCRIPTEN_KEEPALIVE void draw_texture(Texture2D* texturePtr, int posX, int posY)
{
    DrawTexture(*texturePtr,  posX,  posY, WHITE); 

    // EM_ASM({ 
    //     console.log('cpp  ptr ' + UTF8ToString($0) + '  internal ' + UTF8ToString($1));
    // }, x , texture.format );
}

EXTERN EMSCRIPTEN_KEEPALIVE void end_drawing()
{
    EndDrawing();
}


#define PLATFORM_WEB 1

int main(void)
{

#if defined(PLATFORM_WEB)



    EM_ASM({
        FS.mkdir('/working');
        FS.mount(MEMFS, {}, '/working');
        load_asset_memory = Module.cwrap('load_asset_memory', null, ['number', 'number', 'string']);
        load_texture = Module.cwrap('load_texture', 'number', ['string']);
        init_window = Module.cwrap('init_window', null, ['number', 'number', 'string']);
        begin_drawing = Module.cwrap('begin_drawing', null, null);
        clear_background = Module.cwrap('clear_background', null, null);
        draw_rectangle = Module.cwrap('draw_rectangle', null, ['number', 'number', 'number', 'number']);
        draw_texture = Module.cwrap('draw_texture', null, ['number', 'number', 'number']);
        end_drawing = Module.cwrap('end_drawing', null, null);
    });

    emscripten_run_script("start()");


    emscripten_set_main_loop(Update, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        Update();
    }
#endif
    return 0;
}

void Update(void)
{
    // todo
}

/*
emcc -o game.html hello.cpp -Os -Wall ./lib/libraylib.a -I. -Iinclude -L. -Llib -s USE_GLFW=3 -s ASYNCIFY --shell-file C:/Programas/emsdk/upstream/emscripten/src/shell.html -DPLATFORM_WEB --preload-file ./res -sFORCE_FILESYSTEM=1  -sEXPORTED_RUNTIME_METHODS='["cwrap","ccall","getValue","setValue"]' -sEXPORTED_FUNCTIONS='["_malloc","_free","_main"]'   




-sEXPORTED_FUNCTIONS='["_int_sqrt","_init_window","_begin_drawing","_clear_background","_draw_rectangle","_end_drawing"]'  -- isso desabilita main() e marca as funções como mortas




    //const char *teste = "teste";
    EM_ASM({
        FS.mkdir('/res');
        FS.mount(MEMFS, {}, '/res');
        // data_callback(0, 0);

        fetch('/snes.png') // Certifique-se de que o caminho para o arquivo está correto
            .then(function(response) { return response.arrayBuffer(); })
            .then(function(data) {
                var fileData = new Uint8Array(data);

                // Escreva o arquivo no sistema de arquivos em memória
                FS.writeFile('/res/snes.png', fileData);

                console.log('Arquivo carregado e salvo em /res/snes.png');
                console.log(UTF8ToString($0));

                // load_data_wrapped(10, 20);
            })
            .catch(function(error) {
                console.error('Erro ao carregar o arquivo:', error);
            }); 
    }); 




 EM_JS(void, call_alert, (), {
  alert('hello world!');
  throw 'all done';
}); 


*/