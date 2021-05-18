CC = gcc
CFLAGS = -g -Wall
OBJS1 = hrrn_process.o hrrn_queue.o scheduling_io.o hrrn.o
OBJS2 = edf.o
TARGET = hrrn edf

all: $(TARGET)

hrrn: $(OBJS1)
	$(CC) -o $@ $(OBJS1)

edf: $(OBJS2)
	$(CC) -o $@ $(OBJS2)

clean: 
	rm -f *.o
	rm -f $(TARGET)
