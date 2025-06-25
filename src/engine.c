enum {
    ENGINE_MODE_EDIT,
    ENGINE_MODE_RUN,
};

struct TextLine{
    char* text;
    int size;
};

struct TextBlock{
    struct TextLine* lines;
    int size;
};

struct TextCursor{
    int row;
    int column;
};

int global_engine_mode = ENGINE_MODE_EDIT;

struct TextBlock text_edit;
struct TextCursor global_cursor;

//char text[1000]= "hi\nhello!";
//bool edit_mode = false;

void ensure_text_line_length(struct TextLine* line, int size){
    if (size>line->size){
        line->text = calloc(size, sizeof(char));
        line->text[0] = '\0';
        line->size = size;
    }
}

void ensure_text_edit_lines(int n_lines){
    if (n_lines > text_edit.size){
        text_edit.lines = realloc(text_edit.lines, n_lines*sizeof(struct TextLine));
        for (int i=text_edit.size;i<n_lines;i++){
            text_edit.lines[i].size = 0;
            ensure_text_line_length(&text_edit.lines[i], 100);
        }
        text_edit.size = n_lines;
        
    }
}

void engine_init(){
    ensure_text_edit_lines(100);
    strcpy(text_edit.lines[0].text, "hey!:)");
}

int get_line_length(int i){
    return strlen(text_edit.lines[i].text);
}

void engine_set_cursor(int column, int row){
    global_cursor.row = Clamp(row, 0, text_edit.size-1);
    global_cursor.column = Clamp(column, 0, get_line_length(global_cursor.row));
}

void engine_move_cursor(int column, int row){
    global_cursor.row = Clamp(global_cursor.row+row, 0, text_edit.size-1);
    int temp_column = global_cursor.column+column;
    if (temp_column > get_line_length(global_cursor.row)){
        engine_set_cursor(0, global_cursor.row+1);
    }else if (temp_column == -1){
        engine_set_cursor(get_line_length(global_cursor.row-1), global_cursor.row-1);
    }else{
        global_cursor.column = Clamp(global_cursor.column+column, 0, get_line_length(global_cursor.row));
    }
    printf("%i %i", global_cursor.column, global_cursor.row);
    //text_edit.cursor = Clamp(text_edit.cursor+i, 0, text_edit.size-1);
}

void engine_insert_at_cursor(char chr){
    //if (text_edit.cursor)
    //NEEDS REVIEW TO DECREASE NUMBER OF REALLOCS
    char* line_text = text_edit.lines[global_cursor.row].text;
    char* before_cursor = malloc(global_cursor.column+2);
    strncpy(before_cursor, line_text, global_cursor.column);
    before_cursor[global_cursor.column] = chr;
    before_cursor[global_cursor.column+1] = '\0';

    char* after_cursor = malloc(text_edit.lines[global_cursor.row].size-global_cursor.column);
    strcpy(after_cursor, &line_text[global_cursor.column]);

    text_edit.lines[global_cursor.row].size += 1;
    char* code_buffer = malloc(text_edit.lines[global_cursor.row].size);
    if (code_buffer == NULL) exit(-1);
    free(text_edit.lines[global_cursor.row].text);
    text_edit.lines[global_cursor.row].text = code_buffer;
    strcpy(text_edit.lines[global_cursor.row].text, before_cursor);
    strcat(text_edit.lines[global_cursor.row].text, after_cursor);

    engine_move_cursor(1, 0);

    free(before_cursor);
    free(after_cursor);
}

void engine_backspace_at_cursor(){
    /*printf("cursor: %i", text_edit.cursor);
    for (int i=text_edit.cursor-1;i<text_edit.size-2;i++){
        text_edit.text[i] = text_edit.text[i+1];
    }
    engine_move_cursor(-1);*/
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

    if (IsKeyPressed(KEY_RIGHT)){
        engine_move_cursor(1, 0);
    }else if (IsKeyPressed(KEY_LEFT)){
        engine_move_cursor(-1, 0);
    }else if (IsKeyPressed(KEY_UP)){
        engine_move_cursor(0, -1);
    }else if (IsKeyPressed(KEY_DOWN)){
        engine_move_cursor(0, 1);
    }
}

void engine_run_game(){
    //interface_load_game(text_edit.text);
    global_engine_mode = ENGINE_MODE_RUN;
}

void engine_draw(){
    ClearBackground(WHITE);
    for (int i=0;i<text_edit.size;i++){
        DrawTextEx(global_font, text_edit.lines[i].text, (Vector2){0, 10*i}, 9, 1, BLACK);
    }

    DrawRectangle(global_cursor.column*6, global_cursor.row*10, 1, 10, RED);

    //GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_TOP);   // WARNING: Word-wrap does not work as expected in case of no-top alignment
    //GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_WORD);
    //if (GuiTextBox((Rectangle){0,10,RENDER_WIDTH,RENDER_HEIGHT-10}, text, 1000, edit_mode)) edit_mode = !edit_mode;

}