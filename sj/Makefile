PP = g++
WFLAGS = -Wall -Werror -Wextra -g -O3 -DNDEBUG -std=c++11 -DCPUTIME_IN_MILLISECONDS

SDIR = src
ODIR = obj
IDIR = includes

SRC := $(wildcard $(SDIR)/*.cpp)
OBJ := $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SRC))

USDIR := ../util/src
UIDIR := ../util/includes
UODIR := ../util/obj

USRC := $(wildcard $(USDIR)/*.cpp)
UOBJ := $(patsubst $(USDIR)/%.cpp, $(UODIR)/%.o, $(USRC))

all: dir $(UOBJ) $(OBJ)
	$(PP) $(OBJ) $(UOBJ) -o app $(WFLAGS)

dir:
	$(info   OBJECTS is $(OBJ))
	mkdir -p $(ODIR)
	mkdir -p $(UODIR)


$(UODIR)/%.o: $(USDIR)/%.cpp
	$(PP) -I$(UIDIR) -c $< -o $@ $(WFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(PP) -I$(UIDIR) -I$(IDIR) -c $< -o $@ $(WFLAGS)

clean:

	rm -f app $(ODIR)/*.o $(UODIR)/*.o
	rm -f -d $(ODIR) $(UODIR)
