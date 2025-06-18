import pyray as pr
import sprite

texture = sprite.Texture()
sprite = sprite.Sprite()

print(sprite.texture)

pr.init_window(400, 400, 'test')

while not pr.window_should_close():
    pr.begin_drawing()

    pr.end_drawing()

pr.close_window()