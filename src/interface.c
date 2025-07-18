WrenVM* vm = NULL;

WrenHandle* wren_update_handle;
WrenHandle* wren_draw_handle;
WrenHandle* wren_game_handle;

bool interface_call_load_game;
char* interface_code = NULL;

//WrenForeignMethodFn foreign_methods(const char* signature);
void interface_foreign_load_game(WrenVM* vm);

void write_fn(WrenVM* vm, const char* text){
    printf("%s", text);
}

void print_mult(WrenVM* vm){
    double n = wrenGetSlotDouble(vm, 1);
    for (int i=0; i<n; i++){
        //printf("%s", "hi!");
    }
}

WrenForeignMethodFn foreign_methods(const char* signature){ 
    if (!strcmp(signature, "static Engine.load_game(_)")) return interface_foreign_load_game;

    if (!strcmp(signature, "static Graphics.draw_sprite(_,_,_,_)")) return graphics_foreign_draw_sprite;
    if (!strcmp(signature, "static Graphics.draw_image(_,_,_,_)")) return graphics_foreign_draw_image;
    if (!strcmp(signature, "static Graphics.draw_image_scaled(_,_,_,_,_,_)")) return graphics_foreign_draw_image_scaled;
    if (!strcmp(signature, "static Graphics.draw_patch(_,_,_,_,_,_,_,_)")) return graphics_foreign_draw_patch;
    if (!strcmp(signature, "static Graphics.draw(_,_)")) return graphics_foreign_draw;
    if (!strcmp(signature, "static Graphics.clear_screen()")) return graphics_foreign_clear_screen;
    if (!strcmp(signature, "static Graphics.draw_text(_,_,_)")) return graphics_foreign_draw_text;
    if (!strcmp(signature, "static Graphics.draw_icon(_,_,_)")) return graphics_foreign_draw_icon;
    if (!strcmp(signature, "static Graphics.set_draw_colour(_)")) return graphics_foreign_set_draw_colour;
    //if (!strcmp(signature, "static Graphics.get_draw_colour(_)")) return graphics_foreign_get_draw_colour;
    if (!strcmp(signature, "static Graphics.draw_rectangle(_,_,_,_)")) return graphics_foreign_draw_rectangle;
    if (!strcmp(signature, "static Graphics.draw_rectangle_lines(_,_,_,_)")) return graphics_foreign_draw_rectangle_lines;
    if (!strcmp(signature, "static Graphics.blit(_,_)")) return graphics_foreign_blit;
    if (!strcmp(signature, "static Graphics.draw_line(_,_,_,_)")) return graphics_foreign_draw_line;
    //if (!strcmp(signature, "static Graphics.draw_icon(_,_,_)")) return graphics_foreign_draw_icon;

    if (!strcmp(signature, "static Input.key_down(_)")) return input_foreign_key_down;
    if (!strcmp(signature, "static Input.key_pressed(_)")) return input_foreign_key_pressed;
    if (!strcmp(signature, "static Input.get_char()")) return input_foreign_get_char;
    if (!strcmp(signature, "static Input.get_mouse_x()")) return input_foreign_get_mouse_x;
    if (!strcmp(signature, "static Input.get_mouse_y()")) return input_foreign_get_mouse_y;
    if (!strcmp(signature, "static Input.button_pressed(_)")) return input_foreign_button_pressed;
    if (!strcmp(signature, "static Input.button_down(_)")) return input_foreign_button_down;
    return NULL;
}

WrenForeignMethodFn bind_foreign_method_fn(WrenVM* vm, const char* module, const char* class_name, bool is_static, const char* signature){
    if (strcmp(module, "main")) return NULL;

    char full_signature[256];
    full_signature[0] = '\0';
    if (is_static){
        strcat(full_signature, "static ");
    }

    strcat(full_signature, class_name);
    strcat(full_signature, ".");
    strcat(full_signature, signature);

    return foreign_methods(full_signature);
}

