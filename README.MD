# Test code to compare C and C++ implementations
## of functions that calculate time in microseconds
## from epoch

Dependencies: C++-11 compiler, PAPI library to measure execution time

The C++ function is wrapped to produce C-compatible function names.
Compilation:
```g++ -O2 -std=c++11 timenow_func.cpp -c```
This creates the object file containing the `timenow()` function.
```gcc -O2 -Wall c_timetest.c -lpapi -lstdc++``` links the `timenow()` function and the PAPI library to the test code which can be executed using `./a.out`.
