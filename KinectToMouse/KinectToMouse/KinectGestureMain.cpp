/*************************************
Kinect Skeleton
Author: Xin Chen, 2011
Blog:http://blog.csdn.net/chenxin_130
**************************************/
#include <stdlib.h>
#include <iostream>
#include "cv.h"
#include "highgui.h"
#include <XnCppWrapper.h>
// Project header
#include "KinectGesture.h"
#include "Appmessage.h"

// namespace
using namespace std;
using namespace cv;

//use OpenCV 2.1
//#pragma comment (lib,"cv231")
//#pragma comment (lib,"cxcore231")
//#pragma comment (lib,"highgui231")
//#pragma comment (lib,"openNI")


//Generator
xn::GestureGenerator gestureGenerator;
xn::HandsGenerator handsGenerator;
xn::ImageGenerator imageGenerator;

int isRealMouseControl=0;


// output operator for XnPoint3D
ostream& operator<<( ostream& out, const XnPoint3D& rPoint )
{
	out << "(" << rPoint.X << "," << rPoint.Y << "," << rPoint.Z << ")";
	return out;
}

// callback function for gesture recognized
void XN_CALLBACK_TYPE GRecognized( xn::GestureGenerator &generator,
								  const XnChar *strGesture,
								  const XnPoint3D *pIDPosition,
								  const XnPoint3D *pEndPosition,
								  void *pCookie )
{
	cout << strGesture<<" from "<<*pIDPosition<<" to "<<*pEndPosition << endl;
	int imgStartX=0;
	int imgStartY=0;
	int imgEndX=0;
	int imgEndY=0;

	imgStartX=(int)(640/2-(pIDPosition->X));
	imgStartY=(int)(480/2-(pIDPosition->Y));
	imgEndX=(int)(640/2-(pEndPosition->X));
	imgEndY=(int)(480/2-(pEndPosition->Y));

	IplImage* refimage=(IplImage*)pCookie;
	if(strcmp(strGesture,"Wave")==0)
	{
		cvLine(refimage,cvPoint(imgStartX,imgStartY),cvPoint(imgEndX,imgEndY),CV_RGB(255,255,0),6);
		handsGenerator.StartTracking(*pEndPosition);
	}
	else if(strcmp(strGesture,"Click")==0)
	{
		cvCircle(refimage,cvPoint(imgStartX,imgStartY),6,CV_RGB(0,0,255),12);
		if(isRealMouseControl)
		{
			messageHandler(cvPoint(imgStartX,imgStartY),0,REAL_MOUSE_CLICK);
		}
	}
}

// callback function for gesture progress
void XN_CALLBACK_TYPE GProgress( xn::GestureGenerator &generator,
								const XnChar *strGesture,
								const XnPoint3D *pPosition,
								XnFloat fProgress,
								void *pCookie )
{
	//cout << strGesture << ":" << fProgress << " at " << *pPosition << endl;
}


void XN_CALLBACK_TYPE Hand_Create(xn::HandsGenerator& generator, XnUserID nId, const XnPoint3D* pPosition, XnFloat fTime, void* pCookie)
{ 
	//printf("New Hand: %d @ (%f,%f,%f)\n", nId, pPosition->X, pPosition->Y, pPosition->Z);
	addTrackingId(nId);
} 

