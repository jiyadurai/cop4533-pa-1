# Programming Assignment 1: Matching and Verifying

Students: Jeevan Iyadurai (31691151), Luke Gorman ()

To compile / build, you ideally should have CMake. Then use these commands to build:

1. mkdir build
2. cd build
3. cmake ..

To run the matcher, you can either run with generated input or manual input. 
There is some generated input in the inputs directory already, or you can run the generate_cases.py to generate more. 
Alternatively, you can pass the -m flag to pa1 to manually input a test case. First, give the size of the input, 
followed by the preference lists for that many hospitals and then the preference lists for that many students. 

If bad input is given, there are no guarantees about the program's execution. 

![Log-Log Graph for Task C](outputs/chartresultsloglog.png)
![Quadratic Fit Graph for Task C](outputs/chartresultsfit.png)

Here are the charts of the results for matching. Note that the first chart plots on a logarithmic x and y scale. 
During this run, the lowest total execution time was for N=8. 
When the input size is small, the time taken for the I/O is highly variant and makes up most of the execution time.
However, once the input size becomes larger, it is clear from the quadratic fit lines that the algorithm's running time
is approximately quadratic.