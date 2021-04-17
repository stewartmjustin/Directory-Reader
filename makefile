#Justin Stewart 1052722 jstewa28@uougleph.ca Assingment 4
CC = gcc
CFLAGS = -I. -std=c99 -Wall -Wpedantic -ansi -lm

FSreport: FSreport.c
	$(CC) -o FSreport FSreport.c $(CFLAGS)

clean:
	rm FSreport