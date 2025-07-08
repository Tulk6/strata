class TextEditor {
    left_margin {5}
    top_margin {5}
    construct new() {
        _code = [""]
        _c_row = 0
        _c_column = 0
    }

    insert_at_cursor(char) {
        if (_code[_c_row].count == 0){
            _code[_c_row] = char
        }else{
            _code[_c_row] = _code[_c_row][0..._c_column]+char+_code[_c_row][_c_column..-1]
        }
    }

    add_newline() {
        _code.insert(_c_row+1, "")
        _code[_c_row+1] = _code[_c_row][_c_column..-1]
        _code[_c_row] = _code[_c_row][0..._c_column]
    }

    get_code_string(){
        return _code.join("\n")
    }

    update() {
        var char = Input.get_char()
        if (char != ""){
            this.insert_at_cursor(char)
            _c_column = _c_column + 1
            System.print(_c_column)
        }

        if (Input.key_pressed(Key.enter)){
            this.add_newline()
            _c_row = _c_row + 1
            _c_column = 0
        }

        if (Input.key_pressed(Key.tab)){
            this.insert_at_cursor("\t")
            _c_column = _c_column + 1
        }
        
        if (Input.key_pressed(Key.backspace)){
            if (_c_column != 0){
                _code[_c_row] = _code[_c_row][0..._c_column-1]+_code[_c_row][_c_column..-1]
                 _c_column = _c_column - 1
            }else if (_c_column==0 && _c_row != 0){
                _c_column = _code[_c_row-1].count
                _code[_c_row-1] = _code[_c_row-1]+_code[_c_row]
                _code.removeAt(_c_row)
                _c_row = _c_row - 1
            }
        }

        _c_row = _c_row+Input.get_axis_pressed(Key.up, Key.down)
        _c_column = _c_column+Input.get_axis_pressed(Key.left, Key.right)
        
        
        _c_row = _c_row.clamp(0, _code.count-1)
        _c_column = _c_column.clamp(0, _code[_c_row].count)
    }

    draw() {
        Graphics.set_draw_colour(6)
        Graphics.clear_screen()
        Graphics.set_draw_colour(7)
        for (i in 0..._code.count){
            var line = _code[i]
            Graphics.draw_text(line, this.left_margin, this.top_margin+i*12)
        }
        Graphics.set_draw_colour(2)
        Graphics.draw_rectangle(_c_column*6+this.left_margin, _c_row*12+this.top_margin, 1, 9)
    }
}

class SpriteEditor {
    construct new() {
        _left_margin = 10
        _top_margin = 10
        _scale = 5
        _crop = 32
        _crop_x = 0
        _crop_y = 0
        _draw_grid = false
        _atlas_x = 0
        _atlas_y = 0
        _atlas_crop = 128
        _atlas_size = 256
        _pri_colour = 0
        _sec_colour = 1
    }

    get_mouse_pixel_x(){
        return ((Input.get_mouse_x()- _left_margin)/_scale).floor
    }
    get_mouse_pixel_y(){
        return ((Input.get_mouse_y()- _top_margin)/_scale).floor
    }

    update() {
        Graphics.set_draw_colour(5)
        
        if ((0..._crop).contains(get_mouse_pixel_x()) && (0..._crop).contains(get_mouse_pixel_y())){
            if (Input.button_down(Mouse.left)){
                Graphics.set_draw_colour(_pri_colour)
                Graphics.blit(this.get_mouse_pixel_x()+_crop_x, this.get_mouse_pixel_y()+_crop_y)
            }else if (Input.button_down(Mouse.right)){
                Graphics.set_draw_colour(_sec_colour)
                Graphics.blit(this.get_mouse_pixel_x()+_crop_x, this.get_mouse_pixel_y()+_crop_y)
            }            
        }


    }

    draw_palette(x, y){
        var swatch_size = 10
        var swatch_bound = swatch_size
        var pen_size = 20

        Graphics.set_draw_colour(6)
        Graphics.draw_rectangle_border(x, y, 8*swatch_bound+2, 4*swatch_bound+2, 7)
        
        for (i in 0...4){
            for (j in 0...8){
                Graphics.set_draw_colour(i*8+j)
                Graphics.draw_rectangle(x+j*swatch_bound+1, y+i*swatch_bound+1, swatch_size, swatch_size)
            }
        }
        
        Graphics.set_draw_colour(_pri_colour)
        Graphics.draw_rectangle_border(x+9*swatch_bound, y, pen_size, pen_size, 7)
        Graphics.set_draw_colour(_sec_colour)
        Graphics.draw_rectangle_border(x+9*swatch_bound, y+pen_size+2, pen_size, pen_size, 7)

        var m_pal_column = ((Input.get_mouse_x()-x)/swatch_bound).floor
        var m_pal_row = ((Input.get_mouse_y()-y)/swatch_bound).floor
        if ((0...8).contains(m_pal_column) && (0...4).contains(m_pal_row)){
            var m_pal = m_pal_row*8+m_pal_column
            if (Input.button_pressed(Mouse.left)) {_pri_colour = m_pal}
            if (Input.button_pressed(Mouse.right)) {_sec_colour = m_pal}
        }
    }

