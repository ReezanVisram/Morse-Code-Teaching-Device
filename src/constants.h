#define DIT_MIN 60
#define DIT_MAX 95
#define DAH_MIN 180
#define DAH_MAX 275
#define SPACE_LETTER_MIN 285
#define SPACE_LETTER_MAX 300
#define SPACE_WORD_MIN 420
#define SPACE_WORD_MAX 665

const char morseLetters[26][4] = {{'.', '-'}, 
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

const char englishLetters[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
