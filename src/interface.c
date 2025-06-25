WrenVM* vm;

WrenHandle* wren_update_handle;
WrenHandle* wren_draw_handle;
WrenHandle* wren_game_handle;

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
    //if (!strcmp(signature, "static Engine.load_game(_)")) return interface_foreign_load_game;

    if (!strcmp(signature, "static Graphics.draw_sprite(_,_,_,_)")) return graphics_foreign_draw_sprite;
    if (!strcmp(signature, "static Graphics.clear_screen()")) return graphics_foreign_clear_screen;
    if (!strcmp(signature, "static Graphics.draw_text(_,_,_)")) return graphics_foreign_draw_text;
    if (!strcmp(signature, "static Graphics.set_draw_colour(_)")) return graphics_foreign_set_draw_colour;

    if (!strcmp(signature, "static Input.key_down(_)")) return input_foreign_key_down;
    if (!strcmp(signature, "static Input.key_pressed(_)")) return input_foreign_key_pressed;
    if (!strcmp(signature, "static Input.get_char()")) return input_foreign_get_char;
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
    }/*else if (!strcmp(class_name, "Palette")){
        class_methods.allocate = palette_foreign_allocate;
        class_methods.finalize = palette_foreign_finalize;
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

void interface_init(){
    WrenConfiguration config;
    wrenInitConfiguration(&config);
        config.writeFn = &write_fn;
        config.errorFn = &error_fn;

        config.bindForeignMethodFn = &bind_foreign_method_fn;
        config.bindForeignClassFn = &bind_foreign_class;
        /*config.loadModuleFn = &load_module_fn;*/

    vm = wrenNewVM(&config);

    char* engine_objects_str = LoadFileText("res/engine.wren");
    wren_run_code(vm, engine_objects_str);
    UnloadFileText(engine_objects_str);

    char* main_str = LoadFileText("res/game.wren");
    wren_run_code(vm, main_str);
    UnloadFileText(main_str);

    wren_run_code(vm, "var game = Game.init()");

    wren_update_handle = wrenMakeCallHandle(vm, "update(_)");
    wren_draw_handle = wrenMakeCallHandle(vm, "draw(_)");
    wrenEnsureSlots(vm, 1);
    wrenGetVariable(vm, "main", "game", 0);
    wren_game_handle = wrenGetSlotHandle(vm, 0);
}