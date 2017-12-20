#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "gazebo_msgs/LinkStates.h"
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"

//Autores: Isabel Díaz, Rodrigo De Carcer, Victoria Medina y Juan Pablo Castillo.

//Definimos algunas variables auxiliares para el procesamiento.
//Rate de actualización.
double rate_hz = 10; 
float PI = 3.14159265;
float angulo = 0, distancia = 0; 
float x0= 0; 
float ye0= 0;
float aux = 0;

//Vectores para guardar la información recibida de los sensores.
float lecturaRadar[360];
float angulos[360];

//Arreglos en que guardamos los resultados del cálculo de la posición del segundo robot.
float robotPose[2];
float robotVelocity[2];

//Variables necesarias para utilizar el Filtro de Kalman.
float lastP = 0.1;
float R = 0.1; 
float medidaAnteriorPos[2] = {0,0};
float medidaAnteriorVel[2] = {0,0};


/*Definimos las funciones que procesan la información recibida.
void lidarCallback(sensor_msgs::LaserScan);
void divideAngulos();
void infoOtroCoche();
void filtroKalman();*/

//Callback del subscriber.
void lidarCallback(sensor_msgs::LaserScan lidar_from_mini){
	for (int i = 0; i< 360; i++){
		lecturaRadar[i] = lidar_from_mini.ranges[i];
	}
}

//Función que obtiene la posición del otro robot dentro del ambiente.
void infoOtroCoche(){
	//Para cada uno de los puntos, si está dentro de un rango, lo tomamos en cuenta.
	for (int i = 0; i < 360; i++){
		if(lecturaRadar[i] > .4 && lecturaRadar[i] < 5){
			angulo = angulos[i];
			distancia = lecturaRadar[i]; 
		}
	}

	robotPose[0] = distancia*cos(angulo*180/PI);
	robotPose[1] = distancia*sin(angulo*180/PI);

	//La velocidad del otro robot es calculada en base a la distancia que recorrió de la última medición a esta.
	ye0 = (robotPose[1] - ye0)/rate_hz;
	x0 = (robotPose[0] - x0)/rate_hz;

	robotVelocity[0] = x0;
	robotVelocity[1] = ye0;
}

//Función que aplica el filtro de Kalman para la información obtenida en el método anterior.
void filtroKalman(){
	float medidaPos[2];
	float medidaVel[2]; 

	medidaPos[0] = medidaAnteriorPos[0];
	medidaPos[1] = medidaAnteriorPos[1];
	medidaVel[0] = medidaAnteriorVel[0];
	medidaVel[1] = medidaAnteriorVel[1];


	float auxPos[2];
	float auxVel[2];
	float actualP = lastP; 
	float gKalman = actualP /(actualP + R);  

	auxPos[0] = robotPose[0]; 
	auxPos[1] = robotPose[1]; 
	auxVel[0] = robotVelocity[0]; 
	auxVel[1] = robotVelocity[1]; 
	
	float x1, y1, nuevaY0, nuevaX0, nuevaP;

	nuevaP = (1-gKalman)*actualP; 
	x1 = medidaPos[0] + gKalman*(auxPos[0] - medidaPos[0]);
	y1 = medidaPos[1] + gKalman*(auxPos[1] - medidaPos[1]);
	nuevaX0 = medidaVel[0] + gKalman*(auxVel[0] - medidaVel[0]);
	nuevaY0 = medidaVel[1] + gKalman*(auxVel[1] - medidaVel[1]);

	medidaAnteriorPos[0] = x1;
	medidaAnteriorPos[1] = y1;
	medidaAnteriorVel[0] = nuevaX0;
	medidaAnteriorVel[1] = nuevaY0;

	lastP = nuevaP; 
}

int main(int argc, char **argv){
	ros::init(argc, argv, "Segundo ejercicio");
	ros::NodeHandle nh;

	ros::Subscriber getLidarInfo = nh.subscribe("/AutoNOMOS_mini_1/laser_scan", 1000, &lidarCallback);
	ros::Rate rate(rate_hz);

	robotVelocity[0] = x0; 
	robotVelocity[1] = ye0; 

	for(int i = 0; i< 360; i++){
		angulos[i] = aux; 
		aux += 0.0175019223243; 
	}

	while(ros::ok()){
		infoOtroCoche(); 
		filtroKalman(); 
		
		ROS_INFO_STREAM("Posición medida: (" << medidaPos[0] << ", " << medidaPos[1] << ")");
		ROS_INFO_STREAM("Velocidad medida: (" << medidaVel[0] << ", " << medidaVel[1] << ")");
		ros::spinOnce();
		rate.sleep();
	}

	return 0; 
}
