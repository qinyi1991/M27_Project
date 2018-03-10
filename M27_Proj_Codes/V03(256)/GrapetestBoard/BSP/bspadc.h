/***********************定义BSPADC全局变量***************************************************************************/
#ifndef __BSPADC_H
#define __BSPADC_H

#include <bsp.h>

#define	  CalADAdd_Zero			    810
#define	  CalADAdd_1KG  		    820
#define	  CalADAdd_3KG  		    830
#define	  CalADAdd_5KG  		    840

#define	  EXT1_CalADAdd_Zero	            850
#define	  EXT1_CalADAdd_1KG  		    860
#define	  EXT1_CalADAdd_3KG  		    870
#define	  EXT1_CalADAdd_5KG  		    880

void DMA1_Channel1_Configuration(void);
void ADC_Configuration(void);
CPU_INT16U TestADC1_ConversionResult(void);
CPU_INT16S TestADC2_ConversionResult(void);

#endif

