// #define COLOR_LED
#include <stdbool.h>
#include <stdio.h>   
#include <stdlib.h>  

#include "ece198.h"
#include "LiquidCrystal.h"

#define DIT_MIN 65
#define DIT_MAX 75
#define DAH_MIN 195
#define DAH_MAX 225
#define SPACE_LETTER_MIN 65
#define SPACE_LETTER_MAX 75
#define SPACE_WORD_MIN 455
#define SPACE_WORD_MAX 525

void DisplayButton(uint16_t buttonPin, uint16_t buzzerPin);
void Buzzer(uint16_t pin);
void SysTick_Handler(void);

int main(void)
{
    HAL_Init(); 
    __HAL_RCC_GPIOC_CLK_ENABLE();
    SerialSetup(9600);

    InitializePin(GPIOC, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_PULLUP, 0);
    InitializePin(GPIOC, GPIO_PIN_1, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);
    LiquidCrystal(GPIOB, GPIO_PIN_9, GPIO_PIN_8, GPIO_PIN_6, GPIO_PIN_10, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_3);

    

    DisplayButton(GPIO_PIN_0, GPIO_PIN_1);
    // Buzzer(GPIO_PIN_1);


#ifdef COLOR_LED
    // Cycle through all 8 possible colors (including off and white) as the on-board button is pressed.
    // This example assumes that the color LED is connected to pins D11, D12 and D13.

    // Remember that each of those three pins must go through a 220 ohm current-limiting resistor!
    // Also remember that the longest pin on the LED should be hooked up to GND.

    InitializePin(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // initialize color LED output pins
    while (true) {
        for (int color = 0; color < 8; ++color) {
            // bottom three bits indicate which of the three LEDs should be on (eight possible combinations)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, color & 0x01);  // blue  (hex 1 == 0001 binary)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, color & 0x02);  // green (hex 2 == 0010 binary)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, color & 0x04);  // red   (hex 4 == 0100 binary)

            while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));   // wait for button press 
            while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));  // wait for button release
        }
    }
#endif
    return 0;
}


void DisplayButton(uint16_t buttonPin, uint16_t buzzerPin) {
    uint32_t time;
    SerialPutc('X');
    while (true) {
        while (HAL_GPIO_ReadPin(GPIOC, buttonPin)) {
            HAL_GPIO_WritePin(GPIOC, buzzerPin, 0);
        }
        time = HAL_GetTick();

        while (!HAL_GPIO_ReadPin(GPIOC, buttonPin)) {
            HAL_GPIO_WritePin(GPIOC, buzzerPin, GPIO_PIN_SET);
        }

        char buff1[100];
        char buff2[100];
        sprintf(buff2, "%lu ms\r\n", (HAL_GetTick() - time));
        setCursor(0, 0);
        // print(buff);
        if (((HAL_GetTick() - time) >= DIT_MIN) && ((HAL_GetTick() - time) <= DIT_MAX)) {
            sprintf(buff1, "dit");
        } else if (((HAL_GetTick() - time) >= DAH_MIN) && ((HAL_GetTick() - time) <= DAH_MAX)) {
            sprintf(buff1, "dah");
        } else {
            sprintf(buff1, "NOO");
        }

        SerialPuts(buff2);
        print(buff1);
    }
    
}

void Buzzer(uint16_t pin) 
{
    while (true) {
        HAL_GPIO_WritePin(GPIOC, pin, GPIO_PIN_SET);
    }
}


// This function is called by the HAL once every millisecond
void SysTick_Handler(void)
{
    HAL_IncTick(); // tell HAL that a new tick has happened
    // we can do other things in here too if we need to, but be careful
}
