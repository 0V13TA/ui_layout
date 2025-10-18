CC = g++
STD = c++17
LINKS = -lsfml-graphics -lsfml-window -lsfml-system

all: main.cpp
	${CC} -std=${STD} main.cpp -o build/app.o ${LINKS}

