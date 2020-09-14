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
#include "../lib/wp.hpp"

static bool stop = false;
void _stop(int sig) {
  if (sig == SIGUSR1) {
    stop = !(stop);
  }
}

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
 
  cv::Mat frame;
  cv::namedWindow("window", CV_WINDOW_AUTOSIZE);
  cv::VideoCapture vid(std::stoi(argc[1]));
  
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
    cv::imshow("window", frame);
    if(cvWaitKey(1000/fps) > 0)
      break;
  }

  setBox(frame, &one);
  setBox(frame, &two);

  bgr1 = getBGR(&frame, one.col, one.row, one.height);
  addBlackBox(&frame, one.col, one.row, one.height);
  bgr2 = getBGR(&frame, two.col, two.row, two.height);
  addBlackBox(&frame, two.col, two.row, two.height);
  cv::imshow("window", frame);

  std::cout << "one bgr:" << bgr1[0] << " -- " << bgr1[1] << " -- " << bgr1[2] << std::endl;
  std::cout << "one coord:" << one.col << " -- " << one.row<< " -- " << one.height << std::endl;
  std::cout << "two bgr:" << bgr2[0] << " -- " << bgr2[1] << " -- " << bgr2[2] << std::endl;
  std::cout << "two coord:" << two.col << " -- " << two.row << " -- " << two.height << std::endl;
  std::cout << "Press s to save and exit, press anything else to exit" << std::endl;

  c = cv::waitKey(0);
  if(c == 's') {
    std::ofstream coords("coords.txt");
    std::ofstream file;
    std::string filename;
    
    std::cout << "Enter Name of file." << std::endl;
    std::cin >> filename;
    file.open (filename);
    
    //last color recorded coords will be saved
    coords << one.col << std::endl << one.row << std::endl << one.height << std::endl;
    coords << two.col << std::endl << two.row << std::endl << two.height << std::endl;
    file << bgr1[0] << std::endl << bgr1[1] << std::endl << bgr1[2] << std::endl;
    file << bgr2[0] << std::endl << bgr2[1] << std::endl << bgr2[2] << std::endl;
  }

  free(bgr1);
  free(bgr2);
  return EXIT_SUCCESS;
}
