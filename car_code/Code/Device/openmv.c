#include "bsp_usart.h"
#include "openmv.h"
//#include "module_arm.h"
#include "bsp_can.h"
#include "oled.h"

//设备ID 1 OPENMV  

//测试使用
uint8_t check[6];
DEF_OPENMV openmv_1 ; 
uint8_t cm_op[5]={0xff,0x04,0x01,0x00,0x05};
uint16_t oledshow = 321;
//OPENMV信息从串口接收转CAN1发送的数组
uint8_t openmv_u2_can1[8];
uint8_t openmv_color_flag=0;
uint8_t openmv_qr_flag=0;
extern uint8_t guess_color;


//openmv设备初始化
void OPENMV_Init(void)
{
	//设置ID
	openmv_1.id = 1;
	//状态机置首位
	openmv_1.rev.state = HEADER;
	//初始化相应的板载资源
}

//串口数据迭代
void OPMV_Rev(unsigned char data)
 {
	  //校验和
    static unsigned char sum_check=0;
	  //数据计数
    static unsigned char data_num=0;
		//数据迭代代码
    switch(openmv_1.rev.state)
    {
			//报头
			case HEADER:
				//如果是报头
        if(data==0XFF)
        {
					//测试
					printf("0xff\n");
					//状态机跳转至ID
					openmv_1.rev.state = CMDID;
				}
				//如果不是
        else
				{
					//状态机跳转至报头
					openmv_1.rev.state=HEADER;
					
				}
				//清空校验和
        sum_check = 0; 
				//清空数据接收迭代
				data_num = 0;
        break;
				
				//接收ID
				case CMDID:
					if(data==0x01||data==0x02||data==0x03||data==0x04||data==0x05)
          {	
						printf("CMDID=%d\n",data);
						//CAN1给A型主控第一位发数据ID
						openmv_u2_can1[0]=data;
						//状态机跳转至长度位
						openmv_1.rev.state=LENGTH;
						//赋值ID
						openmv_1.rev.Cmd_ID = data;
						//校验和迭代
						sum_check += data;
          }
					//如果非协议规定ID  不予接受
					else
					{
						//状态机跳转至报头		
            openmv_1.rev.state=HEADER;
						//清空校验和
            sum_check=0;
					}
				break;
				
				//长度位
				case LENGTH:
					printf("LENGTH=%d\n",data);
				//CAN1给A型主控第二位发数据长度
				openmv_u2_can1[1]=data;
					//长度赋值
					openmv_1.rev.Length = data;
					//状态机跳转数据位
					openmv_1.rev.state = DATA;
					//校验和迭代
					sum_check += data;
				
					check[0]=1;
					break;
				
				//数据位
				case DATA:
					printf("DATA=%d\n",data);
					//赋值
					openmv_1.rev.Re_Data [data_num++] = data;
					//校验和迭代
					sum_check+=data;
					//判断是否接收完数据位
					if(data_num==openmv_1.rev.Length)
					{
						//状态机跳转至校验
						openmv_1.rev.state=CHECK;
						check[1]=1;
					}
				break;
				
				//校验位
				case CHECK:
				//如果校验通过	
				if(data==sum_check)
        {
					printf("CHECK=%d",data);
					//如果是颜色数据回传
					if(openmv_1.rev.Cmd_ID == 0x01)
          {  
						
						if(!guess_color)
						{
													//信息赋值
						openmv_1.color.data_valid=openmv_1.rev.Re_Data[0];
						openmv_1.color.color_order[0]=openmv_1.rev.Re_Data[3];				
						openmv_1.color.color_order[1]=openmv_1.rev.Re_Data[2];
						openmv_1.color.color_order[2]=openmv_1.rev.Re_Data[1];	
							
							
						openmv_u2_can1[2]=openmv_1.rev.Re_Data[0];
						openmv_u2_can1[3]=openmv_1.color.color_order[0];
						openmv_u2_can1[4]=openmv_1.color.color_order[1];	
						openmv_u2_can1[5]=openmv_1.color.color_order[2];							
					
						
						if(openmv_1.color.data_valid==1||openmv_1.color.color_order[0]!=0||openmv_1.color.color_order[1]!=0||openmv_1.color.color_order[2]!=0)
						{
							openmv_color_flag=1;
						}		
						}						
						check[2]=1;
						//信息赋值
          }
					//如果是二维码数据回传
				  else if(openmv_1.rev.Cmd_ID == 0x02)
          {
						
			      OlED_Key.center = 0 ;
						//信息赋值
						openmv_1.qr.data_valid=openmv_1.rev.Re_Data[0];
						//openmv_u2_can1[2]=openmv_1.rev.Re_Data[0];
						//信息赋值
						openmv_1.qr.Qrcode[0]=openmv_1.rev.Re_Data[1];
						//openmv_u2_can1[3]=openmv_1.rev.Re_Data[1];
						//信息赋值
						openmv_1.qr.Qrcode[1]=openmv_1.rev.Re_Data[2];
						//openmv_u2_can1[4]=openmv_1.rev.Re_Data[2];
						//信息赋值
						openmv_1.qr.Qrcode[2]=openmv_1.rev.Re_Data[3];	
					  //openmv_u2_can1[5]=openmv_1.rev.Re_Data[3];	
								
												if(openmv_1.qr.data_valid==1)
						{
							openmv_qr_flag=1;
						}
						check[3]=1;
          }
					//如果是巡线信息
					else if(openmv_1.rev.Cmd_ID == 0x03)
					{
						openmv_1.ObjLocation.data_valid=openmv_1.rev.Re_Data[0];
						openmv_u2_can1[2]=openmv_1.rev.Re_Data[0];
						//信息赋值
						openmv_1.ObjLocation.x=openmv_1.rev.Re_Data[1];
						openmv_u2_can1[3]=openmv_1.rev.Re_Data[1];
						//信息赋值
						openmv_1.ObjLocation.y=openmv_1.rev.Re_Data[2];
						openmv_u2_can1[4]=openmv_1.rev.Re_Data[2];
						check[4]=1;
          }
					else if(openmv_1.rev.Cmd_ID == 0x05)
          {
						//信息赋值
						openmv_1.op_prepare.ready_op = openmv_1.rev.Re_Data[0];		
          }
        }
					//状态机跳转至首位
					openmv_1.rev.state=HEADER;
					//清空数据迭代
					data_num = 0;
					//清空校验和
					sum_check=0;
				break;
				
				default:break;
    }
}
 


