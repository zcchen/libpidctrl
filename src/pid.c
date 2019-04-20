#include "pid.h"

bool pid_init(struct pid_t *pid, float kp, float ki, float kd, bool open_loop)
{
    if (!pid->is_init) {
        /*pid->target = 0;*/
        pid->kp = kp;
        pid->ki = ki;
        pid->kd = kd;
        pid->max_out = 0;
        pid->min_out = 0;
        pid->max_i = 0;
        pid->max_step = 0;
        pid->integral_sum = 0;
        pid->prev_v[0] = 0;
        pid->prev_v[1] = 0;
        pid->open_loop = open_loop;
        pid->is_init = true;
    }
    return pid->is_init;
}

bool pid_setlimit(struct pid_t *pid, float max_out, float min_out,
                  float max_i, float max_step)
{
    if (max_i < 0 || max_step < 0) {
        return false;   // max_i | max_step should be greater than 0;
    }
    pid->max_out = max_out;
    pid->min_out = min_out;
    pid->max_i = max_i;
    pid->max_step = max_step;
    return true;
}

bool pid_run(struct pid_t *pid, float target, float input, float *output)
{
    if (!pid->is_init) {
        return false;
    }
    float tmp_out = 0;
    float curr_err = 0;
    float last_err = 0;
    // prepare to run the pid controller
    if (pid->open_loop) {
        curr_err = target;
    }   // FIXME: is this OK?
    else {
        curr_err = target - input;
    }
    last_err = target - pid->prev_v[0];     // TODO: check bug here
    // Run the pid controller
    pid->integral_sum += curr_err;
    if (pid->max_i && pid->integral_sum >= pid->max_i) {
        pid->integral_sum = pid->max_i;
    }
    tmp_out = (pid->kp) * curr_err + (pid->ki) * (pid->integral_sum) + \
              (pid->kd) * (curr_err - last_err);
    if (pid->max_step) {
        if (tmp_out > (pid->prev_v[0] + pid->max_step)) {
            tmp_out = pid->prev_v[0] + pid->max_step;
        }
        else if (tmp_out < (pid->prev_v[0] - pid->max_step)) {
            tmp_out = pid->prev_v[0] - pid->max_step;
        }
    }
    if (pid->max_out &&  tmp_out > pid->max_out) {
        tmp_out = pid->max_out;
    }
    else if (pid->min_out &&  tmp_out < pid->min_out) {
        tmp_out = pid->min_out;
    }
    // output the pid result
    pid->prev_v[0] = *output;
    *output = tmp_out;
    return true;
}

