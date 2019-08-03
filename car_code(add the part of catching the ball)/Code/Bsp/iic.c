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
 *  �������ƣ�void IIC_SDA_OUT
 *  �������ܣ��������IIC��SDA�˿�Ϊ���ģʽ
 *  ��    �Σ���
 *  ��    ������
 *  ��    ע����
 ********************************************************************/
static void IIC_SDA_OUT(void)
{
	//SDA PC1
	//��ʼ��GPIO�ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	
    GPIO_InitStructure.Pin = IIC_SDA_GPIO_Pin;	
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;   
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;     
   HAL_GPIO_Init(IIC_SDA_GPIOx, &GPIO_InitStructure);
}

/*********************************************************************
 *  �������ƣ�void IIC_SDA_IN
 *  �������ܣ��������IIC��SDA�˿�Ϊ����ģʽ
 *  ��    �Σ���
 *  ��    ������
 *  ��    ע����
 ********************************************************************/
static void IIC_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = IIC_SDA_GPIO_Pin;
		//����ģʽΪ��������
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;   
	  GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(IIC_SDA_GPIOx, &GPIO_InitStructure);
}

/*********************************************************************
 *  �������ƣ�void IIC_Init
 *  �������ܣ����IIC�˿ڳ�ʼ��
 *  ��    �Σ���
 *  ��    ������
 *  ��    ע����
 ********************************************************************/
void IIC_Init(void)
{			
    GPIO_InitTypeDef GPIO_InitStructure;
    
	//ʹ��GPIOCʱ��
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
 *  �������ƣ�void IIC_Start
 *  �������ܣ����IIC��ʼ�ź�
 *  ��    �Σ���
 *  ��    ������
 *  ��    ע����
 ********************************************************************/
static unsigned char IIC_Start(void)
{
	IIC_SDA_OUT();			//sda�����
	IIC_SDA_HIGH();
	if(!IIC_SDA_READ())     //��������ʧ��
	    return 0;	
	IIC_SCL_HIGH();
	delay_us(1);
 	IIC_SDA_LOW();
	if(IIC_SDA_READ())      //��������ʧ��
	    return 0;
	delay_us(1);
	IIC_SCL_LOW();          //ǯסI2C���ߣ�׼�����ͻ��������
	return 1;
}

/*********************************************************************
 *  �������ƣ�void IIC_Stop
 *  �������ܣ����IICֹͣ�ź�
 *  ��    �Σ���
 *  ��    ������
 *  ��    ע����
 ********************************************************************/
static void IIC_Stop(void)
{
	IIC_SDA_OUT();			//sda�����
	IIC_SCL_LOW();
	IIC_SDA_LOW();
 	delay_us(1);
	IIC_SCL_HIGH(); 
	IIC_SDA_HIGH();			//����I2C���߽����ź�
	delay_us(1);							   	
}

/*********************************************************************
 *  �������ƣ�void IIC_Wait_Ack
 *  �������ܣ����IIC�ȴ�Ӧ��
 *  ��    �Σ���
 *  ��    ������
 *  ��    ע����
 ********************************************************************/
static unsigned char IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	
	IIC_SDA_IN();      //SDA����Ϊ����  
	IIC_SDA_HIGH();
	delay_us(1);	   
	IIC_SCL_HIGH();
	delay_us(1);	 
	while(IIC_SDA_READ())
	{
		if(ucErrTime++ > 50)
		{
			IIC_Stop();     //�Ի���Ӧ��ֱ���˳�
			return 0;
		}
	    delay_us(1);
	}
	IIC_SCL_LOW();//ʱ�����0 	   
	return 1;  
} 

/*********************************************************************
 *  �������ƣ�void IIC_Ack
 *  �������ܣ����IICӦ��
 *  ��    �Σ���
 *  ��    ������
 *  ��    ע����
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
 *  �������ƣ�void IIC_NAck
 *  �������ܣ����IIC��Ӧ��
 *  ��    �Σ���
 *  ��    ������
 *  ��    ע����
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
 *  �������ƣ�void IIC_Send_Byte
 *  �������ܣ����IIC����һ���ֽ�
 *  ��    �Σ�txd��Ҫ���͵�����
 *  ��    ������
 *  ��    ע����
 ********************************************************************/
static void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	IIC_SDA_OUT(); 	    
    IIC_SCL_LOW();//����ʱ�ӿ�ʼ���ݴ���
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
 *  �������ƣ�void IIC_Read_Byte
 *  �������ܣ����IIC��ȡһ���ֽ�
 *  ��    �Σ�ack���Ƿ����Ӧ��
 *  ��    ������ȡ������������
 *  ��    ע����
 ********************************************************************/
static unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA_IN();//SDA����Ϊ����
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
        IIC_Ack(); //����ACK 
    else
        IIC_NAck();//����nACK  
    return receive;
} 

/*********************************************************************
 *  �������ƣ�void IIC_Write
 *  �������ܣ����IICд��һ������
 *  ��    �Σ�addr��Ҫд����豸��ַ
 *			  reg��Ҫд�����ݵ����ݵ�ַ
 *			  len��Ҫд������ݳ���
 *			  *data��Ҫд����ַ���
 *  ��    ����д��״̬��0��д��ɹ�		1��д��ʧ��
 *  ��    ע����
 ********************************************************************/
unsigned char IIC_Write(unsigned char addr, unsigned char reg, unsigned char len, unsigned char * data)  
{
	int i;
    if (!IIC_Start())   //��ʼ�ź�
        return 1;
    IIC_Send_Byte(addr | 0x00);  //����д����
    if (!IIC_Wait_Ack())        //�ȴ�Ӧ��
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);         //����д��ַ
    IIC_Wait_Ack();
    
	for (i = 0; i < len; i++) 
	{
        IIC_Send_Byte(data[i]);     //��������
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
 *  �������ƣ�void IIC_Read
 *  �������ܣ����IIC��ȡһ������
 *  ��    �Σ�addr��Ҫ��ȡ���豸��ַ
 *			  reg��Ҫ��ȡ���ݵ����ݵ�ַ
 *			  len��Ҫ��ȡ�����ݳ���
 *			  *buf��Ҫ��ȡ���ַ���
 *  ��    ������ȡ״̬��0����ȡ�ɹ�		1����ȡʧ��
 *  ��    ע����
 ********************************************************************/
unsigned char IIC_Read(unsigned char addr, unsigned char reg, unsigned char len, unsigned char * buf) 
{
    if (!IIC_Start())   //��ʼ�ź�
        return 1;
    IIC_Send_Byte(addr | 0x00);   //����д����
    if (!IIC_Wait_Ack())        //�ȴ�Ӧ��
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);         //����д��ַ
    IIC_Wait_Ack();    
    IIC_Start();
    IIC_Send_Byte(addr | 0x01);   //���Ͷ�����
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






