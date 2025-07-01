#define FG_COLOUR CLITERAL(Color){111, 128, 155, 255}
#define BG_COLOUR CLITERAL(Color){255, 255, 255, 255}
#define AC_COLOUR CLITERAL(Color){38, 99, 224, 255}
#define IN_COLOUR CLITERAL(Color){239, 243, 254, 255}

enum {
    ENGINE_MODE_CODE,
    ENGINE_MODE_SPRITE,
    ENGINE_MODE_SOUND,
    ENGINE_MODE_MUSIC,
    ENGINE_MODE_MAP,
    ENGINE_MODE_RUN,
};

int global_engine_mode = ENGINE_MODE_SPRITE;

int f_size;
int line_spacing;
int left_margin;
int top_margin;
int bar_height;
int sprite_scale;

bool button;

struct TextBlock text_edit;
struct Painter painter;
char* text_edit_str = NULL;

void engine_init(){
    button = false;
    f_size = global_font.baseSize;
    line_spacing = 4;
    left_margin = 3;
    bar_height = 14;
    top_margin = 3;
    sprite_scale = 4;

    writer_ensure_text_edit_lines(&text_edit, 2);
    painter_load(&painter, &global_atlas);
    painter.rect = (Rectangle){32,0,32,16};
    //strcpy(text_edit.lines[0].text, "hello");
}


void engine_run_game(){
    char* editor_code = writer_get_str(&text_edit);
    printf("%s", text_edit_str);
    interface_load_game(text_edit_str);
    free(editor_code);

    global_engine_mode = ENGINE_MODE_RUN;
}

void engine_update_code(){
    char chr = GetCharPressed();
    if (chr != 0){
        writer_insert_at_cursor(&text_edit, chr);
    }
    if (IsKeyPressed(KEY_BACKSPACE)){
        writer_backspace_at_cursor(&text_edit);
    }
    if (IsKeyPressed(KEY_ENTER)){
        writer_newline_at_cursor(&text_edit);
    }
    if (IsKeyPressed(KEY_TAB)){
        writer_insert_at_cursor(&text_edit, '\t');
    }

    if (IsKeyPressed(KEY_RIGHT)){
        writer_move_cursor(&text_edit, 1, 0);
    }else if (IsKeyPressed(KEY_LEFT)){
        writer_move_cursor(&text_edit, -1, 0);
    }else if (IsKeyPressed(KEY_UP)){
        writer_set_cursor(&text_edit, text_edit.c_column, text_edit.c_row-1);
    }else if (IsKeyPressed(KEY_DOWN)){
        writer_set_cursor(&text_edit, text_edit.c_column, text_edit.c_row+1);
    }
}

void engine_update_sprite(){
    Vector2 pos = graphics_get_mouse_pos();
    if (CheckCollisionPointRec(pos, (Rectangle){left_margin, top_margin, painter.rect.width*sprite_scale, painter.rect.height*sprite_scale})){
        //printf("pos %f %f\n", pos.x, pos.y);
        Vector2 pixel = Vector2Add(Vector2Scale(Vector2Subtract(pos, (Vector2){left_margin, top_margin}), 1.0/sprite_scale), 
                        (Vector2){painter.rect.x, painter.rect.y});
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) painter_paint_pixel(&painter, pixel, RED);
    }
}

void engine_update(){
    if (IsKeyPressed(KEY_GRAVE)){
        global_engine_mode += 1;
        global_engine_mode = global_engine_mode%3;
    }
    switch (global_engine_mode){
        case ENGINE_MODE_CODE:
            engine_update_code();
            break;
        case ENGINE_MODE_SPRITE:
            engine_update_sprite();
    }
}

void engine_draw_bar(){
    int bar_top = RENDER_HEIGHT-bar_height;
    ClearBackground(WHITE);
    //DrawRectangleGradientH(0, bar_top, RENDER_WIDTH, bar_height, (Color){1,54,120,255}, (Color){186,215,233,255});
    graphics_draw_rectangle_raised(0, bar_top, RENDER_WIDTH, bar_height, (Color){178,177,175,255}, (Color){178,177,175,255}, (Color){178,177,175,255}, 1);
    
    if (graphics_draw_button((Rectangle){1, bar_top+1, 40, bar_height-2}, "code", (Color){127,156,219,255}, WHITE, 1, button)){
        button = !button;
        //DrawRectangleGradientV(0, bar_top, RENDER_WIDTH, bar_height, (Color){58,130,229,255}, (Color){26,67,169,255});
    }
    
    //
    //draw_rectangle_bump(0, bar_top, RENDER_WIDTH, bar_height, (Color){127,156,219,255}, (Color){36,93,219,255}, (Color){23,60,147,255}, 3);
    //DrawRectangle(2, bar_top+1, 80, bar_height-1, (Color){58,130,246,255});
    
    /*DrawRectangleRounded((Rectangle){84, 2, 80, bar_height+2}, 0.5, 2, IN_COLOUR);
    DrawRectangle(0, bar_height, RENDER_WIDTH, RENDER_HEIGHT, BG_COLOUR);*/
}

void engine_draw(){
    engine_draw_bar();
    switch (global_engine_mode){
        case ENGINE_MODE_CODE:
            graphics_draw_writer(&text_edit, 2, 2);
            break;
        case ENGINE_MODE_SPRITE:
            graphics_draw_painter(&painter, left_margin, top_margin, sprite_scale);
    }

    //GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_TOP);   // WARNING: Word-wrap does not work as expected in case of no-top alignment
    //GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_WORD);
    //if (GuiTextBox((Rectangle){0,10,RENDER_WIDTH,RENDER_HEIGHT-10}, text, 1000, edit_mode)) edit_mode = !edit_mode;
}

void engine_export(){
    char* text_edit_str = writer_get_str(&text_edit);
    char* out_text = NULL;
    out_text = malloc(strlen(text_edit_str)+2);
    strcpy(out_text, text_edit_str);
    out_text[strlen(out_text)+1] = '\0';
    out_text[strlen(out_text)] = (char)31;
    SaveFileText("out.txt", out_text);
    free(out_text);
    free(text_edit_str);
}

void engine_import(char* path){
    char* file = LoadFileText(path);
    writer_load_text(&text_edit, file);
    free(file);
}

void engine_close(){
    writer_close(&text_edit);
}