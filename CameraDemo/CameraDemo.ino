//
//  Copyright (C) 2021 Pharap (@Pharap)
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include <Arduboy2.h>

struct Camera
{
	int16_t x;
	int16_t y;

	uint8_t width;
	uint8_t height;
};

struct Entity
{
	int16_t x;
	int16_t y;
};

constexpr uint8_t cameraWidth = WIDTH;
constexpr uint8_t cameraHeight = HEIGHT;

constexpr uint8_t entityWidth = 16;
constexpr uint8_t entityHeight = 24;

Arduboy2 arduboy;

// Initial camera position doesn't matter when following the player.
Camera camera { 0, 0 };

// Start the player part way into the world,
// and floating 4 pixels above the bottom of the screen.
Entity player { 200, (cameraHeight - entityHeight - 4) };

// Start the enemy a few pixels right of the player,
// and floating 4 pixels above the bottom of the screen.
Entity enemy { 242, (cameraHeight - entityHeight - 4) };

bool showDebugInfo = true;

void setup()
{
	arduboy.begin();
}

void loop()
{
	if(!arduboy.nextFrame())
		return;

	arduboy.pollButtons();

	arduboy.clear();

	update();
	render();

	arduboy.display();
}

void update()
{
	updatePlayer();
	updateEnemy();
	updateCamera();
	updateDebugInfo();
}

void updateCamera()
{
	// Calculate the player's centre
	int16_t playerCentreX = (player.x + (entityWidth / 2));
	int16_t playerCentreY = (player.y + (entityHeight / 2));

	// Centre the camera around the player
	camera.x = (playerCentreX - (cameraWidth / 2));
	camera.y = (playerCentreY - (cameraHeight / 2));
}

void updatePlayer()
{
	// Move the player
	if(arduboy.pressed(RIGHT_BUTTON))
		++player.x;

	// Move the player
	if(arduboy.pressed(LEFT_BUTTON))
		--player.x;
}

void updateEnemy()
{
	// Do nothing for now
}

void updateDebugInfo()
{
	if(arduboy.justPressed(B_BUTTON))
		showDebugInfo = !showDebugInfo;
}

void render()
{
	renderPlayer();
	renderEnemy();
	renderDebugInfo();
}

void renderPlayer()
{
	// Calculate local/screen coordinates by subtracting camera position.
	int16_t localX = (player.x - camera.x);
	int16_t localY = (player.y - camera.y);

	// Draw player at calculated position.
	// Here the player is drawn with a filled rectangle.
	arduboy.fillRect(localX, localY, entityWidth, entityHeight, WHITE);
}

void renderEnemy()
{
	// Calculate local/screen coordinates by subtracting camera position.
	int16_t localX = (enemy.x - camera.x);
	int16_t localY = (enemy.y - camera.y);

	// Draw enemy at calculated position.
	// Here enemies are drawn with unfilled rectangles.
	arduboy.drawRect(localX, localY, entityWidth, entityHeight, WHITE);
}

void renderDebugInfo()
{
	if(!showDebugInfo)
		return;

	// Camera info
	arduboy.print(F("Camera "));
	arduboy.print(camera.x);
	arduboy.print(F(" "));
	arduboy.print(camera.y);
	arduboy.println();

	// Player info
	arduboy.print(F("Player "));
	arduboy.print(player.x);
	arduboy.print(F(" "));
	arduboy.print(player.y);
	arduboy.println();

	// Enemy info
	arduboy.print(F("Enemy "));
	arduboy.print(enemy.x);
	arduboy.print(F(" "));
	arduboy.print(enemy.y);
	arduboy.println();
}