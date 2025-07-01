// sprites -> stacked images
// image -> 2d image
// atlas -> image atlas all images and sprites are on

struct Atlas global_atlas;
struct Atlas global_icons;

Font global_font;
int global_font_width = 5;
int global_font_height = 9;

Color global_draw_colour;

void graphics_load_atlas(struct Atlas* atlas, char* path){
    atlas->image = LoadImage(path);
    atlas->texture = LoadTextureFromImage(atlas->image);
}

void graphics_reload_texture_rect(struct Atlas* atlas, Rectangle rect){
    printf("%f %f %f %f\n", rect.x, rect.y, rect.width, rect.height);
    Image crop_image = ImageCopy(atlas->image);
    ImageCrop(&crop_image, rect);
    void* pixels = LoadImageColors(crop_image);
    UpdateTextureRec(atlas->texture, rect, pixels);
    UnloadImageColors(pixels);
    UnloadImage(crop_image);
}

void graphics_unload_atlas(struct Atlas* atlas){
    UnloadTexture(atlas->texture);
    UnloadImage(atlas->image);
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

void graphics_draw_writer(struct TextBlock* writer, int x, int y){
    for (int i=0; i<writer->size;i++){
        struct TextLine line = writer->lines[i];
        DrawTextEx(global_font, line.text, (Vector2){x, y+i*(global_font.baseSize+4)}, global_font.baseSize, 1, BLACK);
    }
    //DrawRectangle()
}

void graphics_draw_painter(struct Painter* painter, int x, int y, int scale){
    DrawTexturePro(painter->src->texture, painter->rect, (Rectangle){x, y, painter->rect.width*scale, 
        painter->rect.height*scale}, (Vector2){0,0}, 0, WHITE);

    if (scale>1){
        for (int i=0; i<=painter->rect.width;i++){
            DrawLine(x+i*scale, y, x+i*scale, y+painter->rect.height*scale, BLACK);
        }
        for (int i=0; i<=painter->rect.height;i++){
            DrawLine(x, y+i*scale, x+painter->rect.width*scale, y+i*scale, BLACK);
        }
    }

    for (int i=0;i<4;i++){
        for (int j=0;j<8;j++){
            DrawRectangle(j*10+(RENDER_WIDTH-90), i*10+10, 8, 8, palette_get_colour(i*8+j));
        }
    }
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

        if ((c=='g') | (c=='p') | (c=='q') | (c=='y')){
            descender = 3;
        }else if ((c==';') | (c==',')){
            descender = 1;
        }else if ((c=='j') | (c=='3') | (c=='4') | (c=='5') | (c=='7') | (c=='9')){
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
    graphics_load_font_image("res/font_short.png", "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890"
        ".,|_+-/\\()[]{}!?\"'@#$%^&*=:;~`><");
    //global_font = LoadFont("res/GEORGIA.ttf");//LoadFontEx("res/GEORGIA.ttf", 10, NULL, 0);
    //global_font = LoadFontEx("res/Dina_r700-10.bdf", 10, NULL, 0);
}

void graphics_init(){
    graphics_load_atlas(&global_atlas, "res/sprite.png");
    graphics_load_atlas(&global_icons, "res/icon.bmp");
    graphics_load_default_font();
    palette_load_default();
}

void graphics_close(){
    graphics_unload_atlas(&global_atlas);
    graphics_unload_atlas(&global_icons);
    UnloadFont(global_font);
}

Vector2 graphics_get_mouse_pos(){
    return (Vector2){(double)GetMouseX()/WINDOW_SCALE, (double)GetMouseY()/WINDOW_SCALE};
}

void graphics_draw_rectangle_bump(int x, int y, int w, int h, Color top, Color main, Color bottom, int bump){
    DrawRectangleGradientV(x, y, w, bump, top, main);
    DrawRectangle(x, y+bump, w, h-(2*bump), main);
    DrawRectangleGradientV(x, y+h-bump, w, bump, main, bottom);
}

void graphics_draw_rectangle_raised(int x, int y, int w, int h, Color main, Color top, Color bottom, int bump){
    DrawRectangle(x, y, w, h, top);
    DrawRectangle(x+bump, y+bump, w-bump, h-bump, bottom);
    DrawRectangle(x+bump, y+bump, w-bump*2, h-bump*2, main);
}

int graphics_draw_button(Rectangle rect, char* text, Color bg_col, Color fg_col, int bump, bool active){
    if (active){
        graphics_draw_rectangle_raised(rect.x, rect.y, rect.width, rect.height, bg_col, ColorBrightness(bg_col, -0.5), ColorBrightness(bg_col, 0.5), bump);
    }else{
        graphics_draw_rectangle_raised(rect.x, rect.y, rect.width, rect.height, bg_col, ColorBrightness(bg_col, 0.5), ColorBrightness(bg_col, -0.5), bump);
    }

    DrawTextEx(global_font, text, (Vector2){rect.x+bump+2, rect.y+1}, 8, 1, fg_col);

    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) & CheckCollisionPointRec(graphics_get_mouse_pos(), rect);
}