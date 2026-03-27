#ifndef INC_MOTOR_CONTROL_
#define INC_MOTOR_CONTROL_

#include <stdint.h>
#include <stdio.h>

void motor_control_init(void);
void motor_enable(void);
void motor_reset(void);
void motor_set_speed_rpm(int32_t rpm);
void send_set_acceleration(int32_t accel);

#endif /* INC_MOTOR_CONTROL_ */
