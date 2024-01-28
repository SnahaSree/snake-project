all:
g++ -I src/include -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image


g++ -I src/include -L src/lib -o main new.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image


g++ -I src/include -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer


g++ -I src/include -L src/lib -o test test.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer