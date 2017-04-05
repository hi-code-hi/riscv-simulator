#include "elf-parser.h"

#if 0
#define DEBUG_READ_FILE
#endif

#if 0
#define DEBUG_OUTPUT_EHDR
#define DEBUG_OUTPUT_PHDR
#define DEBUG_OUTPUT_SHDR
#define DEBUG_OUTPUT_SYM
#endif

unsigned long addr_time;
unsigned long addr_clock;

Elf32_Ehdr	Ehdr = {0};
Elf32_Shdr	Shdr[SHDR_NUM] = {0};
Elf32_Sym	Sym[SYM_NUM] = {0};
Elf32_Phdr	Phdr[PHDR_NUM] = {0};
char 		Shstrtab[SHSTRTAB_LEN] = {0};
char 		Strtab[STRTAB_LEN] = {0};

int Shdr_num = 0;
int Sym_num  = 0;
int Phdr_num = 0;

char *buffer;

const char *E_type[]={"NONE", "REL", "EXEC", "DYN", "CORE"};
const char *Sh_type[SH_TYPE_NUM] = {"NULL", "PROGBITS", "SYMTAB", "STRTAB", "RELA", "HASH",
                "DYNAMIC", "NOTE", "NOBITS", "REL", "SHLIB", "DYNSYM"};
const char *P_type[P_TYPE_NUM] = {"NULL", "LOAD", "DYNAMIC", "INTERP", "NOTE", "SHLIB", "PHDR"};

void get_Ehdr(char *buffer)
{
	memcpy(&Ehdr, buffer, sizeof(Ehdr));
	Shdr_num = Ehdr.e_shnum;
	Phdr_num = Ehdr.e_phnum;
}

void get_Phdr(char *buffer)
{
	int start = Ehdr.e_phoff;
	int size  = Ehdr.e_phentsize * Ehdr.e_phnum;

	memcpy(Phdr, &buffer[start], size);	
}

void get_Shdr(char *buffer)
{
	int start = Ehdr.e_shoff;
	int size  = Ehdr.e_shentsize * Ehdr.e_shnum;

	memcpy(Shdr, &buffer[start], size);
}

void get_Shstrtab(char *buffer)
{
	int idx   = Ehdr.e_shstrndx;
	int start = Shdr[idx].sh_offset;
	int size  = Shdr[idx].sh_size;
		
	memcpy(Shstrtab, &buffer[start], size);	
}

void get_Sym_Strtab(char *buffer)
{
	for (int i = 0; i < Shdr_num; i++)
	{
		int idx = Shdr[i].sh_name;
		char name[STRTAB_LEN];

		sscanf(&Shstrtab[idx], "%s", name);
		if (strcmp(name, ".symtab") == 0)
		{
			int start = Shdr[i].sh_offset;
			int size  = Shdr[i].sh_size;
			Sym_num = size / sizeof(Elf32_Sym);
			memcpy(Sym, &buffer[start], size);
		}
		else if (strcmp(name, ".strtab") == 0)
		{
			int start = Shdr[i].sh_offset;
			int size = Shdr[i].sh_size;
			
			memcpy(Strtab, &buffer[start], size);
		}
	}	
}

void print_Ehdr()
{
	cout << "ELF Header:" << endl;
	cout << "ident:" << '\t' << '\t' << '\t';

	for (int i = 0; i < 16; i++)
	{
		cout << hex << (unsigned int)Ehdr.e_ident[i] << ' ';
	}
	cout << endl;

	cout << "type:" <<  dec <<  '\t' << '\t'<< '\t' << Ehdr.e_type << '(' << E_type[Ehdr.e_type] << ')' << endl;
	cout << "machine:" <<  hex << '\t' << '\t' <<  "0x" << Ehdr.e_machine << '(' << "RISC-V" << ')' << endl;
	cout << "version:" <<  hex << '\t' << '\t' << "0x" <<  Ehdr.e_version << endl;
	cout << "e_entry:" <<  hex << '\t' << '\t' << "0x" <<  Ehdr.e_entry << endl;
	cout << "e_phoff:" <<  dec <<  '\t' << '\t' << Ehdr.e_phoff << endl;
	cout << "e_shoff:" <<  dec <<  '\t' << '\t' << Ehdr.e_shoff << endl;
	cout << "e_flags:" <<  hex << '\t' << '\t' << "0x" <<  Ehdr.e_flags << endl;
	cout << "e_ehsize:" <<  dec << '\t' << '\t' << Ehdr.e_ehsize << endl;
	cout << "e_phentsize:" <<  dec << '\t' << '\t' << Ehdr.e_phentsize << endl;
	cout << "e_phnum:" <<  dec <<  '\t' << '\t' << Ehdr.e_phnum << endl;
	cout << "e_shentsize:" <<  dec << '\t' << '\t' << Ehdr.e_shentsize << endl;
	cout << "e_shnum:" <<  dec <<  '\t' << '\t' << Ehdr.e_shnum << endl; 
	cout << "e_shstrndx:" <<  dec << '\t' << '\t' << Ehdr.e_shstrndx << endl;
	cout << endl << endl;
}

