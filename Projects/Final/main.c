#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/adc/adc.h"
#include "mcc_generated_files/uart/uart2.h"
#include "mcc_generated_files/pwm/pwm6.h"
#include "ssd1306.h"
#include <string.h>
#include <math.h>

void main(void)
{
    SYSTEM_Initialize(); 
    SSD1306_Init();
    SSD1306_Clear();
    
    uint16_t adcValue;    // Raw ADC value (0–4095 for 12-bit ADC)
    float voltage;        // Calculated input voltage from ADC
    float rLDR;           // Calculated resistance of the LDR
    float lux;            // Final computed lux value
    
    char buffer[32];
    const float Vref = 5.0;
    const float Rfixed = 8200.0;   // Fixed resistor value in voltage divider

    PWM6_LoadDutyValue(0);  // Start with LED off (duty = 0)

    while (1)
    {
        // Step 1: Read ADC and convert to voltage
        adcValue = ADC_ChannelSelectAndConvert(0);  // RA0
        voltage = ((float)adcValue * Vref) / 4096.0;  // 12-bit ADC conversion

        // Step 2: Compute LDR resistance and lux value
        if ((Vref - voltage) > 0.01) { // Avoid divide by near-zero
            rLDR = (voltage * Rfixed) / (Vref - voltage);
            lux = 5000.0 / powf(rLDR, 0.7);  // LDR approximation
        }
        else lux = 0;  // No light detected
        
        
        // Step 3: Map lux linearly inverted to PWM duty cycle
        // if lux = 0  -> duty = 1000 (bright)
        // if lux = 50 -> duty = 0 (dim)
        if (lux > 40.0) lux = 40.0;
        uint16_t duty = (uint16_t)((40.0 - lux) / 40.0 * 1000.0);
        
        // Apply PWM duty
        PWM6_LoadDutyValue(duty);

        // Step 4: Print values over UART
        printf("Light: %.1f lux\r\n", lux);
        printf("PWM: %u\r\n", duty);
        
        __delay_ms(1000);
    }
}
