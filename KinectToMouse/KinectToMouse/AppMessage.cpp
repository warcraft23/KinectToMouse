/*************************************
Kinect Skeleton
Author: Xin Chen, 2011
Blog:http://blog.csdn.net/chenxin_130
**************************************/
#include "AppMessage.h"
#include <windows.h>

/*
#pragma comment (lib,"user32")
*/

//Location and move angle last time for each userId(Hand Id)
CvPoint lastLocation[MAX_HAND_NUM];

int isHandTracking[MAX_HAND_NUM]={0};
int isClickDown=0;

void addTrackingId(int userId)
{
	isHandTracking[userId]=1;
}

void removeTrackingId(int userId)
{
	isHandTracking[userId]=0;
}

CvPoint getLastLocation(int userId)
{
	return lastLocation[userId];
}

void messageHandler(CvPoint &location, int userId,int flag)
{
	//initialize the lastLocation from the location obtained by the first time
	if(lastLocation[userId].x==0&&lastLocation[userId].y==0)
	{
		lastLocation[userId].x=location.x;
		lastLocation[userId].y=location.y;
	}
		if(flag==REAL_MOUSE_CLICK)
		{
			if(!isClickDown)
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
			}
			else {
				mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);		
			}
			isClickDown=1-isClickDown;
		}
		else if(flag==REAL_MOUSE_MOVE)
		{
			int firstHandId=-1;
			for(int i=0;i<MAX_HAND_NUM;i++)
			{
				if(isHandTracking[i]!=0)
				{
					if(firstHandId==-1)
					{
						firstHandId=i;
						break;
					}
				}
			}
			if(abs(location.x-lastLocation[userId].x)<5)
			{
				location.x=lastLocation[userId].x;
			}
			if(abs(location.y-lastLocation[userId].y)<5)
			{
				location.y=lastLocation[userId].y;
			}

			if(userId==firstHandId)
			{
				//mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE,
					//(location.x-320)*65536/640*2,location.y*65536/480*2,0,0);
				mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE,
					(location.x-160)*65536/640*2,(location.y-120)*65536/480*2,0,0);	
			}		
		}
		lastLocation[userId].x=location.x;
		lastLocation[userId].y=location.y;
	
}