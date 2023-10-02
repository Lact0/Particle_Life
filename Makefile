all:
	g++ -ISDL2/include -LSDL2/lib -o main src/main.cpp src/quadTree.cpp src/box.cpp src/vectorMath.cpp src/particle.cpp -lmingw32 -lSDL2main -lSDL2