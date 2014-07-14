// TransformPoints.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <assert.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <Windows.h>

#include "Framework.h"

int g_simulationTime = 0;
FILE * g_logFile = NULL;

typedef struct
{
	float m_velocity_x;
	float m_velocity_y;
	float m_x;
	float m_y;
	float m_rotation;
	float heading;
	float m_acceleration_x;
	float m_acceleration_y;
	bool m_shoot;
} Ship;

Ship test_ship = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };


typedef struct 
{
	float	m_x;
	float	m_y;
} Point;

void AddPoints(Vertex2D * a, Vertex2D * b, Vertex2D * result)
{
	result->m_x = a->m_x + b->m_x;
	result->m_y = a->m_y + b->m_y;
}

//Calculate the translation of X
float Translation_X(float X, float translate_x)
{
	float final_x_value = 0;
	
	final_x_value = X + translate_x;
	
	return final_x_value;
}

//calculate the translation of Y
float Translation_Y(float Y,float translate_y)
{
	
	float final_y_value = 0;

	final_y_value = Y + translate_y;

	return final_y_value;
}

//calculate the rotation of X
void RotatePoint(float rotate, Vertex2D * in, Vertex2D * out)
{
	out->m_x = in->m_x * cos(rotate) - in->m_y * sin(rotate);
	out->m_y = in->m_x * sin(rotate) + in->m_y * cos(rotate);
}

float Rotation_X(float X, float Y,float rotate)
{
	float new_point_x = 0;
	
	new_point_x = X * cos(rotate) - Y * sin(rotate);
	
	return new_point_x;
}

//calculate the rotaion of Y
float Rotation_Y(float X, float Y, float rotate)
{
	float new_Y_point = 0;
	

	new_Y_point = X * sin(rotate) + Y * cos(rotate);
	
	return new_Y_point;
}

float Degrees_to_radians(float degrees)
{
	return (float) (degrees * (2.0f * M_PI) / 360.0f);
}

void TransformVertex(float rotation, Vertex2D * translation, Vertex2D * in, Vertex2D * out)
{
	Vertex2D rotatedPoint;

	RotatePoint(rotation, in, &rotatedPoint);
	AddPoints(&rotatedPoint, translation, out);
}

void TransformPoints(float rotation, Vertex2D * translation, Vertex2D * in, Vertex2D * out, int count)
{
	while(count-- > 0) 
	{
		TransformVertex(rotation, translation, in++, out++);
	}
}

void PrintPoints(Vertex2D * in, int count)
{
	while(count-- > 0) 
	{
		printf("(%f, %f)\n", in->m_x, in->m_y);
		in++;
	}
}



