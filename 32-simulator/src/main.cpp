#include "sim.h"
#include "elf-parser.h"
#include "cpu.h"

u8 MEM[MEM_SIZE] = {0};

int mp_len;
u32 mp;


void sim_init()
{
	u32 instr_start, instr_end;
	u32 init_addr;
	u32 rd = 1, rs1 = 4, opcode = 0x67;

	//将数据加载到内存中
	for (int i = 0; i < Phdr_num; i++)
	{
		if (Phdr[i].p_type == 1)
		{
			u32 addr = Phdr[i].p_vaddr;
			int size = Phdr[i].p_filesz;
			u32 *data = (u32 *)&buffer[Phdr[i].p_offset];

			sim_mem_write(addr, size, data);
		}
	}

	//生成开始和结束指令
	x[rs1] = Ehdr.e_entry;
	init_addr = 0x0;

	instr_start = opcode & 0x7f | (rd & 0x1f) << 7 | (rs1 & 0x1f) << 15;
	instr_end = 0x0;

	
	//加载开始和结束指令到内存中
	sim_mem_write(init_addr, 4, &instr_start);
	sim_mem_write(init_addr+4, 4, &instr_end);

	for (int i = 0; i < Sym_num; i++)
	{
		if (strcmp(&Strtab[Sym[i].st_name],"_gp") == 0)
		{
			x[3] = Sym[i].st_value;//初始化gp
			break;
		}
	}

	pc = init_addr;	//初始化pc
	x[2] = MEM_SIZE/2;//初始化sp
	mp = MEM_SIZE/2;//初始化mp
	mp_len = MEM_SIZE/2;

}

void bss_output()
{
	u32 g_addr, g_size;
	for (int i = 0; i < Shdr_num; i++)
	{
		if (!strcmp(&Shstrtab[Shdr[i].sh_name], ".bss"))
		{
			g_addr = Shdr[i].sh_addr;
			g_size = Shdr[i].sh_size;

			break;
		}
	}
	/*int bss_out;//只输出结果
	cout<<"bss:"<<endl;
	for(int i = 0; i < (g_size/4); i++ )
	{
		bss_out = MEM[g_addr];
		g_addr = g_addr+4;
		cout<<bss_out<<" ";
	}
	cout<<endl;*/
	for (int i = 0; i < Sym_num; i++)
	{
		u32 addr = Sym[i].st_value;
		u32 size = Sym[i].st_size;

		if (addr >= g_addr && addr < (g_addr+g_size))
		{
			if (Sym[i].st_name)
			{
				//输出bss所有内容
				/*cout << Strtab + Sym[i].st_name << ':';
				for (int j = 0; j < size; j+=4)
				{
					u32 data;
					sim_mem_read(addr+j, 4, &data);
					cout << (int)data << ' ';
				}*/
				//只输出result段
				if (!strcmp(Strtab+Sym[i].st_name, "result"))
				{
					cout << "bss " << Strtab + Sym[i].st_name << ':';
        	                        for (int j = 0; j < size; j+=4)
                	                {
                        	                u32 data;
                                	        sim_mem_read(addr+j, 4, &data);
                                         cout << (int)data << ' ';
                                	}
				}
				//cout << endl;
			}
		}
	}
	cout << endl;
}

int main(int argc, char *argv[])
{

        elf_parser(argv);
	sim_init();
	sim_exec();
	bss_output();
	cout << "total inst is:" << count << endl;
	return 0;
}

