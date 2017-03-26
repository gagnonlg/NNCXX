CXXFLAGS = -O2 -Wall -Wextra -Werror -std=c++11 -pedantic -pedantic-errors -Weffc++
LIBS = -lgsl -lcblas
OBJ = network rng

all: libnncxx.a

libnncxx.a: $(patsubst %,obj/%.o,$(OBJ))
	ar rvs libnncxx.a $^

obj/%.o: %.cxx %.h | obj
	g++ $(CXXFLAGS) -c -o $@ $< $(LIBS)

obj:
	mkdir -p obj

test_Affine: test_Affine.cxx libnncxx.a
	g++ -O2 -Wall -Wextra -Werror -std=c++11 -pedantic -pedantic-errors -o $@ $^ $(LIBS)

clean:
	rm -rf obj/
	rm -f test_Affine
	rm -f libnncxx.a

