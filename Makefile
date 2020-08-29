prog=start
libs=-lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgcodecs
prereq=lib/my.hpp lib/my.cpp lib/TimeState.cpp lib/TimeState.hpp


all: $(prog)

$(prog): $(prog).cpp $(prereq)
	g++ -o $(prog) $< lib/my.cpp lib/TimeState.cpp $(libs) 

clean: 
	rm -f $(prog)

