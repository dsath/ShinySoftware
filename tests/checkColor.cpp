#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <unistd.h>

#include <stdint.h>
#include <iostream>


#include <stdio.h>
#include <fcntl.h>
#include <fstream>
#include <string>
#include <wiringPi.h>
#include <sys/types.h>
#include <signal.h>

#include "../lib/my.hpp"
#include "../lib/box.hpp"

#define buttonA 0
#define buttonStart 2
#define buttonSelect 3

static bool stop = false;
void _stop(int sig) {
  if (sig == SIGUSR1) {
    stop = !(stop);
  }
}

using namespace cv;
using namespace std;

int main(int argv, char** argc)
{
  const int boxSize = 5;
  wiringPiSetup();
  initButtons();
  
  char c;
  struct box one = {135, 225, boxSize};
  struct box two = {410, 70, boxSize};

  const int fps = 20;
  //blue, green, red value for box one
  int *bgr1;
  //blue, green, red value for box two 
  int *bgr2;
 
  Mat frame;
  namedWindow("window", CV_WINDOW_AUTOSIZE);
  VideoCapture vid(stoi(argc[1]));
  
  //set resolution
  make_480(&vid);

  

  signal(SIGUSR1, _stop);
  //start A pressing process
  pid_t pid = fork();
  if(pid == -1) {
    printf("Error when forking");
  }
  else if (pid == 0) {
    while(1) {
      //continuously press A until signal received
      while(!_stop); 
      pressA();
    }
  }

  softReset();
  while(vid.read(frame)) {
    imshow("window", frame);
    if(cvWaitKey(1000/fps) > 0)
      break;
  }

  setBox(frame, &one);
  setBox(frame, &two);

  bgr1 = getBGR(&frame, one.col, one.row, one.height);
  addBlackBox(&frame, one.col, one.row, one.height);
  bgr2 = getBGR(&frame, two.col, two.row, two.height);
  addBlackBox(&frame, two.col, two.row, two.height);
  imshow("window", frame);

  cout << "one bgr:" << bgr1[0] << " -- " << bgr1[1] << " -- " << bgr1[2] << std::endl;
  cout << "one coord:" << one.col << " -- " << one.row<< " -- " << one.height << std::endl;
  cout << "two bgr:" << bgr2[0] << " -- " << bgr2[1] << " -- " << bgr2[2] << std::endl;
  cout << "two coord:" << two.col << " -- " << two.row << " -- " << two.height << std::endl;
  cout << "Press s to save and exit, press anything else to exit" << std::endl;

  c = waitKey(0);
  if(c == 's') {
    ofstream coords("coords.txt");
    ofstream file;
    string filename;
    
    std::cout << "Enter Name of file." << endl;
    std::cin >> filename;
    file.open (filename);
    
    //last color recorded coords will be saved
    coords << one.col << endl << one.row << endl << one.height << endl;
    coords << two.col << endl << two.row << endl << two.height << endl;
    file << bgr1[0] << endl << bgr1[1] << endl << bgr1[2] << endl;
    file << bgr2[0] << endl << bgr2[1] << endl << bgr2[2] << endl;
  }

  free(bgr1);
  free(bgr2);
  return EXIT_SUCCESS;
}
