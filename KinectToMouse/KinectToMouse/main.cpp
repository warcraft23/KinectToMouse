#include <stdlib.h>
#include <iostream>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <XnCppWrapper.h>
#include "KinectGesture.h"
#include "Appmessage.h"

using namespacestd;
using namespacecv;

//Generator
xn::GestureGenerator gestureGenerator;
xn::HandsGenerator handsGenerator;
xn::ImageGenerator imageGenerator;
int isRealMouseControl=0;

//¡¾1¡¿
// main function
void main()
{
	IplImage* drawPadImg=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
	IplImage* cameraImg=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);

	cvNamedWindow("Gesture",1);
	cvNamedWindow("Camera",1);

	clearImg(drawPadImg);

	CvFont font;
	cvInitFont( &font,CV_FONT_VECTOR0,1, 1, 0, 3, 5);

	XnStatus res;
	char key=0;

	&nbsp;// Context
	xn::Context context;
	res = context.Init();
	xn::ImageMetaData imageMD;

	// Generator
	res = imageGenerator.Create( context);
	res = gestureGenerator.Create( context);
	//¡¾2¡¿
	res=handsGenerator.Create(context);

	// Add gesture
	gestureGenerator.AddGesture("Wave", NULL);
	gestureGenerator.AddGesture("Click", NULL);

	// Register callback functions
	XnCallbackHandle gestureCBHandle;
	XnCallbackHandle handsCBHandle;
	gestureGenerator.RegisterGestureCallbacks(GRecognized, GProgress,(void*)drawPadImg,gestureCBHandle );
	//¡¾3¡¿
	handsGenerator.RegisterHandCallbacks(Hand_Create, Hand_Update,Hand_Destroy, (void*)drawPadImg, handsCBHandle);

	// Start generate
	context.StartGeneratingAll();
	res = context.WaitAndUpdateAll(); 
	while( (key!=27) && !(res = context.WaitAndUpdateAll())  )
	{ 
		res = context.WaitAndUpdateAll();
		imageGenerator.GetMetaData(imageMD);
		memcpy(cameraImg->imageData,imageMD.Data(),640*480*3);
		cvCvtColor(cameraImg,cameraImg,CV_RGB2BGR);
		cvPutText(drawPadImg,"Wave Your Hand to Start Tracking",cvPoint(20, 20), &font, CV_RGB(255,0,0));
		cvShowImage("Gesture",drawPadImg);
		cvShowImage("Camera",cameraImg);
		key=cvWaitKey(20);
		switch(key){
		case 'c':
			clearImg(drawPadImg);
			break;
			//¡¾4¡¿
		case 'm'://simulate real mouse
			isRealMouseControl=1-isRealMouseControl;
			break;
		default:
			if(key != -1) printf("You Press%d\n",key);
		}
	}
	cvDestroyWindow("Gesture");
	cvDestroyWindow("Camera");
	cvReleaseImage(&drawPadImg);
	cvReleaseImage(&cameraImg);
	context.StopGeneratingAll();
	context.Shutdown();
}