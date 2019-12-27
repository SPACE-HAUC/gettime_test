#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <papi.h>

extern uint64_t timenow() ;

int main ( void )
{
	int retval;
   	if((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT )
   	{
		printf("Library initialization error! \n");
      		exit(1);
   	}
	long long s = PAPI_get_real_cyc() ;
	uint64_t tnow = timenow() ;
	long long e = PAPI_get_real_cyc() ;
	printf("Time now from epoch: %lli\n", tnow);
	printf("Cycles: %lld\n", e - s );
	return 0 ;
}
