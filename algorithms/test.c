#include <stdio.h>
#include "infNum.c"
#include <stdlib.h>


int main(int argc, char *argv[]) {
    struct infNum *infNum = infNumCreate(), *c = infNumCreate();
    unsigned int test = 65535;
    int i;
    unsigned int sum = 1;
    char filename[] = "test.txt";
    FILE *fptr;

    list_add(infNum, 0);
    list_add(c, 1);
    //clears
    fptr = fopen(filename, "w");
    fclose(fptr);
    fptr = fopen(filename, "a");
    fprintNum(infNum, fptr);


    //for (i=0; i < 10000; i++){
    //    if(i%100==0){
    //        printf("On %d", i);
    //    }
    //    add_nums(infNum,c);
    //    fprintf(fptr, "%s", ",");
    //    fprintNum(infNum, fptr);
    //    c->first->num = c->first->num + 1;
    //    c->sign*=-1;
    //}



    //printf("test1: %d\n", test);
    //sum += test;
    //printf("test2: %d\n", sum);
    
    //printf("test1: %x\n", test);
    //printf("test2: %x\n", sum);

    printf("%d, %d", -11000%10000, -11000/10000);
    
    list_clear(infNum);
    list_clear(c);
    free(infNum);
    free(c);
    fclose(fptr);

}