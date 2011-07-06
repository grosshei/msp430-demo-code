CC=msp430-gcc
CFLAGS=-Os -Wall -g -mmcu=msp430x2012
#CFLAGS=-Os -Wall  -mmcu=msp430x2012

OBJS=sevenseg.o


all: $(OBJS)
	$(CC) $(CFLAGS) -o sevenseg.elf $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -fr sevenseg.elf $(OBJS)
