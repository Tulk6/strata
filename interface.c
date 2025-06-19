#include "wren.c" //only here because of intellisense :( unneeded cos in main.c

WrenVM* vm;

void write_fn(WrenVM* vm, const char* text){
    printf("%s", text);
}

WrenForeignMethodFn foreign_methods(const char* signature){ 
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

void interface_init(){
    WrenConfiguration config;
    wrenInitConfiguration(&config);
        config.writeFn = &write_fn;
        config.errorFn = &error_fn;

        config.bindForeignMethodFn = &bind_foreign_method_fn;
        /*config.bindForeignClassFn = &bind_foreign_class;
        config.loadModuleFn = &load_module_fn;*/

    vm = wrenNewVM(&config);

    /*printf("engine objects\n");
    char* engine_objects_str = LoadFileText("engine_objects.wren");
    wren_run_code(vm, engine_objects_str);
    UnloadFileText(engine_objects_str);

    printf("main.wren\n");
    char* main_str = LoadFileText("main.wren");
    wren_run_code(vm, main_str);
    UnloadFileText(main_str);

    printf("main.init())\n");
    wren_run_code(vm, "var main = Main.init()");

    wren_update_handle = wrenMakeCallHandle(vm, "update(_)");
    wren_render_handle = wrenMakeCallHandle(vm, "render(_)");
    wren_draw_handle = wrenMakeCallHandle(vm, "draw(_)");
    wrenEnsureSlots(vm, 1);
    wrenGetVariable(vm, "main", "main", 0);
    wren_main_handle = wrenGetSlotHandle(vm, 0);*/
}