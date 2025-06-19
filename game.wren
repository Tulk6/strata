class Game{
    construct init(){
        _character = Sprite.new()
        _character_x = 0
        _character_y = 0
    }

    update(){
        _character_x = _character_x + (input.key_down(Key.right)-input.key_down(Key.left))
        _character_y = _character_y + (input.key_down(Key.up)-input.key_down(Key.down))
    }

    draw(){
        Graphics.draw_sprite(_character, _character_x, _character_y, 0)
    }

    screen(){

    }
}