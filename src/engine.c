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

int global_engine_mode = ENGINE_MODE_CODE;

int f_size;
int line_spacing;
int left_margin;
int bar_height;

bool button;

struct TextBlock text_edit;
char* text_edit_str = NULL;

Vector2 engine_get_mouse_pos(){
    return (Vector2){(double)GetMouseX()/WINDOW_SCALE, (double)GetMouseY()/WINDOW_SCALE};
}

void engine_init(){
    button = false;
    f_size = global_font.baseSize;
    line_spacing = 4;
    left_margin = 3;
    bar_height = 14;

    writer_ensure_text_edit_lines(&text_edit, 2);
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
        //engine_insert_at_cursor(chr);
    }
    if (IsKeyPressed(KEY_BACKSPACE)){
        //engine_backspace_at_cursor();
    }
    if (IsKeyPressed(KEY_ENTER)){
        //engine_newline_at_cursor();
    }
    if (IsKeyPressed(KEY_TAB)){
        //engine_insert_at_cursor('\t');
    }

    if (IsKeyPressed(KEY_RIGHT)){
        //engine_move_cursor(1, 0);
    }else if (IsKeyPressed(KEY_LEFT)){
        //engine_move_cursor(-1, 0);
    }else if (IsKeyPressed(KEY_UP)){
        //engine_set_cursor(global_cursor.column, global_cursor.row-1);
    }else if (IsKeyPressed(KEY_DOWN)){
        //engine_set_cursor(global_cursor.column, global_cursor.row+1);
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
    }
}

void draw_rectangle_bump(int x, int y, int w, int h, Color top, Color main, Color bottom, int bump){
    DrawRectangleGradientV(x, y, w, bump, top, main);
    DrawRectangle(x, y+bump, w, h-(2*bump), main);
    DrawRectangleGradientV(x, y+h-bump, w, bump, main, bottom);
}

void draw_rectangle_raised(int x, int y, int w, int h, Color main, Color top, Color bottom, int bump){
    DrawRectangle(x, y, w, h, top);
    DrawRectangle(x+bump, y+bump, w-bump, h-bump, bottom);
    DrawRectangle(x+bump, y+bump, w-bump*2, h-bump*2, main);
}

int draw_button(Rectangle rect, char* text, Color bg_col, Color fg_col, int bump, bool active){
    if (active){
        draw_rectangle_raised(rect.x, rect.y, rect.width, rect.height, bg_col, ColorBrightness(bg_col, -0.5), ColorBrightness(bg_col, 0.5), bump);
    }else{
        draw_rectangle_raised(rect.x, rect.y, rect.width, rect.height, bg_col, ColorBrightness(bg_col, 0.5), ColorBrightness(bg_col, -0.5), bump);
    }

    DrawTextEx(global_font, text, (Vector2){rect.x+bump+2, rect.y+1}, 8, 1, fg_col);

    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) & CheckCollisionPointRec(engine_get_mouse_pos(), rect);
}

void engine_draw_bar(){
    int bar_top = RENDER_HEIGHT-bar_height;
    ClearBackground(WHITE);
    //DrawRectangleGradientH(0, bar_top, RENDER_WIDTH, bar_height, (Color){1,54,120,255}, (Color){186,215,233,255});
    draw_rectangle_raised(0, bar_top, RENDER_WIDTH, bar_height, (Color){178,177,175,255}, (Color){178,177,175,255}, (Color){178,177,175,255}, 1);
    
    if (draw_button((Rectangle){1, bar_top+1, 40, bar_height-2}, "code", (Color){127,156,219,255}, WHITE, 1, button)){
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
            //engine_draw_code();
            break;
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