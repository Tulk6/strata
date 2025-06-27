CC = gcc

STD = c99

CFLAGS = -Wall -g

TARGET = main.exe

LIBS = -lraylib -lwinmm -lgdi32

SRC = src/main.c
MODS = src/graphics.c src/interface.c src/sprite.c src/input.c src/palette.c src/engine.c src/writer.c

all: $(TARGET)

$(TARGET): $(SRC) $(MODS)
	$(CC) -std=$(STD) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

run: $(TARGET)
	./$(TARGET)

profile: $(TARGET)
	$(CC) -std=$(STD) -g -pg -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm $(TARGET)
