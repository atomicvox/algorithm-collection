#include <stdio.h>
#include <stdlib.h>
#include <locale.h>//for using commas in integers in %'d. Is needed for '
#include <stdbool.h>
#include <unistd.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    int i,c;
    int digitCount = 0, lCaseCount = 0, uCaseCount = 0, totalChars = 0;

    if (argc <2){
        return 1;
    }

    FILE *fptr = fopen(argv[1], "r");
    if (fptr == NULL){
        return 1;
    }
    int count[95] = {0};
    char ch;
    while ((ch = fgetc(fptr)) != EOF) {
        c = (int)ch;
        if (c < 126 && c >32){
            count[c-32] ++;
        }
    }


    //add options for coutning A and a as same
    setlocale(LC_NUMERIC, "");
    for (i = 0; i < 95; i++){
        if (count[i]>0){
            printf("Count of %c: %'d\n", (char)i+32, count[i]);
            totalChars+= count[i];

            if (i+32 > 47 && i+32 < 58){
                digitCount += count[i];
            }
            else if (i+32 > 64 && i+32 < 91){
                uCaseCount += count[i];
            }
            else if (i+32 > 96 && i+32 < 123){
                lCaseCount += count[i];
            }
        }
    }

    printf("\n");
    printf("Total Characters: %'d\n", totalChars);
    printf("Digit count: %'d\n", digitCount);
    printf("Letters: %'d\n", lCaseCount + uCaseCount);
    printf("Others: %'d\n", totalChars - digitCount - lCaseCount - uCaseCount);
}