void XN_CALLBACK_TYPE Hand_Update(xn::HandsGenerator& generator, XnUserID nId, const XnPoint3D* pPosition, XnFloat fTime, void* pCookie)
{
	//printf("Hand %d @ (%f,%f,%f)\n", nId, pPosition->X, pPosition->Y, pPosition->Z);
	int imgPosX=0;
	int imgPosY=0;
	char locationinfo[100];
	imgPosX=(int)(640/2-(pPosition->X));
	imgPosY=(int)(480/2-(pPosition->Y));

	IplImage* refimage=(IplImage*)pCookie;

	cvSetImageROI(refimage,cvRect(40,450,640,30));
	CvFont font;
	cvInitFont( &font, CV_FONT_VECTOR0,1, 1, 0, 3, 5);
	cvSet(refimage, cvScalar(255,255,255));

	if(isRealMouseControl)
	{
		sprintf(locationinfo,"MouseCtrl: %dth HandLoc: %d,%d",nId,(int)pPosition->X,(int)pPosition->Y);
	}
	else
	{
		sprintf(locationinfo,"Normal: %dth HandLoc: %d,%d",nId,(int)pPosition->X,(int)pPosition->Y);
	}

	cvPutText(refimage, locationinfo ,cvPoint(30, 30), &font, CV_RGB(0,0,0));
	cvResetImageROI(refimage);
	CvPoint thisLocation=cvPoint(imgPosX,imgPosY);
	if(isRealMouseControl)
	{
		//cvCircle(refimage,cvPoint(imgPosX,imgPosY),1,CV_RGB(255,0,0),2);
		messageHandler(thisLocation,nId,REAL_MOUSE_MOVE);
	}
	else
	{
		cvCircle(refimage,cvPoint(imgPosX,imgPosY),1,CV_RGB(255,0,0),2);
	}

} 

void XN_CALLBACK_TYPE Hand_Destroy(xn::HandsGenerator& generator, XnUserID nId, XnFloat fTime, void* pCookie)
{
	//printf("Lost Hand: %d\n", nId);
	removeTrackingId(nId);
}

void clearImg(IplImage* inputimg)
{
	CvFont font;
	cvInitFont( &font, CV_FONT_VECTOR0,1, 1, 0, 3, 5);
	memset(inputimg->imageData,255,640*480*3);
	cvPutText(inputimg, "Hand Wave!" , cvPoint(20, 50), &font, CV_RGB(255,255,0));
	cvPutText(inputimg, "Hand Push!" , cvPoint(20, 80), &font, CV_RGB(0,0,255));
}

// main function
void main()
{
	IplImage* drawPadImg=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
	IplImage* cameraImg=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);

	cvNamedWindow("Gesture",1);
	cvNamedWindow("Camera",1);

	clearImg(drawPadImg);

	CvFont font;
	cvInitFont( &font, CV_FONT_VECTOR0,1, 1, 0, 3, 5);

	XnStatus res;
	char key=0;

	// Context
	xn::Context context;
	res = context.Init();
	xn::ImageMetaData imageMD;

	// Generator 
	res = imageGenerator.Create( context ); 
	res = gestureGenerator.Create( context );
	res=handsGenerator.Create(context); 

	// Add gesture
	gestureGenerator.AddGesture( "Wave", NULL );
	gestureGenerator.AddGesture( "Click", NULL );

	// Register callback functions
	XnCallbackHandle gestureCBHandle;
	XnCallbackHandle handsCBHandle;
	gestureGenerator.RegisterGestureCallbacks( GRecognized, GProgress, (void*)drawPadImg, gestureCBHandle );
	handsGenerator.RegisterHandCallbacks(Hand_Create, Hand_Update, Hand_Destroy, (void*)drawPadImg, handsCBHandle);

	// Start generate
	context.StartGeneratingAll();

	res = context.WaitAndUpdateAll();  
	while( (key!=27) && !(res = context.WaitAndUpdateAll())  )
	{  
		res = context.WaitAndUpdateAll();
		imageGenerator.GetMetaData(imageMD);
		memcpy(cameraImg->imageData,imageMD.Data(),640*480*3);
		cvCvtColor(cameraImg,cameraImg,CV_RGB2BGR);

		cvPutText(drawPadImg, "Wave Your Hand to Start Tracking" ,cvPoint(20, 20), &font, CV_RGB(255,0,0));
		cvShowImage("Gesture",drawPadImg);
		cvShowImage("Camera",cameraImg);

		key=cvWaitKey(20);
		switch(key){
				case 'c':
					clearImg(drawPadImg);
					break;
				case 'm'://simulate real mouse
					isRealMouseControl=1-isRealMouseControl;
					break;
				default:
					if(key != -1) printf("You Press %d\n",key);
		}
	}
	cvDestroyWindow("Gesture");
	cvDestroyWindow("Camera");
	cvReleaseImage(&drawPadImg);
	cvReleaseImage(&cameraImg);
	context.StopGeneratingAll();
	context.Shutdown();
}