#! /bin/bash

for i in {10..100}
do
	echo "0 1 $i" | ./omp_trap1 $i
done
