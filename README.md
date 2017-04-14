# hpq
Heap Priority Queue

# Compilation
```
> cd hpq
> make clean
> make all
```

# Execution
```
> ./a.out
> ./a.out -x                // hides progress
> ./a.out -t 64             // launch 64 worker threads
> ./a.out -i 5000           // each worker threads commit 5000 calls
> ./a.out -r 70             // 70% remove, 30% remove
> ./a.out -v                // Enable correctness verification
> ./a.out -d 10             // Each thread sleeps 10us between iteration
```

These flags can be combined like the following. 
```
> ./a.out -xv -t 64 -i 5000 -r 70 -d 10
```

Enabling correctness verification will skew timing. The benchmark reported disables this verification.

Our code do not check for invalid value. Make sure you pass valid parameters as defined below:
-d DEFAULT: 0. Any positive or zero integer.
-i DEFAULT: 2000. Any positive non-zero integer.
-r DEFAULT: 75. Any integer between 1 and 99. Anything beyond this range results in undefined behavior.
-t DEFAULT: 32. Any positive non-zero integer