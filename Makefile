all:
	mkdir -p build
	g++ main.cpp -msse2 -O3 -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net -o build/game
