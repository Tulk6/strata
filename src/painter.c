void painter_load(struct Painter* painter, struct Atlas* atlas){
    painter->src = atlas;
    painter->rect = (Rectangle){0, 0, atlas->image.width, atlas->image.height};
}

void painter_set_rectangle(struct Painter* painter, Rectangle rect){
    painter->rect = rect;
}

void painter_paint_pixel(struct Painter* painter, Vector2 pixel, Color col){
    ImageDrawPixel(&painter->src->image, (int)pixel.x, (int)pixel.y, RED);
    //printf("%f %f\n", pixel.x, pixel.y);
    graphics_reload_texture_rect(painter->src, painter->rect);
}