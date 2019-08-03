#ifndef  __VL53L0X_H
#define  __VL53L0X_H

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_i2c.h"


//模块初始化
int8_t Wl_laserdis_init(void);

//启动单次测量
VL53L0X_Error Wl_laserdis_start_single_test(VL53L0X_Dev_t *pdev, VL53L0X_RangingMeasurementData_t *pdata);

//设置测量模式
int8_t Wl_laserdis_measure_init(VL53L0X_Dev_t *pMyDevice, uint8_t ranging_mode, uint8_t accuracy_mode);

//模块串扰校准
int8_t Wl_laserdis_calibration_xtalk_init(VL53L0X_Dev_t *pMyDevice);

//模块偏移校准
int8_t Wl_laserdis_calibration_offset_init(VL53L0X_Dev_t *pMyDevice);

//模块SPAD校准和参考校准
int8_t Wl_laserdis_calibration_init(VL53L0X_Dev_t *pMyDevice);

//获取设备版本信息
int8_t Wl_laserdis_getdeviceinfo(VL53L0X_Dev_t *pMyDevice);

unsigned int WL_LasterDisSignleRangleDistance(unsigned int distance);

#endif
