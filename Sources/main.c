/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "MKL25Z4.h"
#include "dma.h"
#include "adc.h"

static int i = 0;

uint16_t vetor_adc[8];

void processes_adc_data(void);

int main(void)
{
	dma_config_t config;

	config.channel 				 		= DMA_CHANNEL_0;
	config.source_address 		 		= (uint32_t *)&ADC0_RA;
	config.destination_address 	 		= (uint32_t *)&vetor_adc;
	config.number_byte 			 		= DMA_SIZE_16_BIT;
	config.cycle_steal                  = 0;
	config.destination_increment 		= 0;
	config.destination_size 	 		= DMA_SIZE_16_BIT;
	config.source_size 					= DMA_SIZE_16_BIT;
	config.destination_address_modulo 	= 0;
	config.source_address_modulo 		= 0;
	config.channel_source 				= DMA_ADC0;

	dma_init(&config);

	adc_init(_16BIT);

	dma0_callback(processes_adc_data);

    for (;;)
    {
        i++;
        if(i>1000)
        {
        	i = 0;
        	//start conversion with channel 8 PTB0
        	ADC0_SC1A = (ADC_SC1_ADCH(ADC_4) | (ADC0_SC1A & (ADC_SC1_AIEN_MASK | ADC_SC1_DIFF_MASK)));
        }
    }
    /* Never leave main */
    return 0;
}

void processes_adc_data(void)
{
	uint32_t average_adc = 0;
	uint8_t i = 0;

	for(i=0;i<8;i++)
	{
		average_adc += vetor_adc[i];
	}
}
