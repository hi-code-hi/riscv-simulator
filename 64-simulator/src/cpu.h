
#ifndef CPU_H
#define CPU_H

#include "sim.h"

#define RISCV_write	64
#define RISCV_read	63
#define RISCV_time	1062
#define RISCV_exit	93

#define DEBUG 0 

#define I_I_D(instr)	(s64)((s32)((s32)instr >> 20 & 0xffffffff))//先转换成32位带符号类型，再符号扩展为64位
#define S_I_D(instr)	(s64)((s32)((instr >> 7) & 0x1f | ((s32)instr >> 20) & 0xffffffe0)) 
#define B_I_D(instr)	(s64)((s32)((instr >> 7) & 0x1e | (instr >> 20) & 0x7e0 | (instr << 4) & 0x800 | ((s32)instr >> 19) & 0xfffff000))
#define U_I_D(instr)	(s64)((instr & 0xfffff000))
#define UJ_I_D(instr)	(s64)((s32)((instr >> 20) & 0x7fe | (instr >> 9) & 0x800 | instr & 0xff000 | ((s32)instr >> 11) & 0xfff00000))

extern u64 x[32];	//32 registers
extern u32 f[32];	//浮点寄存器是32位拼成的
extern u64 pc;
extern u32 ir;//指令是32位的

extern long count;

extern void sim_mem_read(u32, int, void *);
extern void sim_mem_write(u32, int, void *);
extern void sim_exec(void);
#endif
