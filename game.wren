class Game {
    construct init() {
        _character = Sprite.new(0, 0, 16, 16, 10)
        _character_pos = Vector2.new(100, 100)
        _character_rotation = 0
        _character_speed = 100
        _character_turn_rate = 10
    }

    update(delta) {
        /*_character_x = _character_x + ()
        _character_y = _character_y + (Input.get_axis(Key.up, Key.down))*/
        _character_rotation = _character_rotation + Input.get_axis(Key.left, Key.right)*_character_turn_rate
        var step = Vector2.new(0,0)
        step.x = Maths.cos(_character_rotation)*Input.get_axis(Key.down, Key.up)
        step.y = Maths.sin(_character_rotation)*Input.get_axis(Key.down, Key.up)
        step = step * (delta * _character_speed)
        _character_pos = _character_pos + step
    }

    draw(delta) {
        Graphics.clear_screen(0)

        Graphics.draw_sprite(_character, _character_pos.x, _character_pos.y, _character_rotation)
    }
}