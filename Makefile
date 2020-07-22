prog=test
libs=-lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgcodecs

test: test.cpp
	g++ -o $(prog) $< $(libs) 

clean: 
	rm -f $(prog)

