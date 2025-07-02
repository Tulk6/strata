void sprite_load(struct Sprite* sprite, int src_x, int src_y, int slice_width, int slice_height, int num_slices){
    sprite->src = (Rectangle){src_x, src_y, slice_width*num_slices, slice_height};
    sprite->slice_width = slice_width;
    sprite->slice_height = slice_height;
    sprite->num_slices = num_slices;
}

void sprite_foreign_allocate(WrenVM* vm){
    struct Sprite* sprite = (struct Sprite*)wrenSetSlotNewForeign(vm,
                                            0, 0, sizeof(struct Sprite));
    int src_x = wrenGetSlotDouble(vm, 1);
    int src_y = wrenGetSlotDouble(vm, 2);
    int slice_width = wrenGetSlotDouble(vm, 3);
    int slice_height = wrenGetSlotDouble(vm, 4);
    int num_slices = wrenGetSlotDouble(vm, 5);
    sprite_load(sprite, src_x, src_y, slice_width, slice_height, num_slices);
}