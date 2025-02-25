#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

static inline int min(int x, int y){
    return (x < y) ? x : y;
}

int randBetween(int x, int y){
    return x + (rand() % (y - x + 1));
}

//switch to linked list for que
struct deck {
    struct card *head;
    struct card *tail;
};

struct card{
    int val;
    struct card *next;
};

int deckEmpty(struct deck *deck){
    return deck->head == NULL;
}

int deckLength(struct deck *deck){
    int length = 0;
    struct card *card;

    if (deck == NULL || deckEmpty(deck)){
        return 0;
    }

    card = deck->head;

    while (card != NULL){
        //printf("%d,%d |", length, card->next == NULL);
        length ++;
        card = card->next;
    }


    //printf("\n");
    return length;
}

struct card* popCard(struct deck *deck){
    //pops first card
    struct card *card;

    if (deckEmpty(deck)){
        return NULL;
    }

    card = deck->head;

    //fixes other data
    if(deck->head == deck->tail){
        deck->tail = NULL;
    }
    deck->head = card->next;

    card->next = NULL;

    return card;
}

struct deck* pushCard(struct deck *deck, struct card *card){
    //adds card to front of deck
    
    if(deckEmpty(deck)){
        deck->tail = card;
        card->next = NULL;
    }
    else{
        card->next = deck->head;
    }


    deck->head = card;
    return deck;
}

struct deck* pushCardEnd(struct deck *deck, struct card *card){
    //if (deck == NULL) return NULL;
    //if (card == NULL) return NULL;

    //adds card to end of deck
    card->next = NULL;


    if(deckEmpty(deck)){
        deck->head = card;
    }
    else {
        if (deck->tail == NULL){
            printf("Error");
        }
        deck->tail->next = card;
    }

    deck->tail = card;

    return deck;
}

struct deck* mergeDeck(struct deck *deck1, struct deck *deck2){
    //merges deck2 into deck1


    if (deckEmpty(deck1)){
        deck1->head = deck2->head;
    }
    else{
        deck1->tail->next = deck2->head;
    }

    deck1->tail = deck2->tail;

    deck2->head = NULL;
    deck2->tail = NULL;

    return deck1;
}

struct deck* switchDeck(struct deck *deck1, struct deck *deck2, struct deck *curDec){
    //Switches deck to not current deck, curDec must be either deck1 or deck2

    if (curDec == deck1){
        curDec = deck2;
    }
    else if (curDec == deck2){
        curDec = deck1;
    }
    else{
        abort();//crash
    }

    return curDec;
}

struct deck* randomizeDeck(struct deck *deck){
    //Randomizes deck

    int i, i2, dLength, cardLoc;

    struct card *card1, *card2;

    dLength = deckLength(deck);

    for (i = 0; i < dLength; i++){//randomizes the deck

        //gets random card between i and deckLength
        cardLoc = randBetween(0,dLength-i); //i cards are already randomized

        card1 = deck->head;

        //Gets the cardLoc card
        for (i2 = 0; i2 < cardLoc; i++){
            if (card1==NULL){
                abort();//causes crash I think
            }
            card2 = card1;
            card1 = card1->next;
        }

        //moves card to end of que
        if (card1!= deck->tail){

            if (card1 = deck->head){//if head makes next card head
                deck->head = card1->next;
            }
            else{
                card2->next = card1->next;
            }

            card1->next = NULL;
        }
    }

    return deck;
}

struct deck* freshDeck(){
    //creates a fresh deck

    struct deck *deck = malloc(sizeof(struct deck));
    struct card *card;
    int i,i2;

    deck->head = NULL;
    deck->tail = NULL;

    for (i=0; i<4; i++){//creates a deck that is 0-12 four times
        for (i2=0; i2<13; i2++){
            card = malloc(sizeof(struct card));
            card->val = i2+2;

            pushCardEnd(deck, card);
            //printf("%d\n", i*13 + i2 + 1);
        }
    }

    return deck;
}

struct deck* splitDeck(struct deck *deck){
    struct deck *returnDeck = malloc(sizeof(struct deck));
    int dlength = deckLength(deck);
    int i;

    for (i =0; i<dlength/2; i++){
        pushCard(returnDeck, popCard(deck));
    }

    return returnDeck;
}

struct deck* clearDeck(struct deck *deck){
    struct card *card1, *card2;

    card1 = deck->head;
    while (card1 != NULL){
        card2 = card1;
        card1 = card1->next;

        free(card2);
    }

    return deck;
}

void printDeck(struct deck *deck){
    struct card *card;

    card = deck->head;

    while (card != NULL){
        printf("%d\n", card->val);
        card = card->next;
    }

}

