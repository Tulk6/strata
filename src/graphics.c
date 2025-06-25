// sprites -> stacked images
// image -> 2d image
// atlas -> image atlas all images and sprites are on

struct Atlas {
    Image image;
    Texture texture;
};

struct Atlas global_atlas;

Font global_font;

Color global_draw_colour;

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

void graphics_draw_text(char* text, int x, int y){
    DrawTextEx(global_font, text, (Vector2){x, y}, 9, 1, global_draw_colour);
}

void graphics_foreign_draw_text(WrenVM* vm){
    char* text = wrenGetSlotString(vm, 1);
    int x = wrenGetSlotDouble(vm, 2);
    int y = wrenGetSlotDouble(vm, 3);
    //int c = wrenGetSlotDouble(vm, 2);
    graphics_draw_text(text, x, y);
}

void graphics_clear_screen(){
    ClearBackground(global_draw_colour);
}

void graphics_foreign_clear_screen(WrenVM* vm){
    graphics_clear_screen();
}

void graphics_set_draw_colour(int index){
    global_draw_colour = palette_get_colour(index);
}

void graphics_foreign_set_draw_colour(WrenVM* vm){
    int colour_index = wrenGetSlotDouble(vm, 1);
    graphics_set_draw_colour(colour_index);
}

int get_next_char(Image* img, int x){
    Color key_colour = (Color){255, 0, 0, 255};
    int j = x;
    while (j<img->width){
        j += 1;
        //printf("j: %i\n", j);
        if (ColorIsEqual(GetImageColor(*img, j, 0), key_colour)){
            x = j+1;
            return x;
        }
    }
    return x;
}

void graphics_load_font_image(char* path, char* characters){
    Image img = LoadImage(path);

    global_font.baseSize = img.height;
    global_font.glyphCount = strlen(characters);
    global_font.glyphPadding = 0;
    global_font.texture = LoadTextureFromImage(img);
    global_font.recs = (Rectangle*)malloc(sizeof(Rectangle)*global_font.glyphCount);
    global_font.glyphs = (GlyphInfo*)malloc(sizeof(GlyphInfo)*global_font.glyphCount);

    int start = 0;
    int end = 0;
    //printf("%i\n", GetImageColor(img, 0, 0).r);
    for (int i=0;i<strlen(characters); i++){
        //printf("%i\n", i);
        end = get_next_char(&img, start)-2;
        char c = characters[i];

        int descender = 0;

        if (c=='g' | c=='p' | c=='q' | c=='y'){
            descender = 4;
        }else if (c==';' | c==','){
            descender = 1;
        }else if (c=='j'){
            descender = 2;
        }
        
        //printf("############ %i -> %i\n", start, end);
        Rectangle tmp_rec = (Rectangle){start, 0, end-start+1, img.height};
        Image tmp_img = ImageFromImage(img, tmp_rec);
        global_font.recs[i] = tmp_rec;
        
        global_font.glyphs[i] = (GlyphInfo){c, 0, descender, 0, tmp_img};

        start = end+2;
        if (start>=img.width) break;
    }

    UnloadImage(img);
}

void graphics_load_default_font(){
    graphics_load_font_image("res/font.png", "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890"
        ".,|_+-/\\()[]{}!?\"'@#$%^&*=:;~`><");
    //global_font = LoadFont("res/GEORGIA.ttf");//LoadFontEx("res/GEORGIA.ttf", 10, NULL, 0);
    //global_font = LoadFontEx("res/Dina_r700-10.bdf", 10, NULL, 0);
}

void graphics_init(){
    graphics_load_atlas("res/sprite.png");
    graphics_load_default_font();
    palette_load_default();
}