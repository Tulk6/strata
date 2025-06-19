#include "raylib.h"

struct StackedSprite {
    Image image;
    Texture texture;
    int slice_width;
    int slice_height;
    int num_slices;
};

void load_stacked_sprite(struct StackedSprite* stacked_sprite, char* path, int slice_width, int slice_height){
    stacked_sprite->image = LoadImage(path);
    stacked_sprite->texture = LoadTextureFromImage(stacked_sprite->image);
    stacked_sprite->slice_width = slice_width;
    stacked_sprite->slice_height = slice_height;
    stacked_sprite->num_slices = (int)(stacked_sprite->image.width/slice_width);
}

void draw_sprite(struct StackedSprite* stacked_sprite, int x, int y, int rotation){
    for (int i=0;i<stacked_sprite->num_slices;i++){
        DrawTexturePro(stacked_sprite->texture, (Rectangle){stacked_sprite->slice_width*i, 0,
                                                            stacked_sprite->slice_width, stacked_sprite->slice_height},
                                                (Rectangle){x, y-i, stacked_sprite->slice_width, stacked_sprite->slice_height}, 
                                                (Vector2){0, 0}, rotation, WHITE);
        
    }
}

int main(){
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