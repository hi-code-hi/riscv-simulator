#!/bin/bash
export PATH=$PATH:/opt/riscv/bin
#riscv64-unknown-elf-gcc -o test.out -m32 -static -e main test.c
riscv64-unknown-elf-gcc -o test.out test.c
riscv64-unknown-elf-readelf -a test.out > test-elf
riscv64-unknown-elf-objdump -D test.out > test-dump
spike pk test.out > test-result

c++ -o sim.out sim/.c sim/*.h
./sim.out test.out
