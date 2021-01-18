/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include <avr/sleep.h>
#include "mcc_generated_files/mcc.h"

//Returns the current gain of the PGA
uint8_t getPGAGain(void)
{
    uint8_t PGAsettings = ((ADC0.PGACTRL & ADC_GAIN_gm) >> ADC_GAIN_gp);
    
    /*In the case of the ATtiny1627, the gain can be found by 
     * right shifting by the gain bits */
    
    return (0b1 << PGAsettings);
}

//Returns the bits associated with the ADC reference settings
uint8_t getADCReferenceSetting(void)
{
    return ADC0.CTRLC & ADC_REFSEL_gm;
}

#define ADC_VALUES 4096

int main(void)
{
    /* Initializes MCU, drivers and middleware */
    SYSTEM_Initialize();
    
    float result = 0.0;
        
    const float REF_1V = 1.024 / ADC_VALUES;
    const float REF_2V = 2.048 / ADC_VALUES;
    const float REF_2_5V = 2.5 / ADC_VALUES;
    const float REF_4V = 4.096 / ADC_VALUES;
    
    char unit = 'V';

    while (1){
        //Put the CPU in sleep to save power
        sleep_cpu();
        
        //Reset unit to default of "Volts"
        unit = 'V';
        
        switch(getADCReferenceSetting())
        {
            case 0: //Vdd Reference
                printf("Vref is Vdd");
                result = (float) ADC0_GetConversionResult() / ADC_VALUES;
                unit = '%'; //Unit is % of Vdd, since we don't know what Vdd is
                break;
            case 4: //1.024V reference
                printf("Vref is 1.024V");
                result = (REF_1V) * ADC0_GetConversionResult();
                break;
            case 5: //2.048V reference
                printf("Vref is 2.048V");
                result = (REF_2V) * ADC0_GetConversionResult();
                break;
            case 6:
                printf("Vref is 2.5V");
                result = (REF_2_5V) * ADC0_GetConversionResult();
                break;
            case 7: //4.096V reference
                printf("Vref is 4.096V");
                result = (REF_4V) * ADC0_GetConversionResult();
                break;
            default:
                printf("Vref is ???");
                result = 0.00;
        }
        
        

        //Note! In order to use printf with floats on AVR, you must set the following linker options:
        //-Wl,-u,vfprintf -lprintf_flt -lm 
        printf("\n\rADC Raw Value is %i\n\rMeasured Vdd/10 (with gain of %i): %.3f%c\n\r\n\r", (uint16_t) ADC0_GetConversionResult(), getPGAGain(), result, unit);
        
        //Wait until everything has been sent
        while (USART0_IsTxBusy());
    }
}
/**
    End of File
*/