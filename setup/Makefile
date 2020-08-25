prog=setup
libs=-lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgcodecs

all: $(prog).cpp ../lib/my.h ../lib/my.cpp
	g++ -o $(prog) $< ../lib/my.cpp  $(libs) 
clean: 
	rm -f $(prog)

