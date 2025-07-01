class TextEditor {
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
            }else if (_c_column==0){
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
        Graphics.set_draw_colour(0)
        for (i in 0..._code.count){
            var line = _code[i]
            Graphics.draw_text(line, 0, i*12)
        }
        Graphics.set_draw_colour(2)
        Graphics.draw_rectangle(_c_column*6, _c_row*12, 1, 9)
    }
}

class SpriteEditor {
    construct new() {

    }

    update() {

    }

    draw() {

    }
}

class Game {
    mode_code {0}
    mode_paint {1}
    num_modes {2}

    construct init() {
        _text_editor = TextEditor.new()
        _sprite_editor = SpriteEditor.new()

        _mode = this.mode_code
    }

    update(delta) {
        if (Input.key_pressed(Key.tab)){
            _mode = (_mode+1)%this.num_modes
        }
        if (_mode == this.mode_code){
            _text_editor.update()
        }else if (_mode == this.mode_paint){
            _sprite_editor.update()
        }
        
    }

    draw_statusbar() {
        Graphics.set_draw_colour(4)
        Graphics.draw_rectangle(0, Engine.height-14, Engine.width, 14)
    }

    draw(delta) {
        Graphics.set_draw_colour(1)
        Graphics.clear_screen()

        if (_mode == this.mode_code){
            _text_editor.draw()
        }else if (_mode == this.mode_paint){
            _sprite_editor.draw()
        }
        
        draw_statusbar()
    }
}