class Game{
    construct init(){
        _character = Sprite.new(0, 0, 16, 16, 10)
        _character_x = 0
        _character_y = 0
    }

    update(delta){
        _character_x = _character_x + (Input.get_axis(Key.left, Key.right))
        _character_y = _character_y + (Input.get_axis(Key.up, Key.down))
    }

    draw(delta){
        Graphics.clear_screen()

        Graphics.draw_sprite(_character, _character_x, _character_y, 0)
    }
}