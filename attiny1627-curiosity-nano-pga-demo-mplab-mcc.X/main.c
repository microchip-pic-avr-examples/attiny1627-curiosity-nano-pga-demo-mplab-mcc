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

/**
 * <B><FONT COLOR=BLUE>uint8_t</FONT> getPGAGain(<FONT COLOR=BLUE>void</FONT>)</B>
 * This function returns the integer representation of the current PGA gain. 
 *  */
uint8_t getPGAGain(void)
{
    uint8_t PGAsettings = ((ADC0.PGACTRL & ADC_GAIN_gm) >> ADC_GAIN_gp);
    
    /*In the case of the ATtiny1627, the gain can be found by 
     * right shifting by the gain bits */
    
    return (0b1 << PGAsettings);
}

/**
 * <B><FONT COLOR=BLUE>uint8_t</FONT> getADCReferenceSetting(<FONT COLOR=BLUE>void</FONT>)</B>
 * This function returns the bits that determine which ADC reference is active.
 *  */
uint8_t getADCReferenceSetting(void)
{
    return ADC0.CTRLC & ADC_REFSEL_gm;
}

/**
 * <B><FONT COLOR=BLUE>void</FONT> enablePGAConnection(<FONT COLOR=BLUE>void</FONT>)</B>
 * This function connects the ADC's inputs to the PGA. PGA must be initialized before this.
 *  */
void enablePGAConnection(void)
{
    //Clear all bits in VIA
    ADC0.MUXPOS &= ~(ADC_VIA_gm);
    
    //Set the bits
    ADC0.MUXPOS |= (ADC_VIA_0_bm);
}

/**
 * <B><FONT COLOR=BLUE>void</FONT> disablePGAConnection(<FONT COLOR=BLUE>void</FONT>)</B>
 * This function connects the inputs directly to the ADC, bypassing the PGA. 
 *  */
void disablePGAConnection(void)
{
    ADC0.MUXPOS &= ~(ADC_VIA_gm);
}

/**
 * <B><FONT COLOR=BLUE>bool</FONT> isPGAConnected(<FONT COLOR=BLUE>void</FONT>)</B>
 * This function returns 1 (true) if the PGA is connected to the ADC. Otherwise it returns 0 (false).
 * 
 * Note: This function only works if the bits are set in a valid configuration (0b00 or 0b01).
 *  */
bool isPGAConnected(void)
{
    return ((ADC0.MUXPOS & ADC_VIA_gm) >> ADC_VIA_gp);
}

/**
 * <B><FONT COLOR=BLUE>void</FONT> setDACREF(<FONT COLOR=BLUE>uint8_t</FONT> value)</B>
 * @param value (uint8_t) - Sets the DACREF value in the Analog Comparator
 * 
 * This function is used to set the DACREF output value.
 *  */
void setDACREF(uint8_t value)
{
    AC0.DACREF = value;
}

//Enumeration to set the order of the gain sequencing
typedef enum{
    NO_PGA = 0, GAIN1, GAIN2, GAIN4, GAIN8, GAIN16
} SETUP_STATE;

/**
 * <B><FONT COLOR=BLUE>void</FONT> sequenceGains(<FONT COLOR=BLUE>void</FONT>)</B>
 * This function changes the setup the ADC each time it is run.
 *  */
void sequenceGains(void)
{
    static SETUP_STATE state = GAIN1;
    
    //Change the configuration of the Setup
    switch (state)
    {
        case NO_PGA:
            disablePGAConnection();
            break;
        case GAIN1:
            enablePGAConnection();
            ADC0_PGA_Gain_Setting(ADC_GAIN_1X_gc);
            break;
        case GAIN2:
            ADC0_PGA_Gain_Setting(ADC_GAIN_2X_gc);
            break;
        case GAIN4:
            ADC0_PGA_Gain_Setting(ADC_GAIN_4X_gc);
            break;
        case GAIN8:
            ADC0_PGA_Gain_Setting(ADC_GAIN_8X_gc);
            break;
        case GAIN16:
            ADC0_PGA_Gain_Setting(ADC_GAIN_16X_gc);
            break;
    }
    
    //If at the max gain, loopback to no PGA
    if (state == GAIN16)
    {
        state = NO_PGA;
    }
    else
    {
        state++;
    }
}

//Number of possible values for the ADC to output (4096 = 2^12)
#define ADC_VALUES 4096

int main(void)
{
    /* Initializes MCU, drivers and middleware */
    SYSTEM_Initialize();
    
    //Start with the PGA off
    disablePGAConnection();
    
    //Set the DACREF Value to 100mV (with 1.024V reference)
    setDACREF(25);
    
    //Result to print
    float result = 0.0;
        
    //Pre-calculate volts per bit for each reference setting
    const float REF_1V = 1.024 / ADC_VALUES;
    const float REF_2V = 2.048 / ADC_VALUES;
    const float REF_2_5V = 2.5 / ADC_VALUES;
    const float REF_4V = 4.096 / ADC_VALUES;
    
    //Unit for results
    char unit = 'V';
    
    while (1){
        //Put the CPU in sleep to save power
        sleep_cpu();
        
        //Reset unit to default of Volts
        unit = 'V';

        switch(getADCReferenceSetting())
        {
            case 0:     //Vdd Reference
            case 2:     //External Reference (VREF A)
                printf("Relative Measurement Mode");
                result = (float) ADC0_GetConversionResult() / ADC_VALUES;
                result *= 100; 
                unit = '%'; //Unit is % of Vdd, since we don't know what Vdd is
                break;
            case 4:     //1.024V reference
                printf("Vref is 1.024V");
                result = (REF_1V) * ADC0_GetConversionResult();
                break;
            case 5:     //2.048V reference
                printf("Vref is 2.048V");
                result = (REF_2V) * ADC0_GetConversionResult();
                break;
            case 6:     //2.5V reference
                printf("Vref is 2.5V");
                result = (REF_2_5V) * ADC0_GetConversionResult();
                break;
            case 7:     //4.096V reference
                printf("Vref is 4.096V");
                result = (REF_4V) * ADC0_GetConversionResult();
                break;
            default:    //Unknown Reference
                printf("Vref is ???");
                result = 0.00;
        }

        //Note! In order to use printf with floats on AVR, you must set the following linker options:
        //-Wl,-u,vfprintf -lprintf_flt -lm 
        printf("\n\rADC Raw Value is %i\n\rMeasured Signal ", (uint16_t) ADC0_GetConversionResult());
        
        //Select the correct message based on if the PGA is enabled
        if (isPGAConnected())
        {
            printf("(with gain of %i): ", getPGAGain());
        }
        else
        {
            printf("(with NO PGA): ");
        }
        
        //Switch to the next setup
        sequenceGains();
        
        //Select the correct result formatting
        if (unit == 'V')
        {
            //Volts
            printf("%.3fV\n\r\n\r", result);
        }
        else if (unit == '%')
        {
            //Percentages
            printf("%.1f%%\n\r\n\r", result);
        }
        
        //Wait until everything has been sent
        while (USART0_IsTxBusy());        
    }
}