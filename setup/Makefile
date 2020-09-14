prog=setup
libs=-lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgcodecs -lwiringPi

all: $(prog).cpp ../lib/my.hpp ../lib/my.cpp ../lib/wp.hpp ../lib/wp.cpp
	g++ -o $(prog) $< ../lib/my.cpp ../lib/wp.cpp  $(libs) 
clean: 
	rm -f $(prog)

