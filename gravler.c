#include <stdio.h>
#include <omp.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>


// Used to seed the generator.           
//void fast_srand(int seed) {
//    g_seed = seed;
//}

// Compute a pseudorandom integer.
// Output value in range [0, 32767]
int fast_rand(long int *g_seed) {
    *g_seed = (6364136223846793005*(*g_seed)+1442695040888963407);//black magic
    return (*g_seed>>32)&0x7FFF;
}

void main(int argv, char **argc) {
    unsigned long max = 100;
    double timeTaken;
    struct timeval start, stop;
    int safeTurns=54, turnNessesary = 231, parNessesary = 177, max_parylized = 0, threads;
    int INT_MAX_BITS;
    double z, SD = 6.58122329054;

    setlocale(LC_NUMERIC, "");
    if (argv == 2){
        max = atoi(argc[1]);
    }

    

    gettimeofday(&start, NULL);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        int par, temp_rand;
        int parMax = 0;

        unsigned long i;
        int c;
        long int g_seed;

        srand(time(NULL)+id);
        g_seed = rand();

        for (i = id; i < max; i += nthreads) {
            //g_seed = rand();
            par = 0;
            for (c = 0; c < turnNessesary; c++){
                
                /*
                if (!(c % 15)){
                    temp_rand = rand();
                }
                par += !(temp_rand & 3);
                temp_rand =  temp_rand >>2;
                */
                /*
                if (!(c % 7)){
                    //temp_rand = rand();
                    temp_rand = fast_rand(&g_seed);
                }
                par += !(temp_rand & 3);
                temp_rand =  temp_rand >>2;
                */


                par += !(fast_rand(&g_seed) & 3);
                
                //par += !(c & 3);
            }
            if (parMax < par){
                parMax = par;
            }
        }
        #pragma omp critical
        {
            if (max_parylized < parMax){
                max_parylized = parMax;
            }
        }

        #pragma omp master//Not strictly nessesary
        {
            threads = nthreads;
        }
    }


    gettimeofday(&stop, NULL);
    timeTaken = (double)(stop.tv_sec-start.tv_sec) + 1e-6 *(stop.tv_usec-start.tv_usec);
    z = ((((double)max_parylized) -57.75)/SD);
    printf("Completed %'li trials.\n", max);
    printf("Done in %5.4f seconds doing on average %'.0f battles per sec.\n", timeTaken, max/timeTaken);
    printf("Used %d threads.\n", threads);
    printf("The largest number of paralysis was %d. Z-score of:%f\n", max_parylized, z);
    if (max_parylized >= parNessesary){
        printf("Congrats you escaped.\n");
    }
    else{
        printf("No luck try again.\n");
    }
}