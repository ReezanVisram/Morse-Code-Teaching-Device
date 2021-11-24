// #define COLOR_LED
#include <stdbool.h>
#include <stdio.h>   
#include <stdlib.h>  
#include <string.h>

#include "ece198.h"
#include "LiquidCrystal.h"
#include "constants.h"

// Init Functions
void startProgram(uint16_t buttonPin);

// Message Functions
void toStartMessage();
void correctMessage(uint16_t redPin, uint16_t greenPin, uint16_t bluePin);
void incorrectMessage(uint16_t redPin, uint16_t greenPin, uint16_t bluePin);

// Output Functions
void output(uint16_t buzzerPin, uint16_t redPin, uint16_t greenPin, uint16_t bluePin);
void lcdOutput();
void buzzerOutput(uint16_t buzzerPin);
void ledOutput(uint16_t redPin, uint16_t greenPin, uint16_t bluePin);

// Processing Functions
void DisplayButton(uint16_t buttonPin, uint16_t buzzerPin);
void Buzzer(uint16_t pin);

// Helper Functions
char determineEnglishLetter(char* letter);
char* determineMorseLetter(char letter);
char* convertMorseToEnglish(char* word);
void convertEnglishToMorse(char* phrase);
void clearLetter(char* letter);
void SysTick_Handler(void);

// Global variables
int questionIndex;
char morseQuestion[100][5] = {};
char expectedAnswer[100];
char userAnswer[100] = {};

int main(void)
{
    HAL_Init(); 
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    SerialSetup(9600);

    InitializePin(GPIOC, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_PULLUP, 0);
    InitializePin(GPIOC, GPIO_PIN_1, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);
    InitializePin(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);
    LiquidCrystal(GPIOB, GPIO_PIN_9, GPIO_PIN_8, GPIO_PIN_6, GPIO_PIN_10, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_3);

    startProgram(GPIO_PIN_0);
    output(GPIO_PIN_1, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7);

    DisplayButton(GPIO_PIN_0, GPIO_PIN_1);

    if (strcmp(expectedAnswer, userAnswer) == 0) {
        correctMessage(GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7);
    } else {
        incorrectMessage(GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7);
    }

    while (1);

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

void startProgram(uint16_t buttonPin) {
    while (HAL_GPIO_ReadPin(GPIOC, buttonPin)) {
        toStartMessage();
    }
    clear();
    srand(HAL_GetTick());
    HAL_Delay(1000);
    return;
}

void output(uint16_t buzzerPin, uint16_t redPin, uint16_t greenPin, uint16_t bluePin) {
    questionIndex = rand() % 6;
    convertEnglishToMorse(questions[questionIndex]);
    strcpy(expectedAnswer, answers[questionIndex]);
    SerialPuts(questions[questionIndex]);

    int outputMode = rand() % 3;

    if (outputMode == 0) {
        lcdOutput();
    } else if (outputMode == 1) {
        buzzerOutput(buzzerPin);
    } else {
        ledOutput(redPin, greenPin, bluePin);
    }

    HAL_GPIO_WritePin(GPIOA, redPin, 1);
    HAL_GPIO_WritePin(GPIOA, greenPin, 1);
    HAL_GPIO_WritePin(GPIOA, bluePin, 0);

}


void lcdOutput() {
    for (int i = 0; morseQuestion[i][0] != '\0'; i++) {
        print(morseQuestion[i]);
        HAL_Delay(1500);
        clear();
    }
}

void buzzerOutput(uint16_t buzzerPin) {
    for (int i = 0; morseQuestion[i][0] != '\0'; i++) {
        for (int j = 0; morseQuestion[i][j] != '\0'; j++) {
            if (morseQuestion[i][j] == '.') {
                HAL_GPIO_WritePin(GPIOC, buzzerPin, GPIO_PIN_SET);
                HAL_Delay(DIT_MIN);
                HAL_GPIO_WritePin(GPIOC, buzzerPin, 0);
            } else if (morseQuestion[i][j] == '-') {
                HAL_GPIO_WritePin(GPIOC, buzzerPin, GPIO_PIN_SET);
                HAL_Delay(DAH_MIN);
                HAL_GPIO_WritePin(GPIOC, buzzerPin, 0);
            }
            HAL_Delay(SPACE_LETTER_MIN);
        }
        HAL_Delay(SPACE_WORD_MIN);
    }
}

void ledOutput(uint16_t redPin, uint16_t greenPin, uint16_t bluePin) {
    for (int i = 0; morseQuestion[i][0] != '\0'; i++) {
        for (int j = 0; morseQuestion[i][j] != '\0'; j++) {
            if (morseQuestion[i][j] == '.') {
                HAL_GPIO_WritePin(GPIOA, redPin, 1);
                HAL_GPIO_WritePin(GPIOA, greenPin, 1);
                HAL_GPIO_WritePin(GPIOA, bluePin, 1);
                HAL_Delay(DAH_MIN);
                HAL_GPIO_WritePin(GPIOA, redPin, 0);
                HAL_GPIO_WritePin(GPIOA, greenPin, 0);
                HAL_GPIO_WritePin(GPIOA, bluePin, 0);
            } else if (morseQuestion[i][j] == '-') {
                HAL_GPIO_WritePin(GPIOA, redPin, 1);
                HAL_GPIO_WritePin(GPIOA, greenPin, 1);
                HAL_GPIO_WritePin(GPIOA, bluePin, 1);
                HAL_Delay(DAH_MAX);
                HAL_GPIO_WritePin(GPIOA, redPin, 0);
                HAL_GPIO_WritePin(GPIOA, greenPin, 0);
                HAL_GPIO_WritePin(GPIOA, bluePin, 0);
            }
            HAL_Delay(SPACE_LETTER_MIN);
        }
        HAL_Delay(SPACE_WORD_MIN);
    }
}

void DisplayButton(uint16_t buttonPin, uint16_t buzzerPin) {
    uint32_t time = 0;
    uint32_t intendedTime = 0;

    uint32_t timeOfLastInput = 0;


    char currLetter[4] = "";
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
            char english = determineEnglishLetter(currLetter);
            currPhrase[currPhraseIndex] = english;
            SerialPutc(english);
            currPhraseIndex++;
            clearLetter(currLetter);
            currLetterIndex = 0;
        }

        if ((time-timeOfLastInput >= SPACE_WORD_MIN) && (time-timeOfLastInput <= SPACE_WORD_MAX)) {
            currPhrase[currPhraseIndex] = ' ';
            currPhraseIndex++;
        }

        if ((time-timeOfLastInput >= END_TIME) && (currPhrase[0] != '\0')) {
            strcpy(userAnswer, currPhrase);
            break;
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
            currLetterIndex++;
        } else if (((intendedTime) >= DAH_MIN) && ((intendedTime) <= DAH_MAX)) {
            currLetter[currLetterIndex] = '-';
            currLetterIndex++;
        }
        timeOfLastInput = HAL_GetTick();
    }
    
}

