PP = g++
SDIR = src
UDIR = ../util
ODIR = obj
WFLAGS = -Wall -Wextra -Werror

_DEP = %.cpp
DEP = $(patsubst %, $(SDIR)/%, $(_DEP))

_OBJ = main.o inits.o io.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

_UTIL = points.o
UTIL = $(patsubst %, $(UDIR)/%, $(_UTIL))

UODIR = $(patsubst %, $(UDIR)/%, $(ODIR))

MKDIR_P = mkdir -p

.PHONY: all
all: dirs dirs1 util spg

.PHONY: dirs
dirs: $(ODIR)
$(info    ODIR is $(ODIR))
$(ODIR):
	$(MKDIR_P) $(ODIR)

.PHONY: dirs1
dirs1: $(UODIR)
$(info    UODIR is $(UODIR))
$(UODIR):
	$(MKDIR_P) $(UODIR)

obj/%.o: src/%.cpp $(DEP)
	$(PP) -c -o $@ $< $(WFLAGS)

util: $(UTIL)
	$(PP) -o $@ $^ $(WFLAGS)

spg: $(OBJ)
	$(PP) -o $@ $^ $(WFLAGS)


.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

