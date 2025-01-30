#include <stdio.h>
#include <omp.h>

static inline int min(x,y){
    return (x < y) ? x : y;
}

int randBetween(int x, int y){
    return x + (rand() % (y - x + 1))
}

//switch to linked list for que
struct deck {
    struct card *head;
    struct card *tail;
}

struct card{
    int val;
    struct card *next;
}

int deckEmpty(struct deck *deck){
    return deck->head == NULL
}

int deckLength(struct deck *deck){
    int length = 0;
    struct card *card = deck->head;
    if (deckEmpty(deck)){
        return 0;
    }

    while (card->next != NULL){
        length ++;
        card = card->next;
    }

    return length;
}

struct card* popCard(struct deck *deck){
    //pops first card
    int card;

    if (deckEmpty(deck)){
        return NULL;
    }

    card = deck->head;

    //fixes other data
    if(deck->head = deck->tail){
        deck->tail = NULL;
    }
    deck->head = card->next;

    card->next = NULL;

    return card;
}

struct deck* pushCard(struct deck *deck, struct card *card){
    //adds card to front of deck
    card->next = deck->head;
    deck->head = card;

    if(deckEmpty(deck)){
        deck->tail == card;
    }
}

struct deck* pushEndCard(struct deck *deck, struct card *card){
    //adds card to end of deck
    card->next = NULL;

    if(deck->head == NULL){
        deck->head == card;
    }
    else{
        deck->tail->next = card;
    }

    deck->tail = card;
}

struct deck* mergeDeck(struct deck *deck1, struct deck *deck2){
    //merges deck2 into deck1


    if (deckEmpty(deck1)){
        deck1->head = deck2->head;
    }
    else{
        deck1->tail->next = deck->head;
    }

    deck1->tail = deck2->tail;

    deck2->head = NULL;
    deck2->tail = NULL;
}

struct deck* switchDeck(struct deck *deck1, struct deck *deck2, struct deck *curDec){
    //Switches deck to not current deck, curDec must be either deck1 or deck2

    if (curDec == deck1){
        curDec = deck2;
    }
    else if (curDec == deck2;){
        curDec = deck1;
    }
    else{
        abort();//crash
    }
}

struct deck* randomizeDeck(struct deck *deck){
    //Randomizes deck

    int i, i2, deckLength, cardLoc;

    struct card *card1, *card2;

    deckLength = deckLength(deck);

    for (i = 0; i < deckLength; i++){//randomizes the deck

        //gets random card between i and deckLength
        cardLoc = randBetween(0,deckLength-i); //i cards are already randomized

        card1 = deck->head;

        //Gets the cardLoc card
        for (i2 = 0; i2 < cardLoc; i++){
            if (card1==NULL){
                abort();//causes crash I think
            }
            card2 = card1;
            card1 = card->next;
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

    struct deck *deck = malloc(sizeof(struct list));
    struct card *card;
    int i,i2;

    deck->head = NULL;
    deck->tail = NULL;

    for (i=0; i<4; i++){//creates a deck that is 0-12 four times
        for (i2=0; i<13; i++){
            card = malloc(sizeof(struct card));
            card->val = i2;

            card->next = deck->head;
        }
    }
    deck->tail=card;

    return deck;
}


//how the stack is created
int STACKTYPE = 1;
int LOOSERFIRST = 0; // whether the loosers cards go first

int warRound(struct deck *deck1, struct deck *deck2){
    struct card *card1, *card2, *tempCard;
    struct deck *temp1, *temp2, *temp;

    temp1 = malloc(sizeof(struct deck));
    temp2 = malloc(sizeof(struct deck));

    temp1->head = NULL;
    temp2->head = NULL;

    temp1->tail = NULL;
    temp2->tail = NULL;
    

    int deckLength = min(deckLength(deck1), deckLength(deck2));//get smallest deck as it is max size

    int warBurn = 3, earlyWarExit;

    int winner = -2;//0 for 1, 1 for 2

    int cardUsed = 0;

    int i, i2, i3;


    //actual thing
    while (winner = -2){//winner = -2 means no winner decided
        card1 = popCard(deck1);
        card2 = popCard(deck2);

        pushEndCard(temp1, card1);
        pushEndCard(temp2, card2);

        //adds cards to deliminate aftter first card
        tempCard = malloc(sizeof(struct card));
        tempCard->val=-1;//-1 card after first card
        pushEndCard(temp1, tempCard);
        tempCard = malloc(sizeof(struct card));
        tempCard->val=-1;//-1 card after first card
        pushEndCard(temp2, tempCard);

        if (card1->val != card2->val){//not war
            winner = (card1->val < card2->val) ? 1 : 0; 
            break;
        }

        //war time

        if(cardUsed >= deckLength){//if used all cards and still tied
            winner = -1;
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

            pushEndCard(temp1, card1);
            pushEndCard(temp2, card2);
        }

        if (!earlyWarExit){
            card1 = popCard(deck1);
            card2 = popCard(deck2);
        }

        //Adds cards to delimate last card of war
        tempCard = malloc(sizeof(struct card));
        tempCard->val=-2;//-2 card before tail of war
        pushEndCard(temp1, tempCard);
        tempCard = malloc(sizeof(struct card));
        tempCard->val=-2;//-2 card before tail of war
        pushEndCard(temp2, tempCard);


        pushEndCard(temp1, card1);
        pushEndCard(temp2, card2);

        //Adds cards after last card of war
        tempCard = malloc(sizeof(struct card));
        tempCard->val=-3;//-3 card after tail of war
        pushEndCard(temp1, tempCard);
        tempCard = malloc(sizeof(struct card));
        tempCard->val=-3;//-3 card after tail of war
        pushEndCard(temp2, tempCard);

        if (card1->val != card2->val){//not war
            winner = (card1->val < card2->val) ? 1 : 0; 
            break;
        }
    }

    if (winner = -1){//tie
        return -1;
    }

    //makes winner deck 1, looser deck2
    //also makes it so that you should always take from temp1 first
    //makes it so that no checks for winner and LOOSERFIRST happen later on
    if (winner == 1){// if winner is deck2
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


    switch (STACKTYPE){
    case 0:
        //random result stack
        temp = malloc(sizeof(struct deck));
        temp->head=NULL;
        temp->tail=NULL;

        card1 = popCard(temp1);
        card2 = popCard(temp2);

        while(card1 != NULL){
            if (card1->val < 0){
                free(card1);
                free(card2);
            }
            else{
                pushCard(card1, temp);
                pushCard(card2, temp);
            }

            card1 = popCard(temp1);
            card2 = popCard(temp2);
        }

        randomizeDeck(temp);
        mergeDeck(deck1, temp);

        free(temp);
        break;

    case 1:
        //result stack is one first stack then the second

        card1 = popCard(temp1);
        while(card1 != NULL){
            if (card1->val < 0){
                free(card1);
            }
            else{
                pushCardEnd(card1, deck1);
            }

            card1 = popCard(temp1);
        }

        card2 = popCard(temp2);
        while(card2 != NULL){
            if (card2->val < 0){
                free(card2);
            }
            else{
                pushCardEnd(card2, deck1);
            }

            card2 = popCard(temp2);
        }
        break;
    
    case 2:
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
                pushCardEnd(card1, deck1);

            }
            card1 = popCard(temp);
        }
        break;
    }
}