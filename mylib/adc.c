#include "adc.h"

void ADC_Config_Channel(uint8_t channel) {
    // Khai báo h?t các bi?n ? d?u hàm d? tránh l?i C89
    volatile uint32_t i;
    uint8_t pin;

    RCC->CFGR &= ~(0x3 << 14); 
    RCC->CFGR |=  (0x2 << 14); 

    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; 
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; 

    if (channel <= 7) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; 
        GPIOA->CRL &= ~(0xF << (channel * 4)); 
    } 
    else if (channel <= 9) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; 
        pin = channel - 8; // Gán giá tr? vào bi?n dã khai báo ? d?u
        GPIOB->CRL &= ~(0xF << (pin * 4));
    }

    ADC1->SMPR2 &= ~(0x7 << (channel * 3)); 
    ADC1->SMPR2 |=  (0x7 << (channel * 3)); 

    ADC1->SQR3 &= ~(0x1F); 
    ADC1->SQR3 |= channel; 

    ADC1->CR2 |= (1 << 0);  
    ADC1->CR2 |= (1 << 1);  

    // S?a l?i vòng l?p for
    for(i = 0; i < 1000; i++);

    ADC1->CR2 |= (1 << 2);  
    while(ADC1->CR2 & (1 << 2)); 

    ADC1->CR2 |= (1 << 0);  
}

uint16_t ADC_Read(void) {
    return (uint16_t)ADC1->DR; 
}
// Nh? ?n Enter ? dây d? t?o 1 dòng tr?ng cu?i file