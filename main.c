#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

int main(void)
{
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

    int WIDTH = ws.ws_col > 0 ? ws.ws_col : 80;
    int HEIGHT = ws.ws_row > 1 ? ws.ws_row - 1 : 24;

    float RADIUS = (WIDTH < HEIGHT * 2 ? WIDTH / 2.2f : HEIGHT) * 0.45f; // Adjust radius based on terminal size to maintain aspect radio

    // Density decrement based on radius to maintain visual quality across different sizes
    float thetaStep = 0.045f * (50.0f / RADIUS);
    float phiStep = 0.018f * (50.0f / RADIUS);
    if (thetaStep > 0.05f)
        thetaStep = 0.05f;
    if (phiStep > 0.02f)
        phiStep = 0.02f;

    float A = 0, B = 0;

    char *output = malloc(WIDTH * HEIGHT);
    float *zBuffer = malloc(WIDTH * HEIGHT * sizeof(float));
    char *frame = malloc((WIDTH + 1) * HEIGHT + 32);

    const char *luminance = ".,-~:;=!*#$@";

    printf("\x1b[2J");
    setvbuf(stdout, NULL, _IONBF, 16384);

    while (1)
    {
        memset(output, ' ', WIDTH * HEIGHT);
        memset(zBuffer, 0, WIDTH * HEIGHT * sizeof(float));

        for (float theta = 0; theta < 2 * M_PI; theta += thetaStep)
        {
            for (float phi = 0; phi < M_PI; phi += phiStep)
            {
                float x0 = RADIUS * sin(phi) * cos(theta);
                float y0 = RADIUS * sin(phi) * sin(theta);
                float z0 = RADIUS * cos(phi);

                float y1 = y0 * cos(A) - z0 * sin(A);
                float z1 = y0 * sin(A) + z0 * cos(A);

                float x2 = x0 * cos(B) + z1 * sin(B);
                float z2 = -x0 * sin(B) + z1 * cos(B);

                float distance = RADIUS * 1.4f;
                float ooz = 1 / (z2 + distance);

                int xp = (int)(WIDTH / 2 + 2 * ooz * x2 * WIDTH * 0.4f);
                int yp = (int)(HEIGHT / 2 + ooz * y1 * HEIGHT * 0.4f);

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
                frame[pos++] = output[x + y * WIDTH];
            frame[pos++] = '\n';
        }
        frame[pos] = '\0';

        fputs(frame, stdout);
        fflush(stdout);

        A += 0.04;
        B += 0.02;

        usleep(30000);
    }

    free(output);
    free(zBuffer);
    free(frame);
    return 0;
}