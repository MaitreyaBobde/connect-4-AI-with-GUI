# output: game.o
# 	g++ -I src/include -L src/lib -o build\\game game.cpp -lSDL2main -lSDL2 -lSDL_ttf

# game.o:
# 	g++ -c game.cpp

all:
	g++ -I src/include -L src/lib -o build/game main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf