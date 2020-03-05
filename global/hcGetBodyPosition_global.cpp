//#include <fstream>
//#include <string>
#include "..\tm\hcGetBodyPosition_tm.h"
#include "hcGetBodyPosition_global.h"
#include ".\readFiles\readFiles.h"

cv::Mat funExampleLocBotBody(string path11, string pathSave, string filesFormat, string tmMethod, float scale, int Tg,dispSaveFlag flagDispSave)
{

	string path1Str(path11);
	vector<string> files; //
	GetAllFormatFiles(path1Str, files,filesFormat);
	int filesSize = files.size(); 
	vector<string> filenames2;
	getFilesName(path11, filenames2 );//path1  pathSM

	//results saved in txt file
	string pathMethodSave,pathTextName;
	pathMethodSave = pathSave+"\\"+tmMethod;
	ofstream foutTxt;
	if(flagDispSave.doTxtSave==true ){
		//save parameters
		pathTextName = pathMethodSave+"\\"+tmMethod+".txt" ;
		foutTxt.open(pathTextName) ;
	}

	if(filenames2.size()==0)
	{
		cout<<"error 1 happened! No file!"<<endl;
		system("pause");
		//return -1;
	}
	int numImg=filenames2.size();
	cout<<"numImg: "<<numImg<<endl;
	string templateName ;
	cv::Mat imgTM ;
	cv::Mat imgSeg2; //the segmentation result of the template
	cv::Mat tmImg;
	cv::Mat imgDispFinalRGB ;
	for (int file_i= 0 ; file_i<filesSize;  file_i++)
	{
		//display for warning
		if(flagDispSave.doDispWarning==true){
			if(file_i%4==0)
				cout<<"***********************file_i: "<<file_i<<endl;
		}

		string b =filenames2[file_i].c_str();
		string b0=files[file_i].c_str();
		//cout<<files[file_i].c_str()<<endl;
		char * testChar=str2char(b);

		int	indexPointStart = b0.find_last_of('\\'); 
		int	indexPointEnd = b0.find_last_of('.'); //b0.find_first_of('.',0);
		char noSuffixName[300];
		for(int i=0, j=indexPointStart+1;j<indexPointEnd;j++,i++){//j<indexPoint-5 for pathSM
			noSuffixName[i]=b0[j];
		}
		noSuffixName[indexPointEnd-indexPointStart-1] = '\0';//input the end char, or error will happen

		//display file name (without the file path) 
		if(flagDispSave.doDisp==true){
			cout<<"file_i: "<<file_i<<endl;
			cout<<"testChar(onlyDisp): "<<testChar<<endl;
		}
		//results saved in txt file
		if(flagDispSave.doTxtSave==true ){
			foutTxt<<"file_i: "<<file_i<<endl;
			foutTxt<<"testChar(onlyDisp): "<<testChar<<endl;	
		}

		string nameImgSrc;
		nameImgSrc=path11+"\\"+noSuffixName+filesFormat;
		string name=filenames2[file_i];
		//the original image
		cv::Mat imgSrc=imread(nameImgSrc,CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat srcDisp;
		//template image
		if(tmMethod=="TMBB" ||tmMethod=="TMGB" ||tmMethod=="TMBBN" ||tmMethod=="TMBBspeed"||tmMethod=="TMGBspeed")
			templateName="_tmB.png";
		else
			templateName="_tmN.png";
		templateName="198"+templateName; //noSuffixName
		imgTM = imread(templateName,CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat tmNeck,tmBody;
		tmNeck = imread("198_tmN.png" ,CV_LOAD_IMAGE_GRAYSCALE); //neck template
		tmBody = imread("198_tmB.png" ,CV_LOAD_IMAGE_GRAYSCALE); //bodyy template
		//float scale = 0.2 ;
		cv::Mat imgSrcOrg;
		imgSrc.copyTo(imgSrcOrg);  //save the original size input image
		Size dsize = Size(cvRound(imgSrc.cols*scale),cvRound(imgSrc.rows*scale));
		resize(imgSrc, imgSrc,dsize);
		resize(imgTM,imgTM,Size(cvRound(imgTM.cols*scale),cvRound(imgTM.rows*scale)));
		//for display
		//transpose(imgSrc, srcDisp);    //此函数是转置、（即将图像逆时针旋转90度，然后再关于x轴对称）
		//flip(srcDisp, srcDisp, 0) ;    //此函数是绕x轴旋转180， 也就是关于x轴对称
		//imshow("srcDisp",srcDisp);
		//get the subimages according to position proir
		cv::Mat imgSubLeft ;
		cv::Mat imgSubRight;
		//obtain ROI , copy
		//imgSrc(Rect(500, 31, 200, 250)).copyTo(imgSubLeft);
		//for the left
		//int leftLeft=1;
		//int leftUp  =160/2;//125  300
		//int leftWidth=428/2;//leftRight= 428;//
		//int leftHeight = 230/2; //leftBottom=390;
		int leftLeft= 1 ; 
		int leftUp  =cvRound(124*scale); //125  300
		int leftWidth=cvRound(428*scale)-leftLeft;//leftRight= 428;//
		int leftHeight = cvRound(1230*scale)-leftUp; //leftBottom=390;
		imgSrc(Rect(leftLeft, leftUp, leftWidth, leftHeight)).copyTo(imgSubLeft) ;  

		int rightLeft   = cvRound(514*scale);
		int rightUp     = cvRound(124*scale)    ;
		int rightWidth =cvRound(966*scale)-rightLeft;
		int rightHeight=cvRound(1219*scale)-leftUp;//rightBottom = 1219;
		imgSrc(Rect(rightLeft, rightUp, rightWidth, rightHeight)).copyTo(imgSubRight) ;
		//for display
		if(flagDispSave.doDisp==true){
			//imshow("imgSrc",imgSrc);
			imshow("imgTM",imgTM);
			imshow("imgSubLeft",imgSubLeft);
			imshow("imgSubRight",imgSubRight);
			//waitKey();
		}
		//00000000000000 step 0: segementation
		cv::Mat imgSeg1,imgSeg3;
		threshold(imgSubRight,imgSeg1, 127, 255, THRESH_OTSU);//阈值分割
		threshold(imgTM,imgSeg2, 127, 255, THRESH_OTSU);//阈值分割
		threshold(imgSubLeft,imgSeg3, 127, 255, THRESH_OTSU);//阈值分割
		//adaptiveThreshold(imgSubLeft,imgSeg2,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,11,2);//自动阈值分割,邻域均值
		//adaptiveThreshold(imgSubRight,imgSeg1,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,11,2);//自动阈值分割，高斯邻域
		//adaptiveThreshold(imgTM,imgSeg2,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,11,2);//自动阈值分割，高斯邻域
		//adaptiveThreshold(imgSubLeft,imgSeg3,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,11,2);//自动阈值分割，高斯邻域

		//1111111111111 step 1: template match 
		cv::Mat leftResMatch,rightResMatch;
		int  leftYclResize,rightYclResize ; //central line in the resize image
		int  leftYcl,rightYcl;  //central in the original size image 
		double leftExetime_TM,rightExetime_TM;

		cv::Mat matchInLeft,matchInRight;
		if(tmMethod=="TMGB" || tmMethod=="TMGN" || tmMethod=="TMGNspeed"|| tmMethod=="TMGBspeed"){
			matchInLeft = imgSubLeft;
			matchInRight= imgSubRight;
			tmImg = imgTM;
		}
		else{
			matchInLeft = imgSeg3;
			matchInRight= imgSeg1;
			tmImg = imgSeg2;
		}
		//for display
		if(flagDispSave.doDisp==true){
			//imshow("imgSrc",imgSrc);
			imshow("matchInLeft",matchInLeft);
			imshow("matchInRight",matchInRight);
			imshow("tmImg",tmImg);
			//waitKey();
		}
		//matching methods: CV_TM_SQDIFF  CV_TM_SQDIFF_NORMED   
		//CV_TM_CCORR  CV_TM_CCORR_NORMED    CV_TM_CCOEFF  CV_TM_CCOEFF_NORMED
		int match_method = CV_TM_CCORR;
		//2: imgSeg3  imgSubLeft  2: imgTM   imgSeg2
		leftYclResize = funGetBodyPosition_tm(matchInLeft,tmImg,leftResMatch,match_method, false,leftExetime_TM);//doDisp  imgSeg1
		leftYclResize=leftLeft+ leftYclResize-1 ; //obtain the central line in the original resize image
		//1: imgSubRight  imgSeg1  2: imgTM  imgSeg2
		rightYclResize = funGetBodyPosition_tm(matchInRight,tmImg,rightResMatch,match_method, false,rightExetime_TM);//doDisp  false
		rightYclResize=rightLeft+ rightYclResize-1 ; //obtain the central line in the original resize image
		leftYcl = cvRound((double)leftYclResize/scale);//obtain the central line in the original input image
		rightYcl = cvRound((double)rightYclResize/scale);

		//displya saved in txt file
		if(flagDispSave.doDisp==true){
			cout<<"leftExetime_TM: "<<leftExetime_TM<<endl;
			cout<<"leftExetime_TM: "<<rightExetime_TM<<endl;
			cout<<"leftYcl: "<<leftYcl<<endl;
			cout<<"rightYcl: "<<rightYcl<<endl;
			//cout<<endl;
		}
		//results saved in txt file
		if(flagDispSave.doTxtSave==true ){
			foutTxt<<"leftExetime_TM: "<<leftExetime_TM<<endl;
			foutTxt<<"rightExetime_TM: "<<rightExetime_TM<<endl;
			foutTxt<<"leftYcl: "<<leftYcl<<endl;
			foutTxt<<"rightYcl: "<<rightYcl<<endl;
			//foutTxt<<endl;
		}

		//display the central and text in the original image
		cv::Mat imgDispFinal;
		imgSrcOrg.copyTo(imgDispFinal); //1: imgSrc (resize image) 2: imgSrcOrg
		//void line(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
		line(imgDispFinal,Point(leftYcl,1) ,Point(leftYcl,imgDispFinal.rows),Scalar(255,255,255),5,CV_AA);
		line(imgDispFinal,Point(rightYcl,1),Point(rightYcl,imgDispFinal.rows),Scalar(255,255,255),5,CV_AA);
		char dispInfo_left[200];
		sprintf(dispInfo_left,"Xcl=%d",leftYcl);
		Point textLocation_left=Point(leftYcl+5,30);
		//cv::Mat imgDispFinalRGB ;
		cvtColor(imgDispFinal,imgDispFinalRGB,CV_GRAY2RGB);
		//void putText( Mat& img, const string& text, Point org, int fontFace,double fontScale，  Scalar color, int thickness=1, int lineType=8 );
		putText(imgDispFinalRGB,dispInfo_left,textLocation_left,FONT_HERSHEY_SIMPLEX,1,Scalar(0,0,255),2,28);

		char dispInfo_right[200]              ;
		sprintf(dispInfo_right,"Xcr=%d",rightYcl) ;
		Point textLocation_right=Point(rightYcl+5,30);
		//cv::Mat imgDispFinalRGB ;
		//cvtColor(imgDispFinal,imgDispFinalRGB,CV_GRAY2RGB);
		//void putText( Mat& img, const string& text, Point org, int fontFace,double fontScale，  Scalar color, int thickness=1, int lineType=8 );
		putText(imgDispFinalRGB,dispInfo_right,textLocation_right,FONT_HERSHEY_SIMPLEX,1,Scalar(0,0,255),2,28);
		if(flagDispSave.doDisp==true){
			imshow("leftResMatch" ,leftResMatch);
			imshow("rightResMatch",rightResMatch);
			//imshow("imgDispFinalRGB2",imgDispFinalRGB);
			//waitKey();
		}

		//----------step 2: precise template matching to obtain the top position of the bottle body.
		cv::Mat filterIn ;
		//int Tg = 150;
		//int medianKernel = 3 ;
		int rectSelfWidth = 30 ;
		cv::Mat imgRectLeft,imgRectRight;
		{
			time_t tBegin=clock();

			imgSrcOrg.copyTo(filterIn); //1: imgSrc  2: imgSrcOrg
			//obtain the new left and right subimages
			//for the left
			int leftLeft= leftYcl-rectSelfWidth;
			int leftUp  = 0; 
			int leftWidth  =2*rectSelfWidth+1;
			int leftHeight = cvRound(filterIn.rows/2) ;
			filterIn(Rect(leftLeft, leftUp, leftWidth, leftHeight)).copyTo(imgRectLeft) ;
			//for the right
			int rightLeft   = rightYcl-rectSelfWidth;
			int rightUp     = 0;
			int rightWidth =2*rectSelfWidth+1;
			int rightHeight=filterIn.rows;
			filterIn(Rect(rightLeft, rightUp, rightWidth, rightHeight)).copyTo(imgRectRight) ;

			cv::Mat filterKern = (Mat_<char>(2, 3) << 0, 3, 0,
				-1, -1, -1); 
			cv::Mat filterResLeft,filterResRight;
			filter2D(imgRectLeft ,filterResLeft ,imgRectLeft.depth() , filterKern); //gradient
			filter2D(imgRectRight,filterResRight,imgRectRight.depth(), filterKern); //gradient
			//cv::Mat neckROIforTM;
			cv::Mat imgSegGradLeft,imgSegGradRight;
			threshold(filterResLeft,imgSegGradLeft, Tg, 255, CV_THRESH_BINARY);//global threshold segmentation
			//medianBlur ( imgSegGradLeft, imgSegGradLeft, medianKernel);//median filter to eliminate salt and pepper noise
			threshold(filterResRight,imgSegGradRight, Tg, 255, CV_THRESH_BINARY);//global threshold segmentation
			//medianBlur ( imgSegGradRight, imgSegGradRight, medianKernel);//median filter to eliminate salt and pepper noise
			//get the top edge points of the bottle body
			//int edgesTop[] = zeros(I_h,1);
			//input: edgesTop  imgSegGradLeft
			//output: bottomTopX
			int bottomTopX_left = funGetTopX(imgSegGradLeft);
			int bottomTopX_right= funGetTopX(imgSegGradRight);

			time_t tEnd = clock();
			time_t exetime_topLocation = tEnd-tBegin;

			//-----------for display
			//display the central and text in the original image
			{
				//void line(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
				line(imgDispFinalRGB,Point(leftYcl-4*rectSelfWidth,bottomTopX_left),Point(leftYcl+4*rectSelfWidth,bottomTopX_left),Scalar(0,0,255),5,CV_AA);
				line(imgDispFinalRGB,Point(rightYcl-4*rectSelfWidth,bottomTopX_right),Point(rightYcl+4*rectSelfWidth,bottomTopX_right),Scalar(0,0,255),5,CV_AA);
				char dispInfo_left[200];
				sprintf(dispInfo_left,"Ycl=%d",bottomTopX_left);
				Point textLocation_left=Point(leftYcl+5,bottomTopX_left-10);
				//void putText( Mat& img, const string& text, Point org, int fontFace,double fontScale，  Scalar color, int thickness=1, int lineType=8 );
				putText(imgDispFinalRGB,dispInfo_left,textLocation_left,FONT_HERSHEY_SIMPLEX,1,Scalar(255,0,0),2,28);
				char dispInfo_right[200] ;
				sprintf(dispInfo_right,"Ycr=%d",bottomTopX_right) ;
				Point textLocation_right=Point(rightYcl+5,bottomTopX_right-10);
				//void putText( Mat& img, const string& text, Point org, int fontFace,double fontScale，  Scalar color, int thickness=1, int lineType=8 );
				putText(imgDispFinalRGB,dispInfo_right,textLocation_right,FONT_HERSHEY_SIMPLEX,1,Scalar(255,0,0),2,28);
			}
			//image result save 
			if(flagDispSave.doSaveImg==true){
				string imgSaveName ;
				imgSaveName=pathMethodSave+"\\"+noSuffixName+".png";
				imwrite(imgSaveName,imgDispFinalRGB);
			}
			if(flagDispSave.doDisp==true){
				cout<<"exetime_topLocation: "<<exetime_topLocation<<endl;
				cout<<"bottomTopX_left: "<<bottomTopX_left<<endl;
				cout<<"bottomTopX_right: "<<bottomTopX_right<<endl;
				cout<<endl;
				imshow("imgRectLeft", imgRectLeft); //subimages
				imshow("imgRectRight", imgRectRight);
				imshow("filterResLeft", filterResLeft); //filter results
				imshow("imgSegGradLeft", imgSegGradLeft);
				imshow("filterResRight", filterResRight); //filter results
				imshow("imgSegGradRight",imgSegGradRight);
				imshow("imgDispFinalRGB2",imgDispFinalRGB);
				waitKey();
			}
			if(flagDispSave.doDispFinal==true){
				cout<<"exetime_topLocation: "<<exetime_topLocation<<" ms"<<endl;
				cout<<"bottomTopX_left: "<<bottomTopX_left<<endl;
				cout<<"bottomTopX_right: "<<bottomTopX_right<<endl;
				cout<<endl;
				imshow("imgDispFinalRGB2",imgDispFinalRGB);
				waitKey();
			}

			//result saved in .txt file
			if(flagDispSave.doTxtSave==true ){
				foutTxt<<"exetime_topLocation: "<<exetime_topLocation<<endl;
				foutTxt<<"bottomTopX_left: "<<bottomTopX_left<<endl;
				foutTxt<<"bottomTopX_right: "<<bottomTopX_right<<endl;
				foutTxt<<endl;
			}

		}

	}

	if(flagDispSave.doTxtSave==true)
		foutTxt.close();//close .txt file
	if(flagDispSave.doSaveImg==true){//save the template image
		string imgSaveName ;
		imgSaveName=pathMethodSave+"\\seg_"+templateName;
		imwrite(imgSaveName,tmImg);//  1: imgSeg2   2: imgTM   =》tmImg
	}

	return  imgDispFinalRGB;
}
