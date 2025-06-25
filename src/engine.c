enum {
    ENGINE_MODE_EDIT,
    ENGINE_MODE_RUN,
};

struct EditorCode{
    char* text;
    int length;
    int cursor;
};

int global_engine_mode = ENGINE_MODE_EDIT;

struct EditorCode editor_code;

void engine_init(){
    editor_code.length = 6;
    editor_code.text = malloc(6);
    strcpy(editor_code.text, "hello");
    editor_code.cursor = 2;
}

void engine_insert_at_cursor(char chr){
    char* before_cursor = malloc(editor_code.cursor+2);
    strncpy(before_cursor, editor_code.text, editor_code.cursor);
    before_cursor[editor_code.cursor] = chr;
    before_cursor[editor_code.cursor+1] = '\0';

    char* after_cursor = malloc(editor_code.length-editor_code.cursor);
    strcpy(after_cursor, &editor_code.text[editor_code.cursor]);

    editor_code.length += 1;
    char* code_buffer = realloc(editor_code.text, editor_code.length);
    if (code_buffer == NULL) exit(-1);
    editor_code.text = code_buffer;
    strcpy(editor_code.text, before_cursor);
    strcat(editor_code.text, after_cursor);

    free(before_cursor);
    free(after_cursor);
}

void engine_update(){
    char chr = GetCharPressed();
    if (chr != 0){
        engine_insert_at_cursor(chr);
    }
}

void engine_draw(){
    ClearBackground(WHITE);
    DrawText(editor_code.text, 0, 0, 9, BLACK);
}