    draw_editor(x, y){
        Graphics.set_draw_colour(6)
        Graphics.draw_rectangle(x, y, _crop*_scale, _crop*_scale)
        Graphics.set_draw_colour(7)
        Graphics.draw_rectangle_lines(x, y, _crop*_scale, _crop*_scale)
        Graphics.draw_patch(_crop_x, _crop_y, _crop, _crop, x, y, _crop*_scale, _crop*_scale)

        Graphics.set_draw_colour(_pri_colour)
        //System.print(get_mouse_pixel_x())
        if ((0..._crop).contains(get_mouse_pixel_x()) && (0..._crop).contains(get_mouse_pixel_y())){
            
            Graphics.draw_rectangle(get_mouse_pixel_x().floor*_scale+x, get_mouse_pixel_y().floor*_scale+y, _scale, _scale)
        }

        Graphics.set_draw_colour(0)
        if (_draw_grid){
            for (i in 0.._crop){
                Graphics.draw_line(x, y+i*_scale, x+_crop*_scale, y+i*_scale)
            }
            for (i in 0.._crop){
                Graphics.draw_line(x+i*_scale, y, x+i*_scale, y+_crop*_scale)
            }
        }
    }

    draw_atlas(x, y){
        Graphics.set_draw_colour(6)
        Graphics.draw_rectangle(x, y, _atlas_crop, _atlas_crop)
        Graphics.set_draw_colour(7)
        Graphics.draw_rectangle_lines(x, y, _atlas_crop, _atlas_crop)
        Graphics.draw_patch(_atlas_x, _atlas_y, _atlas_crop, _atlas_crop, x, y, _atlas_crop, _atlas_crop)

        Graphics.set_draw_colour(5)
        if ((x...x+_atlas_crop).contains(x+(_crop_x)-_atlas_x) && (y...y+_atlas_crop).contains(y+(_crop_y)-_atlas_y)){
            Graphics.draw_rectangle_lines(x+(_crop_x)-_atlas_x, y+(_crop_y)-_atlas_y, _crop, _crop)
        }

        if (UI.button(x, y, _atlas_crop, _atlas_crop) == Mouse.left){
            var px = ((Input.get_mouse_x()-x+_atlas_x)/_crop).floor
            var py = ((Input.get_mouse_y()-y+_atlas_y)/_crop).floor
            _crop_x = px*_crop
            _crop_y = py*_crop
        }


        if (UI.button_icon(Icon.left, x, y+_atlas_crop+5) == Mouse.left){
            _atlas_x = (_atlas_x-_atlas_crop).clamp(0, _atlas_size-_atlas_crop)
        }
        if (UI.button_icon(Icon.right, x+15, y+_atlas_crop+5) == Mouse.left){
            _atlas_x = (_atlas_x+_atlas_crop).clamp(0, _atlas_size-_atlas_crop)
        }

        if (UI.button_icon(Icon.up, x+80, y+_atlas_crop+5) == Mouse.left){
            _atlas_y = (_atlas_y-_atlas_crop).clamp(0, _atlas_size-_atlas_crop)
        }
        if (UI.button_icon(Icon.down, x+95, y+_atlas_crop+5) == Mouse.left){
            _atlas_y = (_atlas_y+_atlas_crop).clamp(0, _atlas_size-_atlas_crop)
        }
        //Graphics.draw_icon(0, 0, 0)
    }

    draw() {
        Graphics.set_draw_colour(8)
        Graphics.clear_screen()
        draw_palette(_left_margin, _top_margin+_crop*_scale+5)
        draw_editor(_left_margin, _top_margin)
        draw_atlas(_crop*_scale+20, _top_margin)
    }
}

class Game {
    mode_code {0}
    mode_paint {1}
    modes {["CODE", "PAINT"]}
    num_modes {2}

    construct init() {
        _text_editor = TextEditor.new()
        _sprite_editor = SpriteEditor.new()

        _mode = this.mode_paint

        //Engine.load_game("etest")
    }

    update(delta) {
        if (Input.key_pressed(Key.tab) && Input.key_down(Key.control)){
            _mode = (_mode+1)%this.num_modes
        }
        if (_mode == this.mode_code){
            _text_editor.update()
        }else if (_mode == this.mode_paint){
            _sprite_editor.update()
        }
        
        if (Input.key_pressed(Key.f5)){
            //System.print(_text_editor.get_code_string())
            Engine.load_game(_text_editor.get_code_string())
        }
    }

    draw_statusbar() {
        var button_width = 40
        var bar_height = 14
        var bar_y = Engine.height-bar_height
        var button_m = 5

        Graphics.set_draw_colour(4)
        Graphics.draw_rectangle(0, bar_y, Engine.width, bar_height)

        for (i in 0...this.num_modes){
            var button_x = i*(button_width+button_m)+2
            Graphics.set_draw_colour(3)
            Graphics.draw_rectangle(button_x, bar_y+1, button_width, bar_height-2)
            Graphics.set_draw_colour(7)
            Graphics.draw_text(this.modes[i], button_x+2, bar_y+3)
        }
    }

    draw(delta) {
        

        if (_mode == this.mode_code){
            _text_editor.draw()
        }else if (_mode == this.mode_paint){
            _sprite_editor.draw()
        }

        draw_statusbar()
    }

}