WrenForeignClassMethods bind_foreign_class(WrenVM* vm, const char* module, const char* class_name){
    WrenForeignClassMethods class_methods;
    if (!strcmp(class_name, "Sprite")){
        class_methods.allocate = sprite_foreign_allocate;
        class_methods.finalize = NULL;
    }/*else if (!strcmp(class_name, "Image")){
        class_methods.allocate = image_foreign_allocate;
        class_methods.finalize = NULL;
        //class_methods.finalize = palette_foreign_finalize;
    }*/
    return class_methods;
}

void error_fn(WrenVM* vm, WrenErrorType type, const char* module, int line, const char* message){
    switch (type){
        case WREN_ERROR_COMPILE:
            printf("COMPILATION ERROR:\n \t(%i of %s) %s\n", line, module, message);
            break;
        case WREN_ERROR_STACK_TRACE:
            printf("\t[%i of %s] %s\n", line, module, message);
            break;
        case WREN_ERROR_RUNTIME:
            printf("RUNTIME ERROR:\n[%i of %s] %s\n", line, module, message);
            break;
    }
}

void wren_run_code(WrenVM* vm, char* code){
    WrenInterpretResult result = wrenInterpret(
        vm, "main", code);

    switch (result) {
        case WREN_RESULT_COMPILE_ERROR:
            { printf("Compile Error!\n"); } break;
        case WREN_RESULT_RUNTIME_ERROR:
            { printf("Runtime Error!\n"); } break;
        case WREN_RESULT_SUCCESS:
            { printf("Success!\n"); } break;
    }
}

void interface_update(){
    //printf("begin update\n");
    wrenEnsureSlots(vm, 2);
    wrenSetSlotHandle(vm, 0, wren_game_handle);
    wrenSetSlotDouble(vm, 1, (double)GetFrameTime());
    WrenInterpretResult result = wrenCall(vm, wren_update_handle);
    //printf("end uipdate\n");
}

void interface_draw(){
    //printf("begin intdraw\n");
    wrenEnsureSlots(vm, 2);
    wrenSetSlotHandle(vm, 0, wren_game_handle);
    wrenSetSlotDouble(vm, 1, (double)GetFrameTime());
    WrenInterpretResult result = wrenCall(vm, wren_draw_handle);
    //printf("end intdraw\n");
}

void interface_new_vm(){
    if (vm != NULL) wrenFreeVM(vm);
    WrenConfiguration config;
    wrenInitConfiguration(&config);
        config.writeFn = &write_fn;
        config.errorFn = &error_fn;

        config.bindForeignMethodFn = &bind_foreign_method_fn;
        config.bindForeignClassFn = &bind_foreign_class;
        /*config.loadModuleFn = &load_module_fn;*/

    vm = wrenNewVM(&config);
}

void interface_load_engine(){
    char* engine_objects_str = LoadFileText("res/engine.wren");
    wren_run_code(vm, engine_objects_str);
    UnloadFileText(engine_objects_str);
}

void interface_get_handles(){
    wren_update_handle = wrenMakeCallHandle(vm, "update(_)");
    wren_draw_handle = wrenMakeCallHandle(vm, "draw(_)");
    wrenEnsureSlots(vm, 1);
    wrenGetVariable(vm, "main", "game", 0);
    wren_game_handle = wrenGetSlotHandle(vm, 0);
}

void interface_load_game(char* code){
    interface_new_vm();
    interface_load_engine();

    wren_run_code(vm, code);

    wren_run_code(vm, "var game = Game.init()");

    interface_get_handles();
}

void interface_foreign_load_game(WrenVM* vm){
    if (interface_code != NULL){
        free(interface_code);
    }
    char* wren_code_ptr = wrenGetSlotString(vm, 1);
    interface_code = malloc(sizeof(char)*(strlen(wren_code_ptr)+1));
    strcat(interface_code, wren_code_ptr);
    interface_call_load_game = true;
}

void interface_init(){

}

void interface_close(){
    if (vm != NULL){
        wrenReleaseHandle(vm, wren_update_handle);
        wrenReleaseHandle(vm, wren_draw_handle);
        wrenReleaseHandle(vm, wren_game_handle);
        wrenFreeVM(vm);
    }
}