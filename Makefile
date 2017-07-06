CC = gcc
CFLAGS = -I.
LIBS = -lm

TARGET = a.out
OBJS = BitVec.o DaubDWT.o DWTAnlz.o HaarDWT.o Intrpl.o SigAnlz.o


%.o : %.c *%.h
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

$(TARGET) : $(OBJS) prog.o
	$(CC) -o $(TARGET) $^ $(CFLAGS) $(LIBS)

train.out : train.o $(OBJS)
	$(CC) -o train.out $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean :
	rm -f *.o
	rm -f $(TARGET)
	rm -f train.out