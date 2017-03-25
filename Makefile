all: network.o test_Affine

network.o: network.cxx
	g++ -O2 -Wall -Wextra -Werror -std=c++11 -pedantic -pedantic-errors -c -o $@ $<

rng.o: rng.cxx
	g++ -O2 -Wall -Wextra -Werror -std=c++11 -pedantic -pedantic-errors -c -o $@ $<

test_Affine: test_Affine.cxx network.o rng.o
	g++ -O2 -Wall -Wextra -Werror -std=c++11 -pedantic -pedantic-errors -o $@ $^ -lgsl -lcblas

clean:
	rm -f *.o test_Affine

