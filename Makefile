hrrn : hrrn.c
	gcc -g -o hrrn hrrn.c
edf : edf.c
	gcc -o edf edf.c
clean : 
	rm *.o hrrn
