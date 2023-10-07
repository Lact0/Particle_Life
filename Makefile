all:
	g++ -ISDL2/include -LSDL2/lib -o main src/main.cpp src/growth.cpp -lmingw32 -lSDL2main -lSDL2