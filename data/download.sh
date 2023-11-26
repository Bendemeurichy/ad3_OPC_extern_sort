#!/usr/bin/env sh

for file in small medium smedium large; do
    wget "https://users.ugent.be/~rbmaerte/AD3/$file.txt.gz"
    gzip -d "$file.txt.gz"
done
