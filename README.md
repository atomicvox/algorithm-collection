# algorithm-collection

A collection of random algorithms

1 - Run run.sh

## tidbits

### Gravler.c

The rand is as fast as I am willing to have it. Any further speed improvements lead to unacceptable at least to me losses in randomness. Certain changes seem to lead to maximums that seem to happen due to all values being used. Could be faster but leads to loss of randomness which is somewhat hard to measure.

As most of the time was due to rand, it was the object of improvment.
To understand the optimization it is nessesary to understand how the number is used.
As each "turn" the probabity for parylization is 1/4, it is equivilant to rand() mod 4.
Due computers using base 2 this can be simplified to seeing if the last two bits are 00.
This allows us to reuse the same number multiple times as each "turn" just uses two bits

As for how the rand functions, no clue tested some and this was the best. Has the largest cycle allowing for the it to not run out of numbers.

Simarly there were compilier optimizations done to improve speed.