void TELL_OP_MV()
{
//		cm_op[3]=track_1.state;
//		cm_op[4]=track_1.state+5;
//	  switch(arm1.ArmState) 
//		{
//			case CARRY1_1:
//				cm_op[3]=0x01;
//				cm_op[4]=cm_op[3]+5;
//			  HAL_UART_Transmit(&OpenmvUart,cm_op,5,55); 
//			  break;
//			case CARRY1_2:
//				cm_op[3]=0x02;
//				cm_op[4]=cm_op[3]+5;
//			  HAL_UART_Transmit(&OpenmvUart,cm_op,5,55); 
//			  break;
//			case CARRY1_3:
//				cm_op[3]=0x03;
//				cm_op[4]=cm_op[3]+5;
//			  HAL_UART_Transmit(&OpenmvUart,cm_op,5,55); 
//			  break;
//			case CARRY2_2:
//				cm_op[3]=0x04;
//				cm_op[4]=cm_op[3]+5;
//			  HAL_UART_Transmit(&OpenmvUart,cm_op,5,55); 
//			  break;
//			case CARRY2_3:
//				cm_op[3]=0x05;
//				cm_op[4]=cm_op[3]+5;
//			  HAL_UART_Transmit(&OpenmvUart,cm_op,5,55); 
//			  break;
//			case CARRY3_2:
//				cm_op[3]=0x06;
//				cm_op[4]=cm_op[3]+5;
//			  HAL_UART_Transmit(&OpenmvUart,cm_op,5,55); 
//			  break;
//			case CARRY3_3:
//				cm_op[3]=0x07;
//				cm_op[4]=cm_op[3]+5;
//			  HAL_UART_Transmit(&OpenmvUart,cm_op,5,55); 
//			  break;
//			default:
//				break;
//		}
}

 
 
 
 
