#!/bin/bash
echo "Today is " `date`

# Execute this script using: chmod u+x test.sh; ./test.sh

make
echo -e ''

if test -f "output.txt"; then 
    rm output.txt; 
fi

for N in {50..100}; do
    for K in $(seq 0 1 $((N))); do
        printf "(n, k) = (%d, %d)\n" $N $K >> output.txt
        ./bincoef $N $K >> output.txt
        ./manna_pnueli $N $K >> output.txt
        echo -e '' >> output.txt
    done
done

make clear