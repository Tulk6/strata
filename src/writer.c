int writer_get_line_length(struct TextBlock* block, int i){
    if ((i<0) | (i>block->size)) return 0;
    return strlen(block->lines[i].text);
}

void writer_set_cursor(struct TextBlock* block, int column, int row){
    block->c_row = Clamp(row, 0, block->size-1);
    block->c_column = Clamp(column, 0, writer_get_line_length(block, block->c_row));
}

void writer_ensure_text_line_length(struct TextLine* line, int size){
    if (size>line->size){
        line->text = realloc(line->text, size*sizeof(char));
        line->size = size;
    }
}

void writer_grow_text_line(struct TextLine* line, int size){
    while (line->size <= size){
        writer_ensure_text_line_length(line, (int)line->size*1.5);
    }
}

void writer_init_text_line(struct TextLine* line){
    line->text = NULL;
    line->size = 0;
    writer_ensure_text_line_length(line, 16);
    line->text[0] = '\0';
}

void writer_ensure_text_edit_lines(struct TextBlock* block, int n_lines){
    if (n_lines > block->size){
        block->lines = realloc(block->lines, n_lines*sizeof(struct TextLine));
        for (int i=block->size;i<n_lines;i++){
            writer_init_text_line(&block->lines[i]);
        }
        block->size = n_lines;
        
    }
}

void writer_clear_text(struct TextBlock* block){
    writer_set_cursor(block,0,0);
    for (int i=0;i<block->size;i++){
        block->lines[i].text[0] = '\0';
    }
}

void writer_load_text(struct TextBlock* block, char* text){
    writer_clear_text(block);

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
            writer_ensure_text_edit_lines(block, i+1);
            struct TextLine* text_line = &block->lines[i];
            writer_ensure_text_line_length(text_line, len+1);
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

        writer_ensure_text_edit_lines(block, i+1);
        struct TextLine* text_line = &block->lines[i];
        writer_ensure_text_line_length(text_line, len);
        strcpy(text_line->text, line);
        memcpy(line, start, len);

        free(line);
    }
}

void writer_move_cursor(struct TextBlock* block, int column, int row){
    //printf("row: %i column: %i\n", row, column);
    int temp_column = block->c_column+column;
    if (temp_column<0){
        writer_set_cursor(block, writer_get_line_length(block, block->c_row-1), block->c_row-1);
        writer_move_cursor(block, column+1, row);
    }else if (temp_column > writer_get_line_length(block, block->c_row)){
        writer_set_cursor(block, 0, block->c_row+1);
        writer_move_cursor(block, column-1, row);
    }else{
        writer_set_cursor(block, block->c_column+column, block->c_row+row);
    }
}

void writer_insert_at_cursor(struct TextBlock* block, char chr){
    writer_grow_text_line(&block->lines[block->c_row], writer_get_line_length(block,block->c_row)+2);
    for (int i=writer_get_line_length(block,block->c_row)+1;i>block->c_column;i--){
        block->lines[block->c_row].text[i] = block->lines[block->c_row].text[i-1];
    }
    block->lines[block->c_row].text[block->c_column] = chr;
    writer_move_cursor(block, 1, 0);
}

void writer_delete_newline(struct TextBlock* block){
    if (!(block->c_row>0)) return;

    struct TextLine* prev_line = &block->lines[block->c_row-1];
    struct TextLine* current_line = &block->lines[block->c_row];
    writer_grow_text_line(prev_line, strlen(current_line->text)+strlen(prev_line->text));
    

    strcat(prev_line->text, current_line->text);

    current_line->text[0] = '\0';
}

void writer_backspace_at_cursor(struct TextBlock* block){
    int str_len = writer_get_line_length(block,block->c_row);
    if (block->c_column > 0){
        for (int i=block->c_column-1;i<str_len;i++){
            block->lines[block->c_row].text[i] = block->lines[block->c_row].text[i+1];
        }
        writer_move_cursor(block,-1, 0);
    }else{
        writer_delete_newline(block);
        writer_move_cursor(block,-str_len-1, 0);
    }
}

void writer_newline_at_cursor(struct TextBlock* block){
    writer_ensure_text_edit_lines(block, block->size+1);

    struct TextLine* current = &block->lines[block->c_row];
    int cursor_pos = block->c_column;

    for (int i=block->size-1;i>block->c_row;i--){
        block->lines[i] = block->lines[i-1];
    }
    writer_init_text_line(&block->lines[block->c_row + 1]);
    strcpy(block->lines[block->c_row + 1].text, &current->text[block->c_column]);
    current->text[cursor_pos] = '\0';
    writer_set_cursor(block, 0, block->c_row+1);
}

char* writer_get_str(struct TextBlock* block){
    int size = 1;
    for (int i=0;i<block->size;i++){
        size += strlen(block->lines[i].text)+1; //+1 for newlines too
    }
    
    char* text_edit_str = malloc(size);
    text_edit_str[0] = '\0';
    for (int i=0;i<block->size;i++){
        strcat(text_edit_str, block->lines[i].text);
        text_edit_str[strlen(text_edit_str)+1] = '\0';
        text_edit_str[strlen(text_edit_str)] = '\n';
    } 
    return text_edit_str;
}

void writer_draw_code(struct TextBlock* block){
    /*for (int i=0;i<block->size;i++){
        DrawTextEx(global_font, block->lines[i].text, (Vector2){left_margin, ((f_size+line_spacing)*i+bar_height+2)}, 8, 1, FG_COLOUR);
        //DrawTextEx(global_font, TextFormat("%i", i), (Vector2){0, (global_font.baseSize+4)*i+5}, 8, 1, RED);
    }

    DrawRectangle(block->c_column*6+left_margin-1, (f_size+line_spacing)*block->c_row+bar_height, 1, 13, AC_COLOUR);*/
}

void writer_close(struct TextBlock* block){
    for (int i=0;i<block->size;i++){
        free(block->lines[i].text);
    }
    free(block->lines);
}