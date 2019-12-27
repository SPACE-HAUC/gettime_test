#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <papi.h>

extern uint64_t timenow(void);

uint64_t get_usec(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (uint64_t)ts.tv_sec * 1000000L + ((uint64_t)ts.tv_nsec)/1000;
}

int main(void) {
    int retval;
    if((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT )
    {
        printf("Library initialization error! \n");
        exit(1);
    }
    uint64_t s , e, tnow, usec;
    s = PAPI_get_real_usec();
    tnow = timenow();
    e = PAPI_get_real_usec();
    printf("Using C++ timenow | Time: %lli ms, Execution time: %lli us\n", tnow, e-s);
    s = PAPI_get_real_usec();
    usec = get_usec();
    e = PAPI_get_real_usec();
    printf("Using C get_usec  | Time: %lli ms, Execution time: %lli us\n", usec/1000, e-s);
    printf("Difference: %lli\n", usec/1000-tnow);
    return EXIT_SUCCESS;
}


