#!/bin/bash
echo "Today is " `date`

# Execute this script using: chmod u+x test.sh; ./test.sh

# Build executables
make

# Examine a (n, k) pair's real result (non-overflowing) 
# and computed result using actual 64-bit integers
if [ "$#" -eq 2 ]; then
    # n & k as script parameters
    printf "(n, k) = (%d, %d)\n" $1 $2
    # Real result
    ./bincoef $1 $2
    # Computed result
    ./manna_pnueli $1 $2
    # Clear executables
    make clear
    # Finish
    exit 0
fi

# clear previous output
if test -f "output.txt"; then 
    rm output.txt; 
fi

# If there are no program parameters, test all the following pairs
# of n and k and log the results in the file "output.txt".
for N in {50..70}; do
    for K in $(seq 0 1 $((N))); do
        printf "(n, k) = (%d, %d)\n" $N $K >> output.txt
        # Real binomial coefficient
        ./bincoef $N $K >> output.txt
        # Computed binomial coefficient
        ./manna_pnueli $N $K >> output.txt
        # Line-break
        echo -e '' >> output.txt
    done
done

# Clear executables
make clear