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

#include "../lib/my.h"


using namespace cv;
using namespace std;

int main(int argv, char** argc)
{
  
  char c;
  struct box one = {135, 225, 10};
  struct box two = {410, 70, 10};

  const int fps = 20;
  //blue, green, red value for box one
  int *bgr1;
  //blue, green, red value for box two 
  int *bgr2;
 
  Mat frame;
  namedWindow("window", CV_WINDOW_AUTOSIZE);
  VideoCapture vid(0);
  
  //set resolution
  make_480(&vid);


  while(vid.read(frame)) {
    imshow("window", frame);
    if(cvWaitKey(1000/fps) > 0)
      break;
  }

  setBoxI(frame, &one);
  setBoxI(frame, &two);

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
    ofstream file;
    file.open ("data.txt");
    file << one.col << endl << one.row << endl << one.height << endl;
    file << two.col << endl << two.row << endl << two.height << endl;
    file << bgr1[0] << endl << bgr1[1] << endl << bgr1[2] << endl;
    file << bgr2[0] << endl << bgr2[1] << endl << bgr2[2] << endl;
  }

  free(bgr1);
  free(bgr2);
  return EXIT_SUCCESS;
}
