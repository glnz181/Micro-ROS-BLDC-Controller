/*
 * microros_app.c
 *
 *  Created on: Mar 27, 2026
 *      Author: ubuntu
 */

#include "microros_app.h"
#include "main.h"
#include "usart.h"

#include <std_msgs/msg/int32.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rosidl_runtime_c/message_type_support_struct.h>

int32_t target_rpm = 0;
bool new_speed_command_flag = false;

rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;
rcl_subscription_t speed_subscriber;
rclc_executor_t executor;
std_msgs__msg__Int32 speed_msg;

void speed_command_callback(const void *msgin) {

	const std_msgs__msg__Int32 *msg = (const std_msgs__msg__Int32*) msgin;

	target_rpm = msg->data;
	new_speed_command_flag = true;

}

bool microros_app_init(void) {
	rcl_ret_t rc;

	target_rpm = 0;
	new_speed_command_flag = false;

	rc = rclc_support_init(&support, 0, NULL, &allocator);
	if (rc != RCL_RET_OK) {
		return false;
	}

	rc = rclc_node_init_default(&node, "stm32_motor_node", "", &support);
	if (rc != RCL_RET_OK) {
		return false;
	}

	rc = rclc_subscription_init_default(&speed_subscriber, &node,
			ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
			"/motor_speed_cmd");
	if (rc != RCL_RET_OK) {
		return false;
	}

	rc = rclc_executor_init(&executor, &support.context, 1, &allocator);
	if (rc != RCL_RET_OK) {
		return false;
	}

	rc = rclc_executor_add_subscription(&executor, &speed_subscriber,
			&speed_msg, &speed_command_callback, ON_NEW_DATA);
	if (rc != RCL_RET_OK) {
		return false;
	}

	return true;
}

void microros_app_spin(void) {

	rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));

}
bool microros_has_new_speed_command(void) { // yeni komut var mi kontrolu yapar
	return new_speed_command_flag;
}

int32_t microros_get_target_rpm(void) { // son alinan hedef hiz ne
	return target_rpm;
}

void microros_clear_new_speed_command(void) { // amaci komut islendiginde flagi tekrardan false yapmak
	new_speed_command_flag = false;
}

