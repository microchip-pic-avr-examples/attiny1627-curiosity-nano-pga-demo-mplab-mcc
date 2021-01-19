/**
  @Company
    Microchip Technology Inc.

  @Description
    This Source file provides APIs.
    Generation Information :
    Driver Version    :   1.0.0
*/
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


#include "../include/evsys.h"

int8_t EVSYS_Initialize()
{
    //CHANNEL0 RTC_PIT_DIV2048; 
    EVSYS.CHANNEL0 = 0x0A;
    //CHANNEL1 OFF; 
    EVSYS.CHANNEL1 = 0x00;
    //CHANNEL2 OFF; 
    EVSYS.CHANNEL2 = 0x00;
    //CHANNEL3 OFF; 
    EVSYS.CHANNEL3 = 0x00;
    //CHANNEL4 OFF; 
    EVSYS.CHANNEL4 = 0x00;
    //CHANNEL5 OFF; 
    EVSYS.CHANNEL5 = 0x00;
    //SWEVENTA CH0; 
    EVSYS.SWEVENTA = 0x01;
    //USER CHANNEL0; 
    EVSYS.USERADC0START = 0x01;
    //USER OFF; 
    EVSYS.USERCCLLUT0A = 0x00;
    //USER OFF; 
    EVSYS.USERCCLLUT0B = 0x00;
    //USER OFF; 
    EVSYS.USERCCLLUT1A = 0x00;
    //USER OFF; 
    EVSYS.USERCCLLUT1B = 0x00;
    //USER OFF; 
    EVSYS.USERCCLLUT2A = 0x00;
    //USER OFF; 
    EVSYS.USERCCLLUT2B = 0x00;
    //USER OFF; 
    EVSYS.USERCCLLUT3A = 0x00;
    //USER OFF; 
    EVSYS.USERCCLLUT3B = 0x00;
    //USER OFF; 
    EVSYS.USEREVSYSEVOUTA = 0x00;
    //USER CHANNEL0; 
    EVSYS.USEREVSYSEVOUTB = 0x01;
    //USER OFF; 
    EVSYS.USEREVSYSEVOUTC = 0x00;
    //USER OFF; 
    EVSYS.USERTCA0CNTA = 0x00;
    //USER OFF; 
    EVSYS.USERTCA0CNTB = 0x00;
    //USER OFF; 
    EVSYS.USERTCB0CAPT = 0x00;
    //USER OFF; 
    EVSYS.USERTCB0COUNT = 0x00;
    //USER OFF; 
    EVSYS.USERTCB1CAPT = 0x00;
    //USER OFF; 
    EVSYS.USERTCB1COUNT = 0x00;
    //USER OFF; 
    EVSYS.USERUSART0IRDA = 0x00;
    //USER OFF; 
    EVSYS.USERUSART1IRDA = 0x00;
    
    return 0;
}