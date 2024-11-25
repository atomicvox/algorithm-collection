#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>
#include <locale.h>//for using commas in integers in %'d. Is needed for '

#include <stdbool.h>
#include <unistd.h>


struct node {
    int val;
    int pos;
    struct node *prev;
    struct node *next;
};

struct list {
    struct node *head;
    struct node *tail;
};

int list_empty(struct list *list){
    return list->head == NULL;
}

struct list *list_create(){
    struct list *list = malloc(sizeof(struct list));
    list->head=NULL;
    list->tail=NULL;
    return list;

}

void list_add(struct list *list, int val){
    struct node *newNode = malloc(sizeof(struct node));
    newNode->val = val;
    newNode->next = NULL;
    if (list_empty(list)){
        list->head = newNode;
        list->tail = newNode;
        newNode->prev = NULL;
        newNode->pos = 1;
    }
    else{
        list->tail->next = newNode;
        newNode->prev = list->tail;
        newNode->pos = list->tail->pos+1;
        list->tail = newNode;
    }
}

void list_clear(struct list *list){
    struct node *curNode;
    if (list_empty(list)){
        return;
    }
    while(!list_empty(list)){
        curNode = list->head;
        if (curNode != list->tail){
            curNode = curNode->next;
            free(list->head);
            list->head = curNode;
        }
        else{
            free(curNode);
            list->head=NULL;
            list->tail=NULL;
            break;
        }
    }
}

struct node *list_getNode(struct list *list, int pos){
    struct node *curNode;
    if (pos > list->tail->pos){ //Out of bounds
        return NULL;
    }
    if (pos == 1){
        return list->head;
    }

    if (pos <= (int)(list->tail->pos / 2)){
        curNode = list->head;
        while (curNode != NULL && curNode->pos != pos){
            curNode = curNode->next;
        }
        return curNode;
    }
    else{
        curNode = list->tail;
        while (curNode != NULL && curNode->pos != pos){
            curNode = curNode->prev;
        }
        return curNode;
    }
    return NULL;
}

int list_getVal(struct list *list, int pos){
    struct node *curNode = list_getNode(list, pos);
    if (curNode == NULL){
        return 0;
    }
    
    return curNode->val;
}

int list_exists(struct list *list, int val){
    struct node *curNode = list->head;
    while (curNode != NULL){
        if (curNode->val == val){
            return curNode->pos;
        }
        curNode = curNode->next;
    }
    return 0;
}

int list_overlaps(struct list *list1, struct list *list2){
    //Finds if there is overlap between two lists, if yes return pos in first list
    struct node *curNode1,*curNode2;
    if (list_empty(list1) || list_empty(list2)){
        return 0;
    }
    
    curNode1 = list1->head;
    while (curNode1 != NULL){
        curNode2 = list2->head;
        while (curNode2 != NULL){
            //printf("Testing %d|%d eq%d\n", curNode1->val, curNode2->val, curNode1->val == curNode2->val);
            if (curNode1->val == curNode2->val){
                return curNode1->pos;
            }
            curNode2 = curNode2->next;
        }
        curNode1 = curNode1->next;
    }

    return 0;
}

void list_save(struct list *list){
    struct node *curNode = list->head;
    FILE *fptr;
    char filename[]="yellowstoneAlg.txt";
    char num[64];

    //clears
    fptr = fopen(filename, "w");
    fclose(fptr);

    fptr = fopen(filename, "a");
    //adds first num
    sprintf(num, "%d", curNode->val);
    fprintf(fptr, "%s", num);

    if (curNode == list->tail){
        fclose(fptr);
    }

    while (curNode !=  NULL){
        sprintf(num, "%d", curNode->val);
        fprintf(fptr, "%s",",");
        fprintf(fptr, "%s",num);
        curNode = curNode->next;
    }
    fclose(fptr);
}

void list_print(struct list *list){
    struct node *curNode;
    if (list_empty(list)){
        return;
    }

    printf("%d", list->head->val);
    if (list->head == list->tail){
        printf("%s","\n");
        return;
    }

    curNode = list->head->next;
    while (curNode != NULL){
        printf(", %d", curNode->val);
        curNode = curNode->next;
    }
    printf("%s","\n");
}

/*
Slow as balls
struct list *factor(int val){
    //Outputs a list containing the factors of val
    struct list *list = list_create();

    #pragma omp parallel
    {
        int i;
        int id = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        for (i = 2+id; i <= sqrt(val); i+=nthreads){ 
            if (val % i == 0){
                #pragma omp critical
                {
                    list_add(list, i);
                    if (i != val /i){
                     list_add(list, val / i);
                    }
                }
            }
        }
    }
    list_add(list, val);
    return list;
}

*/

struct list *factor(int val){
    // Outputs a list containing the factors of val
    struct list *list = list_create();
    int i;
    for (i = 2; i <= sqrt(val); i++){
        if (val % i == 0){
            list_add(list, i);
            if (i != val / i){
                list_add(list, val / i);
            }
        }
    }
    list_add(list, val);
    return list;
}

int relativlyPrime(int val1, int val2){
    int temp = val2;
    val2 = val1 % val2;
    val1 = temp;

    if (val2 && val2 !=1){
        return relativlyPrime(val1,val2);
    }

    return val2;
}

int smallestFactor(int val){
    int i;
    for (i = 2; i <= sqrt(val); i++){
        if (val % i == 0){
            return i;
        }
    }
    return val;
}


int main(int argc, char *argv[]) {
    struct list *list = list_create();
    int i,num, prev, prev2;
    double timeTaken;
    struct timeval start, stop;
    int max = 10000;
    bool isQuite = 0;

    if (argc >= 2){
        max = atoi(argv[1]);
    }

    int opt;

    //args
    while ((opt = getopt(argc, argv, "q")) != -1) {
        switch (opt) {
        case 'q': isQuite = 1; break;
        default:
            //fprintf(stderr, "Usage: %s [-q] \n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    gettimeofday(&start, NULL);
    setlocale(LC_NUMERIC, "");
    list_add(list, 1);
    list_add(list, 2);
    list_add(list, 3);
    prev = list->tail->prev->val;

    
    for (i = 0; i < max; i++){
        prev2 = prev;
        prev = list->tail->val;

        num = smallestFactor(prev2); //smallest it can be
        while (relativlyPrime(num,prev2) || !relativlyPrime(num,prev) || list_exists(list,num)){
            //Checks that num and prev2 are relativly prime, num and prev are not and that num does not exist in list
            //printf("Testing %d not coPrime with %d coPrime with %d or exists\n", num, prev, prev2);
            num++;
        }
        list_add(list,num);
        if (!isQuite){
            printf("The %d number added is %d\n", i, num);    
        }
    }
    
    list_save(list);

    gettimeofday(&stop, NULL);
    timeTaken = (double)(stop.tv_sec-start.tv_sec) + 1e-6 *(stop.tv_usec-start.tv_usec);
    printf("Completed list the first %'d numbers of the yellostonePerm w Euclidian GCD.\n", max);
    printf("Done in %8.4f seconds doing on average %'.0f numbers per sec.\n", timeTaken, max/timeTaken);


    list_clear(list);
    free(list);
    return 0;
}