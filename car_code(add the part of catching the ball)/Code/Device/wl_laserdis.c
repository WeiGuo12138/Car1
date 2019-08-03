#include "wl_laserdis.h"
#include "delay.h"

VL53L0X_Dev_t                       wl_laserdis_dev;            //�豸�ṹ������������豸��Ϣ
VL53L0X_RangingMeasurementData_t    wl_laserdis_data;           //�������ݽṹ�壬����������
unsigned int			            wl_laserdis_disance;		//����������
/***************************************************************************************/

/*********************************************************************
 *  �������ƣ�int8_t  Wl_laserdis_start_single_test(VL53L0X_Dev_t *pdev, VL53L0X_RangingMeasurementData_t *pdata)
 *  �������ܣ�ִ�е��β���
 *  ��    �Σ�*pMyDevice���豸�ṹ�壬�������豸��ͨ����Ϣ
 *			  *pdata���������ݽṹ��
 *  ��    ������ʼ��״̬��0���ɹ�	������ʧ��
 *  ��    ע����
 ********************************************************************/
int8_t  Wl_laserdis_start_single_test(VL53L0X_Dev_t *pdev, VL53L0X_RangingMeasurementData_t *pdata)
{
    int8_t Status = 0;
	
    Status = VL53L0X_PerformSingleRangingMeasurement(pdev, pdata);   //VL53L0Xִ�е�һ����
		printf("%d \r\n",wl_laserdis_disance);
	if(Status == 0)
	{
		if((pdata->RangeMilliMeter > 20) && (pdata->RangeMilliMeter < 8100))	//������ȷ
		{		
			wl_laserdis_disance = pdata->RangeMilliMeter;
			printf("%d \r\n",wl_laserdis_disance);
		}
	}
		
    return Status;
}

/*********************************************************************
 *  �������ƣ�int8_t Wl_laserdis_measure_init(VL53L0X_Dev_t *pMyDevice, uint8_t ranging_mode, uint8_t accuracy_mode)
 *  �������ܣ����ò���ģʽ
 *  ��    �Σ�*pMyDevice���豸�ṹ�壬�������豸��ͨ����Ϣ
 *			  ranging_mode������ģʽ	0�����β���ģʽ		1����������ģʽ
 *			  accuracy_mode������ģʽ	0��Ĭ��		1���߾���	2��������	3������
 *  ��    ������ʼ��״̬��0���ɹ�	������ʧ��
 *  ��    ע����
 ********************************************************************/
