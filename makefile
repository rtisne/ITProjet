TESTS_SOURCES=$(wildcard tests/test_*.c)
TESTS=$(TESTS_SOURCES:.c=)

CPPFLAGS=-g -ggdb -O0 -std=c11 -Wall -Werror -I.
CFLAGS=-fPIC -ggdb -I. 
LDFLAGS= -lm

PATH := /opt/local/bin:$(PATH)

all: libautomate.a

check: test
	for i in $(TESTS); do \
	    echo -n "$$i ... "; ( \
	       { /bin/bash -c "$$i"; } &> log && echo -e "\033[32mPASS\033[0m" \
	    ) || ( \
	        echo -e "\033[31mFAIL\033[0m" && echo "" && cat log && echo "" \
	    ); \
	done

checkmemory: test
	for i in $(TESTS); do \
	    echo -n "$$i ... "; ( \
	       { /bin/bash -c "valgrind --error-exitcode=1 --leak-check=full --errors-for-leak-kinds=definite,indirect,possible $$i"; } &>log && echo -e "\033[32mMEMORY PASS\033[0m" \
	    ) || ( \
	        echo -e "\033[31mMEMORY FAIL\033[0m" && echo "" && cat log && echo "" \
	    ); \
	done

test:
	echo "$(TESTS)" |sed -e "s#\([^ ]*\) *#\1: \1.o libautomate.a\n#g" > tests.mk
	make test_2
test_2: $(TESTS)

-include tests.mk

scan.c: scan.l parse.h
	flex scan.l

scan.h: scan.l
	flex scan.l

parse.c: parse.y scan.h
	/opt/local/bin/bison parse.y

parse.h: parse.y
	/opt/local/bin/bison parse.y

libautomate.a: libautomate.a(automate.o table.o ensemble.o avl.o fifo.o outils.o scan.o parse.o rationnel.o)

clean:
	-rm -f scan.c scan.h parse.c parse.h
	-rm -rf *.o
	-rm -rf *.a
	-rm -rf *.mk
	-rm -rf tests/*.o
	-rm -rf $(TESTS)

.PHONY: all clean check checkmemory test 
