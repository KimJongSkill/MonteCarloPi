# MonteCarloPi
#### Estimate Pi using a Monte Carlo simulation

A very inefficient  but fun way to approximate the value of Pi using a Monte Carlo algorithm.

The process is very simple and naive.

1. Generate a pair of random coordinates from 0 to r
2. If the points distance from the origin is less than r (ie the point is within a circle with radius r) then register a hit
3. Divide the total number of hits by the number of trials and multiply by 4 (since we were working with a quadrant)

The result should be a close approximation of Pi.

### RNG

Random number generation is obviously important, so the program defaults to the `RDRAND` instruction on systems that support it (all Intel proccessors starting from Ivy Bridge). If the CPU does not support it or hardware acceleration is disabled the program will use the STD's mersenne twister implementation.

### Usage

```
mcpi rounds [--software] [--threads=<n>]
```
Specifying `--software` disables hardware acceleration and `--threads` specifies how many threads to use (default is auto)