void print_Phdr()
{
	cout<< "Program Header table:" << endl;
	cout<< setiosflags(ios::left) << setw(16) << "Type";
	cout<< setiosflags(ios::left) << setw(12) << "offset";
	cout<< setiosflags(ios::left) << setw(12) << "VirtAddr";
	cout<< setiosflags(ios::left) << setw(12) << "PhysAddr";
	cout<< setiosflags(ios::left) << setw(12) << "Filesize";
	cout<< setiosflags(ios::left) << setw(12) << "Memsize";
        cout<< setiosflags(ios::left) << setw(12) << "Flg";
        cout<< setiosflags(ios::left) << setw(12) << "Align" << endl;
	
	cout << hex;
	for (int i = 0; i <  Phdr_num; i++)
	{
		if (Phdr[i].p_type >= P_TYPE_NUM)
			cout << "\t\t";
		else
			cout<< setfill('\0') << P_type[Phdr[i].p_type] << '\t' <<  '\t';

		cout<< setiosflags(ios::right) << "0x"<< setfill('0') << setw(8) << Phdr[i].p_offset << "  "; 
		cout<< "0x"<< setfill('0') << setw(8) << Phdr[i].p_vaddr << "  ";
		cout<< "0x"<< setfill('0') << setw(8) << Phdr[i].p_paddr << "  ";
		cout<< "0x"<< setfill('0') << setw(8) << Phdr[i].p_filesz << "  ";
		cout<< "0x"<< setfill('0') << setw(8) << Phdr[i].p_memsz << "  ";
		cout<< "0x"<< setfill('0') << setw(8) << Phdr[i].p_flags << "  ";
		cout<< "0x"<< setfill('0') << setw(8) << Phdr[i].p_align << "  " << endl;
	}
	cout << endl << endl;
}

void print_Shdr()
{
	cout<< "Section Header table:" << endl;
	cout << resetiosflags(ios::right) << setfill(' ');
	cout<< setiosflags(ios::left) << setw(30) << "Name";
        cout<< setiosflags(ios::left) << setw(10) << "Type";
        cout<< setiosflags(ios::left) << setw(10) << "Addr";
        cout<< setiosflags(ios::left) << setw(10) << "Off";
        cout<< setiosflags(ios::left) << setw(10) << "Size";
        cout<< setiosflags(ios::left) << setw(5) << "ES";
        cout<< setiosflags(ios::left) << setw(5) << "Flg";
        cout<< setiosflags(ios::left) << setw(5) << "LK";
        cout<< setiosflags(ios::left) << setw(5) << "Inf";
	cout<< setiosflags(ios::left) << setw(5) << "Al" << endl;

	for (int i = 0; i < Shdr_num; i++)
	{
		char name[STRTAB_LEN];
		sscanf(&Shstrtab[Shdr[i].sh_name], "%s", name);

		cout << setiosflags(ios::left) << setfill(' ') << setw(30) << name;
		cout << setiosflags(ios::left) << setfill(' ') << setw(10) << Sh_type[Shdr[i].sh_type];
		cout << setiosflags(ios::right) << setfill('0') << setw(8) << Shdr[i].sh_addr << "  ";
		cout << setiosflags(ios::right) << setfill('0') << setw(6) << Shdr[i].sh_offset << "    ";
		cout << setiosflags(ios::right) << setfill('0') << setw(6) << Shdr[i].sh_size << "    ";
		cout << resetiosflags(ios::right);
		cout << setiosflags(ios::left) << setfill(' ') << setw(5) << Shdr[i].sh_entsize;
		cout << setiosflags(ios::left) << setfill(' ') << setw(5) << Shdr[i].sh_flags;
		cout << setiosflags(ios::left) << setfill(' ') << setw(5) << Shdr[i].sh_link;
                cout << setiosflags(ios::left) << setfill(' ') << setw(5) << Shdr[i].sh_info;
                cout << setiosflags(ios::left) << setfill(' ') << setw(5) << Shdr[i].sh_addralign;
		cout << endl;
	}
	cout << endl << endl;
}

