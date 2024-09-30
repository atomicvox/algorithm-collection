#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <locale.h>//for using commas in integers in %'d. Is needed for '
#include <math.h>//for M_PI and fabs

void main(int argv, char **argc) {
    long int num_steps = 1e10;
    int threads;
    double step, timeTaken, percentDiff;
    double area = 0.0;
    struct timeval start, stop;
    gettimeofday(&start, NULL);
    setlocale(LC_NUMERIC, "");


    if (argv == 2){
        num_steps = atoi(argc[1]);
    }
    step = 1.0 / num_steps;

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        double partial_area = 0.0;

        int i;
        for (i = id; i < num_steps; i += nthreads) {
            double x = (i + 0.5) * step;
            double y = 4 / (1 + (x * x));
            partial_area += step * y;
        }

        #pragma omp critical
        {
            area += partial_area;
        }

        #pragma omp master//Not strictly nessesary
        {
            threads = nthreads;
        }
    }


    gettimeofday(&stop, NULL);
    timeTaken =  (double)(stop.tv_sec-start.tv_sec) + 1e-6 *(stop.tv_usec-start.tv_usec);
    percentDiff = fabs(100 * (M_PI - area)/M_PI);
    printf("PI with %li steps is %.10f. There is a %.8f%% diference.\n", num_steps, area, percentDiff);
    printf("Done in %8.4f seconds doing %'.0f points per sec.\n",timeTaken, num_steps/timeTaken); 
    printf("Used %d threads doing on avg %'.0f points per thread per sec.\n", threads, num_steps/(timeTaken*threads));
}
