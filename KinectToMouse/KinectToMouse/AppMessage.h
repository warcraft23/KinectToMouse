/*************************************
Kinect Skeleton
Author: Xin Chen, 2011
Blog:http://blog.csdn.net/chenxin_130
**************************************/
#include "cv.h"

#define REAL_MOUSE_MOVE 1
#define REAL_MOUSE_CLICK 2

#define MAX_HAND_NUM 999


void addTrackingId(int userId);

void removeTrackingId(int userId);

CvPoint getLastLocation(int userId);

void messageHandler(CvPoint &location, int userId, int flag);