char determineEnglishLetter(char* letter) {
    for (int i = 0; i < 26; i++) {
        if (strncmp(morseLetters[i], letter, sizeof(morseLetters[i])) == 0) {
            return (i+65);
        }
    }
    return 26;
}

char* determineMorseLetter(char letter) {
    for (int i = 0; i < 26; i++) {
        if (letter == englishLetters[i]) {
            return morseLetters[i];
        }
    }

    return spaceArr;
}

char* convertMorseToEnglish(char* word) {
    char* englishWord[100];

    for (int i = 0; word[i] != '\0'; i++) {
        englishWord[i] = determineEnglishLetter(word[i]);
    }

    return englishWord;
}

void convertEnglishToMorse(char* phrase) {
    for (int i = 0; phrase[i] != '\0'; i++) {
        strcpy(morseQuestion[i], determineMorseLetter(phrase[i]));
    }

    return morseQuestion;
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

void toStartMessage() {
    setCursor(0, 0);
    print("To start hold");
    setCursor(0, 1);
    print("the button");
}

void correctMessage(uint16_t redPin, uint16_t greenPin, uint16_t bluePin) {
    clear();
    setCursor(0, 0);
    print("Correct! Reset");
    setCursor(0, 1);
    print("to go again");

    HAL_GPIO_WritePin(GPIOA, redPin, 0);
    HAL_GPIO_WritePin(GPIOA, greenPin, 1);
    HAL_GPIO_WritePin(GPIOA, bluePin, 0);
}

void incorrectMessage(uint16_t redPin, uint16_t greenPin, uint16_t bluePin) {
    clear();
    setCursor(0, 0);
    print("Incorrect :(");
    setCursor(0, 1);
    print("Reset to retry");

    HAL_GPIO_WritePin(GPIOA, redPin, 1);
    HAL_GPIO_WritePin(GPIOA, greenPin, 0);
    HAL_GPIO_WritePin(GPIOA, bluePin, 0);
}