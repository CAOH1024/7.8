#include "usart.h"

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  

#pragma import(__use_no_semihosting)     
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch,FILE *f)
{
	
	usart1_send(ch);
	return ch;
}

uint8_t Serial_Tx_HEXPackage[6]={0};
uint8_t Serial_Rx_HEXPackage[2]={0};


uint16_t Serial_RxFlag;



//////////////////////////////////////////////////////////////////
/**************************ʵ�ֺ���**********************************************
*��    ��:		usart1����һ���ֽ�
*********************************************************************************/
void usart1_send(u8 data)
{
	USART1->DR = data;
	while((USART1->SR&0x40)==0);	
}

void uart_init(u32 bound)
	{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;		//�����ʾ��ǰ״̬��״̬�ľ�̬����
	static uint8_t pRxPacket = 0;	//�����ʾ��ǰ��������λ�õľ�̬����
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		//�ж��Ƿ���USART1�Ľ����¼��������ж�
	{
		uint8_t RxData = USART_ReceiveData(USART1);				//��ȡ���ݼĴ���������ڽ��յ����ݱ���
		
		/*ʹ��״̬����˼·�����δ������ݰ��Ĳ�ͬ����*/
		
		/*��ǰ״̬Ϊ0���������ݰ���ͷ*/
		if (RxState == 0)
		{
			if (RxData == 0xFF)			//�������ȷʵ�ǰ�ͷ
			{
				RxState = 1;			//����һ��״̬
				pRxPacket = 0;			//���ݰ���λ�ù���
			}
		}
		/*��ǰ״̬Ϊ1���������ݰ�����*/
		else if (RxState == 1)
		{
			Serial_Rx_HEXPackage[pRxPacket] = RxData;	//�����ݴ������ݰ������ָ��λ��
			pRxPacket ++;				//���ݰ���λ������
			if (pRxPacket >= 2)			//����չ�4������
			{
				RxState = 2;			//����һ��״̬
			}
		}
		/*��ǰ״̬Ϊ2���������ݰ���β*/
		else if (RxState == 2)
		{
			if (RxData == 0xFE)			//�������ȷʵ�ǰ�β��
			{
				RxState = 0;			//״̬��0
				Serial_RxFlag = 1;		//�������ݰ���־λ��1���ɹ�����һ�����ݰ�
			}
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//�����־λ
	}
}

