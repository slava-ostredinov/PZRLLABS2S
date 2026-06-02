.PHONY: all clean run

TEST=hash_test
LIBTEST=HashTest

all: $(TEST)

$(TEST): hash.cpp
	g++ hash.cpp -L. -l$(LIBTEST) -o $(TEST) -std=c++11

run: all
	./$(TEST)

clean:
	rm -rf *.o $(TEST)
