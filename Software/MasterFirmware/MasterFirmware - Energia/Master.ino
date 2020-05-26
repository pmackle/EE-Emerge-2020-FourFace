//Includes for game
#include <stdbool.h>
#include <stdio.h> 
#include <stdlib.h> 

//////////////MASTER.C GAME CODE//////////////////////////////
typedef struct 
{
    int score;
    bool eliminated;
} Player;

typedef struct 
{
    int totalRoundCount;
    int crntRound;
    int initialSquareCount; // How many squares emitted in the pattern
    //Difficulty
} Game;


//Enums
typedef enum  ledStripPins
{
    SQUARES_A,
    SQUARES_B,
    SQUARES_C,
    SQUARES_D,
    SQUARES_E,
    SQUARES_F,
    SQUARES_G,
    SQUARES_H,
    SQUARES_I,
    NUM_SQUARES
} squareNames;

typedef enum  colors
{
    COLOR_BLUE,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_OFF
} LEDColors;

//Functions
//Communications
void establishContact();
void delayUntil(char c);
//Game
void startGame(int totalRoundCount, int initialSquareCount); //Round time in ms
void initializePlayer(Player *player);
int* generatePattern(const int sequenceLength); //Returns new pattern. Should only be called from newRound();
int* newRound(const int sequenceLength); 
void displayPattern(int* roundPattern, int sequenceLength);
void receiveComparePattern(int* roundPattern, int sequenceLength);


//Globals
static Game game;
static Player* player1 = NULL;
static Player* player2 = NULL;
static Player* player3 = NULL;
static Player* player4 = NULL;


// initialize game and players
uint16_t buttonStates = 0x0;
int sizePatternArray;
int* pattern;
uint16_t pressedPlate = 0;


// the setup routine runs once when you press reset:
void setup() {                
    Serial1.begin(9600); 
    randomSeed(analogRead(0));
    establishContact(); // send a byte to establish contact until receiver responds 
    
    startGame(5, 5);
    player1 = (Player*)malloc(sizeof(Player));
    player2 = (Player*)malloc(sizeof(Player));
    player3 = (Player*)malloc(sizeof(Player));
    player4 = (Player*)malloc(sizeof(Player));
    initializePlayer(player1);
    initializePlayer(player2);
    initializePlayer(player3);
    initializePlayer(player4);
}

//////////////////////////////
//MAIN FUNCTION
//////////////////////////////
int i = 0;
void loop() {
    while (Serial1.read() != 'R') { //Wait for each player to ready up
        delay(300);
    }
    int sequenceLength = game.initialSquareCount + game.crntRound;
    int* pattern = newRound(sequenceLength);
    receiveComparePattern(pattern, sequenceLength);
}


//Ensures connection to slave is good
void establishContact() {
  while (Serial1.read() != 'R') {
    Serial1.print('R');
    delay(300);
  }
}


//Loop until a char is received from UART stream
void delayUntil(char c) {
    while (Serial1.read() != c) {
        delay(300);
    }
}


//Initialize game values
void startGame(int totalRoundCount, int initialSquareCount)
{
    game.totalRoundCount = totalRoundCount;
    game.initialSquareCount = initialSquareCount;
    game.crntRound = 0;
}


//Initialize player values for beginning of game
void initializePlayer(Player *player)
{
    player->score = 0;
    player->eliminated = false;
}


//Generate a new random pattern for the new round
int* generatePattern(const int sequenceLength)
{
    int* newPattern;
    //Allocate a block of memory the length of sequenceLength with each block large enough for an int
    newPattern = (int*)calloc(sequenceLength, sizeof(int));
    int i;
    for (i = 0; i < sequenceLength; i++) {
//        *(newPattern + i) = (rand() % (SQUARES_I - SQUARES_A + 1)) + SQUARES_A;
            *(newPattern + i) = random(SQUARES_A, SQUARES_I + 1);
    }
    return newPattern;
}


int* newRound(const int sequenceLength)
{
    int* pattern = generatePattern(sequenceLength);
    game.crntRound++;
    displayPattern(pattern, sequenceLength);
    return pattern;
}
 
 
void displayPattern(int* roundPattern, int sequenceLength)
{
    uint8_t square = 0;
    int i = 0;
    for (; i < sequenceLength; i++)
    {
        square = roundPattern[i];
        Serial1.print(square);
        Serial1.print(COLOR_BLUE);
    }
    Serial1.print('E');
    delayUntil('D');
}

void receiveComparePattern(int* roundPattern, int sequenceLength) {
    int pressedSquare;
    int i = 0;
    for (; i < sequenceLength; i++) {
        delayUntil('P');
        pressedSquare = Serial1.read();
        if (pressedSquare == roundPattern[i]) {
            Serial1.print(pressedSquare);
            Serial1.print(COLOR_GREEN);
            //Send player not eliminated
            player1->score++;
        }
        else {
            Serial1.print(pressedSquare);
            Serial1.print(COLOR_RED);
            //Send player eliminated
            player1->eliminated = true;
        }
    }
    Serial1.print('E');
    delayUntil('D');
}
