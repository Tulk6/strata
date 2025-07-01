struct Palette global_palette;

void palette_set_colour(int i, Color colour){
    if ((i>=0) & (i<32)){
        global_palette.colours[i] = colour;
    }
}

Color palette_get_colour(int i){
    if ((i>=0) & (i<32)){    
        return global_palette.colours[i];
    }else{
        return (Color){0,0,0,0};
    }
}

void palette_load_from_image(char* path){
    Image img = LoadImage(path);
    if(img.height != 1){
        printf("IMAGE SHOULD BE ONE PIXEL HIGH");
    }
    for (int i=0;i<img.width;i++){
        palette_set_colour(i, GetImageColor(img, i, 0));
    }
    UnloadImage(img);
}

void palette_load_default(){
    palette_load_from_image("res/default_palette.png");
}