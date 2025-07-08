#define RENDER_WIDTH 320
#define RENDER_HEIGHT 240
#define WINDOW_SCALE 2
#define ATLAS_WIDTH 256
#define ATLAS_HEIGHT 256
/*#define ICONS_WIDTH 64
#define ICONS_HEIGHT 64
#define ICON_SIZE 8*/

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "raymath.h"
#include "wren.c"

#include "structs.h"

//wren foreign objects 
#include "sprite.c"

//main functionality (static functions)
#include "palette.c"
#include "input.c"
#include "graphics.c"
#include "interface.c"

enum EngineMode{
    ENGINE_MODE_EDITOR,
    ENGINE_MODE_RUN
};

RenderTexture render_target;

enum EngineMode engine_mode = ENGINE_MODE_EDITOR;

void window_init(){
    InitWindow(RENDER_WIDTH*WINDOW_SCALE, RENDER_HEIGHT*WINDOW_SCALE, "hight");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    render_target = LoadRenderTexture(RENDER_WIDTH, RENDER_HEIGHT);
}

void main_init(){
    window_init();
    graphics_init();
    interface_init();
    //engine_init();
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
    //engine_close();
    printf("edone");
    window_close();
    printf("wdone");
}

int main(){
    main_init();

    char* game = LoadFileText("res/game.wren");
    interface_load_game(game);
    
    
    while (!WindowShouldClose()){
        BeginTextureMode(render_target);

        /*if (IsKeyPressed(KEY_F3)){
            engine_import("res/game.wren");
            //engine_export();1bB2345567
        }
        if (IsKeyPressed(KEY_F4)){
            //engine_import("out.txt");
            engine_export();
        }
        
        if (IsKeyPressed(KEY_F5)){
            engine_run_game();
        }*/
        if (IsKeyPressed(KEY_ESCAPE)){
            interface_load_game(game);
        }

        interface_update();
        interface_draw();

        EndTextureMode();

        BeginDrawing();
        DrawTexturePro(render_target.texture, (Rectangle){0,RENDER_HEIGHT,RENDER_WIDTH,-RENDER_HEIGHT}, 
                                        (Rectangle){0, 0, RENDER_WIDTH*WINDOW_SCALE,RENDER_HEIGHT*WINDOW_SCALE},
                                        (Vector2){0,0}, 0, WHITE);
        //DrawFPS(0,0);
        EndDrawing();

        if (interface_call_load_game){
            interface_call_load_game = false;
            interface_load_game(interface_code);
            if (interface_code != NULL){
                free(interface_code);
            }
        }
    }
    UnloadFileText(game);
    main_close();

}