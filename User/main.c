/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 USART Print debugging routine:
 USART1_Tx(PA9).
 This example demonstrates using USART1(PA9) as a print debug port output.

*/

#include "debug.h"


/* Global typedef */

/* Global define */

/* Global Variable */

/*********************************************************************
 * @fn      IIC_Init
 *
 * @brief   Initializes the IIC peripheral.
 *
 * @return  none
 */
void IIC_Init(u32 bound, u16 address)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    I2C_InitTypeDef  I2C_InitTSturcture = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitTSturcture.I2C_ClockSpeed = bound;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_SMBusHost;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitTSturcture.I2C_OwnAddress1 = address;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C2, &I2C_InitTSturcture);

    I2C_Cmd(I2C2, ENABLE);
}







/*********************************************************************
 * @fn      AT24CXX_ReadOneByte
 *
 * @brief   Read one data from EEPROM.
 *
 * @param   ReadAddr - Read frist address.
 *
 * @return  temp - Read data.
 */
u8 IIC_ReadOneByte(u16 ReadAddr)
{
    u8 temp = 0;
//        printf("111111/r/n");
//        Delay_Ms(100);
    while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY) != RESET)
        ;
    I2C_GenerateSTART(I2C2, ENABLE);

    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
        ;
    I2C_Send7bitAddress(I2C2, 0xB0, I2C_Direction_Transmitter);

    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;

    I2C_SendData(I2C2, (u8)(ReadAddr & 0x00FF));
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    I2C_GenerateSTART(I2C2, ENABLE);

    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
        ;
    I2C_Send7bitAddress(I2C2, 0xB0, I2C_Direction_Receiver);

    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        ;
    while(I2C_GetFlagStatus(I2C2, I2C_FLAG_RXNE) == RESET)
        I2C_AcknowledgeConfig(I2C2, DISABLE);

    temp = I2C_ReceiveData(I2C2);
    I2C_GenerateSTOP(I2C2, ENABLE);

    return temp;
}

u16 IIC_ReadWord(u16 ReadAddr)
{
    u16 temp = 0;
    u16 highByte = 0;
    u16 lowByte = 0;

    while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY) != RESET)
         ;
     I2C_GenerateSTART(I2C2, ENABLE);

     while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
         ;
     I2C_Send7bitAddress(I2C2, 0xB0, I2C_Direction_Transmitter);

     while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
         ;

     I2C_SendData(I2C2, (u8)(ReadAddr & 0x00FF));
     while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
         ;

     I2C_GenerateSTART(I2C2, ENABLE);

     while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
         ;
     I2C_Send7bitAddress(I2C2, 0xB0, I2C_Direction_Receiver);

     while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
         ;
     while(I2C_GetFlagStatus(I2C2, I2C_FLAG_RXNE) == RESET)
         I2C_AcknowledgeConfig(I2C2, ENABLE);

     lowByte = I2C_ReceiveData(I2C2);
     while(I2C_GetFlagStatus(I2C2, I2C_FLAG_RXNE) == RESET)
         I2C_AcknowledgeConfig(I2C2, DISABLE);

     highByte = I2C_ReceiveData(I2C2);
     I2C_GenerateSTOP(I2C2, ENABLE);

    // Combine high byte and low byte into 16-bit value
    temp = ((u16)highByte << 8) | lowByte;

    return temp;
}


/*********************************************************************
 * @fn      AT24CXX_WriteOneByte
 *
 * @brief   Write one data to EEPROM.
 *
 * @param   WriteAddr - Write frist address.
 *
 * @return  DataToWrite - Write data.
 */
void IIC_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
    while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY) != RESET)
        ;
    I2C_GenerateSTART(I2C2, ENABLE);

    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
        ;
    I2C_Send7bitAddress(I2C2, 0XA0, I2C_Direction_Transmitter);

    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;

    I2C_SendData(I2C2, (u8)(WriteAddr & 0x00FF));
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    if(I2C_GetFlagStatus(I2C2, I2C_FLAG_TXE) != RESET)
    {
        I2C_SendData(I2C2, DataToWrite);
    }

    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;
    I2C_GenerateSTOP(I2C2, ENABLE);
}
uint32_t temp[300]={0};

