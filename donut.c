#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#define WINDOWS
#endif

#include <stdio.h>
#include <math.h>
#include <time.h>
#ifdef WINDOWS
    #include <tchar.h>
    #include <windows.h>
#else
    #include <unistd.h>
    #include <string.h>
#endif

#define CANVAS_WIDTH 80
#define CANVAS_HEIGHT 22
#define CANVAS_LENGTH (CANVAS_WIDTH * CANVAS_HEIGHT) // 1760
#define TWO_PI 6.28318f
#define RUNTIME_SECS 60
#define DISPLAY 1

int main() {
	long iter = 0;
	time_t start = time(NULL);

	float A = 0, B = 0; // No stop in rotation after ~4,494,384 frames
	float phi, theta;
	char frameBuffer[CANVAS_LENGTH];
	float zBuffer[CANVAS_LENGTH];
	while (1) {
		A += 0.07; // Increment torus rotation
		B += 0.03; // Increment torus rotation
		memset(frameBuffer, ' ', CANVAS_LENGTH);
		memset(zBuffer, 0, CANVAS_LENGTH * sizeof(float));
		for (theta = 0; theta < TWO_PI; theta += 0.07) {
			for (phi = 0; phi < TWO_PI; phi += 0.02) {
				float sTheta = sin(theta), cTheta = cos(theta);
				float sPhi = sin(phi), cPhi = cos(phi);
				float sA = sin(A), cA = cos(A);
				float sB = sin(B), cB = cos(B);

				float h = cTheta + 2;
				float t = sPhi * h * cA - sTheta * sA;
				float oneOverZ = 1 / (sPhi * h * sA + sTheta * cA + 5);
				int x2d = 40 + 30 * oneOverZ * (cPhi * h * cB - t * sB); // 40 is x offset, and 30 is distance from camera
				int y2d = 12 + 15 * oneOverZ * (cPhi * h * sB + t * cB);
				int bufferIndex = x2d + CANVAS_WIDTH * y2d;
				float illumination = (sTheta * sA - sPhi * cTheta * cA) * cB - sPhi * cTheta * sA - sTheta * cA - cPhi * cTheta * sB;
				int illumination12 = 8 * illumination; // multiplier = floor( # of levels / sqrt(2) )
				if (CANVAS_HEIGHT > y2d && y2d > 0 && x2d > 0 && CANVAS_WIDTH > x2d && oneOverZ > zBuffer[bufferIndex]) {
					zBuffer[bufferIndex] = oneOverZ;
					frameBuffer[bufferIndex] = ".,-~:;=!*#$@"[illumination12 > 0 ? illumination12 : 0];
				}
			}
		}

		#if DISPLAY
			#ifdef WINDOWS
				system("CLS");
			#else
				printf("\x1b[2J\x1b[H");
			#endif
		int i;
		for (i = 0; i < CANVAS_LENGTH + 1; i++) {
			putchar(i % CANVAS_WIDTH ? frameBuffer[i] : '\n');
		}
		#endif

		iter++;
		if (time(NULL) - start > RUNTIME_SECS) {
			printf("%f\n", ((double)iter / RUNTIME_SECS));
			#ifdef WINDOWS
				Sleep(300 * 1000); // Sleep 5 minutes so window doesn't close in visual studio
			#endif
			return 0;
		}
	}
	return 0;
}
