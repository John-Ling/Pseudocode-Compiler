CXX=g++
CFLAGS=-I.
DEPS = compiler.h token.h lexer.h constants.h parser.h
OBJ = main.o compiler.o lexer.o parser.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o