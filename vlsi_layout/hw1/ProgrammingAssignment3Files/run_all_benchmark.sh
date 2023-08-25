# A bash script to run all benchmarks
# For all file inside the benchmarks folder, write the filename to the file parameters and run ./main
# Give me the code


# Run all benchmarks
for file in benchmarks/3QP/*
do
    echo $file > parameters
    echo 6 >> parameters
    ./main
done

for file in benchmarks/8x8QP/*
do
    echo $file > parameters
    echo 6 >> parameters
    ./main
done