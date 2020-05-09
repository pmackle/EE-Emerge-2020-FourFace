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

//Functions
void startGame(int totalRoundCount, int initialSquareCount); //Round time in ms
void initializePlayer(Player *player);
int* generatePattern(const int sequenceLength); //Returns new pattern. Should only be called from newRound();
int* newRound(const int sequenceLength); 
void displayPattern(int* roundPattern, int sequenceLength);
void sendSquareColor(uint16_t squareAndColor);
void sendSquareColor2(uint8_t square, uint8_t color);


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
    establishContact(); // send a byte to establish contact until receiver responds 
    
    startGame(5, 2);
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
    sizePatternArray = game.initialSquareCount + game.crntRound;
    pattern = newRound(sizePatternArray);
}


//Ensures connection to slave is good
void establishContact() {
  while (Serial1.read() != 'A') {
    Serial1.print('A'); // send a capital A
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
    for (i = 0; i < sequenceLength; i++)
        *(newPattern + i) = (rand() % (SQUARES_I - SQUARES_A + 1)) + SQUARES_A;
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
    uint8_t color = 3; //blue
    int i = 0;
    for (; i < sequenceLength; i++)
    {
        square = roundPattern[i];
        Serial1.print(square);
        Serial1.print(color);
        while (Serial1.read() != 'A') {}
    }
    // for (i = 0; i < 2; i++) //2 second timer
    //     __delay_cycles(10 * DELAY_100ms);
    // fillStrip(2);
    // for (i = 0; i < 1; i++) //1 second timer
    //     __delay_cycles(10 * DELAY_100ms);
    // clearStrip();
}