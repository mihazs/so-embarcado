/*
 * File:   adc.c
 * Author: mihazs
 *
 * Created on May 17, 2024, 3:43 PM
 */


#include <xc.h>

unsigned int read_adc()
{
    ADCON0=0x11;
    ADCON0bits.GO=1;
    while(ADCON0bits.GO == 1);
   return ((((unsigned int)ADRESH)<<2)|(ADRESL>>6));
}

void config_adc()
{
    TRISA=0x07;
    ADCON1=0x02;
    ADCON0=0x11;
    ADCON1=0x0B;
    ADCON2=0x01;
}