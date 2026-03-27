/*
 * motor_control.c
 *
 *  Created on: Mar 26, 2026
 *      Author: ubuntu
 */
#include "motor_control.h"
#include "main.h"
#include "usart.h"

void motor_control_init(void) {

}

void motor_enable(void) {
	uint8_t uartFrame[6];

	uint32_t priority = 3;
	uint32_t service_bit = 1;
	uint32_t request_bit = 1;
	uint32_t service_id = 0x00;
	uint32_t axis_id = 2;
	uint32_t dest_id = 2;
	uint32_t source_id = 10;

	uint32_t ExtId = (priority << 26) | (service_bit << 25)
			| (request_bit << 24) | (service_id << 16) | (axis_id << 15)
			| (dest_id << 8) | (source_id << 0);

	uartFrame[0] = (ExtId >> 24) & 0xFF;
	uartFrame[1] = (ExtId >> 16) & 0xFF;
	uartFrame[2] = (ExtId >> 8) & 0xFF;
	uartFrame[3] = (ExtId >> 0) & 0xFF;
	uartFrame[4] = 1;
	uartFrame[5] = 0x01;

	HAL_UART_Transmit(&huart1, uartFrame, 6, HAL_MAX_DELAY);

}

void motor_reset(void) {
	uint8_t uartFrame[6];

	uint32_t priority = 3;
	uint32_t service_bit = 1;
	uint32_t request_bit = 1;
	uint32_t service_id = 0xFF;
	uint32_t axis_id = 2;
	uint32_t dest_id = 2;
	uint32_t source_id = 10;

	uint32_t ExtId = (priority << 26) | (service_bit << 25)
			| (request_bit << 24) | (service_id << 16) | (axis_id << 15)
			| (dest_id << 8) | (source_id << 0);

	uartFrame[0] = (ExtId >> 24) & 0xFF;
	uartFrame[1] = (ExtId >> 16) & 0xFF;
	uartFrame[2] = (ExtId >> 8) & 0xFF;
	uartFrame[3] = (ExtId >> 0) & 0xFF;
	uartFrame[4] = 1;
	uartFrame[5] = 0x01;

	HAL_UART_Transmit(&huart1, uartFrame, 6, HAL_MAX_DELAY);

}

void motor_set_speed_rpm(int32_t rpm) {
	uint8_t uartFrame[9];

	uint32_t priority = 3;
	uint32_t service_bit = 1;
	uint32_t request_bit = 1;
	uint32_t service_id = 0x41;
	uint32_t axis_id = 2;
	uint32_t dest_id = 2;
	uint32_t source_id = 10;

	uint32_t ExtId = (priority << 26) | (service_bit << 25)
			| (request_bit << 24) | (service_id << 16) | (axis_id << 15)
			| (dest_id << 8) | (source_id << 0);

	int32_t speed_iu = rpm * 2048;

	uartFrame[0] = (ExtId >> 24) & 0xFF;
	uartFrame[1] = (ExtId >> 16) & 0xFF;
	uartFrame[2] = (ExtId >> 8) & 0xFF;
	uartFrame[3] = (ExtId >> 0) & 0xFF;

	uartFrame[4] = 0x20;

	uartFrame[5] = (speed_iu >> 0) & 0xFF;
	uartFrame[6] = (speed_iu >> 8) & 0xFF;
	uartFrame[7] = (speed_iu >> 16) & 0xFF;
	uartFrame[8] = (speed_iu >> 24) & 0xFF;

	HAL_UART_Transmit(&huart1, uartFrame, 9, HAL_MAX_DELAY);

}

void send_set_acceleration(int32_t accel) {
	uint8_t uartFrame[11];

	uint32_t priority = 3;
	uint32_t service_bit = 1;
	uint32_t request_bit = 1;
	uint32_t service_id = 0x20;
	uint32_t axis_id = 2;
	uint32_t dest_id = 2;
	uint32_t source_id = 10;

	uint32_t ExtId = (priority << 26) | (service_bit << 25)
			| (request_bit << 24) | (service_id << 16) | (axis_id << 15)
			| (dest_id << 8) | (source_id << 0);

	uartFrame[0] = (ExtId >> 24) & 0xFF;
	uartFrame[1] = (ExtId >> 16) & 0xFF;
	uartFrame[2] = (ExtId >> 8) & 0xFF;
	uartFrame[3] = (ExtId >> 0) & 0xFF;

	uartFrame[4] = 4;

	uartFrame[5] = 0xA1;
	uartFrame[6] = 0x04;

	uartFrame[7] = (accel >> 0) & 0xFF;
	uartFrame[8] = (accel >> 8) & 0xFF;
	uartFrame[9] = (accel >> 16) & 0xFF;
	uartFrame[10] = (accel >> 24) & 0xFF;

	HAL_UART_Transmit(&huart1, uartFrame, 11, HAL_MAX_DELAY);
}