//for use in other stuff if I get to it
struct warGameRound {
    int deck1Length;
    int deck2Length;
    int winner;
    int warCount;
    int cardUsed;

    struct warGameRound *next; //for use for ll
};

enum stackType{
    randomStack, fullStack, interWoven
};

static char *stackTypeStrings[] = {
    "randomStack", "fullStack", "interWoven"
};

//how the stack is created
enum stackType STACKTYPE = fullStack;
int LOOSERFIRST = 0; // whether the loosers cards go first

struct warGameRound *warRound(struct deck *deck1, struct deck *deck2){
    struct card *card1, *card2, *tempCard;
    struct deck *temp1, *temp2, *temp;
    struct warGameRound *curRound;

    temp1 = malloc(sizeof(struct deck));
    temp2 = malloc(sizeof(struct deck));
    curRound = malloc(sizeof(struct warGameRound));

    temp1->head = NULL;
    temp2->head = NULL;

    temp1->tail = NULL;
    temp2->tail = NULL;
    

    int dLength = min(deckLength(deck1), deckLength(deck2));//get smallest deck as it is max size
    //printf("%d\n", dLength);

    int warBurn = 3, earlyWarExit;

    curRound->warCount = 0;
    curRound->winner = -2;//1 for 1, 2 for 2

    curRound->cardUsed = 0;

    int i, i2, i3;

    //actual thing
    while (curRound->winner == -2){//winner = -2 means no winner decided
        card1 = popCard(deck1);
        card2 = popCard(deck2);

        pushCardEnd(temp1, card1);
        pushCardEnd(temp2, card2);
        curRound->cardUsed++;

        //adds cards to deliminate aftter first card
        tempCard = malloc(sizeof(struct card));
        tempCard->val=-1;//-1 card after first card
        pushCardEnd(temp1, tempCard);

        tempCard = malloc(sizeof(struct card));
        tempCard->val=-1;//-1 card after first card
        pushCardEnd(temp2, tempCard);

        if (card1->val != card2->val){//not war
            curRound->winner = (card1->val < card2->val) ? 2 : 1; 
            //printf("Winner: %d\n", winner);
            break;
        }

        //
        //War time
        //

        curRound->warCount++;

        if(curRound->cardUsed >= dLength){//if used all cards and still tied
            curRound->winner = -1;
            curRound->deck1Length = deckLength(deck1);
            curRound->deck2Length = deckLength(deck2);
            break;
        }


        //burn baby burn
        earlyWarExit = 0;
        for (i=0; i< warBurn; i++){
            card1 = popCard(deck1);
            card2 = popCard(deck2);

            //if deck is empty dont add to que, have war
            if (deckEmpty(deck1) || deckEmpty(deck2)){
                earlyWarExit = 1;
                break;
            }

            pushCardEnd(temp1, card1);
            pushCardEnd(temp2, card2);
            curRound->cardUsed++;
        }

        if (!earlyWarExit){
            card1 = popCard(deck1);
            card2 = popCard(deck2);
            curRound->cardUsed++;
        }

        //Adds cards to delimate last card of war
        tempCard = malloc(sizeof(struct card));
        tempCard->val=-2;//-2 card before tail of war
        pushCardEnd(temp1, tempCard);
        tempCard = malloc(sizeof(struct card));
        tempCard->val=-2;//-2 card before tail of war
        pushCardEnd(temp2, tempCard);


        pushCardEnd(temp1, card1);
        pushCardEnd(temp2, card2);
        curRound->cardUsed++;

        //Adds cards after last card of war
        tempCard = malloc(sizeof(struct card));
        tempCard->val=-3;//-3 card after tail of war
        pushCardEnd(temp1, tempCard);
        tempCard = malloc(sizeof(struct card));
        tempCard->val=-3;//-3 card after tail of war
        pushCardEnd(temp2, tempCard);

        if (card1->val != card2->val){
            curRound->winner = (card1->val < card2->val) ? 2 : 1; 
            break;
        }
    }

    if (curRound->winner == -1){//tie and has run through all cards
        curRound->deck1Length = deckLength(deck1);
        curRound->deck2Length = deckLength(deck2);
        return curRound;
    }

    //makes winner deck 1, looser deck2
    //also makes it so that you should always take from temp1 first
    //makes it so that no checks for winner and LOOSERFIRST happen later on
    if (curRound->winner == 2){// if winner is deck2
        temp = deck1;
        deck1 = deck2;
        deck2 = deck1;

        if(!LOOSERFIRST){
            temp = temp1;
            temp1 = temp2;
            temp2 = temp;
        }

        temp = NULL;
    }


