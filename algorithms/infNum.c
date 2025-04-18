#include <stdio.h>
#include <stdlib.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>


enum infNumType{
    base, new
};

struct infNum {
    struct numNode *first;
    struct numNode *last;

    struct numNodeFull *first;
    struct numNodeFull *last;

    short sign;

    enum infNumType numType;
};

struct numNode {
    short int num;
    struct numNode *next;
    struct numNode *previous;
};

struct numNodeFull{
    uint64_t num;
    struct numNodeFull *next;
    struct numNodeFull *previous;
}

int list_empty(struct infNum *infNum){
    return infNum->first == NULL;
}

struct infNum *infNum_Create(enum infNumType type){
    //create infNum
    struct infNum *infNum = malloc(sizeof(struct infNum));
    infNum->first = NULL;
    infNum->last = NULL;
    infNum->sign = 1;
    infNum->numType = type;
    
    return infNum;
}


void infNum_clear(struct infNum *infNum){
    //clears infnum

    switch (infNum->numType)
    {
        case base:
        struct numNode *iterrator;

        infNum->sign=1;
        if (infNum->first == NULL){
            return;
        }

        iterrator = infNum->first;
        while (!list_empty(infNum)){
            if (iterrator != infNum->last){
                iterrator = iterrator->next;
                free(iterrator->previous);
            }
            else{
                free(iterrator);
                infNum->first = NULL;
                infNum->last = NULL;
            }
        }
        break;

    case new:
        struct numNodeFull *iterrator;

        infNum->sign=1;
        if (infNum->first == NULL){
            return;
        }

        iterrator = infNum->first;
        while (!list_empty(infNum)){
            if (iterrator != infNum->last){
                iterrator = iterrator->next;
                free(iterrator->previous);
            }
            else{
                free(iterrator);
                infNum->first = NULL;
                infNum->last = NULL;
            }
        }
        break;

    }



}

int list_length(struct infNum *infNum){
    
    int i = 1;

    switch (infNum->numType){
    case base:
        struct numNode *itterator;
        if (list_empty(infNum)){
            return 0;
        }

        itterator = infNum->first;
        while (itterator != infNum->last){
            itterator = itterator->next;
            i++;
        }

        return i;
        break;
    
    case new:
        struct numNodeFull *itterator;
        if (list_empty(infNum)){
            return 0;
        }

        itterator = infNum->first;
        while (itterator != infNum->last){
            itterator = itterator->next;
            i++;
        }

        return i;
        break;
    }


}

void infNum_add_node(struct infNum *infNum, short int num){
    //adds a node with num of num at end of list

    if (infNum->numType != base){
        printf("Added using wrong add_node type");
        return;
    }

    struct numNode *numNode = malloc(sizeof(struct numNode));
    numNode->next = NULL;
    if (list_empty(infNum)){
        infNum->first = numNode;
        infNum->last = numNode;
        numNode->previous = NULL;
    }
    else{
        numNode->previous = infNum->last;
        infNum->last->next = numNode;
        infNum->last = numNode;
    }

    //deals with num
    if (num < 0){
        infNum->sign = -1;
        numNode->num = num * -1;
    }
    else{
        infNum->sign = 1;
        numNode->num = num;
    }
}

void infNum_add_node_full(struct infNum *infNum, uint64_t num){
    //adds a node with num of num at end of list

    if (infNum->numType != new){
        printf("Added using wrong add_node type");
        return;
    }

    struct numNodeFull *numNode = malloc(sizeof(struct numNodeFull));
    numNode->next = NULL;
    if (list_empty(infNum)){
        infNum->first = numNode;
        infNum->last = numNode;
        numNode->previous = NULL;
    }
    else{
        numNode->previous = infNum->last;
        infNum->last->next = numNode;
        infNum->last = numNode;
    }

    infNum->sign = 1;
    numNode->num = num;

}

void infNum_clean(struct infNum *infNum){
    //removes all leading nodes that are zero

    switch (infNum->numType)
    {
    case base:
        struct numNode *iterrator = infNum->last;

        while (iterrator != NULL & iterrator->num == 0){
            iterrator = iterrator->previous;
            infNum->last = iterrator;
            free(iterrator->next);
            iterrator->next = NULL;
        }
        break;
    
    case new:
        struct numNodeFull *iterrator = infNum->last;

        while (iterrator != NULL & iterrator->num == 0){
            iterrator = iterrator->previous;
            infNum->last = iterrator;
            free(iterrator->next);
            iterrator->next = NULL;
        }
        break;
    }

}

struct infNum *infNum_add (struct infNum *num1, struct infNum *num2) {
    //adds two infNums
    //retruns num3 which is 1 plus the other

    //checks for empty returns oposite
    if (list_empty(num1)){
        return num2;
    }
    else if (list_empty(num2)){
        return num1;
    }

    //checks if both versions are the same

    if (num1->numType != num2->numType){
        return;
    }



