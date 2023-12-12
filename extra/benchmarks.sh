#!/bin/bash



PROGRAM="../cmake-build-debug/AD3_project"  # Vervang dit door de naam van je C-programma
TMP="./tmp"


# 100 MiB
memory="$((1024*1024*32))"

rm -rf $TMP
ulimit -v "$((1024*1024*48))"
mkdir "$TMP"
OUTPUT="./output"
rm -rf $OUTPUT
mkdir "$OUTPUT"

FILES=("example.txt" "tiny.txt" "small.txt" "smedium.txt" "medium.txt" "large.txt")



for file in "${FILES[@]}"; do
    # Voer elke opdracht uit en meet de tijd
    echo "Running benchmarks for $file"
    echo "Running tree for $file"
    tree_time=$( { time $PROGRAM tree -i "../data/${file}" -o "${TMP}/${file}tree.txt" -m $memory; } 2>&1 | grep real | awk '{print $2}' )
    echo "Running compress for $file"
    compress_time=$( { time $PROGRAM compress -i "../data/${file}" -o "${TMP}/${file}compressed" -m $memory; } 2>&1 | grep real | awk '{print $2}' )
    echo "Running extract for $file"
    extract_time=$( { time $PROGRAM extract -i "${TMP}/${file}compressed" -o "${TMP}/${file}extracted.txt" -m $memory; } 2>&1 | grep real | awk '{print $2}' )
    echo "Running sort for $file"
    sort_time=$( { time $PROGRAM sort -i "${TMP}/${file}compressed" -o "${TMP}/${file}sorted" -m $memory; } 2>&1 | grep real | awk '{print $2}' )
    echo "Running bash sort for $file"
    # bash sort moet getest worden met uncompressed bestand, kan header niet aan
    bash_sort_time=$( { time sort --parallel=1 --buffer-size=32M -o "${TMP}/${file}sorted_bash" "../data/${file}"; } 2>&1 | grep real | awk '{print $2}' )

    # Schrijf de resultaten naar het bijbehorende CSV-bestand

    echo "${file};$tree_time" >> $OUTPUT/benchmark_results_tree.csv
    echo "${file};$compress_time" >> $OUTPUT/benchmark_results_compress.csv
    echo "${file};$extract_time" >> $OUTPUT/benchmark_results_extract.csv
    echo "${file};$sort_time" >> $OUTPUT/benchmark_results_sort.csv
    echo "${file};$bash_sort_time" >> $OUTPUT/benchmark_results_sort_bash.csv

done

rm -rf $TMP

