/**
 *	Keil project for Mifare RC522 RFID reader
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
/* Include core modules */

#include "stm32f10x.h"
/* Include my libraries here */
#include "defines.h"
//#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_mfrc522.h"
#include "stm32f10x_flash.h"
#include "main.h"
#include <stdio.h>

#define READER_COUNT 7             // use it for settings readers count 1-7

FLASH_Status StatusFlash;
uint32_t Flash_Buffer;
uint8_t reader=1;
uint8_t MemoryID[10][5];
uint8_t BufferID[10][5];
uint8_t CardID[5];
	/* My cards id */
	/* I read them with program below, and write this here */
	uint8_t MyID[5] = {
		0xC0, 0x86, 0xB7, 0x56, 0xA7	/* My card on my keys */
	};
	char buffer[50];
	
int main(void) {
	

	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	//TM_DELAY_Init();
	
	/* Initialize leds */
	//TM_DISCO_LedInit();
	
	/* Initialize LCD */
	//TM_ILI9341_Init();
	//TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	/* Initialize MFRC522 RFID */
	   FLASH_Init();
	   FLASH_ReadMemotyId();
	   GPIO_InitPins();
	    for(reader=1; reader<=READER_COUNT; reader++)
		   {
	         TM_MFRC522_Init();			 
			 }
			 reader=0;
	   // switch cs for every reader
     MFRC522_CS_PORT->BSRR = GPIO_Pin_1;		
     MFRC522_CS_PORT->BSRR = GPIO_Pin_2;		
     MFRC522_CS_PORT->BSRR = GPIO_Pin_3;   
	   MFRC522_CS_PORT->BSRR = GPIO_Pin_4;
     MFRC522_CS_PORT->BSRR = GPIO_Pin_8;   
     MFRC522_CS_PORT->BSRR = GPIO_Pin_9; 
     MFRC522_CS_PORT->BSRR = GPIO_Pin_10; 	 
	/* Watermark */
	//TM_ILI9341_Puts(90, 310, "stm32f4-discovery.com", &TM_Font_7x10, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
	
			
	while (1) {
		/* If any card detected */
		if (!(GPIOC->IDR & GPIO_IDR_IDR15)) {
			 FLASH_WriteMemotyId();
			 GPIOB->BSRR = GPIO_Pin_10;
			 delay_ms(1000);
			 GPIOB->BSRR = GPIO_Pin_10<<16;
		   delay_ms(1000);
			 NVIC_SystemReset();
		}
		else {
			 for(reader=1; reader<=READER_COUNT; reader++)
				 {
						if (TM_MFRC522_Check(CardID) == MI_OK) {
							/* CardID is valid */
							 memcpy(BufferID[reader],CardID,sizeof(CardID));
							 //FLASH_WriteMemotyId();
							 //FLASH_ReadMemotyId();
							// FLASH_ReadMemotyId((void *)&ReadMemoryID, (uint32_t *) FLASH_PAGE_ADDR_ID, sizeof(MemoryID));
							// (uint32_t)ReadMemoryID[1][1]=*(__IO uint32_t*)FLASH_PAGE_ADDR;  //flash_read(*(uint32_t *) FLASH_PAGE_ADDR_ID);
							/* Check if this is my card */
							if (TM_MFRC522_Compare(MemoryID[reader], BufferID[reader]) == MI_OK) {
								switch (reader)
								{
									case 1: GPIOB->BSRR = GPIO_Pin_0; break;
									case 2: GPIOB->BSRR = GPIO_Pin_1; break;
									case 3: GPIOB->BSRR = GPIO_Pin_5; break;
									case 4: GPIOB->BSRR = GPIO_Pin_6; break;
									case 5: GPIOB->BSRR = GPIO_Pin_7; break;
									case 6: GPIOB->BSRR = GPIO_Pin_8; break;
									case 7: GPIOB->BSRR = GPIO_Pin_9; break;
									
								}
								//TM_ILI9341_Puts(10, 150, "Hello tilz0R!", &TM_Font_11x18, ILI9341_COLOR_GREEN, ILI9341_COLOR_BLACK);
							} else {
								switch (reader)
								{
									case 1: GPIOB->BSRR = GPIO_Pin_0<<16; break;
									case 2: GPIOB->BSRR = GPIO_Pin_1<<16; break;
									case 3: GPIOB->BSRR = GPIO_Pin_5<<16; break;
									case 4: GPIOB->BSRR = GPIO_Pin_6<<16; break;
									case 5: GPIOB->BSRR = GPIO_Pin_7<<16; break;
									case 6: GPIOB->BSRR = GPIO_Pin_8<<16; break;
                  case 7: GPIOB->BSRR = GPIO_Pin_9<<16; break;									
								}
								//TM_ILI9341_Puts(10, 150, "Good bye man!", &TM_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
							}
							/* Print on LCD */
							//TM_ILI9341_Puts(10, 10, "Card detected    ", &TM_Font_11x18, 0x00, 0xFFFF);
							sprintf(buffer, "0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x", CardID[0], CardID[1], CardID[2], CardID[3], CardID[4]);
							//TM_ILI9341_Puts(10, 30, buffer, &TM_Font_11x18, 0x00, 0xFFFF);
						} else {
							
							switch (reader)
								{
									case 1: GPIOB->BSRR = GPIO_Pin_0<<16; break;
									case 2: GPIOB->BSRR = GPIO_Pin_1<<16; break;
									case 3: GPIOB->BSRR = GPIO_Pin_5<<16; break;
									case 4: GPIOB->BSRR = GPIO_Pin_6<<16; break;
									case 5: GPIOB->BSRR = GPIO_Pin_7<<16; break;
									case 6: GPIOB->BSRR = GPIO_Pin_8<<16; break;	
									case 7: GPIOB->BSRR = GPIO_Pin_9<<16; break;		
								}
							
							 // kard on reader
							// GPIOB->BSRR = GPIO_Pin_0<<16;
							/* Some printing to delete content */
							//TM_ILI9341_Puts(10, 10, "Card not detected", &TM_Font_11x18, 0x00, 0xFFFF);
							//TM_ILI9341_Puts(10, 150, "             ", &TM_Font_11x18, ILI9341_COLOR_GREEN, ILI9341_COLOR_WHITE);
							//TM_ILI9341_Puts(10, 30, "    \n    \n    \n    \n    ", &TM_Font_11x18, 0x00, 0xFFFF);
						}
				}
				//MFRC522_CS_PORT->BSRR = GPIO_Pin_3;  
				reader=0;				
	   }
	}
}