    switch (num1->numType){
        case base:
        
        struct infNum *list3 = infNum_Create(base);
        short int num;
        short int carry = 0;
        struct numNode *it1 = num1->first, *it2 = num2->first;
            
        //size of the nodes
        int size = 10000;

        //adds
        while(it1 != NULL || it2 != NULL){
            if (it1 == NULL){
                num = (num2->sign * it2->num) + carry;

                it2 = it2->next;
            }
            else if (it2 == NULL){
                num = (num1->sign * it1->num) + carry;

                it1 = it1->next;
            }
            else {
            num = (num1->sign * it1->num) + (num2->sign * it2->num) + carry;

            it1 = it1->next;
            it2 = it2->next;
            }
            
            //find the carry and make num < size
            carry = num / size;
            num = num % size;

            //printf("%hu, ", num);
            infNum_add_node(list3, num);
        }

        //adds nodes if left over
        if (carry != 0){
            infNum_add_node(list3, carry);
        }

        infNum_clean(list3);
        return list3;

        break;

    case new:
        struct infNum *list3 = infNum_Create(new);
        uint64_t num, num1, num2;
        uint64_t carry = 0;
        short int carryTemp=0;
        short int carrySign=1, numSign=1;
        struct numNodeFull *it1 = num1->first, *it2 = num2->first;

        while (it1 != NULL || it2 != NULL){
            num = carry;
            carryTemp = 0;

            //if negative carry
            if (carrySign == -1){
                num = ~num + 1;
            }

            if (it1 != NULL){

                if (num1->sign == -1){
                    num += ~it1->num + 1;

                    //wierd
                    //as it is adding compliments it will overflow naturally
                    //one can check for overflow using normal methods, must just use compliment
                    if (carrySign == -1 && (~num+1) < it1->num){
                        carryTemp += -1;
                    }
                    //if adding positive to negative and getting negative number effectivly underflow
                    else if(carrySign != -1 && (~num+1) < it1->num){
                        carryTemp += -1;
                    }
                }
                else{
                    num += it1->num;

                    //when dealing with negatives check compliment
                    if (carrySign == -1 && (~num+1) < it1->num){
                        carryTemp += -1;
                    }

                    //using != -1 so it can be copy pasted to part below
                    if (carrySign != -1 && num < it1->num){
                        carryTemp += 1;
                    }

                }
                it1= it1->next;
            }



            if (num2->sign == -1){
                num += ~it2->num + 1;

                //wierd
                //as it is adding compliments it will overflow naturally
                //one can check for overflow using normal methods, must just use compliment
                if (carryTemp == -1 && (~num+1) < it2->num){
                    carryTemp += -1;
                }
                //if adding positive to negative and getting negative number effectivly underflow
                else if(carryTemp != -1 && (~num+1) < it2->num){
                    carryTemp += -1;
                }
            }
            else{
                num += it2->num;

                //when dealing with negatives check compliment
                if (carryTemp == -1 && (~num+1) < it2->num){
                    carryTemp += -1;
                }

                //using != -1 so it can be copy pasted to part below
                if (carryTemp != -1 && num < it2->num){
                    carryTemp += 1;
                }

            }
            it2= it2->next;


            
            //turns carrTemp into carry and sign
            if (carryTemp == -1){
                num  = ~num + 1;
                carry = 1;
                carrySign = -1;
            }
            else{
                carry = carryTemp;
                carrySign = 1;
            }

            //adds node
            //ADD NEW ADD_NODE
            //infNum_add_node(list3, num);


        }

        break;

    }



}

struct infNum *infNum_subtract (struct infNum *num1, struct infNum *num2) {
    //subtract num2 from num1
    // x - y = x + (-y)

    //checks if types are same
    if (num1->numType != num2->numType){
        return;
    }

    switch (num1->numType){
        case base:
            struct infNum *list3;
            num2->sign *= -1;//switches signs
            list3 = infNum_add(num1,num2);
            num2->sign *= -1;//switches signs to initial
            return list3;
            break;
        
        case new:
            struct infNum *list3;
            num2->sign *= -1;//switches signs
            list3 = infNum_add(num1,num2);
            num2->sign *= -1;//switches signs to initial
            return list3;
            break;
    }

}


void fprint_infNum (struct infNum *infNum, FILE *fptr){
    //prints an infNum to a file

    if(list_empty(infNum)){
        return;
    }

    switch (infNum->numType){
        case base:
        struct numNode *iterrator;
        char num[5]; //size logbase 10
        iterrator = infNum->last;

        //if sign is negative adds - sign
        if (infNum->sign == -1){
            fprintf(fptr, "%s", "-");
        }

        while (iterrator != NULL){

            if (iterrator->next != NULL){
                sprintf(num, "%04hu", iterrator->num);
            }
            else{
                sprintf(num, "%hu", iterrator->num);
            }

            fprintf(fptr, "%s", num);

            iterrator = iterrator->previous;
        }
        break;
    
    case new:
        break;
    }


}

int infNums_equal(struct infNum *infNum1, struct infNum *infNum2){
    //returns if two nums are equal to each other

    //checks length
    if (list_length(infNum1)!= list_length(infNum2)){
        return 0;
    }
    //checks signs
    if (infNum1->sign != infNum2->sign){
        return 0;
    }

    //checks type
    if (num1->numType != num2->numType){
        return;
    }

    switch (infNum1->numType){
    case base:
        struct numNode *it1, *it2;
        it1 = infNum1->first;
        it2 = infNum2->first;
        //only checks one as they are the same size
        while (it1 != NULL){
            if (it1->num != it2->num){
                return 0;
            }
            it1 = it1->next;
            it2 = it2->next;
        }
        return 1;
        break;
    
    case new:
        struct numNodeFull *it1, *it2;
        it1 = infNum1->first;
        it2 = infNum2->first;
        //only checks one as they are the same size
        while (it1 != NULL){
            if (it1->num != it2->num){
                return 0;
            }
            it1 = it1->next;
            it2 = it2->next;
        }
        return 1;
        break;
    }


}