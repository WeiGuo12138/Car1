#include "iic.h"

#ifdef	__DELAY_H
	#define delay_us	Delay_us
#else
	void Delay_us(unsigned char nus)
	{
		unsigned char i = 0;
		
		while(nus--)
		{
			for(i=0; i<8 ;i--);
		}
	}
#endif

/*********************************************************************
 *  函数名称：void IIC_SDA_OUT
 *  函数功能：设置软件IIC的SDA端口为输出模式
 *  形    参：无
 *  输    出：无
 *  备    注：无
 ********************************************************************/
static void IIC_SDA_OUT(void)
{
	//SDA PC1
	//初始化GPIO结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	
    GPIO_InitStructure.Pin = IIC_SDA_GPIO_Pin;	
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;   
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;     
   HAL_GPIO_Init(IIC_SDA_GPIOx, &GPIO_InitStructure);
}

/*********************************************************************
 *  函数名称：void IIC_SDA_IN
 *  函数功能：设置软件IIC的SDA端口为输入模式
 *  形    参：无
 *  输    出：无
 *  备    注：无
 ********************************************************************/
static void IIC_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = IIC_SDA_GPIO_Pin;
		//设置模式为上拉输入
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;   
	  GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(IIC_SDA_GPIOx, &GPIO_InitStructure);
}

/*********************************************************************
 *  函数名称：void IIC_Init
 *  函数功能：软件IIC端口初始化
 *  形    参：无
 *  输    出：无
 *  备    注：无
 ********************************************************************/
void IIC_Init(void)
{			
    GPIO_InitTypeDef GPIO_InitStructure;
    
	//使能GPIOC时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    /*  IIC_SCL     */
    GPIO_InitStructure.Pin = IIC_SCL_GPIO_Pin;	
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;      
    GPIO_InitStructure.Speed =GPIO_SPEED_FREQ_HIGH;    
   HAL_GPIO_Init(IIC_SCL_GPIOx, &GPIO_InitStructure);					    

    /*  IIC_SDA     */
    GPIO_InitStructure.Pin = IIC_SDA_GPIO_Pin;	
    GPIO_InitStructure.Mode =GPIO_MODE_OUTPUT_PP;     
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;     
    HAL_GPIO_Init(IIC_SDA_GPIOx, &GPIO_InitStructure);

	 HAL_GPIO_WritePin(IIC_SCL_GPIOx, IIC_SCL_GPIO_Pin,GPIO_PIN_SET);
	 HAL_GPIO_WritePin(IIC_SDA_GPIOx, IIC_SDA_GPIO_Pin,GPIO_PIN_RESET);
}

/*********************************************************************
 *  函数名称：void IIC_Start
 *  函数功能：软件IIC开始信号
 *  形    参：无
 *  输    出：无
 *  备    注：无
 ********************************************************************/
static unsigned char IIC_Start(void)
{
	IIC_SDA_OUT();			//sda线输出
	IIC_SDA_HIGH();
	if(!IIC_SDA_READ())     //总线拉高失败
	    return 0;	
	IIC_SCL_HIGH();
	delay_us(1);
 	IIC_SDA_LOW();
	if(IIC_SDA_READ())      //总线拉低失败
	    return 0;
	delay_us(1);
	IIC_SCL_LOW();          //钳住I2C总线，准备发送或接收数据
	return 1;
}

/*********************************************************************
 *  函数名称：void IIC_Stop
 *  函数功能：软件IIC停止信号
 *  形    参：无
 *  输    出：无
 *  备    注：无
 ********************************************************************/
static void IIC_Stop(void)
{
	IIC_SDA_OUT();			//sda线输出
	IIC_SCL_LOW();
	IIC_SDA_LOW();
 	delay_us(1);
	IIC_SCL_HIGH(); 
	IIC_SDA_HIGH();			//发送I2C总线结束信号
	delay_us(1);							   	
}

/*********************************************************************
 *  函数名称：void IIC_Wait_Ack
 *  函数功能：软件IIC等待应答
 *  形    参：无
 *  输    出：无
 *  备    注：无
 ********************************************************************/
static unsigned char IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	
	IIC_SDA_IN();      //SDA设置为输入  
	IIC_SDA_HIGH();
	delay_us(1);	   
	IIC_SCL_HIGH();
	delay_us(1);	 
	while(IIC_SDA_READ())
	{
		if(ucErrTime++ > 50)
		{
			IIC_Stop();     //丛机不应答直接退出
			return 0;
		}
	    delay_us(1);
	}
	IIC_SCL_LOW();//时钟输出0 	   
	return 1;  
} 

