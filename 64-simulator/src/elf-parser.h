
#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include "sim.h"

#define Elf64_Addr	u64
#define Elf64_Half	u16
#define Elf64_Off	u64
#define Elf64_Sword	s32
#define Elf64_Word	u32
#define Elf64_Xword     u64
#define Elf64_Sxword    s64

#define SHDR_NUM        100
#define SYM_NUM         3000
#define PHDR_NUM        20
#define SHSTRTAB_LEN    2000
#define STRTAB_LEN      50000
#define P_TYPE_NUM	7
#define SH_TYPE_NUM	12

#define EI_NIDENT	16

extern unsigned long addr_time;
extern unsigned long addr_clock;

typedef struct{
	unsigned char	e_ident[EI_NIDENT];
	Elf64_Half	e_type;
	Elf64_Half	e_machine;
	Elf64_Word	e_version;
	Elf64_Addr	e_entry;
	Elf64_Off	e_phoff;
	Elf64_Off	e_shoff;
	Elf64_Word	e_flags;
	Elf64_Half	e_ehsize;
	Elf64_Half	e_phentsize;
	Elf64_Half	e_phnum;
	Elf64_Half	e_shentsize;
	Elf64_Half	e_shnum;
	Elf64_Half	e_shstrndx;
} Elf64_Ehdr;

typedef struct{
	Elf64_Word	sh_name;
	Elf64_Word	sh_type;
	Elf64_Xword	sh_flags;
	Elf64_Addr	sh_addr;
	Elf64_Off	sh_offset;
	Elf64_Xword	sh_size;
	Elf64_Word	sh_link;
	Elf64_Word	sh_info;
	Elf64_Xword	sh_addralign;
	Elf64_Xword	sh_entsize;
} Elf64_Shdr;

typedef struct{
	Elf64_Word	st_name;
	unsigned char   st_info;
	unsigned char   st_other;
	Elf64_Half      st_shndx;
	Elf64_Addr	st_value;
	Elf64_Xword	st_size;
	//unsigned char	st_info;
	//unsigned char	st_other;
	//Elf64_Half	st_shndx;
}Elf64_Sym;

typedef struct{
	Elf64_Word	p_type;
	Elf64_Word      p_flags;
	Elf64_Off	p_offset;
	Elf64_Addr	p_vaddr;
	Elf64_Addr	p_paddr;
	Elf64_Xword	p_filesz;
	Elf64_Xword	p_memsz;
	//Elf64_Word	p_flags;
	Elf64_Xword	p_align;
}Elf64_Phdr;

extern Elf64_Ehdr      Ehdr;
extern Elf64_Shdr      Shdr[SHDR_NUM];
extern Elf64_Sym       Sym[SYM_NUM];
extern Elf64_Phdr      Phdr[PHDR_NUM];

extern char Shstrtab[SHSTRTAB_LEN];
extern char Strtab[STRTAB_LEN];

extern int Shdr_num;
extern int Sym_num;
extern int Phdr_num;

extern char *buffer;

extern void elf_parser(char *argv[]);

#endif
