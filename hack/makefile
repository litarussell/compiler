CC = clang

AS_SRC = ./as
AS_OUT = ${AS_SRC}/out
AS_TEST = ${AS_SRC}/test

mode = debug
ifeq ($(mode), debug)
CCFLAGS = -g -c -o
BUILDFLAG = -g -o
else
CCFLAGS = -c -o3
BUILDFLAG = -o
endif

as: $(AS_OUT)/main.o $(AS_OUT)/parser.o $(AS_OUT)/symbol_table.o
	${CC} $(BUILDFLAG) $(AS_OUT)/$@ $^

$(AS_OUT)/%.o: $(AS)/%.c
	${CC} $(CCFLAGS) $@ $^ -I $(AS_SRC)

test:
	$(AS_OUT)/as $(AS_TEST)/Add.asm

clean:
	rm $(AS_OUT)/*

.PHONY: clean test