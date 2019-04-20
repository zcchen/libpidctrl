#ifndef LIB_PID_CTRL_H
#define LIB_PID_CTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

struct pid_t {
    //float   target;     ///< PID control target
    float   kp;         ///< Proportional control gain
    float   ki;         ///< Integral Control Gain
    float   kd;         ///< Differential Control Gain
    // ----------- the limit settings ----------------------------- //
    float   max_out;    ///< maximum output
    float   min_out;    ///< minimum output
    float   max_i;      ///< abs maximum integral value passed through
    float   max_step;   ///< abs maximum changing step value
    // ----------- set the above value when struct init ----------- //
    float   integral_sum;
    float   prev_v[0];  ///< Previous Value, 0 means the latest one
    bool    open_loop;  ///< Open loop flag
    bool    is_init;
};

/**
 * Initialize the pid struct.
 *
 * @param pid The controller to initialize
 * @param k_i Integral control gain
 * @param k_p Proportional control gain
 * @param k_i Integral control gain
 * @param k_d Differential control gain
 * @param max_i Maximum integral passed in, 0 for disable
 * @param open_loop Open loop controller flag
 */
bool pid_init(struct pid_t *pid, float kp, float ki, float kd, bool open_loop);
    // init the pid controller, return pid->is_init

bool pid_setlimit(struct pid_t *pid, float max_out, float min_out,
                  float max_i, float max_step);
    // set the pid controller, return void

bool pid_run(struct pid_t *pid, float target, float in, float *out);

#ifdef __cplusplus
extern "C" {
#endif

#endif
