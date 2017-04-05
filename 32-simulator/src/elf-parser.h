
#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include "sim.h"

#define Elf32_Addr	u32
#define Elf32_Half	u16
#define Elf32_Off	u32
#define Elf32_Sword	s32
#define Elf32_Word	u32

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
	Elf32_Half	e_type;
	Elf32_Half	e_machine;
	Elf32_Word	e_version;
	Elf32_Addr	e_entry;
	Elf32_Off	e_phoff;
	Elf32_Off	e_shoff;
	Elf32_Word	e_flags;
	Elf32_Half	e_ehsize;
	Elf32_Half	e_phentsize;
	Elf32_Half	e_phnum;
	Elf32_Half	e_shentsize;
	Elf32_Half	e_shnum;
	Elf32_Half	e_shstrndx;
} Elf32_Ehdr;

typedef struct{
	Elf32_Word	sh_name;
	Elf32_Word	sh_type;
	Elf32_Word	sh_flags;
	Elf32_Addr	sh_addr;
	Elf32_Off	sh_offset;
	Elf32_Word	sh_size;
	Elf32_Word	sh_link;
	Elf32_Word	sh_info;
	Elf32_Word	sh_addralign;
	Elf32_Word	sh_entsize;
} Elf32_Shdr;

typedef struct{
	Elf32_Word	st_name;
	Elf32_Addr	st_value;
	Elf32_Word	st_size;
	unsigned char	st_info;
	unsigned char	st_other;
	Elf32_Half	st_shndx;
}Elf32_Sym;

typedef struct{
	Elf32_Word	p_type;
	Elf32_Off	p_offset;
	Elf32_Addr	p_vaddr;
	Elf32_Addr	p_paddr;
	Elf32_Word	p_filesz;
	Elf32_Word	p_memsz;
	Elf32_Word	p_flags;
	Elf32_Word	p_align;
}Elf32_Phdr;

extern Elf32_Ehdr      Ehdr;
extern Elf32_Shdr      Shdr[SHDR_NUM];
extern Elf32_Sym       Sym[SYM_NUM];
extern Elf32_Phdr      Phdr[PHDR_NUM];

extern char Shstrtab[SHSTRTAB_LEN];
extern char Strtab[STRTAB_LEN];

extern int Shdr_num;
extern int Sym_num;
extern int Phdr_num;

extern char *buffer;

extern void elf_parser(char *argv[]);

#endif
