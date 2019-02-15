#pragma once
#ifndef hcGetBodyPosition_tm_H
#define hcGetBodyPosition_tm_H

#include<iostream>
#include <vector>
#include<io.h>
#include <fstream>
#include <time.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
 
int  funGetBodyPosition_tm(cv::Mat matchIn,cv::Mat imgTM,cv::Mat &resMatch,int match_method, bool doDisp,double &exetime_TM);
int funGetTopX(cv::Mat imgSegGradLeft);
 

#endif