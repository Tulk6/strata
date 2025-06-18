class Texture:
    texture = 0
    def __init__(self):
        Texture.texture = "hello!"

class Image(Texture):
    def __init__(self):
        pass

class Sprite(Image):
    def __init__(self):
        pass