#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#define WIDTH 120
#define HEIGHT 45
#define RADIUS 20

int main(void)
{
    float A = 0, B = 0;

    char output[WIDTH * HEIGHT];
    float zBuffer[WIDTH * HEIGHT];

    const char *luminance = ".,-~:;=!*#$@";

    char frame[(WIDTH + 1) * HEIGHT + 32];

    printf("\x1b[2J");
    setvbuf(stdout, NULL, _IONBF, 16384);

    while (1)
    {
        memset(output, ' ', WIDTH * HEIGHT);
        memset(zBuffer, 0, sizeof(zBuffer));

        for (float theta = 0; theta < 2 * M_PI; theta += 0.045)
        {
            for (float phi = 0; phi < M_PI; phi += 0.018)
            {

                float x0 = RADIUS * sin(phi) * cos(theta);
                float y0 = RADIUS * sin(phi) * sin(theta);
                float z0 = RADIUS * cos(phi);

                float y1 = y0 * cos(A) - z0 * sin(A);
                float z1 = y0 * sin(A) + z0 * cos(A);

                float x2 = x0 * cos(B) + z1 * sin(B);
                float z2 = -x0 * sin(B) + z1 * cos(B);

                float distance = 70.0f;
                float ooz = 1 / (z2 + distance);

                int xp = (int)(WIDTH / 2 + 40 * ooz * x2 * 2.2);
                int yp = (int)(HEIGHT / 2 + 20 * ooz * y1);

                int idx = xp + yp * WIDTH;

                float luminanceValue = sin(phi) * cos(theta - A);

                if (idx >= 0 && idx < WIDTH * HEIGHT && ooz > zBuffer[idx])
                {
                    zBuffer[idx] = ooz;
                    int lumIndex = (luminanceValue + 1) * 5.5f;
                    if (lumIndex < 0)
                        lumIndex = 0;
                    if (lumIndex > 11)
                        lumIndex = 11;
                    output[idx] = luminance[lumIndex];
                }
            }
        }

        int pos = 0;
        pos += sprintf(frame + pos, "\x1b[H");
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                frame[pos++] = output[x + y * WIDTH];
            }
            frame[pos++] = '\n';
        }
        frame[pos] = '\0';

        fputs(frame, stdout);
        fflush(stdout);

        A += 0.04;
        B += 0.02;

        usleep(30000);
    }

    return 0;
}