#include <stdio.h>
#include <stdlib.h>

struct infNum {
    struct numNode *first;
    struct numNode *last;
    short sign;
};

struct numNode {
    unsigned short int num;
    struct numNode *next;
    struct numNode *previous;
};

int list_empty(struct infNum *infNum){
    return infNum->first == NULL;
}

struct infNum *infNumCreate(){
    struct infNum *infNum = malloc(sizeof(struct infNum));
    infNum->first = NULL;
    infNum->last = NULL;
    infNum->sign = 1;
    return infNum;
}

void list_clear(struct infNum *infNum){
    struct numNode *iterrator;

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
    infNum->sign=1;
}

int list_length(struct infNum *infNum){
    int i = 1;
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
}

void list_add(struct infNum *infNum, short int num){
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
    if (num < 0){
        infNum->sign = -1;
        numNode->num = num * -1;
    }
}

void clean_list(struct infNum *infNum){
    struct numNode *iterrator = infNum->last;

    while (iterrator != NULL & iterrator->num == 0){
        infNum->last =  iterrator->previous;
        free(iterrator);
        iterrator = infNum->last;
        iterrator->next = NULL;
    }
}

struct infNum *add_nums (struct infNum *num1, struct infNum *num2) {
    //no protections use caution does not check for empty
    struct infNum *list3 = infNumCreate();
    unsigned short int num;
    short int carry = 0;
    struct numNode *it1 = num1->first, *it2 = num2->first;

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
        carry = num / 10000;

        num = num % 10000;

        //printf("%hu, ", num);
        list_add(list3, num);
    }

    if (carry != 0){
        list_add(list3, carry);
    }

    clean_list(list3);
    return list3;
}

struct infNum *subtract_nums (struct infNum *num1, struct infNum *num2) {
    //subtract num2 from num1
    // x - y = x + (-y)
    struct infNum *list3;
    num2->sign *= -1;//switches signs
    list3 = add_nums(num1,num2);
    num2->sign *= -1;//switches signs
    return list3;
}


void fprintNum (struct infNum *infNum, FILE *fptr){
    struct numNode *iterrator;
    char num[5];
    iterrator = infNum->last;

    if (!list_empty(infNum) & infNum->sign == -1){
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
}

int infNums_equal(struct infNum *infNum1, struct infNum *infNum2){
    struct numNode *it1, *it2;
    if (list_length(infNum1)!= list_length(infNum2)){
        return 0;
    }
    it1 = infNum1->first;
    it2 = infNum2->first;
    while (it1 != NULL || it2 != NULL){
        if (it1->num != it2->num){
            return 0;
        }
        it1 = it1->next;
        it2 = it2->next;
    }
    return 1;
}

struct infNum int_toInfNum(int num){
    struct infNum *infNum = infNumCreate();
}