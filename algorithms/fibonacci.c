#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <locale.h>//for using commas in integers in %'d. Is needed for '
#include <stdbool.h>
#include <unistd.h>


#include "infNum.c"

int main(int argc, char *argv[]) {
    double timeTaken;
    struct timeval start, stop;
    int max = 100, i, print_last = 0;
    struct infNum *infNum1 = infNum_Create(base), *infNum2 = infNum_Create(base), *infNum3;
    char filename[] = "fibonacci.txt";
    FILE *fptr;


    if (argc >=2){
        max = atoi(argv[1]);
    }

    int opt;
    //args
    while ((opt = getopt(argc, argv, "l")) != -1) {
        switch (opt) {
        case 'l': print_last = 1; break;
        default:
            //fprintf(stderr, "Usage: %s [-q] \n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    //Initiates number
    infNum_add_node(infNum1, 0);
    infNum_add_node(infNum2, 1);

    //clears
    fptr = fopen(filename, "w");
    fclose(fptr);

    fptr = fopen(filename, "a");
    //adds first num
    if (!print_last){
        fprintf(fptr, "%s", "0,1");
    }

    gettimeofday(&start, NULL);


    for (i=2; i < max; i ++){
        infNum3 = infNum_add(infNum1, infNum2);
        if (!print_last){
            fprintf(fptr, "%s", ",");
            fprint_infNum(infNum3, fptr);
        }

        
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

    if (print_last){
        fprint_infNum(infNum2, fptr);
    }

    infNum_clean(infNum1);
    infNum_clean(infNum2);
    free(infNum1);
    free(infNum2);
    fclose(fptr);

    setlocale(LC_NUMERIC, "");
    gettimeofday(&stop, NULL);
    timeTaken = (double)(stop.tv_sec-start.tv_sec) + 1e-6 *(stop.tv_usec-start.tv_usec);
    printf("Completed list the first %'d numbers of the Fibonachi numbers.\n", max);
    printf("Done in %8.4f seconds doing on average %'.0f numbers per sec.\n", timeTaken, max/timeTaken);

}