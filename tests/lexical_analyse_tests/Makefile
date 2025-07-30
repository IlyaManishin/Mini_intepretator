RESULT := tokenizer_test
SRC_DIR := ../../lexical_analyse
INCLUDE_DIR := ../../include
CFLAGS := -Wall -I$(INCLUDE_DIR)

OBJS := tokenizer_error.o tokenizer.o

TESTS_FILE := tests.c
TESTS_OBJ := ${TESTS_FILE:.c=.o}

all: $(RESULT)

tokenizer_error.o: ${SRC_DIR}/tokenizer_error.c $(wildcard $(INCLUDE_DIR)/*.h)
	gcc $(CFLAGS) -c $< -o $@

tokenizer.o: ${SRC_DIR}/tokenizer.c $(wildcard $(INCLUDE_DIR)/*.h)
	gcc $(CFLAGS) -c $< -o $@

${TESTS_OBJ}: ${TESTS_FILE} $(wildcard $(INCLUDE_DIR)/*.h)
	gcc ${CFLAGS} -c $< -o $@

$(RESULT): $(OBJS) ${TESTS_OBJ}
	gcc $(CFLAGS) $(OBJS) ${TESTS_OBJ} -o $(RESULT)

clean:
	rm -f *.o $(RESULT)
