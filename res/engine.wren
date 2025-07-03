class Engine {
    static width {320}
    static height {240}
    static font_w {5}
    static font_h {8}
    static font_a {3}
}

class Input {
    foreign static key_down(key_code)

    foreign static key_pressed(key_code)

    static get_axis(neg_key, pos_key) {
        var neg_int = Input.key_down(neg_key) ? -1 : 0
        var pos_int = Input.key_down(pos_key) ?  1 : 0
        return neg_int+pos_int
    }

    static get_axis_pressed(neg_key, pos_key) {
        var neg_int = Input.key_pressed(neg_key) ? -1 : 0
        var pos_int = Input.key_pressed(pos_key) ?  1 : 0
        return neg_int+pos_int
    }

    foreign static get_char()

    foreign static get_mouse_x()
    foreign static get_mouse_y()
    static get_mouse() {
        return [get_mouse_x(), get_mouse_y()]
    }
    foreign static button_pressed(button_code)
    foreign static button_down(button_code)
}

class Key {
    static f5 {294}
    static backspace {259}
    static delete {261}
    static enter {257}
    static tab {258}
    static left {263}
    static right {262}
    static up {265}
    static down {264}
  
    static q {81}
    static w {87}
    static e {69}
}

class Mouse {
    static none {-1}
    static left {0}
    static right {1}
}

class Graphics {
    foreign static draw_sprite(sprite, x, y, rotation) 
    foreign static draw_image(sprite, x, y, rotation)
    foreign static draw_image_scaled(sprite, x, y, sx, sy, rotation)
    foreign static draw_patch(px, py, pw, ph, x, y, w, h)
    foreign static draw(x, y)

    foreign static draw_rectangle(x, y, w, h)
    foreign static draw_rectangle_lines(x, y, w, h)

    foreign static draw_line(x, y, ex, ey)

    foreign static clear_screen()

    foreign static draw_text(text, x, y)

    foreign static set_draw_colour(col)

    foreign static blit(x, y)

    //foreign static draw_icon(x, y, i)
}

foreign class Sprite {
    construct new(src_x, src_y, tile_width, tile_height, n_tiles) {}
}

class Shape {
    static point_in_rec(x, y, rx, ry, w, h){
        if (x > rx && x < rx+w && y > ry && y < ry+h){
            return true
        }
        return false
    }
}

class UI {
    static button(x,y,w,h){
        if (!Shape.point_in_rec(Input.get_mouse_x(), Input.get_mouse_y(), x, y, w, h)) {
            return false
        }
        var left = Input.button_pressed(Mouse.left)
        var right = Input.button_pressed(Mouse.right)
        if (left == true) { 
            return Mouse.left 
        }
        if (right == true) { 
            return Mouse.right 
        }
        return Mouse.none
    }
}

class Maths {
    static to_deg(x) {
        return (180*x)/Num.pi
    }
    static to_rad(x) {
        return (x*Num.pi)/180
    }
    static sin(x) {
        return Maths.to_rad(x).sin
    }
    static cos(x) {
        return Maths.to_rad(x).cos
    }
}

class Vector2 {
    x{_x}
    y{_y}
    x=(v){_x=v}
    y=(v){_y=v}

    construct new(x,y){
        _x = x
        _y = y
    }

    *(scalar){
        return Vector2.new(_x*scalar,_y*scalar)
    }
    +(vector){
        return Vector2.new(_x+vector.x,_y+vector.y)
    }
}