
#ifndef CPU_H
#define CPU_H

#include "sim.h"

#define RISCV_write	64
#define RISCV_read	63
#define RISCV_time	1062
#define RISCV_exit	93

#define DEBUG 0 

#define I_I_D(instr)	((s32)instr >> 20 & 0xffffffff)
#define S_I_D(instr)	((instr >> 7) & 0x1f | ((s32)instr >> 20) & 0xffffffe0) 
#define B_I_D(instr)	((instr >> 7) & 0x1e | (instr >> 20) & 0x7e0 | (instr << 4) & 0x800 | ((s32)instr >> 19) & 0xfffff000)
#define U_I_D(instr)	((instr & 0xfffff000))
#define UJ_I_D(instr)	((instr >> 20) & 0x7fe | (instr >> 9) & 0x800 | instr & 0xff000 | ((s32)instr >> 11) & 0xfff00000)

extern u32 x[32];	//32 registers
extern u64 f[32];
extern u32 pc;
extern u32 ir;

extern int count;

extern void sim_mem_read(u32, int, u32 *);
extern void sim_mem_write(u32, int, u32 *);
extern void sim_exec(void);
#endif
