# Run all benchmarks
for file in benchmarks/*.grid
do
    # remove the extension
    file="${file%.*}"
    echo $file > parameters
    ./main
done