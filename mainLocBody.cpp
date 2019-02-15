#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>
//#include <fstream>
#include ".\readFiles\readFiles.h"
#include ".\tm\hcGetBodyPosition_tm.h"
#include ".\global\hcGetBodyPosition_global.h"

using namespace std ;
using namespace cv;

#ifdef _DEBUG // choose the proper .lib file
#pragma comment(lib,"botBodyLocDebug32.lib")
#else
#pragma comment(lib,"botBodyLocRelease32.lib")  
#endif


int main()
{
	bool doDisp    = false ;
	bool doTxtSave = false ;  // false  true
	bool doSaveImg = false ;
	bool doDispWarning = false ;
	bool doDispFinal   = true ;

	dispSaveFlag flagDispSave;
	flagDispSave.doDisp=doDisp;
	flagDispSave.doTxtSave=doTxtSave;
	flagDispSave.doSaveImg=doSaveImg;
	flagDispSave.doDispWarning=doDispWarning;
	flagDispSave.doDispFinal  =doDispFinal  ;
	//read all files including the subforder files 
	string path11=".\\inputImages";

	//results including .txt and image save
	string pathSave=".\\detResults";

	string filesFormat = ".png"; //the processed file format: bmp   png
	//TMBB: Template Matching Binary Body
	//our proposed framework: 1: TMBBspeed  2: TMBNspeed 
	string tmMethod ="TMBBspeed";    
	float  scale    = 0.2 ; //resize parameter: r
	int    Tg       = 150;       //segmentation threshold
	cv::Mat imgDispFinalRGB ;

	imgDispFinalRGB= funExampleLocBotBody(path11, pathSave, filesFormat, tmMethod, scale, Tg,flagDispSave);

	system("pause");
	return 0;
}

