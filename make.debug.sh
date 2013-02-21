for f in *.c; do gcc -c -g -DDEBUG -o ${f%*.c}.o $f; done
cc -o calc *.o
