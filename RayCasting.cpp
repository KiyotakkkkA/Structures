#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <stdio.h>
#include <Windows.h>

struct WindowSettings {
    int w_width = 120;
    int w_height = 40;
    int map_cells = 16;

    bool game_state = true;
};

struct Player {
    // Movement
    float x_coord = 14.7f;
    float y_coord = 5.0f;
    float speed = 4.0f;
    float angle_speed = 0.75f;

    // Rays supports
    float angle = 0.0f;
    float FOV = 3.14159f / 4.0f;
    float depth = 16.0f;
};

int main(void){
    // Objects initialization
    Player player;
    WindowSettings window;
    wchar_t *game_screen = new wchar_t[window.w_width*window.w_height];


    // Console management
	HANDLE console_settings = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console_settings);
	DWORD buffer = 0;

    // Map initializing
    std::wstring map;
    map += L"#########.......";
	map += L"#...............";
	map += L"#.......########";
	map += L"#.......#......#";
	map += L"#..............#";
	map += L"#.......#......#";
	map += L"#.......#......#";
	map += L"#.......#......#";
	map += L"##......#......#";
	map += L"#......#########";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#....#####..####";
	map += L"#..............#";
	map += L"################";

    // initializing clocks for keys press events
    auto timer1 = std::chrono::system_clock::now();
	auto timer2 = std::chrono::system_clock::now();

    while (window.game_state){
        // Time management for movement and raycast what based on frames difference
	    timer2 = std::chrono::system_clock::now();
        std::chrono::duration<float> time_passed = timer2 - timer1;
		timer1 = timer2;
		float frames_passed = time_passed.count();

        // [Movement processing]
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			player.angle += (player.speed * player.angle_speed) * frames_passed; // right rotation

		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			player.angle -= (player.speed * player.angle_speed) * frames_passed; // left rotation
		
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000) // forward move
		{
			player.x_coord += sinf(player.angle) * player.speed * frames_passed;;
			player.y_coord += cosf(player.angle) * player.speed * frames_passed;;
			if (map.c_str()[(int)player.x_coord * window.map_cells + (int)player.y_coord] == '#') // check collisions
			{
				player.x_coord -= sinf(player.angle) * player.speed * frames_passed;;
				player.y_coord -= cosf(player.angle) * player.speed * frames_passed;; // push player if collision is occured
			}			
		}

        if (GetAsyncKeyState((unsigned short)'S') & 0x8000) // backward move
		{
			player.x_coord -= sinf(player.angle) * player.speed * frames_passed;;
			player.y_coord -= cosf(player.angle) * player.speed * frames_passed;;
			if (map.c_str()[(int)player.x_coord * window.map_cells + (int)player.y_coord] == '#') // check collisions
			{
				player.x_coord += sinf(player.angle) * player.speed * frames_passed;;
				player.y_coord += cosf(player.angle) * player.speed * frames_passed;; // push player if collision is occured
			}			
		}


        // RAYCAST ALGORITHM 
        for (int x = 0; x < window.w_width; x++)
		{
            // Create rays
			float rays = (player.angle - player.FOV/2.0f) + ((float)x / (float)window.w_width) * player.FOV;
			float step = 0.1f, dist_to_walls = 0.0f;

			bool in_wall = false, bounds = false;

            float perspective_x = sinf(rays);
            float perspective_y = cosf(rays);

            // Check rays collision
            // Hi! Its Zakhar) <3
			while (!in_wall && dist_to_walls < player.depth)
			{
				dist_to_walls += step;
				int x_ = (int)(player.x_coord + perspective_x * dist_to_walls);
				int y_ = (int)(player.y_coord + perspective_y * dist_to_walls);
				
				// Check ray is out of bounds
				if (x_ < 0 || x_ >= window.map_cells || y_ < 0 || y_ >= window.map_cells)
				{
                    // if ray is out of bounds then we make this distance new ray depth
					in_wall = true;
					dist_to_walls = player.depth;
				}
				else
				{
					// Check rays collision
					if (map.c_str()[x_ * window.map_cells + y_] == '#')
					{
						in_wall = true;

                        // Make shaders what make illusion of 3D
						std::vector<std::pair<float, float>> storage;

                        // Check corners collision by using dot product of two rays
						for (int dx = 0; dx < 2; dx++)
							for (int dy = 0; dy < 2; dy++)
							{
								// Angle of corner to eye
								float vy = (float)y_ + dy - player.y_coord;
								float vx = (float)x_ + dx - player.x_coord;
								float d = sqrt(vx*vx + vy*vy); 
								float dot = (perspective_x * vx / d) + (perspective_y * vy / d);
								storage.push_back(std::make_pair(d, dot));
							}

						// Sort Pairs from closest to farthest
						std::sort(storage.begin(), storage.end(), [](const std::pair<float, float> &left, const std::pair<float, float> &right) {return left.first < right.first; });
						
						// We can see no more three sides of imaginary 3d block
						float bound_ = 0.01;
						if (acos(storage.at(0).second) < bound_) bounds = true;
						if (acos(storage.at(1).second) < bound_) bounds = true;
						if (acos(storage.at(2).second) < bound_) bounds = true;
					}
				}
			}
		
			// Calculate distance
			int ceiling = (float)(window.w_height/2.0) - window.w_height / ((float)dist_to_walls);
			int floor = window.w_height - ceiling;

			// Shade walls based on distance
			short shader = ' ';
			if (dist_to_walls <= player.depth / 4.0f)	{
                shader = 0x2588;
            }	
			else if (dist_to_walls < player.depth / 3.0f){
                shader = 0x2593;
            }
			else if (dist_to_walls < player.depth / 2.0f){
                shader = 0x2592;
            }
			else if (dist_to_walls < player.depth){
                shader = 0x2591;
            }	
			else{
                shader = ' ';
            }			

			if (bounds){
                shader = ' ';
            }
			
			for (int y = 0; y < window.w_height; y++)
			{
				if(y <= ceiling){
                    game_screen[y*window.w_width + x] = ' ';
                }
				else if(y > ceiling && y <= floor){
                    game_screen[y*window.w_width + x] = shader;
                }
				else
				{				
					// Shade floor based on distance
					float floor_dist = 1.0f - (((float)y - window.w_height/2.0f) / ((float)window.w_height / 2.0f));
					if (floor_dist < 0.25){
                        shader = '#';
                    }
					else if (floor_dist < 0.5){
                        shader = 'x';
                    }
					else if (floor_dist < 0.75){
                        shader = '.';
                    }
					else if (floor_dist < 0.9){
                        shader = '-';
                    }
					else{
                        shader = ' ';
                    }
					game_screen[y*window.w_width + x] = shader;
				}

            }
        }

        // Display Map
        for (int x = 0; x < window.map_cells; x++)
            for (int y = 0; y < window.map_cells; y++)
            {
                game_screen[(y+1)*window.w_width + x] = map[y * window.map_cells + x];
            }
        game_screen[((int)player.x_coord+1) * window.w_height + (int)player.y_coord] = 'G';

        // Display Frame
        game_screen[window.w_width * window.w_height - 1] = '\0';
        WriteConsoleOutputCharacterW(console_settings, game_screen, window.w_width * window.w_height, { 0,0 }, &buffer);
    
        // USE g++ -std=c++11  finalTask.cpp -o finalTask TO COMPILE THIS PROGRAMM))

    }
}