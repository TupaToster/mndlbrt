CC=g++
OBJ_ARGS=-lsfml-graphics -lsfml-window -lsfml-system
CC_ARGS=-c -I /usr/include/SFML
CC_SRCS=main.cpp

all: mndlbrt.out

%.o: %.cpp
	$(CC) $(CC_ARGS) $^ -o $@

mndlbrt.out: $(CC_SRCS:.cpp=.o)
	$(CC) $(^:.cpp=.o) -o $@ $(OBJ_ARGS)

clean:
	rm -rf *.o *.out*