int8_t Wl_laserdis_measure_init(VL53L0X_Dev_t *pMyDevice, uint8_t ranging_mode, uint8_t accuracy_mode)
{
    int8_t Status = 0;

	if(Status == 0)		//���ò���ģʽ
	{
		if(ranging_mode)					//��������ģʽ
			Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING); 
		else								//���β���ģʽ
			Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_SINGLE_RANGING); 
	}

    if(Status == 0)// ����/�����ض����Ƽ��
        Status = VL53L0X_SetLimitCheckEnable(pMyDevice, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
    if(Status == 0)
        Status = VL53L0X_SetLimitCheckEnable(pMyDevice, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);

	//��ͬ�ľ�����Ҫ���õĲ�����һ��
	if(accuracy_mode == 0)		//Ĭ��ģʽ
	{
		if (Status == 0)			//�趨�ź����ʷ�Χ��Χ
			Status = VL53L0X_SetLimitCheckValue(pMyDevice, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.25*65536));
		if (Status == 0)			//�趨SIGMA��Χ
			Status = VL53L0X_SetLimitCheckValue(pMyDevice, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(18*65536));			
		if (Status == 0)			//�趨��������ʱ��
			Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice, 33000);
		if (Status == 0)			//�趨VCSEL��������
			Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 14);
		if (Status == 0) 			//�趨VCSEL�������ڷ�Χ
			Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 10);
	}
	else if(accuracy_mode == 1)		//�߾��Ȳ���ģʽ
	{
		if (Status == 0)			//�趨�ź����ʷ�Χ��Χ			
			Status = VL53L0X_SetLimitCheckValue(pMyDevice, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.25*65536));
		if (Status == 0)			//�趨SIGMA��Χ		
			Status = VL53L0X_SetLimitCheckValue(pMyDevice, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(18*65536));			
		if (Status == 0)			//�趨��������ʱ��
			Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice, 200000);
		if (Status == 0)			//�趨VCSEL��������
			Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 14);
		if (Status == 0) 			//�趨VCSEL�������ڷ�Χ 
			Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 10);
	}
	if(accuracy_mode == 2)			//���������ģʽ
	{
		if (Status == 0)			//�趨�ź����ʷ�Χ��Χ 
			Status = VL53L0X_SetLimitCheckValue(pMyDevice,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,(FixPoint1616_t)(0.1*65536));
		if (Status == 0)			//�趨SIGMA��Χ 
			Status = VL53L0X_SetLimitCheckValue(pMyDevice,VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,(FixPoint1616_t)(60*65536));
		if (Status == 0)			//�趨��������ʱ�� 
			Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice,33000);
		if (Status == 0)		    //�趨VCSEL��������
			Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);
		if (Status == 0) 			//�趨VCSEL�������ڷ�Χ 
			Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);
	}
	if(accuracy_mode == 3)		//���ٲ���ģʽ
	{
		if (Status == 0)			//�趨�ź����ʷ�Χ��Χ
			Status = VL53L0X_SetLimitCheckValue(pMyDevice,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,(FixPoint1616_t)(0.25*65536));
		if (Status == 0)			//�趨SIGMA��Χ 
			Status = VL53L0X_SetLimitCheckValue(pMyDevice,VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,(FixPoint1616_t)(32*65536));
		if (Status == 0)			//�趨��������ʱ��
			Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice, 20000);
		if (Status == 0)		    //�趨VCSEL��������
			Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 14);
		if (Status == 0) 		    //�趨VCSEL�������ڷ�Χ 
			Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 10);
	}
	
    return Status;   //����0
}

/*********************************************************************
 *  �������ƣ�int8_t Wl_laserdis_calibration_xtalk_init(VL53L0X_Dev_t *pMyDevice)
 *  �������ܣ�ģ��У׼��ʼ��
 *  ��    �Σ�*pMyDevice���豸�ṹ�壬�������豸��ͨ����Ϣ
 *  ��    ������ʼ��״̬��0���ɹ�	������ʧ��
 *  ��    ע�������������뽫У׼���д���ⲿEEPROM��FLASH����
 ********************************************************************/
int8_t Wl_laserdis_calibration_xtalk_init(VL53L0X_Dev_t *pMyDevice)
{
    int8_t Status = 0;

	uint32_t XTalkCalDistance = 100;
	uint32_t XTalkCompensationRateMegaCps;

    if(Status == 0)
		Status = VL53L0X_StaticInit(pMyDevice);
        
    if(Status == 0)	/*  ִ�д���У׼  */	
		Status = VL53L0X_PerformXTalkCalibration(pMyDevice,XTalkCalDistance,&XTalkCompensationRateMegaCps);

	if(Status == 0)	//�趨����У׼ֵ
		Status = VL53L0X_SetXTalkCompensationRateMegaCps(pMyDevice,XTalkCompensationRateMegaCps);
			
    return Status;
}
/*********************************************************************
 *  �������ƣ�int8_t Wl_laserdis_calibration_offset_init(VL53L0X_Dev_t *pMyDevice)
 *  �������ܣ�ģ��У׼��ʼ��
 *  ��    �Σ�*pMyDevice���豸�ṹ�壬�������豸��ͨ����Ϣ
 *  ��    ������ʼ��״̬��0���ɹ�	������ʧ��
 *  ��    ע�������������뽫У׼���д���ⲿEEPROM��FLASH����
 ********************************************************************/
