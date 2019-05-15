Brief After-Action Report for Homework:

Essentially, the process that I used to modify the SRT implementation
so that it could handle multiple threads was first to identify
the segments of code that could be done in parallel. This was mainly
focused in the in the massive repeating quadruple loop in the middle
of the main function. To parallelize this program, I removed the
aforementioned loop from the main function and put it into its own
separate function. This function took in a parameter that indicated
which thread number it was on and looped through the available
pixels incrementing by the number of threads that the user wanted
to create. This skipped the appropriate amount of rows in the
screen and after all the threads had run, ever row was accounted
for. Meanwhile, in the main function, all that was left to do was
create the specified number of threads, and after they were created,
wait for each one to finish and post-processing, print out the
array of pixels.

The problems that I ran into when implementing this change were:
- determining which part of the main function to remove and
parallelize in its own function.
- determining how to pass the necessary data into the new function
which I ended up doing with global variables.
- determining when to print the array of pixels (after or during
the threads were processing). I landed on after because with
multiple threads processing and analyzing the data I didn't want
to print during and accidently print the incorrect data.

The conclusion, similar to that in the lab is that implementing
threading into the SRT implementation significantly decreased
the runtime for the processing of this image. With the default
one thread it took over 40 seconds to process, while by simply
increasing the number of threads to 8, the process took short of
5 seconds. 
