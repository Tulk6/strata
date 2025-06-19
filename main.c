#define RENDER_WIDTH 320
#define RENDER_HEIGHT 240
#define WINDOW_SCALE 2

#include "raylib.h"
#include "wren.c"

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
}

int main(){
    engine_init();

    struct Sprite sprite;
    graphics_load_sprite(&sprite, (Rectangle){0, 0, 160, 16}, 16, 16);

    int x = 0;
    int y = 0;

    render_target = LoadRenderTexture(RENDER_WIDTH, RENDER_HEIGHT);

    while (!WindowShouldClose()){
        x += IsKeyDown(KEY_RIGHT)-IsKeyDown(KEY_LEFT);
        y += IsKeyDown(KEY_DOWN)-IsKeyDown(KEY_UP);
        
        //BeginDrawing();
        BeginTextureMode(render_target);

        ClearBackground(BLACK);
        //DrawTexture(sprite.texture, 0, 0, WHITE);
        graphics_draw_sprite(&sprite, x, y, x);

        EndTextureMode();

        BeginDrawing();
        DrawTexturePro(render_target.texture, (Rectangle){0,RENDER_HEIGHT,RENDER_WIDTH,-RENDER_HEIGHT}, 
                                        (Rectangle){0, 0, RENDER_WIDTH*WINDOW_SCALE,RENDER_HEIGHT*WINDOW_SCALE},
                                        (Vector2){0,0}, 0, WHITE);
        EndDrawing();
        //EndDrawing();
    }
    CloseWindow();
}