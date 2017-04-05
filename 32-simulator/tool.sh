#!/bin/bash
SOR_FILE=dhstone/dhrystone
RUN_SOR=riscv-simulator

rm sim.out
#rm sim-result

export PATH=$PATH:/opt/riscv/bin
riscv64-unknown-linux-gnu-readelf -a $SOR_FILE >dhstone/des-elf
riscv64-unknown-linux-gnu-objdump -D $SOR_FILE >dhstone/des-dump
xxd $SOR_FILE >dhstone/des-bin

c++ -o sim.out $RUN_SOR/*.cpp $RUN_SOR/*.h
./sim.out $SOR_FILE #>sim-result
#vi sim-result
