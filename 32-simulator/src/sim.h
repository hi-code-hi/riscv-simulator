
#ifndef SIM_H
#define SIM_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>

using namespace std;

#define MEM_SIZE 	0x20000000
//#define MAP_SIZE	0x100

#define u8		unsigned char
#define u16		unsigned short
#define u32		unsigned int
#define u64		unsigned long long
#define	s8		signed char
#define s16		signed short
#define s32		signed int
#define s64		signed long long

extern u8 MEM[MEM_SIZE];

extern int mp_len;
extern u32 mp;

/*typedef struct{
        u32 value;      //this func's address in MEM
        int idx;        //this func's index in funcs_table
}Map_Vlu_Func;*/

//extern int cnt_maps;
//extern Map_Vlu_Func maps[MAP_SIZE];

#endif
