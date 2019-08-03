#ifndef  __VL53L0X_H
#define  __VL53L0X_H

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_i2c.h"


//ģ���ʼ��
int8_t Wl_laserdis_init(void);

//�������β���
VL53L0X_Error Wl_laserdis_start_single_test(VL53L0X_Dev_t *pdev, VL53L0X_RangingMeasurementData_t *pdata);

//���ò���ģʽ
int8_t Wl_laserdis_measure_init(VL53L0X_Dev_t *pMyDevice, uint8_t ranging_mode, uint8_t accuracy_mode);

//ģ�鴮��У׼
int8_t Wl_laserdis_calibration_xtalk_init(VL53L0X_Dev_t *pMyDevice);

//ģ��ƫ��У׼
int8_t Wl_laserdis_calibration_offset_init(VL53L0X_Dev_t *pMyDevice);

//ģ��SPADУ׼�Ͳο�У׼
int8_t Wl_laserdis_calibration_init(VL53L0X_Dev_t *pMyDevice);

//��ȡ�豸�汾��Ϣ
int8_t Wl_laserdis_getdeviceinfo(VL53L0X_Dev_t *pMyDevice);

unsigned int WL_LasterDisSignleRangleDistance(unsigned int distance);

#endif
