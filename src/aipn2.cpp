#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
//#include "std_msgs/Int32MultiArray.h"
#include <iostream>
#include "std_msgs/Int16.h"
//#include <opencv2/highgui/highgui.hpp>
//#include "opencv2/imgproc/imgproc.hpp"
//#include <cv_bridge/cv_bridge.h>

#include <stdlib.h>
//#include <ionmanip>
#include <geometry_msgs/Pose2D.h>
#include <std_msgs/Int32MultiArray.h>
#include <math.h>
#include <string.h>


int Arr[90];
int der=2;
int izq=2;
int cen=2;
int caso=0;
int steer=0;
int steering=0;
int estado=0;
double gar=0.03;
double efr=0.05;
double sal=0.06;
double die=0.07;
double jp=0.1;
double sof=0.14;
double ro=0.2;
double mig=0.26;
double lui=0.27;
double mar=0.4;
double isa=0.8;
double vic=0.9;

/*double c1[]={1,0,0,0,0,0,0};
double c2[]={0,jp,0,jp,0};
double c3[]={0,0,isa,0,isa,0,0};
double c4[]={0,0,0,0,0,0,1};*/
double c1_s1[]={1,0,0,0,0,0};
double c1_s2[]={vic,jp,0,0,0,0,0};
double c1_s3[]={ro,isa,0,0,0,0,0};
double c2_s1[]={mig,die,mig,die,mig,die,0};
double c2_s2[]={gar,lui,sal,lui,sal,lui,gar};
double c2_s3[]={0,die,mig,die,mig,die,mig};
double c3_s1[]={0,mar,jp,mar,jp,0,0};
double c3_s2[]={0,efr,mar,jp,mar,efr,0};
double c3_s3[]={0,0,jp,mar,jp,mar,0};
double c4_s1[]={0,0,0,0,0,isa,ro};
double c4_s2[]={0,0,0,0,0,jp,vic};
double c4_s3[]={0,0,0,0,0,0,1};
double prob[]={sof,sof,sof,sof,sof,sof,sof};

using namespace std;
void arrayCallbackD(const std_msgs::Int32MultiArray& msg)
{
//ROS_INFO_STREAM("HOLA");
der=1;
}

void arrayCallbackI(const std_msgs::Int32MultiArray& msg)
{
//ROS_INFO_STREAM("HOLA1");
izq=1;
}

void arrayCallbackC(const std_msgs::Int32MultiArray& msg)
{
//ROS_INFO_STREAM("HOLA2");
cen=1;
}

void arrayCallbackS(std_msgs::Int16 msg){
	steering=msg.data;
ROS_INFO_STREAM("Steer"<<steering);
}

void multProb(double ant[], double caso[]){
	int n=7;
	for(int i=0; i<n; i++){
	ant[i]=ant[i]*caso[i];
	ROS_INFO_STREAM("Ant[i]"<<ant[i]);
	}	
	}

int defEst(double arr[]){
	int resp=0;
	double max=-1;
	int pos=-1;
	for(int i=0; i<7; i++){
		if(arr[i]>max){
	//ROS_INFO_STREAM("arr[i] "<<arr[i]);
		max=arr[i];
		pos=i;			
		}
	}
	resp=pos+1;
ROS_INFO_STREAM("pos "<<resp);
	return pos;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "arraySubscriber");
	ROS_INFO_STREAM("Empieza el código");
	ros::NodeHandle n;	

	ros::Subscriber sub1 = n.subscribe("points/right", 10, &arrayCallbackD);
	ros::Subscriber sub2 = n.subscribe("points/left", 10, &arrayCallbackI);
	ros::Subscriber sub3 = n.subscribe("points/center", 10, &arrayCallbackC);
	ros::Subscriber sub4 = n.subscribe("manual_control/steering", 10, &arrayCallbackS);

	while(ros::ok){
	
	if(izq==0&&cen==0&&der==1){
		caso=1;
	}
	else{
		if(izq==0&&cen==1&&der==0){
			caso=2;
		}
		else{
			if(izq==1&&cen==0&&der==1){
				caso=3;
			}
			else{
				if(izq==1&&cen==0&&der==0){
					caso=4;
				}
			}
		}
	}
//ROS_INFO_STREAM(caso);

	if(steering>=0&&steering<=60){
		steer=1;
	}
	else{
		if(steering>=61&&steering<=120){
			steer=2;
		}
		else{
			if(steering>=121&&steering<=160){
				steer=3;
			}
		}
	}
ROS_INFO_STREAM(steer);
	
	if(caso==1){
		if(steer==1){
			estado = 1;
		}	
		if(steer==2){
			estado = 1;
		}
		if(steer==3){
			estado = 2;
		}
	}
	else{
		if(caso==2){
			if(steer==1){
				estado = 2;
			}	
			if(steer==2){
				estado = 3;
			}
			if(steer==3){
				estado = 3;
			}
		}	
		else{
			if(caso==3){
				if(steer==1){
					estado = 4;
				}	
				if(steer==2){
					estado = 5;
				}
				if(steer==3){
					estado = 6;
				}
			}	
			else{
				if(caso==4){
					if(steer==1){
						estado = 6;
					}	
					if(steer==2){
						estado = 7;
					}
					if(steer==3){
						estado = 7;
					}
				}
			}
		}
	}

		ROS_INFO_STREAM("Estado: " << estado);
		izq = 0;
		der = 0;
		cen = 0;

		ros::spinOnce();
	}
}

