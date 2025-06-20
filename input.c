bool input_key_down(int key_code){
    return IsKeyDown(key_code);
}

void input_foreign_key_down(WrenVM* vm){
    int key_code = (int)wrenGetSlotDouble(vm, 1);
    bool key_state = input_key_down(key_code);
    wrenSetSlotBool(vm, 0, key_state);
}