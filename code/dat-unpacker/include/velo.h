#pragma once

#include <stdint.h>

typedef struct __attribute__((packed)) {
	uint16_t return_dist;
	uint8_t intensity;
}
velo_return_t;

typedef struct __attribute__((packed)) {
	uint16_t block_id;
	uint16_t rot;
	velo_return_t lasers[32];
}
velo_block_t;

typedef struct __attribute__((packed)) {
	velo_block_t data_blocks[12];
	uint32_t gpstime;
	uint8_t status_type;
	uint8_t status_code;
}
velo_packet_t;

typedef struct __attribute__((packed)) {
	uint8_t laser_id;
	int16_t vertical_correction;
	int16_t rotational_correction;
	int16_t far_correction;
	int16_t distance_x_correction;
	int16_t distance_y_correction;
	int16_t v_off_correction;
	int16_t h_off_correction;
	int16_t focal_distance;
	int16_t focal_slope;
	uint8_t min_intensity;
	uint8_t max_intensity;
	uint8_t up_year_warning;
	uint8_t up_month;
	uint8_t up_day;
	uint8_t up_hour;
	uint8_t up_minute;
	uint8_t up_sec;
	uint8_t humidity;
}
velo_calib_laser_t;

typedef struct __attribute__((packed)) {
	uint8_t start[5];
	uint8_t upper_thresh;
	uint8_t lower_thresh;
	velo_calib_laser_t lasers[64];
	int16_t motor_speed;
	uint16_t fov_start;
	uint16_t fov_end;
	uint16_t real_hour;
	uint8_t src_ip[4];
	uint8_t dst_ip[4];
	uint8_t multiple;
	uint8_t warning;
	uint8_t power;
	uint16_t crc;
}
velo_calib_t;
