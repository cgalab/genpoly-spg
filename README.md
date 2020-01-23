# SPG

Steinþór's simple polygon generator.

# Algorithm

(0) generate random pointset with `testdata/random_pnts.py`

(1) compute "random" perturbation on a given pointset

(2) use sweep-line combined with two-opt to resolve intersections


# Compiling

	git clone https://gitlab.cosy.sbg.ac.at/cg/code/spg
	mkdir -p spg/build && cd spg/build
	cmake .. 
	make -j 6

For the `RELEASE` version use `cmake -D CMAKE_BUILD_TYPE=Release ..`

# Run SPG

	./spg -i <pointset_file> -a 2opt -c line -o <output_file>
	./spg -i <pointset_file> -a 6 -c line -o <output_file>

Variants (i) and (iii) described in our EuroCG'20 paper.

# License

Copyright 2019 Steinþór Jasonarson - sjas@cs.sbg.ac.at
SPG is licenced under [GPLv3](https://www.gnu.org/licenses/gpl-3.0.html).
