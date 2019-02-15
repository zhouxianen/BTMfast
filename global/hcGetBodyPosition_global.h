#pragma once
#ifndef hcGetBodyPosition_global_H
#define hcGetBodyPosition_global_H

#include<iostream>
#include <vector>
#include<io.h>
#include <fstream>
#include <time.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
 
struct dispSaveFlag{
	bool doDisp;
	bool doTxtSave      ;  // false  true
	bool doSaveImg      ;
	bool doDispWarning  ;
	bool doDispFinal    ;
};

cv::Mat funExampleLocBotBody(string path11, string pathSave, string filesFormat, string tmMethod, float scale, int Tg,dispSaveFlag flagDispSave) ;
 

#endif