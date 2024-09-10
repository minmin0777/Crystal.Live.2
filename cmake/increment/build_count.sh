#!/bin/bash
count_file="build_count.txt"
if [ ! -f "$count_file" ]; then
  echo 0 > "$count_file"
fi
count=$(cat "$count_file")
count=$((count+1))
echo $count > "$count_file"
echo "Build number: $count"