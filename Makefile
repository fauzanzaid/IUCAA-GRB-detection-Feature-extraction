CC = gcc
CFLAGS = -I.
LIBS = -lm

TARGET = a.out
OBJS = prog.o BitVec.o DaubDWT.o DWTAnlz.o HaarDWT.o Intrpl.o SigAnlz.o


%.o : %.c *%.h
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $^ $(CFLAGS) $(LIBS)


.PHONY: clean
clean :
	rm -f *.o
	rm -f $(TARGET)