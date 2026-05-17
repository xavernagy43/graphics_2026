all:
	gcc -Iinclude/ src/app.c src/camera.c src/main.c src/scene.c src/texture.c src/utils.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lobj -lopengl32 -lm -o game.exe -Wall -Wextra -Wpedantic -lglu32

linux:
	gcc -Iinclude/ src/app.c src/camera.c src/main.c src/scene.c src/texture.c src/utils.c -lobj -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lm -o game -Wall -Wextra -Wpedantic -lglu32