int8_t Wl_laserdis_calibration_offset_init(VL53L0X_Dev_t *pMyDevice)
{
    int8_t Status = 0;

	uint32_t CalDistanceMilliMeter = 100<<16;
	int32_t  OffsetMicroMeter = 30000;

    if(Status == 0)
		Status = VL53L0X_StaticInit(pMyDevice);
        
    if(Status == 0)	/*  ִ��ƫ��У׼  */	
		Status = VL53L0X_PerformOffsetCalibration(pMyDevice,CalDistanceMilliMeter,&OffsetMicroMeter);

	if(Status == 0)	//�趨ƫ��У׼ֵ
		Status = VL53L0X_SetOffsetCalibrationDataMicroMeter(pMyDevice,OffsetMicroMeter);
			
    return Status;
}

/*********************************************************************
 *  �������ƣ�int8_t Wl_laserdis_calibration_init(VL53L0X_Dev_t *pMyDevice)
 *  �������ܣ�ģ��У׼��ʼ��
 *  ��    �Σ�*pMyDevice���豸�ṹ�壬�������豸��ͨ����Ϣ
 *  ��    ������ʼ��״̬��0���ɹ�	������ʧ��
 *  ��    ע�������������뽫У׼���д���ⲿEEPROM��FLASH����
 ********************************************************************/
int8_t Wl_laserdis_calibration_init(VL53L0X_Dev_t *pMyDevice)
{
    int8_t Status = 0;

    uint32_t refSpadCount;
    uint8_t  isApertureSpads;
    uint8_t  VhvSettings;
    uint8_t  PhaseCal;
        
    if(Status == 0)	/*  ִ��SPAD��������ѩ�������ܣ�У׼  */	
		Status = VL53L0X_PerformRefSpadManagement(pMyDevice,&refSpadCount, &isApertureSpads);
    if(Status == 0)	/*  ִ�вο�У׼  */
		Status = VL53L0X_PerformRefCalibration(pMyDevice, &VhvSettings, &PhaseCal);

	if(Status == 0)	//�趨SpadsУ׼ֵ
		Status = VL53L0X_SetReferenceSpads(pMyDevice,refSpadCount,isApertureSpads);
	if(Status == 0)	//�趨RefУ׼ֵ
		Status = VL53L0X_SetRefCalibration(pMyDevice,VhvSettings,PhaseCal);
			
    return Status;
}

/*********************************************************************
 *  �������ƣ�int8_t Wl_laserdis_getdeviceinfo(VL53L0X_Dev_t *pMyDevice)
 *  �������ܣ���ȡ�豸��Ϣ
 *  ��    �Σ�*pMyDevice���豸�ṹ�壬�������豸��ͨ����Ϣ
 *  ��    ������ʼ��״̬��0���ɹ�	������ʧ��
 *  ��    ע����
 ********************************************************************/
int8_t Wl_laserdis_getdeviceinfo(VL53L0X_Dev_t *pMyDevice)
{
	int8_t Status = 0;  
	VL53L0X_DeviceInfo_t  vl53l0x_dev_info;       //�豸ID�ṹ�壬�����豸��Ϣ

    if(Status == 0)
		Status = VL53L0X_GetDeviceInfo(pMyDevice, &vl53l0x_dev_info);		//��ȡ�豸��Ϣ

    printf("VL53L0X_GetDeviceInfo:\n");
    printf("Device Name : %s\n", vl53l0x_dev_info.Name);     	//�豸��
    printf("Device Type : %s\n", vl53l0x_dev_info.Type);    	//��Ʒ����VL53L0X = 1, VL53L1 = 2 
    printf("Device ID : %s\n", vl53l0x_dev_info.ProductId);   	// �豸ID
    printf("ProductRevisionMajor : %d\n", vl53l0x_dev_info.ProductRevisionMajor);
    printf("ProductRevisionMinor : %d\n", vl53l0x_dev_info.ProductRevisionMinor);

    if ((vl53l0x_dev_info.ProductRevisionMajor != 1) && (vl53l0x_dev_info.ProductRevisionMinor != 1))	//ͨ��ʧ��
	{
        Status = VL53L0X_ERROR_NOT_SUPPORTED;
        return Status;
    }

	return Status;
}

