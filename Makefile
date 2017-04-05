CXXFLAGS = -O2 -Wall -Wextra -Werror -std=c++11 -pedantic -pedantic-errors
LIBS = -lgsl -lcblas
OBJ = network rng linalg

all: libnncxx.a

unit_tests: unit_tests_linalg

unit_tests_linalg: unit_tests_linalg.cxx libnncxx.a 
	g++ $(CXXFLAGS) -o $@ $^ $(LIBS) -lboost_unit_test_framework

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

