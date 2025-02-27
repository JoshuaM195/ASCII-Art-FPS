#include <iostream>
#include <windows.h>
#include <chrono>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

int nScreenWidth = 120;			
int nScreenHeight = 40;

float fPlayerXPos = 8.0f;
float fPlayerYPos = 8.0f;
float fPlayerAngle = 0.0f;

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.14159 / 4.0;
float fDepth = 16.0f;


int main() {
	// Create Screen Buffer
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	wstring map;
	map += L"################";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..........#...#";
	map += L"#..........#...#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#......#########";
	map += L"#..............#";
	map += L"#..............#";
	map += L"################";

	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();


	//Game loop
	while (1) {

		//Controls
		// Handle CCW Rotation

		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
			fPlayerAngle -= (1.0f) * fElapsedTime;
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
			fPlayerAngle += (1.0f) * fElapsedTime;
		}

		if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
			fPlayerXPos += sinf(fPlayerAngle) * 5.0f * fElapsedTime;
			fPlayerYPos += cosf(fPlayerAngle) * 5.0f * fElapsedTime;

			if (map[(int)fPlayerYPos * nMapWidth + (int)fPlayerXPos] == '#') {
				fPlayerXPos -= sinf(fPlayerAngle) * 5.0f * fElapsedTime;
				fPlayerYPos -= cosf(fPlayerAngle) * 5.0f * fElapsedTime;
			}
		}

		if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
			fPlayerXPos -= sinf(fPlayerAngle) * 5.0f * fElapsedTime;
			fPlayerYPos -= cosf(fPlayerAngle) * 5.0f * fElapsedTime;

			if (map[(int)fPlayerYPos * nMapWidth + (int)fPlayerXPos] == '#') {
				fPlayerXPos += sinf(fPlayerAngle) * 5.0f * fElapsedTime;
				fPlayerYPos += cosf(fPlayerAngle) * 5.0f * fElapsedTime;
			}
		}


		//Potter Algorithm
		for (int x = 0; x < nScreenWidth; x++) {
			// For each column, calculate the projected ray angle into world space
			float fRayAngle = (fPlayerAngle - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

			float fDistanceToWall = 0;
			bool bitHitWall = false;
			bool bBoundary = false;

			//Unit vector for ray in player space
			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			while (!bitHitWall && fDistanceToWall < fDepth) {

				fDistanceToWall += 0.1f;

				int nTestX = (int)(fPlayerXPos + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerYPos + fEyeY * fDistanceToWall);

				//Test if ray is out of bounds
				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) {
					bitHitWall = true;						// Setting distance to maximum depth
					fDistanceToWall = fDepth;
				}
				else {
					//Ray is inbounds so test to see if the ray cell is a wall block
					if (map[nTestY * nMapWidth + nTestX] == '#') {
						bitHitWall = true;

						vector<pair<float, float>> p; // distance, dot

						for (int tx = 0; tx < 2; tx++) {
							for (int ty = 0; ty < 2; ty++) {
								float vy = (float)nTestY + ty - fPlayerYPos;
								float vx = (float)nTestX + tx - fPlayerXPos;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
								p.push_back(make_pair(d, dot));
							}

							//Sort Pairs from closest to farthest
							sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right) {return left.first < right.first; });

							float fBound = 0.01;
							if (acos(p.at(0).second) < fBound) bBoundary = true;
							if (acos(p.at(1).second) < fBound) bBoundary = true;
							//if (acos(p.at(2).second) < fBound) bBoundary = true;
						}
					}
				}
			}

			// Calculate distance to ceiling and floor
			int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

			short nShade = ' ';

			if (fDistanceToWall <= fDepth / 4.0f)		nShade = 0x2588;	// Very close
			else if (fDistanceToWall < fDepth / 3.0f)	nShade = 0x2593;
			else if (fDistanceToWall < fDepth / 2.0f)	nShade = 0x2592;
			else if (fDistanceToWall < fDepth)			nShade = 0x2591;
			else										nShade = ' ';		// Too far away

			if (bBoundary)		nShade = ' '; // Black it out

			for (int y = 0; y < nScreenHeight; y++)
			{
				// Each Row
				if (y <= nCeiling)
					screen[y * nScreenWidth + x] = ' ';
				else if (y > nCeiling && y <= nFloor)
					screen[y * nScreenWidth + x] = nShade;
				else // Floor
				{
					// Shade floor based on distance
					float b = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
					if (b < 0.25)		nShade = '#';
					else if (b < 0.5)	nShade = 'x';
					else if (b < 0.75)	nShade = '.';
					else if (b < 0.9)	nShade = '-';
					else				nShade = ' ';
					screen[y * nScreenWidth + x] = nShade;
				}
			}
		}

		// Display Stats
		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f", fPlayerXPos, fPlayerYPos, fPlayerAngle, 1.0f / fElapsedTime);

		// Display Map
		for (int nx = 0; nx < nMapWidth; nx++){
			for (int ny = 0; ny < nMapHeight; ny++) {
				screen[(ny + 1) * nScreenWidth + nx] = map[ny * nMapWidth + nx];
			}
		}

		screen[((int)fPlayerYPos + 1) * nScreenWidth + (int)fPlayerXPos] = 'P';

		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	

	return 0;	
}