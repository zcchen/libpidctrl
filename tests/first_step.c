#include <stdio.h>
#include <math.h>
#include "pid.h"

float first_step_obj(float K, float T_ms, long time_ms)
{
    float ret = 0;
    if (time_ms >= 0) {
        ret = K * (1 - exp((0 - (float)time_ms) / T_ms));
    }
    return ret;
}

int main(void)
{
    static float K = 1.0;
    static float T_ms = 1.0;

    struct pid_t pid;
    pid_init(&pid, 1, 0, 0, false);

    float input = 0.0;
    float output = 0.0;
    for (long i = -10; i < 30; ++i) {
        if (i >= 0) {
            input = output;
            pid_run(&pid, 1, input, &output);
            output = first_step_obj(K, T_ms, i);
        }
        printf("time_ms: %ld, input: %f, output: %f\n", i, input, output);
    }
    return 0;
}
