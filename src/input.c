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

Vector2 input_get_mouse(){
    return Vector2Scale(GetMousePosition(), 1.0/WINDOW_SCALE);
}

int input_get_mouse_x(){
    return input_get_mouse().x;
}

void input_foreign_get_mouse_x(WrenVM* vm){
    wrenSetSlotDouble(vm, 0, input_get_mouse_x());
}

int input_get_mouse_y(){
    return input_get_mouse().y;
}

void input_foreign_get_mouse_y(WrenVM* vm){
    wrenSetSlotDouble(vm, 0, input_get_mouse_y());
}

bool input_button_pressed(int button_code){
    return IsMouseButtonPressed(button_code);
}

void input_foreign_button_pressed(WrenVM* vm){
    int button_code = (int)wrenGetSlotDouble(vm, 1);
    bool button_state = input_button_pressed(button_code);
    wrenSetSlotBool(vm, 0, button_state);
}

bool input_button_down(int button_code){
    return IsMouseButtonDown(button_code);
}

void input_foreign_button_down(WrenVM* vm){
    int button_code = (int)wrenGetSlotDouble(vm, 1);
    bool button_state = input_button_down(button_code);
    wrenSetSlotBool(vm, 0, button_state);
}