#!/bin/bash

ITEREATIONS=(1000 10000 100000 1000000 10000000 100000000)
THREADS=(1 2 4 8 16)
NRUNS=5

for IT in "${ITEREATIONS[@]}"; do
  for TH in "${THREADS[@]}"; do
    for R in `seq 1 $NRUNS`; do
            ./cmake-build-debug/approxPiPar $IT $TH -f
    done
  done
done | tee output_file.csv