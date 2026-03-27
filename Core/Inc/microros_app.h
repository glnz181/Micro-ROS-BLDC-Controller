/*
 * microros_app.h
 *
 *  Created on: Mar 27, 2026
 *      Author: ubuntu
 */

#ifndef INC_MICROROS_APP_H_
#define INC_MICROROS_APP_H_

#include <stdint.h>
#include <stdbool.h>

bool microros_app_init(void);
void microros_app_spin(void);
bool microros_has_new_speed_command(void);
int32_t microros_get_target_rpm(void);
void microros_clear_new_speed_command(void);

#endif /* INC_MICROROS_APP_H_ */
