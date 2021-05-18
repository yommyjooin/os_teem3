hrrn : hrrn.c
	gcc -o hrrn hrrn.c

edf : edf.c
	gcc -o edf edf.c

clean:
	rm edf
	rm hrrn
