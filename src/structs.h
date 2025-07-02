struct Atlas {
    Image image;
    Texture texture;
};

struct Palette{
    Color colours[32];
};

struct Sprite {
    Rectangle src;
    int slice_width;
    int slice_height;
    int num_slices;
};

struct SpriteImage {
    Rectangle src;
};