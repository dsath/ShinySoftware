#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <unistd.h>

#include <stdint.h>
#include <iostream>

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "my.hpp"
#include "box.hpp"

void make_1080(VideoCapture *v) {
  v->set(3, 1920);
  v->set(4, 1080);
}

void make_720p(VideoCapture *v) {
  v->set(3, 1280);
  v->set(4, 720);
}
void make_480(VideoCapture *v) {
  v->set(3, 640);
  v->set(4, 480);
}

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

bool inRange(int low, int high, int val) {
  if (low <= val && val <= high) {
    return true;
  } else {
    return false;
  }
}

void setBox(Mat f, box *b) {
  Mat temp;
  int change = 5;
  namedWindow("window", CV_WINDOW_AUTOSIZE);

  bool done = false;;
  while ( !done ) {
    char c;
    temp = f.clone();
    addBlackBox(&temp, b->col, b->row, b->height);
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



