#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <locale.h>//for using commas in integers in %'d. Is needed for '

#include "infNum.c"

int main(int argc, char *argv[]) {
    double timeTaken;
    struct timeval start, stop;
    int max = 100,i;
    struct infNum *infNum1 = infNumCreate(), *infNum2 = infNumCreate(), *infNum3;
    char filename[] = "fibonachi.txt";
    FILE *fptr;

    //Initiates number
    infNum_add_node(infNum1, 0);
    infNum_add_node(infNum2, 1);

    //clears
    fptr = fopen(filename, "w");
    fclose(fptr);

    fptr = fopen(filename, "a");
    //adds first num
    fprintf(fptr, "%s", "0,1");
    

    if (argc ==2){
        max = atoi(argv[1]);
    }
    gettimeofday(&start, NULL);
    setlocale(LC_NUMERIC, "");

    for (i=2; i < max; i ++){
        infNum3 = infNum_add(infNum1, infNum2);
        fprintf(fptr, "%s", ",");
        fprint_infNum(infNum3, fptr);
        
        infNum_clear(infNum1);
        free(infNum1);

        infNum1 = infNum2;
        infNum2 = infNum3;
        infNum3 = NULL;

        //every so often prints out a number
        if (i%1000 == 0){
            printf("On %d\n", i);
        }

    }
    fclose(fptr);


    gettimeofday(&stop, NULL);
    timeTaken = (double)(stop.tv_sec-start.tv_sec) + 1e-6 *(stop.tv_usec-start.tv_usec);
    printf("Completed list the first %'d numbers of the Fibonachi numbers.\n", max);
    printf("Done in %8.4f seconds doing on average %'.0f numbers per sec.\n", timeTaken, max/timeTaken);

}