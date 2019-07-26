#ifndef __OPEN_MV
#define __OPEN_MV

#include "bsp_usart.h"

#define OpenmvUart huart2   //串口宏定义，方便移植

//OPENMV识别到红色
#define OPENMV_RED 1
#define OPENMV_GREEN 2
#define OPENMV_BLUE 3

//颜色识别顺序信息结构体
typedef struct 
{
	//数据是否有效
   unsigned char  data_valid;
	//颜色顺序 从第一个到第三个
   unsigned char color_order[3];
	
	   unsigned char oled_color[3];
}DEF_COLOR;

//二维码信息
typedef struct 
{
	//数据是否有效
	unsigned char data_valid;
	//二维码确定搬运顺序
	unsigned char Qrcode[3];
}DEF_QR;

//openmv返回的物块位置
typedef struct
{
	//数据是否有效
	unsigned char data_valid;
	//物块相对于机械臂的x坐标值
	uint8_t x;
	//物块相对于机械臂的y坐标值
	uint8_t y;
}DEF_OBJECTLOCATION;

//状态机枚举类型
typedef enum
{
		//报头
    HEADER,
		//ID	
    CMDID,
		//长度
    LENGTH,
		//数据
    DATA,
	  //校验
    CHECK
}DEF_STATE;

//OPENMV接收信息结构体
typedef struct
{
		DEF_STATE state;
    unsigned char Cmd_ID;
    unsigned char  Length;
    unsigned char Re_Data[20];
    unsigned char Check;
}DEF_RE_OPENMV;

//openmv就绪标志位
typedef struct
{
	unsigned char ready_op;
}DEF_READ;
//OPENMV特征结构体
typedef struct
{
	//id
	uint8_t id;
	//串口接收迭代结构体
	DEF_RE_OPENMV rev;
	//颜色数据
	DEF_COLOR color;
	//二维码数据
	DEF_QR qr;
	//巡线数据
	DEF_OBJECTLOCATION ObjLocation;
	DEF_READ op_prepare;
}DEF_OPENMV;

//测试使用
extern uint8_t openmv_color_flag;
extern uint8_t openmv_qr_flag;
extern uint8_t check[6];
extern uint8_t openmv_u2_can1[8];
extern DEF_OPENMV openmv_1;
extern uint8_t cm_op[5];
extern uint16_t oledshow ;//123 321 132 ...表示顺序
void TELL_OP_MV(void);
void OPENMV_Init(void);
void OPMV_Rev(unsigned char data);
#endif
