CXX=g++
CFLAGS=-I.
DEPS = compiler.h token.h lexer.h constants.h 
OBJ = main.o compiler.o lexer.o token.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o