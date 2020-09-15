prog=setup
libs=-lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgcodecs -lwiringPi

all: setup

setup: setup.cpp ../lib/my.hpp ../lib/my.cpp ../lib/NDS.hpp ../lib/NDS.cpp
	g++ -o $(prog) $< ../lib/my.cpp ../lib/NDS.cpp  $(libs) 
clean: 
	rm -f $(prog)

