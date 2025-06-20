class Input{
    foreign static key_down(key_code)

    static get_axis(neg_key, pos_key){
        var neg_int = Input.key_down(neg_key) ? -1 : 0
        var pos_int = Input.key_down(pos_key) ?  1 : 0
        return neg_int+pos_int
    }
}

class Key{
    static left {263}
    static right {262}
    static up {265}
    static down {264}
  
    static q {81}
    static w {87}
    static e {69}
}

class Graphics{
    foreign static draw_sprite(sprite, x, y, rotation) 
    
    foreign static clear_screen()
}

foreign class Sprite{
    construct new(src_x, src_y, tile_width, tile_height, n_tiles) {}
}