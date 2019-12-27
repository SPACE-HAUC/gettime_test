#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <papi.h>

extern uint64_t timenow() ;

#define MAX_RUN 1000

float rnd()
{
	return rand()/((float)RAND_MAX);
}

static inline float qrsqrt(float in)
{
	float xhalf = 0.5f*in ;
	int i = *(int*)&in ; // convert float to bits
	i = 0x5f375a86 - (i>>1) ; // initial guess
	in = *(float*)&i ; // convert back to float
	in = in*(1.5f-xhalf*in*in); // 1 iteration of Newton
	return in ;
}

static inline float q2rsqrt(float in)
{
	float xhalf = 0.5f*in ;
    int i = *(int*)&in ; // convert float to bits
    i = 0x5f375a86 - (i>>1) ; // initial guess
    in = *(float*)&i ; // convert back to float
	in = in*(1.5f-xhalf*in*in); // 1 iteration of Newton
	in = in*(1.5f-xhalf*in*in); // 2 iteration of Newton
	return in ;
}

static inline float q3rsqrt(float in)
{
	float xhalf = 0.5f*in ;
    int i = *(int*)&in ; // convert float to bits
    i = 0x5f375a86 - (i>>1) ; // initial guess
    in = *(float*)&i ; // convert back to float
	in = in*(1.5f-xhalf*in*in); // 1 iteration of Newton
	in = in*(1.5f-xhalf*in*in); // 2 iteration of Newton
	in = in*(1.5f-xhalf*in*in); // 3 iteration of Newton
	return in ;
}

static inline float srsqrt(float in)
{
	return 1./sqrt(in);
}


int main(void)
{
	// randomize seed
	srand(timenow()) ;
	// set up our vector
	float x, y, z, r2 ;
	float res1, res2, res3 ;
	x = rnd() ; y = rnd() ; z = rnd() ;
	r2 = x*x + y*y + z*z ;
	
	// initialize PAPI
	int retval;
   	if((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT )
   	{
		printf("Library initialization error! \n");
      		exit(1);
   	}

	double cyc1 = 0, cyc2 = 0, cyc3 = 0 ;
	double sig1 = 0, sig2 = 0, sig3 = 0;
	float err1, err2, err3 ;
	for ( unsigned int i = MAX_RUN ; i > 0 ; i-- )
	{
		long long s = PAPI_get_real_usec() ;
		res1 = q2rsqrt(r2) ;
		long long e = PAPI_get_real_usec() ;
		cyc1 += e - s ;
		sig1 += (e-s)*(e-s) ;

		s = PAPI_get_real_usec() ;
		res3 = srsqrt(r2) ;
		e = PAPI_get_real_usec() ;
		cyc3 += e - s ;
		sig3 += (e-s)*(e-s) ;

		s = PAPI_get_real_usec() ;
		res2 = q3rsqrt(r2) ;
		e = PAPI_get_real_usec() ;
		cyc2 += e - s ;
		sig2 += (e-s)*(e-s) ;
	}
	cyc1 /= MAX_RUN ;
	cyc2 /= MAX_RUN ;
	cyc3 /= MAX_RUN ;
	
	sig1 /= MAX_RUN ; sig1 = sqrt(sig1 -cyc1*cyc1) ;
	sig2 /= MAX_RUN ; sig2 = sqrt(sig2 -cyc2*cyc2) ;
	sig3 /= MAX_RUN ; sig3 = sqrt(sig3 -cyc3*cyc3) ;

	err1 = (res3 - res1)/res3 * 100 ;
	err2 = (res3 - res2)/res3 * 100 ;
	err3 = 0 ;

	printf("Vector: <%f %f %f>, r^2 = %f\n"
			"Result for %d runs:\n"
			"Q2RSQRT: %lf --> Cycle: %lf | Stdev: %lf | Error: %.5f%%\n"
			"Q3RSQRT: %lf --> Cycle: %lf | Stdev: %lf | Error: %.5f%%\n"
			"SRSQRT : %lf --> Cycle: %lf | Stdev: %lf | Error: %.5f%%\n"
			, x, y, z, r2, MAX_RUN,
			res1, cyc1, sig1, err1,
			res2, cyc2, sig2, err2,
			res3, cyc3, sig3, err3) ;
	
	return 0 ;

}
