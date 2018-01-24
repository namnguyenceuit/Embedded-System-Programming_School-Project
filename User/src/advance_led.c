#include "advance_led.h"

int chooseOP4;
char* OPTION4 = "4. Advance led(a,b,c):\n\
								a. Set set led\n\
								b. Set direction\n\
								c. Start\n\
								ESC: return previous menu\n\
								Your option: ";


void advance_led()
{
	uart_send(OPTION4);
	uart_receive();
	SPI_Configuration();
	SS_DISABLE;

	SPI_I2S_SendData(SPI1, 99);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
	SS_ENABLE;
}

void SPI_Configuration()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/* SPI_MASTER configuration -----------------------------*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  
	/*-------- Configuring SlaveSelect-Pin PA4 --------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*-------- Configuring SCK, MISO, MOSI --------*/
	//																SCK					MISO				MOSI
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
  
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
  
  /* SPI_SLAVE configuration ------------------------------*/ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	/*-------- Configuring SCK, MISO, MOSI --------*/
	//																	SS					SCK						MISO				MOSI
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_SPI3);  
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_SPI3);   
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_SPI3);  
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_SPI3);  
  
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI3, &SPI_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
  SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_RXNE, ENABLE);
 
  /* Enable SPI_SLAVE */
  SPI_Cmd(SPI3, ENABLE);
  /* Enable SPI_MASTER */
  SPI_Cmd(SPI1, ENABLE); 
}
