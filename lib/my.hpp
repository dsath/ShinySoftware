#ifndef MY_HPP
#define MY_HPP
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <unistd.h>
#include "box.hpp"

using namespace cv;
using namespace std;


void make_1080(VideoCapture *v);
void make_720(VideoCapture *v);
void make_480(VideoCapture *v);
int* getBGR(Mat *f, int sc, int sr, int bh);
void addBlackBox(Mat *f, int sc, int sr, int bh);
bool inRange(int low, int high, int val);
void setBox(Mat f, box *b); 
void pressA();
void softReset();
void initButtons();
int* getAverageColorFrames(int n, VideoCapture *vc, Mat *f); 
void showFrames(int n, VideoCapture *vc, Mat *f); 
#endif
