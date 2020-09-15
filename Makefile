prog=start
libs=-lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgcodecs -lwiringPi
prereq=lib/my.hpp lib/my.cpp lib/TimeState.cpp lib/TimeState.hpp lib/NDS.cpp lib/NDS.hpp

defines=



all: $(prog)

$(prog): $(prog).cpp $(prereq)
	g++ -std=c++11 -o $(prog) $< lib/my.cpp lib/TimeState.cpp lib/NDS.cpp $(libs) $(defines)

clean: 
	rm -f $(prog)