bool SimulateAndDraw(void)
{
	static DWORD64 lastTick = 0;

	DWORD64 nowTick = __rdtsc();

	if (lastTick != 0)
	{
		DWORD64 elapsedTick = nowTick - lastTick;

		if (elapsedTick < 10000000)
			return true;
	}

	lastTick = nowTick;

	bool continueRunning = true;
	float max_acceleration = 0.001;
	float max_rotation = 1.0;
	float max_velocity = 0.1;
	float border_size = 90.0;

	fprintf(g_logFile, "simulating time = %d\n", g_simulationTime);

	bool keyState[eKey_count];
	

	GetKeys(keyState);

	
	
	float acceleration = 0;

	test_ship.m_rotation = 0;
	
	// Matthew writes his code here
	if(keyState[eKey_w])
	{
		acceleration = max_acceleration;
	}
	if(keyState[eKey_s])
	{
		acceleration = -max_acceleration;
	}
	if(keyState[eKey_a])
	{
		test_ship.m_rotation = max_rotation;
	}
	if(keyState[eKey_d])
	{
		test_ship.m_rotation = -max_rotation;
	}
	if(keyState[eKey_space])
	{
		test_ship.m_shoot = true;
	}
	test_ship.m_acceleration_x = acceleration * cos(Degrees_to_radians(test_ship.heading));
	test_ship.m_acceleration_y = acceleration * sin(Degrees_to_radians(test_ship.heading));
	
	test_ship.m_velocity_x = test_ship.m_velocity_x + test_ship.m_acceleration_x;
	test_ship.m_velocity_y = test_ship.m_velocity_y + test_ship.m_acceleration_y;

	test_ship.heading = test_ship.heading + test_ship.m_rotation;

	
	

	if(test_ship.heading < 0)
	{
		test_ship.heading = test_ship.heading + 360;
	}
	if(test_ship.heading >= 360)
	{
		test_ship.heading = test_ship.heading -360;
	}

#if 0
	// TODO: need to fix how we handle max velocity
	if(test_ship.m_velocity_x > max_velocity)
	{
		test_ship.m_velocity_x = max_velocity;
	}
	if(test_ship.m_velocity_x < -max_velocity)
	{
		test_ship.m_velocity_x = -max_velocity;
	}
	if(test_ship.m_velocity_y > max_velocity)
	{
		test_ship.m_velocity_y = max_velocity;
	}
	if(test_ship.m_velocity_y < -max_velocity)
	{
		test_ship.m_velocity_y = -max_velocity;
	}
#endif
	 
	test_ship.m_x = test_ship.m_x + test_ship.m_velocity_x;
	test_ship.m_y = test_ship.m_y + test_ship.m_velocity_y;

	if(test_ship.m_x >= border_size|| test_ship.m_x <= -border_size || test_ship.m_y >= border_size || test_ship.m_y <= -border_size)
	{
		test_ship.m_velocity_x = test_ship.m_velocity_y = 0;
	}
		
	fprintf(g_logFile, "acceleration = %f , %f\n ship volocity %f , %f\n", test_ship.m_acceleration_x, test_ship.m_acceleration_y, test_ship.m_velocity_x, test_ship.m_velocity_y);
	printf("\n\tPosition of ship %f , %f\n \t testship heading %f\n ship velocity %f , %f\n", test_ship.m_x , test_ship.m_y , test_ship.heading , test_ship.m_velocity_x, test_ship.m_velocity_y);

	Vertex2D translation;

	translation.m_x = test_ship.m_x;
	translation.m_y = test_ship.m_y;

	float ship_size = 3.0;
	Vertex2D list_of_verticies[6] = {
		{-ship_size, ship_size, 0, 0} , 
		{ ship_size, ship_size, 0.25, 0} ,
		{ -ship_size, -ship_size, 0, 0.25} , 
		
		{ ship_size, ship_size, 0.25, 0}, 
		{ ship_size, -ship_size, 0.25, 0.25,},
		{ -ship_size, -ship_size, 0, 0.25}};

	Vertex2D new_list_of_vertecies[6];

	TransformPoints(Degrees_to_radians(test_ship.heading), &translation , list_of_verticies, new_list_of_vertecies, 6);
	for (int i = 0; i < 6; i++)
	{
		new_list_of_vertecies[i].m_u = list_of_verticies[i].m_u;
		new_list_of_vertecies[i].m_v = list_of_verticies[i].m_v;
	}

	Clear();
	DrawVertices(new_list_of_vertecies, 6);
	Present();

	if (keyState[eKey_x]) continueRunning = false;

	g_simulationTime++;

	return continueRunning;
}

void InitializeSimulation(void)
{
	fopen_s(&g_logFile, "UltimateSpaceWar.txt", "w+");
	assert(g_logFile != NULL);
}

void ExitSimulation(void)
{
	fclose(g_logFile);
}

#if 0
int main(int argc, char* argv[])
{
	/*
	Vertex originalPoints[3] = {{10, 0}, {-5, 3}, {6, -7}};
	Vertex translation = { 4, -2 };
	Vertex new_points[3];

	printf("First set of numbers\n");
	TransformPoints(Degrees_to_radians(32), &translation, originalPoints, new_points, 3);
	PrintPoints(new_points, 3);
	
	printf("\nSecond set of numbers\n");
	TransformPoints(Degrees_to_radians(180), &translation, originalPoints, new_points, 3);
	PrintPoints(new_points, 3);
	*/


	test_ship.m_velocity_x = 0;
	test_ship.m_rotation = 0;

	while(SimulateAndDraw()) 
		; // do nothing

	fclose(g_logFile);
		

	return 0;
}
#endif

