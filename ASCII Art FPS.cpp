#include <iostream>
#include <windows.h>
#include <chrono>
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
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
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
			fPlayerAngle -= (0.1f) * fElapsedTime;
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
			fPlayerAngle += (0.1f) * fElapsedTime;
		}


		//Potter Algorithm
		for (int x = 0; x < nScreenWidth; x++) {
			// For each column, calculate the projected ray angle into world space
			float fRayAngle = (fPlayerAngle - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

			float fDistanceToWall = 0;
			bool bitHitWall = false;

			//Unit vector for ray in player space
			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			while (!bitHitWall && fDistanceToWall < fDepth) {

				fDistanceToWall += 0.1f;

				int nTestX = (int)(fPlayerXPos + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerYPos + fEyeY * fDistanceToWall);

				//Test if ray is out of bounds
				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) {
					bitHitWall = true;
					fDistanceToWall = fDepth;
				}
				else {
					//Ray is inbounds so test to see if the ray cell is a wall block
					if (map[nTestY * nMapWidth + nTestX] == '#') {
						bitHitWall = true;
					}
				}
			}

			// Calculate distance to ceiling and floor
			int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

			for (int y = 0; y < nScreenHeight; y++) {
				if (y < nCeiling) {
					screen[y * nScreenWidth + x] = L' ';
				}
				else if (y > nCeiling && y <= nFloor) {
					screen[y * nScreenWidth + x] = L'#';
				}
				else {
					screen[y * nScreenWidth + x] = L' ';
				}
			}
		}

		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	

	return 0;	
}