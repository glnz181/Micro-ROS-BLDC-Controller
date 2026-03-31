/*
 * microros_app.c
 *
 *  Created on: Mar 27, 2026
 *      Author: __ubuntu__
 */

#include "microros_app.h"
#include "main.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"

#include <std_msgs/msg/int32.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rosidl_runtime_c/message_type_support_struct.h>
#include <rmw_microros/rmw_microros.h>

extern bool cubemx_transport_open(struct uxrCustomTransport *transport);
extern bool cubemx_transport_close(struct uxrCustomTransport *transport);
extern size_t cubemx_transport_write(struct uxrCustomTransport *transport,
		const uint8_t *buf, size_t len, uint8_t *errcode);
extern size_t cubemx_transport_read(struct uxrCustomTransport *transport,
		uint8_t *buf, size_t len, int timeout, uint8_t *errcode);

extern void* microros_allocate(size_t size, void *state);
extern void microros_deallocate(void *pointer, void *state);
extern void* microros_reallocate(void *pointer, size_t size, void *state);
extern void* microros_zero_allocate(size_t number_of_elements,
		size_t size_of_element, void *state);

int32_t target_rpm = 0;
bool new_speed_command_flag = false;

rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;
rcl_subscription_t speed_subscriber;
rclc_executor_t executor;
std_msgs__msg__Int32 speed_msg;

static void led_blink(int count) {
	for (int i = 0; i < count; i++) {
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay(200);
	}
	HAL_Delay(600);
}

static void error_blink(void) {
	while (1) {
		HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_14);
		HAL_Delay(100);
	}
}

void speed_command_callback(const void *msgin) {

	const std_msgs__msg__Int32 *msg = (const std_msgs__msg__Int32*) msgin;
	target_rpm = msg->data;
	new_speed_command_flag = true;

}

bool microros_app_init(void) {
	rcl_ret_t rc;

	target_rpm = 0;
	new_speed_command_flag = false;

	rmw_uros_set_custom_transport(
	true, (void*) &huart3, cubemx_transport_open, cubemx_transport_close,
			cubemx_transport_write, cubemx_transport_read);

	led_blink(1);

	allocator = rcutils_get_zero_initialized_allocator();
	allocator.allocate = microros_allocate;
	allocator.deallocate = microros_deallocate;
	allocator.reallocate = microros_reallocate;
	allocator.zero_allocate = microros_zero_allocate;

	if (!rcutils_set_default_allocator(&allocator)) {
		error_blink();
		return false;
	}
	led_blink(2);

	/* Agent hazır olana kadar bekle */
	while (rmw_uros_ping_agent(100, 1) != RMW_RET_OK) {
		HAL_Delay(500);
	}
	led_blink(3);

	rc = rclc_support_init(&support, 0, NULL, &allocator);
	if (rc != RCL_RET_OK) {
		error_blink();
		return false;
	}
	led_blink(4);

	rc = rclc_node_init_default(&node, "stm32_motor_node", "", &support);
	if (rc != RCL_RET_OK) {
		error_blink();
		return false;
	}
	led_blink(5);
	rc = rclc_subscription_init_default(&speed_subscriber, &node,
			ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
			"/motor_speed_cmd");
	if (rc != RCL_RET_OK) {
		error_blink();
		return false;
	}
	led_blink(6);
	rc = rclc_executor_init(&executor, &support.context, 1, &allocator);
	if (rc != RCL_RET_OK) {
		error_blink();
		return false;

	}
	led_blink(7);

	rc = rclc_executor_add_subscription(&executor, &speed_subscriber,
			&speed_msg, &speed_command_callback, ON_NEW_DATA);
	if (rc != RCL_RET_OK) {
		error_blink();
		return false;
	}
	led_blink(8);
	return true;
}

void microros_app_spin(void) {

	rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));

}

bool microros_has_new_speed_command(void) {
	return new_speed_command_flag;
}

int32_t microros_get_target_rpm(void) {
	return target_rpm;
}

void microros_clear_new_speed_command(void) {
	new_speed_command_flag = false;
}
