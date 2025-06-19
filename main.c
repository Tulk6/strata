#include "raylib.h"
#include "wren.c"

#include "sprite.c"
#include "interface.c"

int main(){
    interface_init();
    InitWindow(400, 400, "hight");
    SetTargetFPS(30);

    struct StackedSprite sprite;
    load_stacked_sprite(&sprite, "sprite.png", 16, 16);

    int x = 0;
    int y = 0;

    while (!WindowShouldClose()){
        x += IsKeyDown(KEY_RIGHT)-IsKeyDown(KEY_LEFT);
        y += IsKeyDown(KEY_DOWN)-IsKeyDown(KEY_UP);

        BeginDrawing();
        ClearBackground(BLACK);
        //DrawTexture(sprite.texture, 0, 0, WHITE);
        draw_sprite(&sprite, x, y, x);

        EndDrawing();
    }
    CloseWindow();
}