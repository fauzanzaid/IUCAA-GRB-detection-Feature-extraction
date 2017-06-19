CC = gcc
CFLAGS = -I.
LIBS = -lm

OBJS = prog.o BitVec.o DaubDWT.o DWTAnlz.o HaarDWT.o Intrpl.o SigAnlz.o


%.o : %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

exe : $(OBJS)
	$(CC) -o exe $^ $(CFLAGS) $(LIBS)


.PHONY: clean
clean :
	rm -f *.o
	rm -f exe