/*********************************************************************
 *  �������ƣ�int8_t Wl_laserdis_init
 *  �������ܣ�ģ���ʼ��
 *  ��    �Σ���
 *  ��    ������ʼ��״̬��0���ɹ�	������ʧ��
 *  ��    ע����
 ********************************************************************/
int8_t Wl_laserdis_init(void)
{
    int8_t Status = 0;   
	
    VL53L0X_Dev_t *pMyDevice = &wl_laserdis_dev; 
	
    pMyDevice->I2cDevAddr      =  0x52;           //iic��ַ
    pMyDevice->comms_type      =  1;              //iic=1
    pMyDevice->comms_speed_khz =  400;            //iic����
    
    VL53L0X_i2c_init();								//IIC����    

	if(Status == 0)
		Status = VL53L0X_DataInit(pMyDevice); 		//���ݳ�ʼ��

	if(Status == 0)
		Status = VL53L0X_StaticInit(pMyDevice); 		//��̬��ʼ��

	//�������Ŀ����Ҫ���ڵ��Թۿ��豸��Ϣ��ʵ��Ӧ����Ϊ�Ż����룬��ֱ������
	//���д�ӡ��Ϣ����䶼��Ӧ���ڵ��ԣ����²����ظ�
//	if(Status == 0)
//		Status = Wl_laserdis_getdeviceinfo(pMyDevice); 		

	/********************************************************************
	 * ģ���У׼˳���밴�����´������ν���У׼
	 ********************************************************************/

	//SPADУ׼�Ͳο�У׼���޻���Ҫ�󣬿�ֱ��ִ��
	if(Status == 0)
		Status = Wl_laserdis_calibration_init(pMyDevice); 	
	
	//ƫ��У׼��Ҫ�󻷾����޺�����ŵĻ����£�����һ�Ű�ɫ��ֽ����ģ����ǰ��10cm��������85%���ϣ�
	if(Status == 0)
		Status = Wl_laserdis_calibration_offset_init(pMyDevice);
	
	//ƫ��У׼�봮��У׼֮������ʱһС��ʱ�����ģ���ڲ���������Ӧ������
	delay_ms(500);	
	
	//����У׼��Ҫ�󻷾����޺�����ŵĻ����£�����һ�Ż�ɫ��ֽ����ģ����ǰ��10cm��������15%���£�
	if(Status == 0)
		Status = Wl_laserdis_calibration_xtalk_init(pMyDevice); 	

	//У׼��ɺ����ò���ģʽ
	if(Status == 0)
		Status = Wl_laserdis_measure_init(pMyDevice, 0,1); 		
	
    return Status;
}

/*********************************************************************
 *  �������ƣ�unsigned int WL_LasterDisSignleRangle
 *  �������ܣ���Ȩ�������
 *  ��    �Σ�distance
 *  ��    ��������������Ľ��
 *  ��    ע����
 ********************************************************************/
unsigned int WL_LasterDisSignleRangleDistance(unsigned int distance)
{
	static unsigned int distancetemp[8] = { 0 };
	static unsigned char distancecount = 0;
	unsigned char suni = 0;

	//�ٽ����β���ֵ���150mm����
	if(((distance > distancetemp[4]) && (distance - distancetemp[4] > 150))

		|| ((distancetemp[4] > distance) && (distancetemp[4] - distance > 150)))	
	{
		distancecount = 0;
		for(suni=0; suni<5; suni++)
		{
			distancetemp[suni] = 0;
		}
	}

	distancetemp[5] = distance;			//���汾�β������
	distancecount += 1;					//������Ч�����������
	if(distancecount > 4)
	{
		distancecount = 5;
	}
	
	for(suni=0; suni<5; suni++)			//��ʷ���������λ���ۼ�
	{
		distancetemp[suni] = distancetemp[suni+1];
		distancetemp[6] += distancetemp[suni];
	}

	distancetemp[7] = distancetemp[6] / distancecount;
	distancetemp[6] = 0;				//�ۼ�ֵ���㣬������һ�μ���

	return distancetemp[7];
}





