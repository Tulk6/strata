class Game {
    construct init() {
        _code = [""]
        _c_row = 0
        _c_column = 0
    }

    insert_at_cursor(char){
        if (_code[_c_row].count == 0){
            _code[_c_row] = char
        }else{
            _code[_c_row] = _code[_c_row][0.._c_column]+char+_code[_c_row][_c_column+1..-1]
        }
    }

    edit_text(){
        var char = Input.get_char()
        if (char != ""){
            this.insert_at_cursor(char)
            _c_column = _c_column + 1
            System.print(_c_column)
        }

        if (Input.key_pressed(Key.enter)){
            this.insert_at_cursor("\n")
            _c_column = _c_column + 1
        }

        if (Input.key_pressed(Key.tab)){
            this.insert_at_cursor("\t")
            _c_column = _c_column + 1
        }
        
        if (Input.key_pressed(Key.backspace)){
            if (_code[_c_row].count > 1){
                _code[_c_row] = _code[_c_row][0..._c_column]+_code[_c_row][_c_column+1..-1]
            }else{
                _code[_c_row] = ""
            }
            _c_column = _c_column - 1
        }

        if (Input.key_pressed(Key.up)){

        }

        _c_column = _c_column+Input.get_axis_pressed(Key.left, Key.right)
        
        _c_column = _c_column.clamp(0, _code[_c_row].count-1)
    }

    update(delta) {
        this.edit_text()
    }

    draw(delta) {
        Graphics.set_draw_colour(6)
        Graphics.clear_screen()
        Graphics.set_draw_colour(19)

        for (i in 0..._code.count){
            var line = _code[i]
            Graphics.draw_text(line, 0, i*10)
        }
        Graphics.draw_rectangle()
    }
}