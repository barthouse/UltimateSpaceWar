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
	float m_x;
	float m_y;
	float m_heading;
	float m_velocity_x;
	float m_velocity_y;
	bool alive;

}Bullet;

Bullet player_bullet = { 0, 0, 0, 0, 0, false };

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
	bool alive;
} Ship;

Ship test_ship = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, false, true};

Ship Enemy_Ship = { 0.0, 0.0, 10.0, 10.0, 0.0, 0.0, 0.001, 0.0, false, true };

Sprite g_shipSprite = 
{
    { 0, 0 },
    { 0.0f, 0.0f },
    3.0f,
    0.0f
};

Sprite Enemy_Sprite =
{
	{ 3, 0 },
	{ 0.0f, 0.0f },
	6.0f,
	0.0f
};

Sprite Bullet_Sprite =
{
	{ 1, 0 },
	{ 0.0f, 0.0f },
	2.0f,
	0.0f
};

Sprite Explosion =
{
	{ 2, 0 },
	{ 0.0f, 0.0f },
	10.0f,
	0.0f
};

float Degrees_to_radians(float degrees)
{
	return (float) (degrees * (2.0f * M_PI) / 360.0f);
}

// test_ship.m_acceleration_x = acceleration * cos(Degrees_to_radians(test_ship.heading));
void update_position_ship(Ship * ship, float acceleration, float ship_heading)
{
	//calculating the new acceleration of the ship

	ship->m_acceleration_x = acceleration * cos(Degrees_to_radians(ship->heading));
	ship->m_acceleration_y = acceleration * sin(Degrees_to_radians(ship->heading));

	//Updating the ship velocity
	ship->m_velocity_x = ship->m_velocity_x + ship->m_acceleration_x;
	ship->m_velocity_y = ship->m_velocity_y + ship->m_acceleration_y;

	ship->heading = ship->heading + ship_heading;

	if(ship->heading < 0)
		ship->heading = ship->heading + 360;

	if(ship->heading >= 360)
		ship->heading = ship->heading - 360;

	ship->m_x = ship->m_x + ship->m_velocity_x;
	ship->m_y = ship->m_y + ship->m_velocity_y;
}


void update_position_Bullet(Bullet * bullet)
{
	bullet->m_velocity_x = 1 * cos(Degrees_to_radians(bullet->m_heading));
	bullet->m_velocity_y = 1 * sin(Degrees_to_radians(bullet->m_heading));

	//update position of the bullet
	bullet->m_x = bullet->m_x + bullet->m_velocity_x;
	bullet->m_y = bullet->m_y + bullet->m_velocity_y;

}

void Enemy_AI(Ship * E_ship)
{
	
	

}

bool bullet_hit_test(Ship * ship, Bullet * bullet)
{

	if (bullet->m_x <= (ship->m_x + 3.5) && bullet->m_x >= (ship->m_x - 3.5) && bullet->m_y <= (ship->m_y + 3.5) && bullet->m_y >= (ship->m_y - 3.5) && ship->alive == true)
	{

		Explosion.m_position = Float2D(ship->m_x, ship->m_y);
		Explosion.m_rotation = Degrees_to_radians(ship->heading);
		

		bullet->alive = false;

		ship->alive = false;

		ship->m_velocity_x = 0;
		ship->m_velocity_y = 0;
		
		return true;
	}
	return false;
}

int bomb_count = 0;
bool drawExplosion = false;
int shoot_count_player = 0;


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
	
	float border_size = 90.0;

	fprintf(g_logFile, "simulating time = %d\n", g_simulationTime);


	test_ship.m_rotation = 0;


	bool keyState[eKey_count];

	GetKeys(keyState);
	
	
	
	
	// Get input from user in what direction they want to go.
	
	float acceleration = 0;
	float max_acceleration = 0.001;
	float max_rotation = 1.0;


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
	

	if (test_ship.m_shoot == true)
	{
		if (player_bullet.alive == false && shoot_count_player >= 170)
		{
			player_bullet.m_heading = test_ship.heading;
			player_bullet.m_x = test_ship.m_x;
			player_bullet.m_y = test_ship.m_y;
			
			player_bullet.alive = true;
			shoot_count_player = 0;
		}
		else
			;//do nothing
		
		test_ship.m_shoot = false;
	}

	shoot_count_player++;
	

	update_position_ship(&test_ship, acceleration, test_ship.m_rotation);

	Enemy_AI(&Enemy_Ship);
	
	if (player_bullet.alive == true)
		update_position_Bullet(&player_bullet);
	
	
	//testing if the ship is outside the side of the map
	if(test_ship.m_x >= border_size|| test_ship.m_x <= -border_size || test_ship.m_y >= border_size || test_ship.m_y <= -border_size)
	{
		test_ship.m_velocity_x = test_ship.m_velocity_y = 0;
	}
	

	//testing if bullet is hitting the edge of the map
	if (player_bullet.m_x >= border_size || player_bullet.m_x <= -border_size || player_bullet.m_y >= border_size || player_bullet.m_y <= -border_size)
		player_bullet.alive = false;


	//test if bullet hits enemy ship

	if (bullet_hit_test(&Enemy_Ship, &player_bullet))
		drawExplosion = true;

	fprintf(g_logFile, "acceleration = %f , %f\n ship volocity %f , %f\n", test_ship.m_acceleration_x, test_ship.m_acceleration_y, test_ship.m_velocity_x, test_ship.m_velocity_y);
	printf("\n\tPosition of ship %f , %f\n \t testship heading %f\n ship velocity %f , %f\n", test_ship.m_x , test_ship.m_y , test_ship.heading , test_ship.m_velocity_x, test_ship.m_velocity_y);

	// Update sprite with new position
    g_shipSprite.m_position = Float2D(test_ship.m_x, test_ship.m_y);
    g_shipSprite.m_rotation = Degrees_to_radians(test_ship.heading);

	
	Bullet_Sprite.m_position = Float2D(player_bullet.m_x, player_bullet.m_y);
	Bullet_Sprite.m_rotation = Degrees_to_radians(player_bullet.m_heading);

	Enemy_Sprite.m_position = Float2D(Enemy_Ship.m_x, Enemy_Ship.m_y);
	Enemy_Sprite.m_rotation = Degrees_to_radians(Enemy_Ship.heading);

	Clear();

	if (drawExplosion)
	{
		
		DrawSprite(&Explosion);
		
		if (bomb_count >= 170)
		{

			drawExplosion = false;
			bomb_count = 0;

		}
			
		bomb_count++;
	}

	//only if player bullet is alive draw it
	if (player_bullet.alive == true)
		DrawSprite(&Bullet_Sprite);
   
	//if ship is alive draw it
	if (test_ship.alive == true)
		DrawSprite(&g_shipSprite);
	//if enemy ship is alive draw it
	if (Enemy_Ship.alive == true)
		DrawSprite(&Enemy_Sprite);
	
	Present();

	//testing if user want to end
	if (keyState[eKey_x]) 
		continueRunning = false;

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

