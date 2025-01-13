#include <stdio.h>
#include <omp.h>

static inline int min(x,y){
    return (x < y) ? x : y;
}

int randBetween(int x, int y){
    return x + (rand() % (y - x + 1))
}

struct deck {
    int cardArray[52] ;
    int start;
    int end;
    int deckLength;
}

int popCard(struct deck *deck){
    //pops first card

    int card;

    if (deck->deckLength = 0){
        return -1;
    }

    card = deck->cardArray[deck->start];

    //fixes other data
    deck->deckLength = deck->deckLength -1;

    if (deck->start == 0){
        deck->start = 51;
    }
    else{
        deck->start = deck->start -1;
    }

    return card;
}

struct deck* randomizeDeck(struct deck *deck){
    //Randomizes deck and starts it at zero

    int newCardArray[52];
    int deckSize = deck->deckLength; //initial size
    int i;
    int cardLoc, card;

 
    memset(newCardArray, 0, 52);

    for (i = 0; i < deckSize; i++){//randomizes the deck
        cardLoc = randBetween(0,deck->deckLength);
        cardLoc = (cardLoc + deck->start) % 52; //wraps around

        if (cardLoc > deck->end && cardLoc < deck->start){
            //throw error
        }

        newCardArray[i] = deck->cardArray[cardLoc]; //adds chosen card starting from 0

        if (cardLoc != deck->end){//not nessesary if last card is chosen
            deck->cardArray[cardLoc] = deck->cardArray[deck->end]//moves the last card to the location of the chosen one
        }

        if (deck->end == 0){//if end is zero sets to 51
            deck->end = 51;
        }
        else{
            deck->end = deck->end - 1;
        }

        deck->deckLength = deck->deckLength - 1;//lowers cardlength
    }

    //fixes deck
    memcpy(deck->cardArray, newCardArray, 52);//replaces cardArray with newCardArray
    deck->start = 0;
    deck->end = deckSize - 1;
    deck->deckLength = deckSize;//sets deckLength to actual size

    return deck;
}

struct deck* freshDeck(){
    struct deck *deck = malloc(sizeof(struct list));
    deck->start = 0;
    deck->end = 51;
    deck->deckLength = 52;

    int cardArray[52];
    int i,i2;

    for (i=0; i<4; i++){//creates a deck that is 0-12 four times
        for (i2=0; i<13; i++){
            cardArray[i*13+i2]=i2
        }
    }
    deck->cardArray = cardArray;
}


//how the stack is created
int STACKTYPE = 1;
int LOOSERFIRST = 0; // whether the loosers cards go first

int warRound(struct deck *deck1, struct deck *deck2){
    int card1;
    int card2;

    int stackLength = min(deck1->deckLength, deck2->deckLength);//get smallest deck as it is max size

    int stack[2][stackLength];
    int resultStack[52]; //the stack the is returned

    int warCount = 0;
    int curWarLength = 0;
    int warBurn = 3;

    int winner = -2;//0 for 1, 1 for 2
    int stackUsed;

    int i, i2, i3;


    //actual thing
    while (winner = -2){//winner = -2 means no winner decided
        card1 = popCard(deck1);
        card2 = popCard(deck2);

        stackUsed ++;

        stack[0][(warBurn+1)*warCount] = card1;
        stack[1][(warBurn+1)*warCount] = card2;

        if (card1 != card2){//not war
            winner = (card1 < card2) ? 1 : 0; 
            continue;
        }

        //war time

        if(stackUsed == stackLength){//if used all cards and still tied
            winner = -1;
            continue;
        }

        //burn baby burn
        curWarLength = min(warBurn + 1, stackLength - ((warCount+1)*(warBurn+1)) - 1); //Can make last war burn less than 3
        stackUsed += curWarLength-1;
        for (i=0; i < curWarLength-1; i++){
            stack[0][(warBurn+1)*warCount] = popCard(card1);
            stack[1][(warBurn+1)*warCount] = popCard(card2);
        }
    }

    if (winner = -1){//tie
        return winner;
    }

    switch (STACKTYPE){
    case 0:
        //random result stack
        for (i=0; i<2; i++){
            for (i2=0; i<stackUsed; i++){
                resultStack[i*stackUsed + i2] = stack[(i+1)%2][i2]
            }
        }

        struct deck temp;
        temp.start = 0;
        temp.end = stackUsed * 2 - 1;
        temp.deckLength = stackUsed * 2;
        temp.cardArray = resultStack; //changes to temp.cardArray change resultStack
        randomizeDeck(*temp);
        break;

    case 1:
        //result stack is one first stack then the second

        for (i=0; i<2; i++){
            for (i2=0; i<stackUsed; i++){
                resultStack[i*stackUsed + i2] = stack[(winner+i+LOOSERFIRST)%2][i2]
            }
        }
        break;
    
    case 2:
        //result stack is first stack then second seperated by war
        // first from first stack, first from second stack, then all cards from war from first stack, then all cards from war from second stack, repeat

        //first two cards
        resultStack[0] = stack[(winner+LOOSERFIRST)%2][0];
        resultStack[1] = stack[(winner+LOOSERFIRST+1)%2][0];//+1 to change stack

        for(i=0; i < warCount; i++){
            curWarLength = min(warBurn + 1, stackUsed - ((i+1)*(warBurn+1)) - 1);//finds if a war is less than warBurn + 1 long
            for(i2; i< 2; i2++){
                for(i3; i3 < curWarLength; i3++){
                    //warCount*(warBurn+1) gives haow many cards are used in previous wars, the + 1 is for the first card being taken
                    //the first two terms are *2 as it contains the cards from 2 decks, warLenght *i2 is to see if this if first or second time
                    resultStack[(warCount*(warBurn+1) + 1)*2 + (curWarLength*i2) + i3] = stack[(winner+LOOSERFIRST+i2)%2][warCount*(warBurn+1) + 1 + i3]
                }
            }
        }
        break;
    }
}