# Compile: tests
all: tests

.PHONY: tests
tests:
	$(MAKE) -C tests all

# Εκτέλεση
run: run-tests

run-tests:
	$(MAKE) -C tests run

# Εκτέλεση με valgrind
valgrind: valgrind-tests

valgrind-tests:
	$(MAKE) -C tests valgrind

# Εκκαθάριση
clean:
	$(MAKE) -C tests clean
