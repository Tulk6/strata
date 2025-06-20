#define RENDER_WIDTH 320
#define RENDER_HEIGHT 240
#define WINDOW_SCALE 2

#include "raylib.h"
#include "wren.c"

//wren foreign objects 
#include "sprite.c"

//main functionality (static functions)
#include "input.c"
#include "graphics.c"
#include "interface.c"

RenderTexture render_target;

void window_init(){
    InitWindow(RENDER_WIDTH*WINDOW_SCALE, RENDER_HEIGHT*WINDOW_SCALE, "hight");
    SetTargetFPS(30);
}

void engine_init(){
    window_init();
    graphics_init();
    interface_init();

    render_target = LoadRenderTexture(RENDER_WIDTH, RENDER_HEIGHT);
}

int main(){
    engine_init();
    
    while (!WindowShouldClose()){
        interface_update();
    
        BeginTextureMode(render_target);
        
        interface_draw();
        
        EndTextureMode();

        BeginDrawing();
        DrawTexturePro(render_target.texture, (Rectangle){0,RENDER_HEIGHT,RENDER_WIDTH,-RENDER_HEIGHT}, 
                                        (Rectangle){0, 0, RENDER_WIDTH*WINDOW_SCALE,RENDER_HEIGHT*WINDOW_SCALE},
                                        (Vector2){0,0}, 0, WHITE);
        EndDrawing();
    }
    CloseWindow();

    /*
    ClearBackground(BLACK);
        //DrawTexture(sprite.texture, 0, 0, WHITE);
        graphics_draw_sprite(&sprite, x, y, x);*/
}