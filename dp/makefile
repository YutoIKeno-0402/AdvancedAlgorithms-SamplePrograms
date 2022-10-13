PROGRAM=backgammon.exe

all: ${PROGRAM} run

%.exe : %.c
	gcc -o $@ $^

run : $(PROGRAM)
	./$(PROGRAM)

clean :
	rm -f $(PROGRAM)
