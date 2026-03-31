/*
 * motor_control.h
 */

#ifndef INC_MOTOR_CONTROL_
#define INC_MOTOR_CONTROL_

#include <stdint.h>
#include <stdio.h>

void motor_enable(uint32_t axis_id);
void motor_reset(uint32_t axis_id);
void motor_set_speed_rpm(uint32_t axis_id, int32_t speed);  /* int32_t: negatif hız için */
void motor_set_acceleration(uint32_t axis_id, int32_t accel);

#endif /* INC_MOTOR_CONTROL_ */
