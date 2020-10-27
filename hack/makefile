CC = clang

AS_SRC = ./as
AS_OUT = ${AS_SRC}/out
AS_TEST = ${AS_SRC}/test

mode = debug
ifeq ($(mode), debug)
CCFLAGS = -std=c11 -g -c -Ddebug -o
BUILDFLAG = -g -o
else ifeq ($(mode), release)
CCFLAGS = -std=c11 -c -o
BUILDFLAG = -o
endif

# $(AS_OUT)/symbol_table.o
as: $(AS_OUT)/main.o $(AS_OUT)/parser.o $(AS_OUT)/lexer.o
	${CC} $(BUILDFLAG) $(AS_OUT)/$@ $^

$(AS_OUT)/%.o: $(AS)/%.c
	${CC} $(CCFLAGS) $@ $^ -I $(AS_SRC)

test:
	$(AS_OUT)/as $(AS_TEST)/Add.asm

clean:
	rm $(AS_OUT)/*

.PHONY: clean test