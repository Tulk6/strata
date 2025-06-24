bool input_key_down(int key_code){
    return IsKeyDown(key_code);
}

void input_foreign_key_down(WrenVM* vm){
    int key_code = (int)wrenGetSlotDouble(vm, 1);
    bool key_state = input_key_down(key_code);
    wrenSetSlotBool(vm, 0, key_state);
}

bool input_key_pressed(int key_code){
    return IsKeyPressed(key_code);
}

void input_foreign_key_pressed(WrenVM* vm){
    int key_code = (int)wrenGetSlotDouble(vm, 1);
    bool key_state = input_key_pressed(key_code);
    wrenSetSlotBool(vm, 0, key_state);
}

char input_get_char(){
    return GetCharPressed();
}

void input_foreign_get_char(WrenVM* vm){
    int chr = input_get_char();
    char* str = "";
    if (chr != 0){
        printf("%i\n", chr);
        wrenSetSlotString(vm, 0, (char*)&chr);
    }else{
        wrenSetSlotString(vm, 0, str);
    }
    /*if (chr != NULL){
        
    }else{
        
    }*/
}