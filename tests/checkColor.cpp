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
#include <sys/types.h>
#include <signal.h>

#include "../lib/box.hpp"

#define buttonA 0
#define buttonStart 2
#define buttonSelect 3

using namespace cv;
using namespace std;


int* getBGR(Mat *f, int sc, int sr, int bh) {
    int *vals = new int[3] {0};

    for (int c = sc; c < sc + bh; ++c) {
      for(int r = sr; r < sr + bh; ++r) {
        //store rgb values of current frame
        vals[0] += f->at<Vec3b>(r, c)[0]; 
        vals[1] += f->at<Vec3b>(r, c)[1]; 
        vals[2] += f->at<Vec3b>(r, c)[2];
      }
    }

    //divide values by how many pixels
    vals[0] /= (bh * bh);
    vals[1] /= (bh * bh);
    vals[2] /= (bh * bh);
    return vals;
}

void addBlackBox(Mat *f, int sc, int sr, int bh) {
    for (int c = sc; c < sc + bh; ++c) {
      for(int r = sr; r < sr + bh; ++r) {
        f->at<Vec3b>(r, c)[0] = 0; 
        f->at<Vec3b>(r, c)[1] = 0; 
        f->at<Vec3b>(r, c)[2] = 0;
      }
    }
}
void setBoxP(Mat f, box *b) {
  Mat temp;
  int *bgr;
  int change = 2;
  namedWindow("window", CV_WINDOW_AUTOSIZE);

  bool done = false;;
  while ( !done ) {
    char c;
    temp = f.clone();
    bgr = getBGR(&temp, b->col, b->row, b->height);
    addBlackBox(&temp, b->col, b->row, b->height);

    std::cout << "BGR: " << bgr[0] << " -- " <<  bgr[1] << " -- " << bgr[2] << std::endl;
    imshow("window", temp);
    c = waitKey( 0 ); 
    switch(c) {
      case 119:
          *b = {b->col, b->row - change, b->height};
          break;
      case 115:
          *b = {b->col, b->row + change, b->height};
          break;
      case 97:
          *b = {b->col - change, b->row, b->height};
          break;
      case 100:
          *b = {b->col + change, b->row, b->height};
          break;
      default:
          done = true;
          break;
      }
  }
}
int main(int argv, char** argc)
{
  const int boxSize = 5;
  struct box one;
  

  Mat frame;
  namedWindow("window", CV_WINDOW_AUTOSIZE);
  frame = imread("assets/colors1.png", 1);
  one = {frame.cols/2, frame.rows/2, boxSize};
  
  setBoxP(frame, &one);

  frame = imread("assets/colors2.png", 1);
  one = {frame.cols/2, frame.rows/2, boxSize};

  setBoxP(frame, &one);
  return EXIT_SUCCESS;
}
