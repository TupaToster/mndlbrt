CC=g++
OBJ_ARGS=-lsfml-graphics -lsfml-window -lsfml-system
CC_ARGS=-c -I /usr/include/SFML
CC_SRCS=main.cpp
NOSSE_SRC=nosse.cpp

all: nosse

%.o: %.cpp
	$(CC) $(CC_ARGS) $^ -o $@

nosse: $(NOSSE_SRC:.cpp=.o) $(CC_SRCS:.cpp=.o)
	$(CC) $^ -o $@ $(OBJ_ARGS)

clean:
	rm -rf *.o nosse
	clear