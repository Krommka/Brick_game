CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g -O0
GCOVFLAGS = -fprofile-arcs -ftest-coverage
LDFLAGS = -lgcov
CANON_FLAG = -DCANON

DIST_DIR = $(COMMON_DIR) Doxyfile
TEST_DIR = tests
ARCHIVE_NAME = brickgame.tar.gz

TARGET = tetris
TETRIS = brick_game/tetris
CLI = gui/cli

COMMON_DIR = $(TETRIS) $(CLI)
INSTALL_DIR = build
SRC_DIR = .


SRC_DIRS = $(addprefix $(SRC_DIR)/, $(COMMON_DIR)) $(SRC_DIR)
TEST_DIRS = $(addprefix $(TEST_DIR)/, $(TETRIS)) $(TEST_DIR)

TEST_SRCS = $(wildcard $(addsuffix /*.c, $(TEST_DIRS)))
BACK_SRCS = $(wildcard $(addsuffix /*.c, $(TETRIS)))
FRONT_SRCS = $(wildcard $(addsuffix /*.c, $(CLI)))

OBJS = $(SRCS:.c=.o)
FRONT_OBJS = $(FRONT_SRCS:.c=.o)
BACK_OBJS = $(BACK_SRCS:.c=.o)
TEST_OBJS = $(TEST_SRCS:.c=.o)

OS = $(shell uname)

ifeq ($(OS), Linux)
	CHECKFLAGS = -lcheck -lm -lsubunit 
else
	CHECKFLAGS = -lcheck 
endif

all : install
	./$(INSTALL_DIR)/$(TARGET)

create_directory :
	mkdir -p $(INSTALL_DIR)

install: create_directory $(TARGET)
	install -m755 $(TARGET) $(INSTALL_DIR)/$(TARGET)


$(TARGET) : $(FRONT_OBJS) brick_game.c tetris.a
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

tetris.a: $(BACK_OBJS) 
	ar rc $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(CANON_FLAG) -c $< -o $@

test : $(TEST_OBJS) $(BACK_OBJS)
	$(CC) $^ $(CHECKFLAGS) $(CFLAGS) -o $@

uninstall:
	rm -f $(INSTALL_DIR)/$(TARGET)

dist: clean
	tar -czf $(ARCHIVE_NAME) $(DIST_DIR)

clang:
	@clang-format -i -style=Google **/*.c **/*/*.c *.c *.h **/*.h **/*/*.h

gcov_report:
	$(CC) $(BACK_SRCS) $(TEST_SRCS) $(CHECKFLAGS) $(CFLAGS) $(GCOVFLAGS) $(LDFLAGS) -o report.out
	./report.out
	find ./ -type f -iname '*.gcda'
	lcov -o report.info -c -d .
	rm -rf ./cov-report
	mkdir ./cov-report
	genhtml -o ./cov-report/ report.info
	rm *.gcno *.gcda report.out report.info


dvi: create_directory 
	doxygen Doxyfile

.PHONY: tetris.a install test gcov_report debug clean *.o 

SRC_CLEAN = $(addsuffix /*.o, $(SRC_DIRS))
TEST_CLEAN = $(addsuffix /*.o, $(TEST_DIRS)) $(TEST_DIR)/*.o

clean:
	rm -f $(SRC_CLEAN) $(TEST_CLEAN)  rm *.gcno *.gcda \
	report.out report.info test tetris.a $(TARGET) $(ARCHIVE_NAME) hi_score.txt
	rm -rf build
	rm -rf cov-report