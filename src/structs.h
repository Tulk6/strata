struct Atlas {
    Image image;
    Texture texture;
};

struct Palette{
    Color colours[32];
};

struct TextLine{
    char* text;
    int size;
};

struct TextBlock{
    struct TextLine* lines;
    int size;
    int c_row;
    int c_column;
};

struct Painter{
    struct Atlas* src;
    Rectangle rect;
};