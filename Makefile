
# Very Simple Makefile #
#        Giffi         #
#      For Linux       #

SOURCES := game/engine/*.c game/engine/widgets/*.c game/source/*.cpp
CFLAGS  := -std=c++14 -O2
LIBS    := -lSDL2 -lSDL2_image -lSDL2_ttf
BUILD   := PixelSimulation.AppImage

all:
	g++ $(CFLAGS) $(SOURCES) $(LIBS) -o $(BUILD)

clean:
	rm $(BUILD)
