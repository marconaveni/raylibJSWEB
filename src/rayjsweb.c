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

EXTERN EMSCRIPTEN_KEEPALIVE void init()
{
    EM_ASM({
        FS.mkdir('/working');
        FS.mount(MEMFS, {}, '/working');
    });

    //emscripten_run_script("start()");
    //emscripten_set_main_loop(Update, 0, 1);
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
}

EXTERN EMSCRIPTEN_KEEPALIVE void end_drawing()
{
    EndDrawing();
}


#define PLATFORM_WEB 1

int main(void)
{

#if defined(PLATFORM_WEB)

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
