prog=test
libs=-lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgcodecs -lwiringPi
prereq=../lib/my.hpp ../lib/my.cpp




all: test videoTest

test: test.cpp ../lib/TimeState.cpp ../lib/TimeState.hpp
	g++ -o test test.cpp ../lib/TimeState.cpp

videoTest: videoTest.cpp $(prereq)
	g++ -o videoTest $< ../lib/my.cpp ../lib/TimeState.cpp $(libs) 

checkColor: checkColor.cpp
	g++ -o checkColor $< -lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgcodecs 

clean:
	rm -f test videoTest

