#define DIT_MIN 60
#define DIT_MAX 95
#define DAH_MIN 180
#define DAH_MAX 275
#define SPACE_LETTER_MIN 285
#define SPACE_LETTER_MAX 300
#define SPACE_WORD_MIN 420
#define SPACE_WORD_MAX 665
#define END_TIME 2000

const char morseLetters[26][5] = {{'.', '-'}, 
                            {'-', '.', '.', '.'}, 
                            {'-', '.', '-', '.'}, 
                            {'-', '.', '.'}, 
                            {'.'}, 
                            {'.', '.', '-', '.'}, 
                            {'-', '-', '.'}, 
                            {'.', '.', '.', '.'}, 
                            {'.', '.'}, 
                            {'.', '-', '-', '-'}, 
                            {'-', '.', '-'}, 
                            {'.', '-', '.', '.'}, 
                            {'-', '-'}, 
                            {'-', '.'}, 
                            {'-', '-', '-'}, 
                            {'.', '-', '-', '.'}, 
                            {'-', '-', '.', '-'}, 
                            {'.', '-', '.'}, 
                            {'.', '.', '.'}, 
                            {'-'}, 
                            {'.', '.', '-'}, 
                            {'.', '.', '.', '-'}, 
                            {'.', '-', '-'}, 
                            {'-', '.', '.', '-'}, 
                            {'-', '.', '-', '-'}, 
                            {'-', '-', '.', '.'}
                            };

const char englishLetters[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
const char spaceArr[5] = {32, 32, 32, 32};


const char questions[10][100] = {
    {"WHY DID THE CHICKEN CROSS THE ROAD"},
    {"WHAT IS FOUR PLUS FOUR"},
    {"WHAT IS THE NAME OF THIS DEVICE"},
    {"HI HOW IS IT GOING"},
    {"WHY DO MUSHROOMS GET INVITIED TO ALL THE PARTIES"},
};

const char answers[10][100] = {
    {"TO GET TO THE OTHER SIDE"},
    {"EIGHT"},
    {"MORSE CODE TEACHING DEVICE"},
    {"GOOD HOW ARE YOU"},
    {"THEY ARE FUNGIS"}
};