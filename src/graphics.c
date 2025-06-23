// sprites -> stacked images
// image -> 2d image
// atlas -> image atlas all images and sprites are on

struct Atlas {
    Image image;
    Texture texture;
};

struct Atlas global_atlas;

void graphics_load_atlas(char* path){
    global_atlas.image = LoadImage(path);
    global_atlas.texture = LoadTextureFromImage(global_atlas.image);
}

void graphics_draw_sprite(struct Sprite* stacked_sprite, int x, int y, int rotation){
    for (int i=0;i<stacked_sprite->num_slices;i++){
        DrawTexturePro(global_atlas.texture, (Rectangle){stacked_sprite->slice_width*i, 0,
                                                            stacked_sprite->slice_width, stacked_sprite->slice_height},
                                                (Rectangle){x, y-i, stacked_sprite->slice_width, stacked_sprite->slice_height}, 
                                                (Vector2){stacked_sprite->slice_width/2, stacked_sprite->slice_height/2}, rotation, WHITE);
        
    }
}

void graphics_foreign_draw_sprite(WrenVM* vm){
    struct Sprite* sprite = (struct Sprite*)wrenGetSlotForeign(vm, 1);
    int x = wrenGetSlotDouble(vm, 2);
    int y = wrenGetSlotDouble(vm, 3);
    int rotation = wrenGetSlotDouble(vm, 4);
    graphics_draw_sprite(sprite, x, y, rotation);
}

void graphics_clear_screen(int colour){
    Color col = palette_get_colour(colour);
    ClearBackground(col);
}

void graphics_foreign_clear_screen(WrenVM* vm){
    int colour = wrenGetSlotDouble(vm, 1);
    graphics_clear_screen(colour);
}

void graphics_init(){
    graphics_load_atlas("sprite.png");
    palette_load_default();
}