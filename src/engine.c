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
    //printf("GROWING STRING %i\n", size);
    if (size>line->size){
        line->text = realloc(line->text, size*sizeof(char));
        line->size = size;
    }
}

void grow_text_line(struct TextLine* line, int size){
    //printf("grow attempt: %i, is %i\n", size, line->size);
    while (line->size <= size){
        ensure_text_line_length(line, (int)line->size*1.5);
    }
}

void init_text_line(struct TextLine* line){
    line->text = NULL;
    line->size = 0;
    ensure_text_line_length(line, 6);
    line->text[0] = '\0';
}

void ensure_text_edit_lines(int n_lines){
    if (n_lines > text_edit.size){
        text_edit.lines = realloc(text_edit.lines, n_lines*sizeof(struct TextLine));
        for (int i=text_edit.size;i<n_lines;i++){
            init_text_line(&text_edit.lines[i]);
        }
        text_edit.size = n_lines;
        
    }
}

void engine_init(){
    ensure_text_edit_lines(2);
    strcpy(text_edit.lines[0].text, "hello");
}

int get_line_length(int i){
    if ((i<0) | (i>text_edit.size)) return 0;
    return strlen(text_edit.lines[i].text);
}

void engine_set_cursor(int column, int row){
    global_cursor.row = Clamp(row, 0, text_edit.size-1);
    global_cursor.column = Clamp(column, 0, get_line_length(global_cursor.row));
}

void engine_move_cursor(int column, int row){
    printf("row: %i column: %i\n", row, column);
    int temp_column = global_cursor.column+column;
    if (temp_column<0){
        engine_set_cursor(get_line_length(global_cursor.row-1), global_cursor.row-1);
        engine_move_cursor(column+1, row);
    }else if (temp_column > get_line_length(global_cursor.row)){
        engine_set_cursor(0, global_cursor.row+1);
        engine_move_cursor(column-1, row);
    }else{
        engine_set_cursor(global_cursor.column+column, global_cursor.row+row);
    }

    /*global_cursor.row = Clamp(global_cursor.row+row, 0, text_edit.size-1);
    int temp_column = global_cursor.column+column;
    if (temp_column > get_line_length(global_cursor.row)){
        engine_set_cursor(0, global_cursor.row+1);
    }else if ((temp_column < 0) & (global_cursor.row>0)){
        printf("yay! %i\n", temp_column);
        engine_set_cursor(get_line_length(global_cursor.row-1), global_cursor.row-1);
        engine_move_cursor(temp_column, 0);
    }else{
        global_cursor.column = Clamp(global_cursor.column+column, 0, get_line_length(global_cursor.row));
    }
    printf("%i %i", global_cursor.column, global_cursor.row);
    //text_edit.cursor = Clamp(text_edit.cursor+i, 0, text_edit.size-1);*/
}

void engine_insert_at_cursor(char chr){
    grow_text_line(&text_edit.lines[global_cursor.row], get_line_length(global_cursor.row)+2);
    for (int i=get_line_length(global_cursor.row)+1;i>global_cursor.column;i--){
        text_edit.lines[global_cursor.row].text[i] = text_edit.lines[global_cursor.row].text[i-1];
    }
    text_edit.lines[global_cursor.row].text[global_cursor.column] = chr;
    engine_move_cursor(1, 0);
}

void engine_delete_newline(){
    if (!(global_cursor.row>0)) return;

    struct TextLine* prev_line = &text_edit.lines[global_cursor.row-1];
    struct TextLine* current_line = &text_edit.lines[global_cursor.row];
    grow_text_line(prev_line, strlen(current_line->text)+strlen(prev_line->text));
    

    strcat(prev_line->text, current_line->text);

    current_line->text[0] = '\0';
}

void engine_backspace_at_cursor(){
    int str_len = get_line_length(global_cursor.row);
    if (global_cursor.column > 0){
        for (int i=global_cursor.column-1;i<str_len;i++){
            text_edit.lines[global_cursor.row].text[i] = text_edit.lines[global_cursor.row].text[i+1];
        }
        engine_move_cursor(-1, 0);
    }else{
        engine_delete_newline();
        engine_move_cursor(-str_len-1, 0);
    }
}

void engine_newline_at_cursor(){
    if (global_cursor.row >= text_edit.size-1){
        ensure_text_edit_lines(text_edit.size*1.5);
    }

    struct TextLine* current = &text_edit.lines[global_cursor.row];
    int cursor_pos = global_cursor.column;

    for (int i=text_edit.size-1;i>global_cursor.row;i--){
        text_edit.lines[i] = text_edit.lines[i-1];
    }
    init_text_line(&text_edit.lines[global_cursor.row + 1]);
    strcpy(text_edit.lines[global_cursor.row + 1].text, &current->text[global_cursor.column]);
    current->text[cursor_pos] = '\0';
    engine_set_cursor(0, global_cursor.row+1);
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
        engine_newline_at_cursor();
    }
    if (IsKeyPressed(KEY_TAB)){
        engine_insert_at_cursor('\t');
    }

    if (IsKeyPressed(KEY_RIGHT)){
        engine_move_cursor(1, 0);
    }else if (IsKeyPressed(KEY_LEFT)){
        engine_move_cursor(-1, 0);
    }else if (IsKeyPressed(KEY_UP)){
        engine_set_cursor(global_cursor.column, global_cursor.row-1);
    }else if (IsKeyPressed(KEY_DOWN)){
        engine_set_cursor(global_cursor.column, global_cursor.row+1);
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