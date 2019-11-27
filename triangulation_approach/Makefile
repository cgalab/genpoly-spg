PP = g++
# maybe add -Werror -Wall  
WFLAGS = -Wextra -O3 -Wall -Wno-format -std=c++11

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
	$(PP) $(OBJ) $(UOBJ) -o fpg $(WFLAGS)

all_win: $(UOBJ) $(OBJ)
	$(PP) $(OBJ) $(UOBJ) -o fpg $(WFLAGS)

dir:
	mkdir -p $(ODIR)
	mkdir -p $(UODIR)

test: clean all
	./fpg default.fpg


$(UODIR)/%.o: $(USDIR)/%.cpp
	$(PP) -I$(UIDIR) -c $< -o $@ $(WFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(PP) -I$(UIDIR) -I$(IDIR) -c $< -o $@ $(WFLAGS)

clean:

	rm -f fpg $(ODIR)/*.o $(UODIR)/*.o
	rm -f -d $(ODIR) $(UODIR)
	rm -f *.dat
	rm -f output/*.graphml
