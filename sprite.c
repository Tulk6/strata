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
                                                (Vector2){stacked_sprite->slice_width/2, stacked_sprite->slice_height/2}, rotation, WHITE);
        
    }
}