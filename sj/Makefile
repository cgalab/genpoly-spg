PP = g++
SDIR = src
ODIR = obj
WFLAGS = -Wall -Wextra -Werror

_DEP = %.cpp
DEP = $(patsubst %,$(SDIR)/%,$(_DEP))

_OBJ = main.o inits.o points.o io.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

.PHONY: all
all: dirs spg

MKDIR_P = mkdir -p

.PHONY: dirs
dirs: $(ODIR)

$(ODIR):
	$(MKDIR_P) $(ODIR)

obj/%.o: src/%.cpp $(DEP)
	$(PP) -c -o $@ $< $(WFLAGS)

spg: $(OBJ)
	$(PP) -o $@ $^ $(WFLAGS)


.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
