#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>
#include <locale.h>//for using commas in integers in %'d. Is needed for '
#include <math.h>//for M_PI and fabs

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
    long points = 2.5e6;
    int out=0, threads;
    double pi,timeTaken, percentDiff;
    struct timeval start, stop;
    gettimeofday(&start, NULL);
    setlocale(LC_NUMERIC, "");

    if (argv == 2){
        points = atoi(argc[1]);
    }

    fast_srand(rand());
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        long pOut = 0;
        double x,y;

        long i;
        for (i = id; i < points; i += nthreads) {
            x = ((double) rand() / RAND_MAX)*2.0 - 1.0;//See if I can get better random functions
            y = ((double) rand() / RAND_MAX)*2.0 - 1.0;
            if ((x*x + y*y)>1){//Circle geometricly is x^2+y^2=1
                pOut++;
            }
        }

        #pragma omp critical
        {
            out += pOut;
        }

        #pragma omp master//Not strictly nessesary
        {
            threads = nthreads;
        }
    }

    gettimeofday(&stop, NULL);
    timeTaken =  (double)(stop.tv_sec-start.tv_sec) + 1e-6 *(stop.tv_usec-start.tv_usec);
    pi = ((float)(points - out) / (float)points)*4.0;
    percentDiff = fabs(100 * (M_PI - pi)/M_PI);
    printf("PI with %'ld points is %f. There is a %.8f%% diference.\n", points, pi, percentDiff);
    printf("Done in %8.4f seconds doing %'.0f points per sec.\n",timeTaken, points/timeTaken); 
    printf("Used %d threads doing on avg %'.0f points per thread per sec.\n", threads, points/(timeTaken*threads));
}
