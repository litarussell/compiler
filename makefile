CC = clang
OUT = ./out
SRC = ./src
TEST = ./test

mode = debug
ifeq ($(mode), debug)
CCFLAGS = -g -c
BUILDFLAG = -g -o
else
CCFLAGS = -c -o3
BUILDFLAG = -o
endif

test-vm: $(OUT)/vm.o $(TEST)/test-vm.c
	${CC} $(BUILDFLAG) $(OUT)/$@ $^ -I $(SRC)/

test-lexer: $(OUT)/lexer.o $(TEST)/test-lexer.c
	${CC} $(BUILDFLAG) $(OUT)/$@ $^ -I $(SRC)/

$(OUT)/lexer.o: $(SRC)/lexer.c $(SRC)/lexer.h
	$(CC) $(CCFLAGS) $< -o $@

$(OUT)/vm.o: $(SRC)/vm.c $(SRC)/vm.h
	$(CC) $(CCFLAGS) $< -o $@

clean:
	rm -r $(OUT)/*

.PHONY: clean