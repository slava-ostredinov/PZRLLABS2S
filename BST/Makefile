.PHONY: all clean run

TEST=bst_test
LIBTEST=BSTTest

all: $(TEST)

$(TEST): bst.cpp
	g++ bst.cpp -L. -l$(LIBTEST) -o $(TEST) -std=c++11

run: all
	./$(TEST)

clean:
	rm -rf *.o $(TEST)
