CC = gcc
CFLAGS = -std=c99 -lm -lncurses
TARGET = NCurveCalc
OBJECTS = $(TARGET).o clist.o point.o curve.o

$(TARGET) : $(OBJECTS)
		$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o : %.c
		$(CC) $(CFLAGS) -c $<
