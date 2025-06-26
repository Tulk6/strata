#define BG_COLOUR CLITERAL(Color){255, 238, 204, 255}
#define FG_COLOUR CLITERAL(Color){70, 66, 94, 255}
#define AC_COLOUR CLITERAL(Color){255, 105, 115, 255}
#define IN_COLOUR CLITERAL(Color){255, 176, 163, 255}

enum {
    ENGINE_MODE_CODE,
    ENGINE_MODE_SPRITE,
    ENGINE_MODE_SOUND,
    ENGINE_MODE_MUSIC,
    ENGINE_MODE_MAP,
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

int global_engine_mode = ENGINE_MODE_CODE;

struct TextBlock text_edit;
struct TextCursor global_cursor;

char* text_edit_str = NULL;

int f_size;
int line_spacing;
int left_margin;
int top_bar_height;

//char text[1000]= "hi\nhello!";
//bool edit_mode = false;

int get_line_length(int i){
    if ((i<0) | (i>text_edit.size)) return 0;
    return strlen(text_edit.lines[i].text);
}

void engine_set_cursor(int column, int row){
    global_cursor.row = Clamp(row, 0, text_edit.size-1);
    global_cursor.column = Clamp(column, 0, get_line_length(global_cursor.row));
}

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
    ensure_text_line_length(line, 16);
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

void engine_clear_text(){
    engine_set_cursor(0,0);
    for (int i=0;i<text_edit.size;i++){
        text_edit.lines[i].text[0] = '\0';
    }
}

void engine_init(){
    f_size = global_font.baseSize;
    line_spacing = 4;
    left_margin = 3;
    top_bar_height = 11;

    ensure_text_edit_lines(2);
    //strcpy(text_edit.lines[0].text, "hello");
}

void engine_load_text(char* text){
    engine_clear_text();

    char* start = text;
    char* ptr = text;
    int i = 0;

    while (*ptr){
        if (*ptr == '\n'){
            int len = ptr-start;
            char* line = malloc(len+1);
            memcpy(line, start, len);
            line[len] = '\0';
            
            //printf("%s (%i)\n", line, i);
            ensure_text_edit_lines(i+1);
            struct TextLine* text_line = &text_edit.lines[i];
            ensure_text_line_length(text_line, len+1);
            strcpy(text_line->text, line);

            free(line);
            start=ptr+1;
            i++;
        }
        ptr++;
    }

    if (ptr != start) {
        size_t len = ptr - start;
        char *line = malloc(len + 1);
        memcpy(line, start, len);
        line[len] = '\0';

        ensure_text_edit_lines(i+1);
        struct TextLine* text_line = &text_edit.lines[i];
        ensure_text_line_length(text_line, len);
        strcpy(text_line->text, line);
        memcpy(line, start, len);

        free(line);
    }

    /*while (token != NULL){
        printf("%i..\n", i);
        ensure_text_edit_lines(i+1);
        printf("tok: %s\n", token);
        struct TextLine* line = &text_edit.lines[i];

        ensure_text_line_length(line, strlen(token)+1);
        strcpy(line->text, token);
        printf("%s\n", token);

        token = strtok(NULL, "\n");
        i++;
    }*/
}

void engine_move_cursor(int column, int row){
    //printf("row: %i column: %i\n", row, column);
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
    ensure_text_edit_lines(text_edit.size+1);

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

void engine_get_str(){
    int size = 1;
    for (int i=0;i<text_edit.size;i++){
        size += strlen(text_edit.lines[i].text)+1; //+1 for newlines too
    }
    
    free(text_edit_str);
    text_edit_str = malloc(size);
    text_edit_str[0] = '\0';
    for (int i=0;i<text_edit.size;i++){
        strcat(text_edit_str, text_edit.lines[i].text);
        text_edit_str[strlen(text_edit_str)+1] = '\0';
        text_edit_str[strlen(text_edit_str)] = '\n';
    } 
}

void engine_run_game(){
    engine_get_str();
    printf("%s", text_edit_str);
    interface_load_game(text_edit_str);
    global_engine_mode = ENGINE_MODE_RUN;
}

void engine_update_code(){
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

void engine_draw_code(){
    //DrawR

    for (int i=0;i<text_edit.size;i++){
        DrawTextEx(global_font, text_edit.lines[i].text, (Vector2){left_margin, ((f_size+line_spacing)*i+top_bar_height+2)}, 8, 1, FG_COLOUR);
        //DrawTextEx(global_font, TextFormat("%i", i), (Vector2){0, (global_font.baseSize+4)*i+5}, 8, 1, RED);
    }

    DrawRectangle(global_cursor.column*6+left_margin-1, (f_size+line_spacing)*global_cursor.row+top_bar_height, 1, 13, AC_COLOUR);
}

void engine_draw_bar(){
    ClearBackground(WHITE);
    DrawRectangle(0, 0, RENDER_WIDTH, top_bar_height, AC_COLOUR);
    DrawRectangleRounded((Rectangle){2, 1, 80, top_bar_height+2}, 0.5, 2, BG_COLOUR);
    DrawTextEx(global_font, "Code", (Vector2){8, 2}, 8, 1, FG_COLOUR);
    DrawRectangleRounded((Rectangle){84, 2, 80, top_bar_height+2}, 0.5, 2, IN_COLOUR);
    DrawRectangle(0, top_bar_height, RENDER_WIDTH, RENDER_HEIGHT, BG_COLOUR);
}

void engine_draw(){
    engine_draw_bar();
    switch (global_engine_mode){
        case ENGINE_MODE_CODE:
            engine_draw_code();
            break;
    }

    //GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_TOP);   // WARNING: Word-wrap does not work as expected in case of no-top alignment
    //GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_WORD);
    //if (GuiTextBox((Rectangle){0,10,RENDER_WIDTH,RENDER_HEIGHT-10}, text, 1000, edit_mode)) edit_mode = !edit_mode;
}

void engine_export(){
    engine_get_str();
    char* out_text = NULL;
    out_text = malloc(strlen(text_edit_str)+2);
    strcpy(out_text, text_edit_str);
    out_text[strlen(out_text)+1] = '\0';
    out_text[strlen(out_text)] = (char)31;
    SaveFileText("out.txt", out_text);
    free(out_text);
}

void engine_import(char* path){
    char* file = LoadFileText(path);
    engine_load_text(file);
    free(file);
}

void engine_close(){
    for (int i=0;i<text_edit.size;i++){
        free(text_edit.lines[i].text);
    }
    free(text_edit.lines);
    free(text_edit_str);
}