// sprites -> stacked images
// image -> 2d image
// atlas -> image atlas all images and sprites are on

struct Atlas global_atlas;

Font global_font;
int global_font_width = 5;
int global_font_height = 9;

Color global_draw_colour;

void graphics_load_atlas(struct Atlas* atlas, int width, int height){
    atlas->image = GenImageColor(width, height, palette_get_colour(0));//LoadImage(path);
    atlas->texture = LoadTextureFromImage(atlas->image);
}

void graphics_reload_texture_rect(struct Atlas* atlas, Rectangle rect){
    //printf("%f %f %f %f\n", rect.x, rect.y, rect.width, rect.height);
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

void graphics_draw(int x, int y){
    DrawPixel(x, y, global_draw_colour);
}

void graphics_foreign_draw(WrenVM* vm){
    int x = wrenGetSlotDouble(vm, 1);
    int y = wrenGetSlotDouble(vm, 2);
    graphics_draw(x, y);
}

void graphics_draw_sprite(struct Sprite* stacked_sprite, int x, int y, float rotation){
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
    float rotation = wrenGetSlotDouble(vm, 4);
    graphics_draw_sprite(sprite, x, y, rotation);
}

void graphics_draw_image(struct Sprite* sprite, int x, int y, float rotation){
    DrawTexturePro(global_atlas.texture, sprite->src, (Rectangle){x, y, sprite->src.width, sprite->src.height}, (Vector2){0,0},
                    0, WHITE);
}

void graphics_foreign_draw_image(WrenVM* vm){
    struct Sprite* sprite = (struct Sprite*)wrenGetSlotForeign(vm, 1);
    int x = wrenGetSlotDouble(vm, 2);
    int y = wrenGetSlotDouble(vm, 3);
    float rotation = wrenGetSlotDouble(vm, 4);
    graphics_draw_image(sprite, x, y, rotation);
}

void graphics_draw_image_scaled(struct Sprite* sprite, int x, int y, float sx, float sy, float rotation){
    DrawTexturePro(global_atlas.texture, sprite->src, (Rectangle){x, y, sprite->src.width*sx, sprite->src.height*sy}, (Vector2){0,0},
                    0, WHITE);
}

void graphics_foreign_draw_image_scaled(WrenVM* vm){
    struct Sprite* sprite = (struct Sprite*)wrenGetSlotForeign(vm, 1);
    int x = wrenGetSlotDouble(vm, 2);
    int y = wrenGetSlotDouble(vm, 3);
    float sx = wrenGetSlotDouble(vm, 4);
    float sy = wrenGetSlotDouble(vm, 5);
    float rotation = wrenGetSlotDouble(vm, 6);
    graphics_draw_image_scaled(sprite, x, y, sx, sy, rotation);
}

void graphics_draw_patch_tint(struct Atlas* atlas, int px, int py, int pw, int ph, int x, int y, int w, int h){
    DrawTexturePro(atlas->texture, (Rectangle){px, py, pw, ph}, (Rectangle){x, y, w, h}, (Vector2){0,0},
                    0, global_draw_colour);
}

void graphics_draw_patch(struct Atlas* atlas, int px, int py, int pw, int ph, int x, int y, int w, int h){
    DrawTexturePro(atlas->texture, (Rectangle){px, py, pw, ph}, (Rectangle){x, y, w, h}, (Vector2){0,0},
                    0, WHITE);
}

void graphics_foreign_draw_patch(WrenVM* vm){
    int px = wrenGetSlotDouble(vm, 1);
    int py = wrenGetSlotDouble(vm, 2);
    int pw = wrenGetSlotDouble(vm, 3);
    int ph = wrenGetSlotDouble(vm, 4);
    int x = wrenGetSlotDouble(vm, 5);
    int y = wrenGetSlotDouble(vm, 6);
    int w = wrenGetSlotDouble(vm, 7);
    int h = wrenGetSlotDouble(vm, 8);
    graphics_draw_patch(&global_atlas, px, py, pw, ph, x, y, w, h);
}

void graphics_draw_line(int x, int y, int ex, int ey){
    DrawLine(x, y, ex, ey, global_draw_colour);
}

void graphics_foreign_draw_line(WrenVM* vm){
    int x = wrenGetSlotDouble(vm, 1);
    int y = wrenGetSlotDouble(vm, 2);
    int ex = wrenGetSlotDouble(vm, 3);
    int ey = wrenGetSlotDouble(vm, 4);
    graphics_draw_line(x, y, ex, ey);
}

void graphics_blit(int x, int y){
    ImageDrawPixel(&global_atlas.image, x, y, global_draw_colour);
    graphics_reload_texture_rect(&global_atlas, (Rectangle){x, y, 1, 1});
}

void graphics_foreign_blit(WrenVM* vm){
    int x = wrenGetSlotDouble(vm, 1);
    int y = wrenGetSlotDouble(vm, 2);
    graphics_blit(x, y);
}

void graphics_blit_patch(struct Atlas* atlas, Image* src, int px, int py, int pw, int ph, int x, int y, int w, int h){
    ImageDraw(&atlas->image, *src, (Rectangle){px, py, pw, ph}, (Rectangle){x, y, w, h}, WHITE);
    graphics_reload_texture_rect(atlas, (Rectangle){x, y, w, h});
}

void graphics_draw_text(char* text, int x, int y){
    DrawTextEx(global_font, text, (Vector2){x, y}, global_font.baseSize, 1, global_draw_colour);
}

void graphics_foreign_draw_text(WrenVM* vm){
    char* text = wrenGetSlotString(vm, 1);
    int x = wrenGetSlotDouble(vm, 2);
    int y = wrenGetSlotDouble(vm, 3);
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

void graphics_draw_rectangle(int x, int y, int w, int h){
    DrawRectangle(x, y, w, h, global_draw_colour);
}

void graphics_foreign_draw_rectangle(WrenVM* vm){
    int x = wrenGetSlotDouble(vm, 1);
    int y = wrenGetSlotDouble(vm, 2);
    int w = wrenGetSlotDouble(vm, 3);
    int h = wrenGetSlotDouble(vm, 4);
    graphics_draw_rectangle(x, y, w, h);
}

void graphics_draw_rectangle_lines(int x, int y, int w, int h){
    DrawRectangleLines(x, y, w, h, global_draw_colour);
}

void graphics_foreign_draw_rectangle_lines(WrenVM* vm){
    int x = wrenGetSlotDouble(vm, 1);
    int y = wrenGetSlotDouble(vm, 2);
    int w = wrenGetSlotDouble(vm, 3);
    int h = wrenGetSlotDouble(vm, 4);
    graphics_draw_rectangle_lines(x, y, w, h);
}

/*void graphics_draw_icon(int x, int y, int i){
    int iy = floor(i/(ICONS_HEIGHT/ICON_SIZE));
    int ix = i-(iy*(ICONS_HEIGHT/ICON_SIZE));
    graphics_draw_patch_tint(&global_icons, ix*ICON_SIZE, iy*ICON_SIZE, ICON_SIZE, ICON_SIZE, x, y, ICON_SIZE, ICON_SIZE);
}*/

/*void graphics_foreign_draw_icon(WrenVM* vm){
    int x = wrenGetSlotDouble(vm, 1);
    int y = wrenGetSlotDouble(vm, 2);
    int i = wrenGetSlotDouble(vm, 3);
    graphics_draw_icon(x, y, i);
}*/

int get_next_char(Image* img, int x){
    Color key_colour = (Color){255, 0, 0, 255};
    int j = x;
    while (j<img->width){
        j += 1;
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

    int codepoint_count = 0;
    int* codepoints = LoadCodepoints(characters, &codepoint_count);
    for (int i=0;i<codepoint_count; i++){
        end = get_next_char(&img, start)-2;
        int c = codepoints[i];

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

    free(codepoints);
    UnloadImage(img);
}

void graphics_load_default_font(){
    graphics_load_font_image("res/font_short.png", "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890"
        ".,|_+-/\\()[]{}!?\"'@#$%^&*=:;~`><"
        "¡");
    //global_font = LoadFont("res/GEORGIA.ttf");//LoadFontEx("res/GEORGIA.ttf", 10, NULL, 0);
    //global_font = LoadFontEx("res/Dina_r700-10.bdf", 10, NULL, 0);
}

void graphics_init(){
    palette_load_default();
    graphics_load_atlas(&global_atlas, ATLAS_WIDTH, ATLAS_HEIGHT); //,"res/sprite.png");

    /*graphics_load_atlas(&global_icons, ICONS_WIDTH, ICONS_HEIGHT);
    Image icon_src = LoadImage("res/icon.bmp");
    graphics_blit_patch(&global_icons, &icon_src, 0, 0, ICONS_WIDTH, ICONS_HEIGHT, 0, 0, ICONS_WIDTH, ICONS_HEIGHT);*/

    graphics_load_default_font();
}

void graphics_close(){
    graphics_unload_atlas(&global_atlas);
    //graphics_unload_atlas(&global_icons);
    UnloadFont(global_font);
}

/*Vector2 graphics_get_mouse_pos(){
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
}*/