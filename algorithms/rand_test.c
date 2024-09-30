#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>


static unsigned int g_seed;

// Used to seed the generator.           
void fast_srand(int seed) {
    g_seed = seed;
}

// Compute a pseudorandom integer.
// Output value in range [0, 32767]
int fast_rand(void) {
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}

void main(int argv, char **argc) {
    unsigned long max = 100, i;
    unsigned long rand_c=0,rand_c2=0, fast_rand_c=0, fast_rand_c2=0;
    int rand_temp2, rand_temp, rand_num, fast_rand_num;

    char filename[] = "rand_test.csv";
    FILE *fptr;

    //clears
    fptr = fopen(filename, "w");
    fclose(fptr);

    fptr = fopen(filename, "a");
    //adds first num


    srand(time(NULL));
    fast_srand(time(NULL));

    if (argv == 2){
        max = atoi(argc[1]);
    }

    for (i = 0; i < max; i ++){
        rand_num = rand();
        fast_rand_num = fast_rand();

        rand_c += !(rand_num & 3);

        fast_rand_c += !(fast_rand_num & 3);

        if (!(i % 15)){
            rand_temp = rand_num;
            }
        rand_c2 += !(rand_temp & 3);
        rand_temp =  rand_temp >>2;

        if (!(i % 6)){
            rand_temp2 = fast_rand_num;
            }
        fast_rand_c2 += !(rand_temp2 & 3);
        rand_temp2 =  rand_temp2 >>2;

        //fprintf(fptr, "%d,%d\n", rand_num, fast_rand_num);
    }

    fclose(fptr);
    printf("Rand_: %.8f. Distance from: %.8f\n", (double)rand_c/max,fabs((double)rand_c/max-.25));
    printf("Rand2: %.8f. Distance from: %.8f\n", (double)rand_c2/max, fabs((double)rand_c2/max-.25));
    printf("fast_: %.8f. Distance from: %.8f\n", (double)fast_rand_c/max, fabs((double)fast_rand_c/max-.25));
    printf("fast2: %.8f. Distance from: %.8f\n", (double)fast_rand_c2/max, fabs((double)fast_rand_c2/max-.25));

}