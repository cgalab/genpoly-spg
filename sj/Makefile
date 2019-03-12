PP = g++
SDIR = src
UDIR = ../util
ODIR = obj
WFLAGS = -Wall -Wextra -Werror

_DEP = %.cpp
DEP = $(patsubst %, $(SDIR)/%, $(_DEP))

_OBJ = main.o inits.o io.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

_UTIL = points.cpp
UTIL = $(patsubst %, $(UDIR)/%, $(_UTIL))

_UOBJ = points.o
UODIR = $(patsubst %, $(UDIR)/%, $(ODIR))
UOBJ = $(patsubst %, $(UODIR)/%, $(_UOBJ))

_INCLUDES = basicDefinitions.h
INCLUDES =$(patsubst %, $(UDIR)/%, $(_INCLUDES))

MKDIR_P = mkdir -p

.PHONY: all
all: dirs spg

.PHONY: dirs
dirs: $(ODIR) $(UODIR)

$(ODIR):
	$(MKDIR_P) $(ODIR)

$(UODIR):
	$(MKDIR_P) $(UODIR)

$(UODIR)/%.o: $(UTIL)
	$(PP) -c -o $@ $< $(WFLAGS)

obj/%.o: $(DEP)
	$(PP) -c -o $@ $< $(WFLAGS)

spg: $(OBJ) $(UOBJ) $(INCLUDES)
	$(PP) -o $@ $^ $(WFLAGS)


.PHONY: clean
clean:
	rm -f $(ODIR)/*.o $(UODIR)/*.o $(UDIR)/*.o *~ core $(INCDIR)/*~ 

