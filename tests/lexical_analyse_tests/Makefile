RESULT := main
BASE_DIR := ../..
SRC_DIR := ${BASE_DIR}/ast_parser/lexical_analyze
INCLUDE_DIR := ${BASE_DIR}/include
CFLAGS := -Wall -g -I $(INCLUDE_DIR)

OBJS := tokenizer_api.o tokenizer.o token_buffer.o files.o parser_errors.o 

TESTS_FILE := tests.c
TESTS_OBJ := ${TESTS_FILE:.c=.o}

all: $(RESULT)

tokenizer_api.o: ${SRC_DIR}/tokenizer_api.c $(wildcard $(INCLUDE_DIR)/*.h) ${SRC_DIR}/tokenizer_api.h
	gcc $(CFLAGS) -c $< -o $@

tokenizer.o: ${SRC_DIR}/tokenizer.c $(wildcard $(INCLUDE_DIR)/*.h) ${SRC_DIR}/tokenizer.h
	gcc $(CFLAGS) -c $< -o $@

token_buffer.o: ${SRC_DIR}/token_buffer.c ${SRC_DIR}/token_buffer.h $(wildcard $(INCLUDE_DIR)/*.h)
	gcc $(CFLAGS) -c $< -o $@

files.o: ${BASE_DIR}/system_tools/files.c $(wildcard $(INCLUDE_DIR)/*.h)
	gcc $(CFLAGS) -c $< -o $@

parser_errors.o: ${BASE_DIR}/utils/parser_errors.c $(wildcard $(INCLUDE_DIR)/*.h)
	gcc $(CFLAGS) -c $< -o $@

${TESTS_OBJ}: ${TESTS_FILE} $(wildcard $(INCLUDE_DIR)/*.h)
	gcc ${CFLAGS} -c $< -o $@

$(RESULT): $(OBJS) ${TESTS_OBJ}
	gcc $(CFLAGS) $(OBJS) ${TESTS_OBJ} -o $(RESULT)

clear:
	rm -f *.o $(RESULT)