int16_t temp_val=0;
uint8_t BufferTransTCP[1000]={0};

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();

    IIC_Init(100000, 0x10);

	USART_Printf_Init(115200);	
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
	printf("This is printf example\r\n");




	while(1)
    {

	    for (int var = 0; var <= 0xff; ++var) {

//	        var=0x86;


	    temp[var]=IIC_ReadWord(var);

//        temp1[var]=AT24CXX_ReadOneByte(var);
//	    printf("var,Value:0x%02x,%04x,%lf\r\n",var,temp[var],decodeLinear11(temp[var]));
//	    I2C_ReadRegister(I2C2, I2C_Register)

        Delay_Ms(10);

        }

//        printf("VIN:%lf \r\n",(temp[0x88]&0x7ff)/4.0);
//        printf("IIN:%lf \r\n",(temp[0x89]&0x7ff)/256.0);
//        printf("VOUT:%lf \r\n",(temp[0x8b])/512.0);
//        printf("IOUT:%lf \r\n",(temp[0x8c]&0x7ff)/256.0);
//        printf("temp1:%lf \r\n",(temp[0x8d]&0x07ff)/2.0);
//        printf("temp2:%lf \r\n",(temp[0x8e]&0x07ff)/2.0);
//        printf("fanspeed:%lf \r\n",(temp[0x90])/1.0);
//        printf("VI_POUT:%lf \r\n",((temp[0x8c]&0x7ff)/256.0)*((temp[0x8b])/512.0));


//        printf("VIN: %lf IIN: %lf VOUT: %lf IOUT: %lf temp1: %lf temp2: %lf fanspeed: %lf VI_POUT: %lf \r\n",
//            (temp[0x88] & 0x7ff) / 4.0,
//            (temp[0x89] & 0x7ff) / 256.0,
//            (temp[0x8b]) / 512.0,
//            (temp[0x8c] & 0x7ff) / 256.0,
//            (temp[0x8d] & 0x07ff) / 2.0,
//            (temp[0x8e] & 0x07ff) / 2.0,
//            (temp[0x90]) / 1.0,
//            ((temp[0x8c] & 0x7ff) / 256.0) * ((temp[0x8b]) / 512.0));
//
//
//
//        sprintf(BufferTransTCP,"DATA: VIN: %lf IIN: %lf VOUT: %lf IOUT: %lf temp1: %lf temp2: %lf fanspeed: %lf VI_POUT: %lf \r\n ",
//            (temp[0x88] & 0x7ff) / 4.0,
//            (temp[0x89] & 0x7ff) / 256.0,
//            (temp[0x8b]) / 512.0,
//            (temp[0x8c] & 0x7ff) / 256.0,
//            (temp[0x8d] & 0x07ff) / 2.0,
//            (temp[0x8e] & 0x07ff) / 2.0,
//            (temp[0x90]) / 1.0,
//            ((temp[0x8c] & 0x7ff) / 256.0) * ((temp[0x8b]) / 512.0));

        printf("DATA: VIN: %lf IIN: %lf VOUT: %lf IOUT: %lf temp1: %lf temp2: %lf fanspeed: %lf VI_POUT: %lf \r\n ",
            (temp[0x88] & 0x7ff) / 4.0,
            (temp[0x89] & 0x7ff) / 256.0,
            (temp[0x8b]) / 512.0,
            (temp[0x8c] & 0x7ff) / 256.0,
            (temp[0x8d] & 0x07ff) / 2.0,
            (temp[0x8e] & 0x07ff) / 2.0,
            (temp[0x90]) / 1.0,
            ((temp[0x8c] & 0x7ff) / 256.0) * ((temp[0x8b]) / 512.0));


	}
}

