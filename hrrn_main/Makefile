EXEC = hrrn
CC = gcc
CFLAGS = -Wall

$(EXEC): hrrn.o
	$(CC) -o ../hrrn hrrn.o -L../hrrn_lib -l_hrrn_func

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ -I../hrrn_include

clean: 
	rm hrrn.o ../hrrn
