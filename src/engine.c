enum {
    ENGINE_MODE_EDIT,
    ENGINE_MODE_RUN,
};

struct TextEdit{
    char* text;
    int size;
    int cursor;
};

int global_engine_mode = ENGINE_MODE_EDIT;

struct TextEdit text_edit;
//char text[1000]= "hi\nhello!";
//bool edit_mode = false;

void engine_init(){
    text_edit.text = malloc(2);
    text_edit.text[0] = 0;
    text_edit.size = 2;
    text_edit.cursor = 0;
}

void engine_move_cursor(int i){
    text_edit.cursor = Clamp(text_edit.cursor+i, 0, text_edit.size-1);
}

void engine_insert_at_cursor(char chr){
    //if (text_edit.cursor)
    //NEEDS REVIEW TO DECREASE NUMBER OF REALLOCS
    char* before_cursor = malloc(text_edit.cursor+2);
    strncpy(before_cursor, text_edit.text, text_edit.cursor);
    before_cursor[text_edit.cursor] = chr;
    before_cursor[text_edit.cursor+1] = '\0';

    char* after_cursor = malloc(text_edit.size-text_edit.cursor);
    strcpy(after_cursor, &text_edit.text[text_edit.cursor]);

    text_edit.size += 1;
    char* code_buffer = malloc(text_edit.size);
    if (code_buffer == NULL) exit(-1);
    free(text_edit.text);
    text_edit.text = code_buffer;
    strcpy(text_edit.text, before_cursor);
    strcat(text_edit.text, after_cursor);

    engine_move_cursor(1);

    free(before_cursor);
    free(after_cursor);
}

void engine_backspace_at_cursor(){
    printf("cursor: %i", text_edit.cursor);
    for (int i=text_edit.cursor-1;i<text_edit.size-2;i++){
        text_edit.text[i] = text_edit.text[i+1];
    }
    engine_move_cursor(-1);
}

void engine_update(){
    char chr = GetCharPressed();
    if (chr != 0){
        engine_insert_at_cursor(chr);
    }
    if (IsKeyPressed(KEY_BACKSPACE)){
        engine_backspace_at_cursor();
    }
    if (IsKeyPressed(KEY_ENTER)){
        engine_insert_at_cursor('\n');
    }
    if (IsKeyPressed(KEY_TAB)){
        engine_insert_at_cursor('\t');
    }

    if (IsKeyPressed(KEY_F5)){
        interface_load_game(text_edit.text);
        global_engine_mode = ENGINE_MODE_RUN;
    }
}

void engine_draw(){
    ClearBackground(WHITE);
    DrawTextEx(global_font, text_edit.text, (Vector2){0, 0}, 9, 1, BLACK);
    //GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_TOP);   // WARNING: Word-wrap does not work as expected in case of no-top alignment
    //GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_WORD);
    //if (GuiTextBox((Rectangle){0,10,RENDER_WIDTH,RENDER_HEIGHT-10}, text, 1000, edit_mode)) edit_mode = !edit_mode;

}