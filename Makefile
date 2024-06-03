CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11
TEST_FLAGS = 
CHECK_FLAGS = -lcheck -lpthread
SRCMODULES = s21_matrix.c extra_functions.c
OBJMODULES = $(SRCMODULES:.c=.o)

ifeq ($(shell uname), Darwin)
# MacOS
    FLAGS += -I /usr/local/include
    OPEN_HTML = open
else
# Linux
    CHECK_FLAGS += -lrt -lsubunit -lm
    OPEN_HTML = xdg-open
endif

ifeq ($(MAKECMDGOALS), $ )
    TEST_FLAGS += -fprofile-arcs -ftest-coverage
else ifeq ($(MAKECMDGOALS), all)
    TEST_FLAGS += -fprofile-arcs -ftest-coverage
else ifeq ($(MAKECMDGOALS), test)
    TEST_FLAGS += -fprofile-arcs -ftest-coverage
else ifeq ($(MAKECMDGOALS), gcov_report)
    TEST_FLAGS += -fprofile-arcs -ftest-coverage
endif

ifeq ($(MAKECMDGOALS), nice_test)
    FLAGS += -D NICE
else ifeq ($(MAKECMDGOALS), nice_man_test)
    FLAGS += -D NICE
else ifeq ($(MAKECMDGOALS), nice_converter_test)
    FLAGS += -D NICE
endif

%.o: %.c %.h s21_matrix.h
	$(CC) $(FLAGS) $(TEST_FLAGS) -c $< -o $@

all: clean s21_matrix.a test gcov_report

s21_matrix.a: $(OBJMODULES)
	ar -rcs $@ $(OBJMODULES)

test: s21_matrix.a tests/*.c tests/*.h
	$(CC) $(FLAGS) -c tests/*.c
	$(CC) $(FLAGS) $(TEST_FLAGS) s21_matrix.a *.o -o $@ $(CHECK_FLAGS)
	./test

test_val: test
	valgrind --tool=memcheck --leak-check=yes -s ./test

gcov_report: clean s21_matrix.a test
	lcov -t "test" -o $(tests_info)test.info -c -d .
	genhtml -o report $(tests_info)test.info
	$(OPEN_HTML) report/index.html

clang:
	cp ../materials/linters/.clang-format .
	clang-format -i *.c *.h */*.c */*.h 
	rm -f .clang-format

clang_check:
	cp ../materials/linters/.clang-format .
	clang-format -n *.c *.h */*.c */*.h
	rm -f .clang-format


clean:
	rm -f *.o *.a *.gcno *.gcda *.info converter_test man_test test .clang-format
	rm -rf report

.PHONY: all clean clang clang_check