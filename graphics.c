// sprites -> stacked images
// image -> 2d image
// atlas -> image atlas all images and sprites are on

struct Atlas {
    Image image;
    Texture texture;
};

struct Sprite {
    Rectangle src;
    int slice_width;
    int slice_height;
    int num_slices;
};

struct Atlas global_atlas;

void graphics_load_atlas(char* path){
    global_atlas.image = LoadImage(path);
    global_atlas.texture = LoadTextureFromImage(global_atlas.image);
}

void graphics_load_sprite(struct Sprite* stacked_sprite, Rectangle src, int slice_width, int slice_height){
    stacked_sprite->src = src;
    stacked_sprite->slice_width = slice_width;
    stacked_sprite->slice_height = slice_height;
    stacked_sprite->num_slices = (int)(stacked_sprite->src.width/slice_width);
}

void graphics_draw_sprite(struct Sprite* stacked_sprite, int x, int y, int rotation){
    for (int i=0;i<stacked_sprite->num_slices;i++){
        DrawTexturePro(global_atlas.texture, (Rectangle){stacked_sprite->slice_width*i, 0,
                                                            stacked_sprite->slice_width, stacked_sprite->slice_height},
                                                (Rectangle){x, y-i, stacked_sprite->slice_width, stacked_sprite->slice_height}, 
                                                (Vector2){stacked_sprite->slice_width/2, stacked_sprite->slice_height/2}, rotation, WHITE);
        
    }
}

void graphics_init(){
    graphics_load_atlas("sprite.png");
}