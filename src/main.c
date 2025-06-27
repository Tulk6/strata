#define RENDER_WIDTH 320
#define RENDER_HEIGHT 240
#define WINDOW_SCALE 2

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "raymath.h"
#include "wren.c"
//wren foreign objects 
#include "sprite.c"

//main functionality (static functions)
#include "palette.c"
#include "input.c"
#include "graphics.c"
#include "interface.c"
#include "writer.c"
#include "engine.c"

RenderTexture render_target;

void window_init(){
    InitWindow(RENDER_WIDTH*WINDOW_SCALE, RENDER_HEIGHT*WINDOW_SCALE, "hight");
    SetTargetFPS(30);
    SetExitKey(KEY_NULL);
    render_target = LoadRenderTexture(RENDER_WIDTH, RENDER_HEIGHT);
}

void main_init(){
    window_init();
    graphics_init();
    interface_init();
    engine_init();
}

void window_close(){
    UnloadRenderTexture(render_target);
    CloseWindow();
}

void main_close(){
    graphics_close();
    printf("gdone");
    interface_close();
    printf("idone");
    engine_close();
    printf("edone");
    window_close();
    printf("wdone");
}

int main(){
    main_init();
    
    while (!WindowShouldClose()){
        BeginTextureMode(render_target);

        if (IsKeyPressed(KEY_F3)){
            engine_import("res/game.wren");
            //engine_export();1bB2345567
        }
        if (IsKeyPressed(KEY_F4)){
            //engine_import("out.txt");
            engine_export();
        }
        
        if (IsKeyPressed(KEY_F5)){
            engine_run_game();
        }
        if (IsKeyPressed(KEY_ESCAPE)){
            global_engine_mode = ENGINE_MODE_CODE;
        }
        if (global_engine_mode == ENGINE_MODE_RUN){
            interface_update();
            interface_draw();
        }else{
            engine_update();
            engine_draw();
        }

        EndTextureMode();

        BeginDrawing();
        DrawTexturePro(render_target.texture, (Rectangle){0,RENDER_HEIGHT,RENDER_WIDTH,-RENDER_HEIGHT}, 
                                        (Rectangle){0, 0, RENDER_WIDTH*WINDOW_SCALE,RENDER_HEIGHT*WINDOW_SCALE},
                                        (Vector2){0,0}, 0, WHITE);
        EndDrawing();
    }
    main_close();
}