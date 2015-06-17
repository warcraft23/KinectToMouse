#include <stdlib.h>
#include <iostream>
#include <string>

#include <XnCppWrapper.h>

using namespace std;
using namespace xn;

void CheckOpenNIError(XnStatus eResult, string sStatus) {
	if (eResult != XN_STATUS_OK) {
		cerr << sStatus<< "Error:"<<xnGetStatusString(eResult)<<endl;
	}
}

int main(int argc, const char** argv) {
	XnStatus eResult;

	//2. initialize context
	Context mContext;
	eResult = mContext.Init();
	CheckOpenNIError(eResult, "initialize context");

	//3. create depth generator
	DepthGenerator mDepthGenerator;
	eResult = mDepthGenerator.Create(mContext);
	CheckOpenNIError(eResult, "depth generator");

	//4. create image generator
	ImageGenerator mImageGenerator;
	eResult = mImageGenerator.Create(mContext);
	CheckOpenNIError(eResult, "Image Generator");

	//5. set map mode
	XnMapOutputMode mapMode;
	mapMode.nXRes = 640;
	mapMode.nYRes = 480;
	mapMode.nFPS = 30;
	eResult = mDepthGenerator.SetMapOutputMode(mapMode);
	eResult = mImageGenerator.SetMapOutputMode(mapMode);

	//6. correct view port
	mDepthGenerator.GetAlternativeViewPointCap().SetViewPoint(mImageGenerator);

	//7. start generate data
	eResult = mContext.StartGeneratingAll();

	//8. Read Data
	eResult = mContext.WaitNoneUpdateAll();

	if (eResult == XN_STATUS_OK) {
		//9-1 get depth map
		const XnDepthPixel * pDepthMap = mDepthGenerator.GetDepthMap();

		//9-2 get image map
		const XnUInt8 * pImageMap = mImageGenerator.GetImageMap();

	}

	//10. stop
	mContext.StopGeneratingAll();
	//mContext.Shutdown();
	mContext.Release();
	return 0;
}