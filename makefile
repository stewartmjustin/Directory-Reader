#
#	Created by: Justin Stewart
#	Email: stewartm.justin@outlook.com
#
CC = gcc
CFLAGS = -I. -std=c99 -Wall -Wpedantic -ansi -lm

FSreport: FSreport.c
	$(CC) -o FSreport FSreport.c $(CFLAGS)

clean:
	rm FSreport