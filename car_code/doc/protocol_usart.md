## 协议说明

 控制信息：上层对底层底盘运动的控制信息；

### 协议数据说明

一帧协议数据分为 6个部分，分别是包头、ID、长度位、数据、校验位、包尾。


| 协议数据 | 占用字节 | 详细描述                            |
| :------------| :-------| :--------------------------------- |
| HEADER1   | 1       | 数据的包头1                 |
| HEADER2 | 1      | 数据的包头2     |
| CMDID | 1       | 发送者地址ID           |
| LENGTH     | 1       |数据长度位                          |
| DATA  | 6      |数据位                          |
| CHECK     | 1      |校验位                              |
| END        | 1      |包尾                              |

在protocol.h中如下定义：

```c
typedef enum
{
		//包头1
    HEADER1,
	  //包头2
    HEADER2,
		//ID	
    CMDID,
		//长度位
    LENGTH,
		//数据位
    DATA,
	  //校验位
    CHECK,
	  //包尾
	  END
	
}DEF_STATE;

//接收信息结构体
typedef struct
{
		DEF_STATE state;
    unsigned char Cmd_ID;
    unsigned char  Length;
    unsigned char Re_Data[20];
    unsigned char Check;
}DEF_RE_RECEIVE;

typedef struct
{
	unsigned char ready_op;
}DEF_READ;

extern uint8_t check[20];
extern int16_t pack_velocity[4];
extern int16_t pack_imu[4];
void RECEIVE_PACK(unsigned char data);

```

## 数据发送和接收

### 1、数据发送

​       下位机暂不需要向上位机发送数据，可在未来版本加入。


### 2、数据接收

```c

void RECEIVE_PACK(unsigned char data)
{
     //校验和
    static unsigned char sum_check=0;
	  //数据计数
    static unsigned char data_num=0;
	   
	 switch(receive_data.state)
	 {
	    //包头1
			case HEADER1:
        if(data==0xB5)
        {
				//	printf("0x5B\n");
					receive_data.state = HEADER2;
					sum_check += data;
	
				}
				else if(data==0xC5)
				{
				  receive_data.state = HEADER2;
					sum_check += data;
				}
        else
				{
					receive_data.state=HEADER1;
					data_num = 0;
				}
        //sum_check = 0;   
				
        break;
			
      //包头2				
		  case HEADER2:
			  if(data==0x5B)
				{
				//   printf("oxB5\n");
					receive_data.state = CMDID1;
					sum_check += data;

				}else if(data==0x5C)
				{
				  receive_data.state = CMDID2;
					sum_check += data;
				}
				else 
				{
					receive_data.state=HEADER1;
					data_num = 0;
				}
				//sum_check = 0;
				
				break;
				
			//ID
			case CMDID1:
				if(data==0x01)
        {	
					 // printf("CMDID=%d\n",data);
					//	pack[0]=data;
						receive_data.state=LENGTH1;
						receive_data.Cmd_ID = data;
						sum_check += data;

        }
				else
				{	
            receive_data.state=HEADER1;
            sum_check=0;
				}
				break;
			case CMDID2:
				if(data==0x02)
				{
				    receive_data.state=LENGTH2;
						receive_data.Cmd_ID = data;
						sum_check += data; 
				}
				else
				{	
            receive_data.state=HEADER1;
            sum_check=0;
				}
				break;
			//长度
			case LENGTH1:
				//	printf("LENGTH=%d\n",data);
			  //	pack[1]=data;
					receive_data.Length = data;
					receive_data.state = DATA1;
					sum_check += data;
				
					break;
			case LENGTH2:
          receive_data.Length = data;
					receive_data.state = DATA2;
					sum_check += data;	

         break;			
			//数据
			case DATA1:
				//	printf("DATA=%d\n",data);
			    receive_data.Re_Data [data_num++] = data;
		    	check[data_num + 3] = data;
					sum_check+=data;
					if(data_num==receive_data.Length)
					{
						receive_data.state=CHECK1;
					}
				break;
					
			case DATA2:
				 receive_data.Re_Data [data_num++] = data;
		    	check[data_num + 3] = data;
					sum_check+=data;
					if(data_num==receive_data.Length)
					{
						receive_data.state=CHECK2;
					}
				break;
					
			//校验
			case CHECK1:
				  if(data == sum_check % 0xff)
					{
					    pack_velocity[0] = ((receive_data.Re_Data[0]<<8)| (receive_data.Re_Data[1]));
						  pack_velocity[1] = ((receive_data.Re_Data[2]<<8)| (receive_data.Re_Data[3]));
					   	pack_velocity[2] = ((receive_data.Re_Data[4]<<8)| (receive_data.Re_Data[5]));
			      //pack[3] = ((receive_data.Re_Data[6]<<8)| receive_data.Re_Data[7]);
					}
				  receive_data.state = END;
					data_num = 0;
					sum_check=0;
				break;
					
			case CHECK2:
				 if(data == sum_check % 0xff)
					{
					    pack_imu[0] = ((receive_data.Re_Data[0]<<8)| (receive_data.Re_Data[1]));
						  pack_imu[1] = ((receive_data.Re_Data[2]<<8)| (receive_data.Re_Data[3]));
					   	pack_imu[2] = ((receive_data.Re_Data[4]<<8)| (receive_data.Re_Data[5]));
			      //pack[3] = ((receive_data.Re_Data[6]<<8)| receive_data.Re_Data[7]);
					}
				  receive_data.state = END;
					data_num = 0;
					sum_check=0;
				break;
			//包尾
			case END:
				  if(data == 0xBB)
					{
					  receive_data.state = HEADER1;
						data_num = 0;
					  sum_check=0;
					}
					else if(data==0xCC)
					{
					  receive_data.state = HEADER1;
						data_num = 0;
					  sum_check=0;
					}
				 break;
					 
	 }
}
```