    //printDeck(temp1);
    //printDeck(temp2);


    switch (STACKTYPE){
    case randomStack:
        //random result stack
        temp = malloc(sizeof(struct deck));
        temp->head=NULL;
        temp->tail=NULL;

        card1 = popCard(temp1);
        card2 = popCard(temp2);

        while(card1 != NULL){
            if (card1->val < 0){//gets rid of temp vals
                free(card1);
                free(card2);
            }
            else{
                pushCard(temp, card1);
                pushCard(temp, card2);
            }

            card1 = popCard(temp1);
            card2 = popCard(temp2);
        }

        randomizeDeck(temp);
        mergeDeck(deck1, temp);

        free(temp);
        break;

    case fullStack:
        //result stack is one first stack then the second


        card1 = popCard(temp1);
        while(card1 != NULL){
            //printf("test\n");
            if (card1->val < 0){
                free(card1);//gets rid of temp vals
            }
            else{
                pushCardEnd(deck1, card1);
            }

            card1 = popCard(temp1);
        }

        card2 = popCard(temp2);
        while(card2 != NULL){
            if (card2->val < 0){
                free(card2);//gets rid of temp vals
            }
            else{
                pushCardEnd(deck2, card2);
            }

            card2 = popCard(temp2);
        }
        break;
    
    case interWoven:
        //result stack is first stack then second seperated by war
        // first from first stack, first from second stack, then all cards from war from first stack, then all cards from war from second stack, repeat

        temp = temp1;
        
        card1 = popCard(temp);
        while(card1 != NULL){
            if (card1->val < 0){//if comes in contact with a seperator frees it then switches
                free(card1);
                switchDeck(temp1, temp2, temp);
            }
            else{
                pushCardEnd(deck1, card1);

            }
            card1 = popCard(temp);
        }
        break;
    }

    curRound->deck1Length = deckLength(deck1);
    curRound->deck2Length = deckLength(deck2);
    return curRound;
}


//return type for game to allow lots of data
struct warGame{
    int winner;
    int rounds;

    struct warGameRound *head;
    struct warGameRound *tail;
};

void clearWarGame(struct warGame *warGame){
    //printf("%d\n", warGame->head == NULL);
    struct warGameRound *curRound = warGame->head;
    struct warGameRound *prev = NULL;

    //while (curRound != NULL){
        //printf("%d|", curRound->next == NULL);
        prev = curRound;
        curRound = curRound->next;
        free(prev);
        prev = NULL;
    //}
}

struct warGame* addRound(struct warGame *warGame, struct warGameRound *round){
    //Adds a round to end of list
    round->next = NULL;

    if (warGame->head == NULL){
        warGame->head = round;
    }
    else{
        warGame->tail->next = round;
    }


    warGame->tail = round;
}

struct warGame* fullGame(struct deck *deck1, struct deck *deck2){
    struct warGame *curGame = malloc(sizeof(struct warGame)); //one game of war
    struct warGameRound *curRound;
    curGame->rounds = 0;
    curGame->winner = -1;

    curGame->head = NULL;
    curGame->tail = NULL;

    while (!(deckEmpty(deck1) || deckEmpty(deck2))){
        curRound = warRound(deck1, deck2);
        curGame->winner = curRound->winner;
        curGame->rounds ++;
        addRound(curGame, curRound);
        //printf("%d|", curRound->next == NULL);
        curRound = NULL;
    }

    return curGame;
}

void printGame (struct warGame* game){
    struct warGameRound *round = game->head;
    int i = 0;

    while (round != NULL){
        //printf("%d,", i);
        i++;
        round = round->next;
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    struct deck *deck1;
    struct deck *deck2;
    struct warGame *game1;

    deck1 = freshDeck();
    deck2 = splitDeck(deck1);

    STACKTYPE = fullStack;//how are the stacks merged
    LOOSERFIRST = 0;//Wheather the looser is put first

    printf("Deck1: %d, Deck2: %d \n", deckLength(deck1), deckLength(deck2));
    printf("STACKTYPE: %s, LOOSERFIRST: %d\n", stackTypeStrings[STACKTYPE], LOOSERFIRST);

    game1 = fullGame(deck1, deck2);

    printf("Winner: %d, Deck1: %d, Deck2: %d, NumRounds: %d\n", game1->winner, deckLength(deck1), deckLength(deck2), game1->rounds);

    //printGame(game1);

    clearDeck(deck1);
    clearDeck(deck2);
    clearWarGame(game1);

    free(deck1);
    free(deck2);
    free(game1);

}