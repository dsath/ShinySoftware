FILE = receive
DEVICE = atmega128 
COMPILE = avr-gcc -Wall -Os -mmcu=$(DEVICE) 

default: compile upload

compile: $(FILE).c
	$(COMPILE) $(FILE).c -o $(FILE).hex


upload: $(FILE).hex
	avrdude -c usbasp -p $(DEVICE) -U flash:w:$(FILE).hex

clean:
	rm -f $(FILE).hex
