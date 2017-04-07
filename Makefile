CXXFLAGS = -O2 -Wall -Wextra -Werror -std=c++11 -pedantic -pedantic-errors
LIBS = -lgsl -lcblas
OBJ = network rng linalg layers

UNIT_TESTS_TGT = $(patsubst %.cxx, unit_tests/%, $(wildcard unit_tests_*.cxx))

default: libnncxx.a

test: $(UNIT_TESTS_TGT)

unit_tests/%: %.cxx libnncxx.a | unit_tests
	g++ $(CXXFLAGS) -o $@ $^ $(LIBS) -lboost_unit_test_framework

libnncxx.a: $(patsubst %,obj/%.o,$(OBJ))
	ar rvs libnncxx.a $^

obj/%.o: %.cxx %.h | obj
	g++ $(CXXFLAGS) -c -o $@ $< $(LIBS)

obj:
	mkdir -p obj

unit_tests:
	mkdir -p unit_tests

clean:
	rm -rf obj/
	rm -rf unit_tests/
	rm -f unit_tests_linalg
	rm -f libnncxx.a

