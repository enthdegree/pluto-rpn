CC = gcc
CFLAGS  = -g -Wall 
LIBS = -lm

default: mrpn

mrpn: main.o calc.o mc.o
	$(CC) $(CFLAGS) -o mrpn main.o calc.o mc.o $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c 

mc.o: mc.c mc.h
	$(CC) $(CFLAGS) -c mc.c 

calc.o: calc.c calc.h
	$(CC) $(CFLAGS) -c calc.c

clean: 
	$(RM) mrpn *.o *~

