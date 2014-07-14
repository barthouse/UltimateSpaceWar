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

Sprite g_shipSprite = 
{
    { 0, 0 },
    { 0.0f, 0.0f },
    3.0f,
    0.0f
};

float Degrees_to_radians(float degrees)
{
	return (float) (degrees * (2.0f * M_PI) / 360.0f);
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
    float max_velocity = 0.1;
    
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

	// Update sprite with new position
    g_shipSprite.m_position = Float2D(test_ship.m_x, test_ship.m_y);
    g_shipSprite.m_rotation = Degrees_to_radians(test_ship.heading);

	Clear();
    DrawSprite(&g_shipSprite);
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

