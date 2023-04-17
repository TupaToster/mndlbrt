CC=g++
OBJ_ARGS=-lsfml-graphics -lsfml-window -lsfml-system
CC_ARGS=-I /usr/include/SFML
CC_SRCS=main.cpp
NOSSE_SRC=nosse.cpp
SSE_SRC=sse.cpp
AVX_SRC=avx.cpp
OBJDIR=obj/
DEPDIR=dep/

all: $(OBJDIR) $(DEPDIR) nosse

$(OBJDIR)%.o: %.cpp
	$(CC) -M $(CC_ARGS) $< -o $(DEPDIR)$(<:.cpp=.d)
	echo "$(OBJDIR)" > line.tmp
	mv $(DEPDIR)$(<:.cpp=.d) input.tmp
	head -c -1 -q line.tmp input.tmp > $(DEPDIR)$(<:.cpp=.d)
	rm input.tmp line.tmp
	$(CC) -c $(CC_ARGS) $< -o $@

include $(wildcard $(DEPDIR)/*.d)

nosse: $(addprefix $(OBJDIR), $(NOSSE_SRC:.cpp=.o) $(CC_SRCS:.cpp=.o))
	$(CC) $^ -o $@ $(OBJ_ARGS)

$(OBJDIR):
	mkdir $@

$(DEPDIR):
	mkdir $@

clean:
	rm -rf *.o *.d nosse
	rm -rf $(DEPDIR) $(OBJDIR)
	clear

.PHONY: clean all