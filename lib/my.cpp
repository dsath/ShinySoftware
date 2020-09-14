#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <unistd.h>

#include <stdint.h>
#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <chrono>
#include <thread>

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "my.hpp"
#include "box.hpp"
#include "TimeState.hpp"

void make_1080(cv::VideoCapture *v) {
  v->set(3, 1920);
  v->set(4, 1080);
}

void make_720p(cv::VideoCapture *v) {
  v->set(3, 1280);
  v->set(4, 720);
}
void make_480(cv::VideoCapture *v) {
  v->set(3, 640);
  v->set(4, 480);
}

int* getBGR(cv::Mat *f, int sc, int sr, int bh) {
    int *vals = new int[3] {0};

    for (int c = sc; c < sc + bh; ++c) {
      for(int r = sr; r < sr + bh; ++r) {
        //store rgb values of current frame
        vals[0] += f->at<cv::Vec3b>(r, c)[0]; 
        vals[1] += f->at<cv::Vec3b>(r, c)[1]; 
        vals[2] += f->at<cv::Vec3b>(r, c)[2];
      }
    }

    //divide values by how many pixels
    vals[0] /= (bh * bh);
    vals[1] /= (bh * bh);
    vals[2] /= (bh * bh);
    return vals;
}

void addBlackBox(cv::Mat *f, int sc, int sr, int bh) {
    for (int c = sc; c < sc + bh; ++c) {
      for(int r = sr; r < sr + bh; ++r) {
        f->at<cv::Vec3b>(r, c)[0] = 0; 
        f->at<cv::Vec3b>(r, c)[1] = 0; 
        f->at<cv::Vec3b>(r, c)[2] = 0;
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

void setBox(cv::Mat f, box *b) {
  cv::Mat temp;
  int change = 2;
  cv::namedWindow("window", CV_WINDOW_AUTOSIZE);

  bool done = false;;
  while ( !done ) {
    char c;
    temp = f.clone();
    addBlackBox(&temp, b->col, b->row, b->height);
    cv::imshow("window", temp);
    c = cv::waitKey( 0 ); 
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

void setBoxP(cv::Mat f, box *b) {
  cv::Mat temp;
  int *bgr;
  int change = 2;
  cv::namedWindow("window", CV_WINDOW_AUTOSIZE);

  bool done = false;;
  while ( !done ) {
    char c;
    temp = f.clone();
    bgr = getBGR(&temp, b->col, b->row, b->height);
    addBlackBox(&temp, b->col, b->row, b->height);

    std::cout << "BGR: " << bgr[0] << " -- " <<  bgr[1] << " -- " << bgr[2] << std::endl;
    cv::imshow("window", temp);
    c = cv::waitKey( 0 ); 
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

int* getAverageColorFrames(int n, cv::VideoCapture *vc, cv::Mat *f, TimeState *state, int fps) {
  int *ret = new int[3] {0};
  int *temp;
  for(int i = 0; i < n; i++) {
    vc->read(*f);
    temp = getBGR(f, state->CurState[1].col, state->CurState[1].row, state->CurState[1].height);
    ret[0] += temp[0];
    ret[1] += temp[1];
    ret[2] += temp[2];
    addBlackBox(f, state->CurState[1].col, state->CurState[1].row, state->CurState[1].height);
    cv::imshow("window", *f);
    free(temp);
    cv::waitKey(1000 / fps);
  }
  ret[0] = ret[0] / n;
  ret[1] = ret[1] / n;
  ret[2] = ret[2] / n;
  return ret;
}

void showFrames(int n, cv::VideoCapture *vc, cv::Mat *f, int fps) {
  for(int i = 0; i < n; i++) {
    vc->read(*f);
    cv::imshow("window", *f);
    cv::waitKey(1000 / fps);
  }
}