void print_Sym()
{
	cout << "Symbol table:" << endl;
	cout << resetiosflags(ios::right) << setfill(' ');
        cout<< setiosflags(ios::left) << setw(10) << "Value";
        cout<< setiosflags(ios::right) << setw(8) << "Size";
        cout<< setiosflags(ios::right) << setw(8) << "Info";
        cout<< setiosflags(ios::right) << setw(8) << "Other";
        cout<< setiosflags(ios::right) << setw(8) << "Ndx";
        cout<< setiosflags(ios::right) << setw(19) << "Name";
	cout<< endl;

	for (int i = 0; i < Sym_num; i++)
	{
		char name[STRTAB_LEN]={0};

		if (Sym[i].st_name)
			sscanf(&Strtab[Sym[i].st_name], "%s", name);

		cout << hex;
		cout << setiosflags(ios::right) << setfill('0') << setw(8) << Sym[i].st_value << "  " << dec;	
		cout << setiosflags(ios::right) << setfill(' ') << setw(8) << dec << Sym[i].st_size;
		cout << setiosflags(ios::right) << setfill(' ') << setw(8) << dec << (int)Sym[i].st_info;
		cout << setiosflags(ios::right) << setfill(' ') << setw(8) << dec << (int)Sym[i].st_other;
		cout << setiosflags(ios::right) << setfill(' ') << setw(8) << dec << (int)Sym[i].st_shndx << "\t\t" << name;
		//cout << setiosflags(ios::right) << setfill(' ') << setw(15) << dec << name;
		cout << endl;
		/*if(!strcmp("time",&Strtab[Sym[i].st_name]))
			addr_time=Sym[i].st_value;
		else if(!strcmp("clock",&Strtab[Sym[i].st_name]))
			addr_clock=Sym[i].st_value;*/
	}
	cout << endl << endl;
}





void elf_parser(char *argv[])
{
	int FileLength;

#if 0
	cout <<  "test file name: " << argv[1] << endl;
#endif

	ifstream fin(argv[1], ios::binary);
	if (!fin)
	{
		cout << "open file failed!" << endl;
		exit(0);
	}

	fin.seekg(0, ios::end);
	FileLength = fin.tellg();
	fin.seekg(0, ios::beg);

	buffer = new char[FileLength+1];
	if (!buffer)
	{
		cout << "error when malloc buffer!" << endl;
		exit(0);
	}
	memset(buffer, 0, sizeof(buffer));

	fin.read(buffer, sizeof(char) * FileLength);
	fin.close();

#ifdef DEBUG_READ_FILE
	ofstream fout("test-out", ios::binary);
	fout.write(buffer, sizeof(char) * FileLength);
	fout.close();
#endif

	get_Ehdr(buffer);
#ifdef DEBUG_OUTPUT_EHDR
	print_Ehdr();
#endif

	get_Phdr(buffer);
#ifdef DEBUG_OUTPUT_PHDR
	print_Phdr();
#endif
	
	get_Shdr(buffer);	
	get_Shstrtab(buffer);
#ifdef DEBUG_OUTPUT_SHDR
	print_Shdr();
#endif

	get_Sym_Strtab(buffer);
#ifdef DEBUG_OUTPUT_SYM
	print_Sym();
#endif

	for (int i = 0; i < Sym_num; i++)
         {
 
                if (Sym[i].st_name)
                {
			if(!strcmp("time",&Strtab[Sym[i].st_name]))
        	                addr_time=Sym[i].st_value;
                	else if(!strcmp("clock",&Strtab[Sym[i].st_name]))
                        	addr_clock=Sym[i].st_value;
		}
         }

	return;	
}

