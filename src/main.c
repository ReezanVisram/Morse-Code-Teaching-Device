// #define COLOR_LED
#include <stdbool.h>
#include <stdio.h>   
#include <stdlib.h>  
#include <string.h>

#include "ece198.h"
#include "LiquidCrystal.h"
#include "constants.h"

void DisplayButton(uint16_t buttonPin, uint16_t buzzerPin);
void Buzzer(uint16_t pin);
char determineLetter(char* letter);
char* convertMorseToEnglish(char* word);
void clearLetter(char* letter);
void SysTick_Handler(void);


int main(void)
{
    HAL_Init(); 
    __HAL_RCC_GPIOC_CLK_ENABLE();
    SerialSetup(9600);

    InitializePin(GPIOC, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_PULLUP, 0);
    InitializePin(GPIOC, GPIO_PIN_1, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);
    LiquidCrystal(GPIOB, GPIO_PIN_9, GPIO_PIN_8, GPIO_PIN_6, GPIO_PIN_10, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_3);

    SerialPutc('\n');

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
    uint32_t intendedTime;

    uint32_t timeOfLastInput;


    char currLetter[4];
    int currLetterIndex = 0;

    char currPhrase[100] = "";
    int currPhraseIndex = 0;

    while (true) {
        // Button is not pressed
        while (HAL_GPIO_ReadPin(GPIOC, buttonPin)) {
            HAL_GPIO_WritePin(GPIOC, buzzerPin, 0);
        }
        time = HAL_GetTick();

        if (((time-timeOfLastInput >= SPACE_LETTER_MIN) && (currLetterIndex > 0)) || (currLetterIndex >= 4)) {
            currPhrase[currPhraseIndex] = determineLetter(currLetter);
            currPhraseIndex++;
            clearLetter(currLetter);
            currLetterIndex = 0;
        }

        if ((time-timeOfLastInput >= SPACE_WORD_MIN) && (time-timeOfLastInput <= SPACE_WORD_MAX)) {
            currPhrase[currPhraseIndex] = ' ';
            currPhraseIndex++;
        }

        // Button is pressed
        while (!HAL_GPIO_ReadPin(GPIOC, buttonPin)) {
            HAL_GPIO_WritePin(GPIOC, buzzerPin, GPIO_PIN_SET);
        }
        intendedTime = HAL_GetTick() - time;
        char buff[100];
        setCursor(0, 0);

        if (((intendedTime) >= DIT_MIN) && ((intendedTime) <= DIT_MAX)) {
            currLetter[currLetterIndex] = '.';
            sprintf(buff, "dit\n");
            currLetterIndex++;
        } else if (((intendedTime) >= DAH_MIN) && ((intendedTime) <= DAH_MAX)) {
            currLetter[currLetterIndex] = '-';
            sprintf(buff, "dah\n");
            currLetterIndex++;
        } else {
            sprintf(buff, "None\n");
        }
        timeOfLastInput = HAL_GetTick();

        print(currPhrase);
        SerialPuts(buff);
    }
    
}

char determineLetter(char* letter) {
    for (int i = 0; i < 26; i++) {
        if (strncmp(morseLetters[i], letter, sizeof(morseLetters[i])) == 0) {
            return (i+65);
        }
    }
    return 26;
}

char* convertMorseToEnglish(char* word) {
    char buff[100];
    sprintf(buff, "Entered convert to word function\n");
    SerialPuts(buff);
    char* englishWord[100];

    for (int i = 0; word[i] != '\0'; i++) {
        englishWord[i] = determineLetter(word[i]);
    }

    return englishWord;
}

void clearLetter(char* letter) {
    for (int i = 0; i < 4; i++) {
        letter[i] = '\0';
    }
}



// This function is called by the HAL once every millisecond
void SysTick_Handler(void)
{
    HAL_IncTick(); // tell HAL that a new tick has happened
    // we can do other things in here too if we need to, but be careful
}
