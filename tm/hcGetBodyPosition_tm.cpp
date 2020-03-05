//#include <fstream>
//#include <string>
#include "hcGetBodyPosition_tm.h"

int  funGetBodyPosition_tm(cv::Mat matchIn,cv::Mat imgTM,cv::Mat &resMatch, int match_method,bool doDisp,double &exetime_TM)
{
		//cv::Mat resMatch;
		//cv::Mat matchIn = imgSubLeft ;
		int  leftYcl ;
		{
			time_t tBegin=clock();
			//matching methods: CV_TM_SQDIFF  CV_TM_SQDIFF_NORMED   
			//CV_TM_CCORR  CV_TM_CCORR_NORMED    CV_TM_CCOEFF  CV_TM_CCOEFF_NORMED
			//int match_method;
			//match_method= CV_TM_CCORR;
			matchTemplate( matchIn, imgTM, resMatch, CV_TM_CCORR);
			normalize( resMatch, resMatch, 0, 1, NORM_MINMAX, -1, Mat() );//normalize
			/// 通过函数 minMaxLoc 定位最匹配的位置
			double minVal; double maxVal; Point minLoc; Point maxLoc;
			Point matchLoc;

			minMaxLoc( resMatch, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

			/// 对于方法 SQDIFF 和 SQDIFF_NORMED, 越小的数值代表更高的匹配结果. 而对于其他方法, 数值越大匹配越好
			if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
			{ matchLoc = minLoc; }
			else
			{ matchLoc = maxLoc; }
			//the central line of the bottle body
			leftYcl= matchLoc.x + cvRound(imgTM.cols/2); //coordinate in the subimage(note that using the matlab coordinates)

			time_t tEnd=clock();
			time_t exetime_TM0=tEnd-tBegin;
			exetime_TM = exetime_TM0 ;
			//for display the matching region
			cv::Mat img_display;
			matchIn.copyTo( img_display );
			//rectangle( img_display, matchLoc, Point( matchLoc.x + imgTM.cols , matchLoc.y + imgTM.rows ), Scalar::all(0), 2, 8, 0 );
			//rectangle(resMatch, matchLoc, Point( matchLoc.x + imgTM.cols , matchLoc.y + imgTM.rows ), Scalar::all(0), 2, 8, 0 );
			//rectangle(img_display, matchLoc, Point( matchLoc.x +50 , matchLoc.y + 100 ), Scalar::all(0), 2, 8, 0 );
			if(doDisp==true){
				cout<<"----------begin: hcGetBodyPosition diaply------------------------------------------------------------"<<endl;
				cout<<"exetime_TM: "<<exetime_TM<<endl;
				cout<<"imgTM.rows: "<<imgTM.rows<<" ; imgTM.cols: "<<imgTM.cols<<endl;
				cout<<"matchIn.rows: "<<matchIn.rows<<" ; matchIn.cols: "<<matchIn.cols<<endl;
				cout<<"resMatch.rows: "<<resMatch.rows<<" ; resMatch.cols: "<<resMatch.cols<<endl;
				cout<<"matchLoc.x + imgTM.cols: "<<matchLoc.x + imgTM.cols<<" ; matchLoc.y + imgTM.rows: "<<matchLoc.y + imgTM.rows<<endl;
				cout<<"leftYcl: "<<leftYcl<<endl;
				cout<<endl;
				cout<<"----------  end: hcGetBodyPosition diaply------------------------------------------------------------"<<endl;
				//cout<<"matchLoc.x + imgTM.cols-scale: "<<(matchLoc.x + imgTM.cols)/scale<<" ; matchLoc.y + imgTM.rows-scale: "<<(matchLoc.y + imgTM.rows)/scale<<endl;
				imshow("resMatch",resMatch);
				imshow( "img_display", img_display );
			}
		}
		return leftYcl;
}

int funGetTopX(cv::Mat imgSegGradLeft)
{
	//input: edgesTop  imgSegGradLeft
	//output: bottomTopX
	vector <int> edgesTop ;
	for(int i=0;i<imgSegGradLeft.cols;i++){  
		for(int j=0;j<imgSegGradLeft.rows;j++){
			uchar *ptr = imgSegGradLeft.ptr<uchar>(j) ;
			if(ptr[i]>0){
				edgesTop.push_back(j);
				break;
			}
		}
	}
	//obtain the minimal value
	int bottomTopX = 999999;
	for(int i=0;i<edgesTop.size();i++){
		if(edgesTop[i]<bottomTopX)
			bottomTopX = edgesTop[i];
	}
	return bottomTopX;
}