/*********************************************************************
 *  函数名称：void IIC_Ack
 *  函数功能：软件IIC应答
 *  形    参：无
 *  输    出：无
 *  备    注：无
 ********************************************************************/
static void IIC_Ack(void)
{
	IIC_SCL_LOW();
	IIC_SDA_OUT();
	IIC_SDA_LOW();
	delay_us(1);
	IIC_SCL_HIGH();
	delay_us(1);
	IIC_SCL_LOW();
}
	
/*********************************************************************
 *  函数名称：void IIC_NAck
 *  函数功能：软件IIC不应答
 *  形    参：无
 *  输    出：无
 *  备    注：无
 ********************************************************************/
static void IIC_NAck(void)
{
	IIC_SCL_LOW();
	IIC_SDA_OUT();
	IIC_SDA_HIGH();
	delay_us(1);
	IIC_SCL_HIGH();
	delay_us(1);
	IIC_SCL_LOW();
}

/*********************************************************************
 *  函数名称：void IIC_Send_Byte
 *  函数功能：软件IIC发送一个字节
 *  形    参：txd：要发送的数据
 *  输    出：无
 *  备    注：无
 ********************************************************************/
static void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	IIC_SDA_OUT(); 	    
    IIC_SCL_LOW();//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {     
		if(txd & 0x80)
		{
			IIC_SDA_HIGH();			
		}
		else
		{
			IIC_SDA_LOW();
		}
        txd <<= 1; 	  
		delay_us(1);   
		IIC_SCL_HIGH();
		delay_us(1); 
		IIC_SCL_LOW();	
		delay_us(1);
    }	 
} 	 
  
/*********************************************************************
 *  函数名称：void IIC_Read_Byte
 *  函数功能：软件IIC读取一个字节
 *  形    参：ack：是否进行应答
 *  输    出：读取到的数据内容
 *  备    注：无
 ********************************************************************/
static unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_LOW(); 
        delay_us(2);
		IIC_SCL_HIGH();
        receive<<=1;
        if(IIC_SDA_READ())
            receive++;   
		delay_us(2); 
    }					 
    if (ack)
        IIC_Ack(); //发送ACK 
    else
        IIC_NAck();//发送nACK  
    return receive;
} 

/*********************************************************************
 *  函数名称：void IIC_Write
 *  函数功能：软件IIC写入一串数据
 *  形    参：addr：要写入的设备地址
 *			  reg：要写入数据的数据地址
 *			  len：要写入的数据长度
 *			  *data：要写入的字符串
 *  输    出：写入状态：0：写入成功		1：写入失败
 *  备    注：无
 ********************************************************************/
unsigned char IIC_Write(unsigned char addr, unsigned char reg, unsigned char len, unsigned char * data)  
{
	int i;
    if (!IIC_Start())   //起始信号
        return 1;
    IIC_Send_Byte(addr | 0x00);  //发送写命令
    if (!IIC_Wait_Ack())        //等待应答
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);         //发送写地址
    IIC_Wait_Ack();
    
	for (i = 0; i < len; i++) 
	{
        IIC_Send_Byte(data[i]);     //发送数据
        if (!IIC_Wait_Ack()) 
        {
            IIC_Stop();
            return 0;
        }
    }
    IIC_Stop();
    return 0;
}

/*********************************************************************
 *  函数名称：void IIC_Read
 *  函数功能：软件IIC读取一串数据
 *  形    参：addr：要读取的设备地址
 *			  reg：要读取数据的数据地址
 *			  len：要读取的数据长度
 *			  *buf：要读取的字符串
 *  输    出：读取状态：0：读取成功		1：读取失败
 *  备    注：无
 ********************************************************************/
unsigned char IIC_Read(unsigned char addr, unsigned char reg, unsigned char len, unsigned char * buf) 
{
    if (!IIC_Start())   //起始信号
        return 1;
    IIC_Send_Byte(addr | 0x00);   //发送写命令
    if (!IIC_Wait_Ack())        //等待应答
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);         //发送写地址
    IIC_Wait_Ack();    
    IIC_Start();
    IIC_Send_Byte(addr | 0x01);   //发送读命令
    IIC_Wait_Ack();    
    while (len) 
    {
        if (len == 1)
            *buf = IIC_Read_Byte(0);
        else
            *buf = IIC_Read_Byte(1);
        buf++;
        len--;
    }
    IIC_Stop();
    return 0;
}






