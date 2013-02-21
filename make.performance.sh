for f in *.c; do gcc -c -O3 -o ${f%*.c}.o $f; done
cc -o calc *.o
