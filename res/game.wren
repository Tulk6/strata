class Game {
    construct init() {
        _code = ""
        _cursor_index = 0
    }

    insert_at_cursor(char){
        if (_code.count == 0){
            _code = char
        }else{
            _code = _code[0.._cursor_index]+char+_code[_cursor_index+1..-1]
        }
    }

    edit_text(){
        var char = Input.get_char()
        if (char != ""){
            this.insert_at_cursor(char)
            _cursor_index = _cursor_index + 1
        }

        if (Input.key_pressed(Key.enter)){
            this.insert_at_cursor("\n")
            _cursor_index = _cursor_index + 1
        }

        if (Input.key_pressed(Key.tab)){
            this.insert_at_cursor("\t")
            _cursor_index = _cursor_index + 1
        }
        
        if (Input.key_pressed(Key.backspace)){
            if (_code.count > 1){
                _code = _code[0..-2]
            }else{
                _code = ""
            }
            _cursor_index = _cursor_index - 1
        }

        if (Input.key_pressed(Key.up)){

        }

        _cursor_index = _cursor_index+Input.get_axis_pressed(Key.left, Key.right)
        
        _cursor_index = _cursor_index.clamp(0, _code.count-1)
    }

    update(delta) {
        this.edit_text()
    }

    draw(delta) {
        Graphics.clear_screen(19)
        var disp_code
        if (_code.count > 0){
            disp_code = _code[0.._cursor_index]+"|"+_code[_cursor_index+1..-1]
        }else{
            disp_code = "|"
        }
        var lines = disp_code.split("\n")
        for (i in 0...lines.count){
            var line = lines[i]
            Graphics.draw_text(line, 0, i*10)
        }
    }
}