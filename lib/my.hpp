#ifndef MY_HPP
#define MY_HPP
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <unistd.h>
#include "box.hpp"
#include "TimeState.hpp"

void make_1080(cv::VideoCapture *v);
void make_720(cv::VideoCapture *v);
void make_480(cv::VideoCapture *v);
int* getBGR(cv::Mat *f, int sc, int sr, int bh);
void addBlackBox(cv::Mat *f, int sc, int sr, int bh);
bool inRange(int low, int high, int val);
void setBox(cv::Mat f, box *b); 
void setBoxP(cv::Mat f, box *b); 
int* getAverageColorFrames(int n, cv::VideoCapture *vc, cv::Mat *f, TimeState *state, int fps);  
void showFrames(int n, cv::VideoCapture *vc, cv::Mat *f, int fps);
#endif