void CS_Pins_HIGH(void)
{	
		MFRC522_CS_PORT->BSRR = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
}

void CS_Pin_LOW(uint8_t rdr)
{
	switch(rdr)
				{
		      // switch cs for every reader
          case 1: MFRC522_CS_PORT->BSRR = GPIO_Pin_1<<16;	break;	
          case 2: MFRC522_CS_PORT->BSRR = GPIO_Pin_2<<16;	break;	
          case 3: MFRC522_CS_PORT->BSRR = GPIO_Pin_3<<16; break;
					case 4: MFRC522_CS_PORT->BSRR = GPIO_Pin_4<<16;	break;	
          case 5: MFRC522_CS_PORT->BSRR = GPIO_Pin_8<<16;	break;	
          case 6: MFRC522_CS_PORT->BSRR = GPIO_Pin_9<<16;	break;	
          case 7: MFRC522_CS_PORT->BSRR = GPIO_Pin_10<<16; break;						
				}
}


void FLASH_Init(void) {
    /* Next commands may be used in SysClock initialization function
       In this case using of FLASH_Init is not obligatorily */
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable);
    /* Flash 2 wait state */
    FLASH_SetLatency( FLASH_Latency_2);
}

//(uint32_t)ReadMemoryID[1][1]=*(__IO uint32_t*)FLASH_PAGE_ADDR;

void FLASH_ReadMemotyId(void) {
    //Read settings
	  uint16_t i;
    uint8_t *source_addr = (__IO uint8_t *)FLASH_PAGE_ADDR_ID;
    uint8_t *dest_addr = (void *)MemoryID;
	  memcpy(dest_addr, source_addr, sizeof(MemoryID));
    /*for ( i=0; i<5; i++) {
        ReadMemoryID[1]= *source_addr;
        source_addr++;
        dest_addr++;
    }*/
}

/*
void FLASH_ReadMemotyId(void) {
    //Read settings
	  uint16_t i;
    uint32_t *source_addr = (uint32_t *)FLASH_PAGE_ADDR;
    uint32_t *dest_addr = (void *)ReadMemoryID;
	  memcpy(dest_addr, source_addr, sizeof(ReadMemoryID));
    for ( i=0; i<sizeof(ReadMemoryID)/4; i++) {
        *dest_addr = *(__IO uint32_t*)source_addr;
        source_addr++;
        dest_addr++;
    }
}

*/

void FLASH_WriteMemotyId(void) {
	  uint16_t i;
	  uint32_t *source_addr = (void *)&BufferID;
    uint32_t *dest_addr = (uint32_t *) FLASH_PAGE_ADDR_ID;
    FLASH_Unlock();
    FLASH_ErasePage(FLASH_PAGE_ADDR_ID);
    // Write settings
    for ( i=0; i<sizeof(BufferID)/4; i++) {
        StatusFlash=FLASH_ProgramWord((uint32_t)dest_addr, *source_addr);
        source_addr++;
        dest_addr++;
    }
 
    FLASH_Lock();
}

void GPIO_InitPins(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	//Enable clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1  | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOC, &GPIO_InitStruct);	
}



void delay_ms(uint32_t time)
			{
				uint32_t i;
				for (i=0; i<time*8000; i++ );
			}
/*
uint32_t flash_read(uint32_t address)
{
	return (*(__IO uint32_t*) address);
}

	
void FLASH_ReadMemotyId(uint32_t *dest_addr, uint32_t *source_addr, uint16_t size)
{
	uint16_t i;
  //uint32_t *source_addr = (uint32_t *) FLASH_PAGE_ADDR_ID;
  //uint32_t *dest_addr = (void *)&ReadMemoryID;
	for (i=0; i<size; i++)
	{
			*dest_addr = flash_read(*source_addr);    //+(i*4)
		  dest_addr++;
		  source_addr++;
	}
}
*/
