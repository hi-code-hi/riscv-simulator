#include "cpu.h"
#include <unistd.h>
#include <time.h>
#include "elf-parser.h"
u32 x[32] = {0};	//32位整点寄存器
u64 f[32] = {0};	//64位浮点寄存器
u32 pc;			//pc指针
u32 ir;
int count = 0;		//指令计数
//读内存
void sim_mem_read(u32 address, int size, u32 *data)
{
	if (address >= MEM_SIZE)
	{
		cout << "illegal memory access!" << endl;
		exit(0);
	}

	memcpy(data, &MEM[address], size);
}
//写内存
void sim_mem_write(u32 address, int size, u32 *data)
{
	if (address >= MEM_SIZE)
	{
		cout << "illegal memory access!" << endl;
		exit(0);
	}
	
	memcpy(&MEM[address], data, size);
}

//取指、译码和执行
void sim_exec()
{
	u32 opcode, func1, func2;//编码
	u32 rs1, rs2, rd, immd, pc_old;//数据
	int i;

	while (true)
	{
		//取指
		sim_mem_read(pc, 4, &ir);
		count = count + 1;
		pc_old = pc;
		pc += 4;
		//译码
		opcode 	= ir & 0x7f;
		func1 = (ir >> 12) & 0x07;
		func2 = (ir >> 25) & 0x7f;
		rs1 = (ir >> 15) & 0x1f;
		rs2 = (ir >> 20) & 0x1f;
		rd = (ir >> 7) & 0x1f;
		//译码和执行
		switch(opcode)
		{
			case 0b0010011://整数计算指令
				switch(func1)
				{
                                        case 0b000:////ADDI rd, rs1, imm12
						x[rd] = x[rs1] + I_I_D(ir);
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "addi" << '\t' 
							<< "x"<< rd << '(' << x[rd] << ')' 
							<< ", x"<< rs1 << '(' << x[rs1] << ')' 
							<< ", " << (s32)I_I_D(ir)<< endl;
                                                #endif
                                                break;
                                        case 0b001:////SLLI rd, rs1, shamt
						if (func2 == 0b0000000)
						{
							x[rd] = x[rs1] << (I_I_D(ir) & 0x1f);
                                                	#if DEBUG
                                                	cout << hex << pc_old << dec <<  '	' << "slli" << '\t'  
							<< "x" << rd << '(' << x[rd] << ')' 
							<< ", x" << rs1 << '(' << x[rs1] << ')' 
							<< ", " << (s32)I_I_D(ir) <<endl;
                                                	#endif
							break;
						}	
                                        case 0b010://SLTI rd, rs1, imm12
						x[rd] = (s32)x[rs1] < (s32)I_I_D(ir) ? 1 : 0;
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "slti" << '\t'
							<< "x" << rd << '(' << x[rd] << ')' 
							<< ", x" << rs1 << '(' << x[rs1] << ')' 
							<< ", " << (s32)I_I_D(ir)<< endl;
                                                #endif
                                                break;
                                        case 0b011://SLTIU rd, rs1, imm12
						x[rd] = x[rs1] < I_I_D(ir) ? 1 : 0;
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "sltiu" << '\t'  
							<< "x" << rd << '(' << x[rd] << ')' 
							<< ", x" << rs1 << '(' << x[rs1] << ')' 
							<< ", " << (s32)I_I_D(ir)<< endl;
                                                #endif
                                                break;
                                        case 0b100://XORI rd, rs1, imm 
						x[rd] = x[rs1] ^ I_I_D(ir);
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "xori" << '\t'  
							<< "x" << rd << '(' << x[rd] << ')' 
							<< ", x" << rs1 << '(' << x[rs1] << ')' 
							<< ", " << (s32)I_I_D(ir)<< endl;
                                                #endif
                                                break;
                                        case 0b101://shift right
						switch(func2)
						{
							case 0b0000000://SRLI rd, rs1, shamt逻辑右移
								x[rd] = x[rs1] >> (I_I_D(ir) & 0x1f);
                                                        	#if DEBUG
                                                        	cout << hex << pc_old << dec <<  '	' << "srli"<< '\t' 
								<< "x" << rd << '(' << x[rd] << ')'
								<< ", x" << rs1 << '(' << x[rs1] << ')'
								<< ", " << (s32)I_I_D(ir)<< endl;
                                                        	#endif
								break;
							case 0b0100000://SRAI rd, rs1, shamt逻辑右移
								x[rd] = (s32)x[rs1] >> (I_I_D(ir) & 0x1f);
                                                                #if DEBUG
                                                                cout << hex << pc_old << dec <<  '	' << "srai"<< '\t'
								<< "x" << rd << '(' << x[rd] << ')' 
								<< ", x" << rs1 << '(' << x[rs1] << ')' 
								<< ", " << (s32)I_I_D(ir)<< endl;
                                                                #endif
								break;
							default:
                                                		cout << "1-Error:Unknown Instruction" << endl;
                                                		exit(0);
								break;
						};
                                                break; 
                                        case 0b110://ORI rd, rs1, imm
						x[rd] = x[rs1] | I_I_D(ir);
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "ori" << '\t' 
							<< "x" << rd << '(' << x[rd] << ')' 
							<< ", x" << rs1 << '(' << x[rs1] << ')' 
							<< ", " << (s32)I_I_D(ir)<< endl;
                                                #endif
                                                break;
                                        case 0b111://ANDI rd,rs1,imm12
						x[rd] = x[rs1] & I_I_D(ir);
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "andi" << '\t' 
							<< "x" << rd << '(' << x[rd] << ')' 
							<< ", x" << rs1 << '(' << x[rs1] << ')' 
							<< ", " << (s32)I_I_D(ir)<< endl;
                                                #endif
                                                break;
					default:
                                                cout << "2-Error:Unknown Instruction" << endl;
                                                exit(0);
						break;
				};
				break;
			case 0b0110011://整数计算指令
				switch(func1)
				{
					case 0b000:
                                                switch(func2)
                                                {
                                                        case 0b0000000://ADD rd,rs1,rs2
								x[rd] = x[rs1] + x[rs2];
                                                                #if DEBUG
                                                                cout << hex << pc_old << dec <<  '	' << "add" << '\t'  
								<< "x" << rd << '(' << x[rd] << ')' 
								<< ", x" << rs1 << '(' << x[rs1] << ')' 
								<< ", x" << rs2 << '(' << x[rs2] << ')' 
								<< endl;
                                                                #endif
                                                                break;
                                                        case 0b0100000://SUB rd,rs1,rs2
								x[rd] = x[rs1] - x[rs2];
                                                                #if DEBUG
                                                                cout << hex << pc_old << dec <<  '	' << "sub" << '\t' 
								<< "x" << rd << '(' << x[rd] << ')' 
								<< ", x" << rs1 << '(' << x[rs1] << ')' 
								<< ", x" << rs2 << '(' << x[rs2] << ')' 
								<< endl;
                                                                #endif
                                                                break;
                                                        case 0b0000001://MUL rd, rs1, rs2
                                                                x[rd] = (s32)x[rs1] * (s32)x[rs2];
                                                                #if DEBUG
                                                                cout << hex << pc_old << dec << '	' << "mul" << '\t' 
                                                                << "x" << rd << '(' << x[rd] << ')'
                                                                << ", x" << rs1 << '(' << x[rs1] << ')'
                                                                << ", x" << rs2 << '(' << x[rs2] << ')'
                                                                << endl;
                                                                #endif
                                                                break;
                                                        default:
	                                                	cout << "3-Error:Unknown Instruction" << endl;
        	                                        	exit(0);
                                                                break;
                                                };
						break;
                                        case 0b001://SLL rd,rs1,rs2
						if (func2 == 0b0000000)
						{
							x[rd] = x[rs1] << (x[rs2] & 0x1f);
                                                        #if DEBUG
                                                        cout << hex << pc_old << dec <<  '	' << "sll" << '\t' 
								<< "x" << rd << '(' << x[rd] << ')' 
								<< ", x" << rs1 << '(' << x[rs1] << ')' 
								<< ", x" << rs2 << '(' << x[rs2] << ')' 
								<< endl;
                                                        #endif
							break;
						}
                                        case 0b010://SLT rd,rs1,rs2
                                                if (func2 == 0b0000000)
                                                {
							x[rd] = (s32)x[rs1] < (s32)x[rs2]? 1 : 0;
                                                        #if DEBUG
                                                        cout << hex << pc_old << dec <<  '	' << "slt" << '\t' 
								<< "x" << rd << '(' << x[rd] << ')' 
								<< ", x" << rs1 << '(' << x[rs1] << ')' 
								<< ", x" << rs2 << '(' << x[rs2] << ')' 
								<< endl;
                                                        #endif
							break;
                                                }
                                        case 0b011://SLTU rd,rs1,rs2
                                                if (func2 == 0b0000000)
                                                {
							x[rd] = x[rs1] < x[rs2]? 1 : 0;
                                                        #if DEBUG
                                                        cout << hex << pc_old << dec <<  '	' << "sltu" << '\t' 
								<< "x" << rd << '(' << x[rd] << ')' 
								<< ", x" << rs1 << '(' << x[rs1] << ')' 
								<< ", x" << rs2 << '(' << x[rs2] << ')' 
								<< endl;
                                                        #endif
							break;
                                                }
                                        case 0b100://XOR rd,rs1,rs2 
                                                if (func2 == 0b0000000)
                                                {
							x[rd] = x[rs1] ^ x[rs2];
                                                        #if DEBUG
                                                        cout << hex << pc_old << dec <<  '	' << "xor" << '\t' 
								<< "x" << rd << '(' << x[rd] << ')' 
								<< ", x" << rs1 << '(' << x[rs1] << ')' 
								<< ", x" << rs2 << '(' << x[rs2] << ')' 
								<< endl;
                                                        #endif
							break;
                                                }
						else if (func2 == 0b0000001)//DIV rd, rs1, rs2
                                                {
							if(!x[rs2])
							{
								cout<<"DIV ZERO TRAP!"<<endl;
								exit(0);
                                                                break;
							}
                                                        x[rd] = (s32)x[rs1] / (s32)x[rs2];
                                                        #if DEBUG
                                                        cout << hex << pc_old << dec <<  '\t' << "div" << '\t' 
                                                                << "x" << rd << '(' << x[rd] << ')'
                                                                << ", x" << rs1 << '(' << x[rs1] << ')'
                                                                << ", x" << rs2 << '(' << x[rs2] << ')'
                                                                << endl;
                                                        #endif
                                                        break;
                                                }
                                        case 0b101:
                                                switch(func2)
                                                {
                                                        case 0b0000000://SRL rd,rs1,rs2
								x[rd] = x[rs1] >> (x[rs2] & 0x1f);
                                                        	#if DEBUG
                                                        	cout << hex << pc_old << dec <<  '	' << "srl" << '\t' 
									<< "x" << rd << '(' << x[rd] << ')' 
									<< ", x" << rs1 << '(' << x[rs1] << ')' 
									<< ", x" << rs2 << '(' << x[rs2] << ')' 
									<< endl;
                                                        	#endif
                                                                break;
                                                        case 0b0100000://SRA rd,rs1,rs2
								x[rd] = (s32)x[rs1] >> (x[rs2] & 0x1f);
                                                        	#if DEBUG
                                                        	cout << hex << pc_old << dec <<  '	' << "sra" << '\t' 
									<< "x" << rd << '(' << x[rd] << ')' 
									<< ", x" << rs1 << '(' << x[rs1] << ')' 
									<< ", x" << rs2 << '(' << x[rs2] << ')' 
									<< endl;
                                                        	#endif
                                                                break;
                                                        case 0b0000001://DIVU rd, rs1, rs2
                                                                x[rd] = x[rs1] / x[rs2];
                                                                #if DEBUG
                                                                cout << hex << pc_old << dec <<  '\t' << "divu" << '\t' 
                                                                        << "x" << rd << '(' << x[rd] << ')'
                                                                        << ", x" << rs1 << '(' << x[rs1] << ')'
                                                                        << ", x" << rs2 << '(' << x[rs2] << ')'
                                                                        << endl;
                                                                #endif
                                                                break;
                                                        default:
                                                		cout << "4-Error:Unknown Instruction" << endl;
                                                		exit(0);
                                                                break;
                                                };
                                                break;
                                        case 0b110://OR rd,rs1,rs2
                                                if (func2 == 0b0000000)
                                                {
							x[rd] = x[rs1] | x[rs2];
                                                        #if DEBUG
                                                        cout << hex << pc_old << dec <<  '	' << "or" << '\t' 
								<< "x" << rd << '(' << x[rd] << ')' 
								<< ", x" << rs1 << '(' << x[rs1] << ')' 
								<< ", x" << rs2 << '(' << x[rs2] << ')' 
								<< endl;
                                                        #endif
							break;
                                                }
                                                if (func2 == 0b0000001)//REM rd, rs1, rs2
                                                {
                                                        x[rd] = (s32)x[rs1] % (s32)x[rs2];
                                                        #if DEBUG
                                                        cout << hex << pc_old << dec <<  '\t' << "rem" << '\t' 
                                                                << "x" << rd << '(' << x[rd] << ')'
                                                                << ", x" << rs1 << '(' << x[rs1] << ')'
                                                                << ", x" << rs2 << '(' << x[rs2] << ')'
                                                                << endl;
                                                        #endif
                                                        break;
                                                }
                                        case 0b111://AND rd,rs1,rs2
                                                if (func2 == 0b0000000)
                                                {
							x[rd] = x[rs1] & x[rs2];
                                                        #if DEBUG
                                                        cout << hex << pc_old << dec <<  '	' << "and" << '\t' 
								<< "x" << rd << '(' << x[rd] << ')' 
								<< ", x" << rs1 << '(' << x[rs1] << ')' 
								<< ", x" << rs2 << '(' << x[rs2] << ')' 
								<< endl;
                                                        #endif
							break;
                                                }
                                                if (func2 == 0b0000001)//REMU rd, rs1, rs2
                                                {
                                                        x[rd] = x[rs1] % x[rs2];
                                                        #if DEBUG
                                                        cout << hex << pc_old << dec <<  '\t' << "remu" << '\t' 
                                                                << "x" << rd << '(' << x[rd] << ')'
                                                                << ", x" << rs1 << '(' << x[rs1] << ')'
                                                                << ", x" << rs2 << '(' << x[rs2] << ')'
                                                                << endl;
                                                        #endif
                                                        break;
                                                }
					default:
                                                cout << "5-Error:Unknown Instruction" << endl;
                                                exit(0);
						break;
				};
				break;
			case 0b1100011://条件跳转指令(SB类型)
				switch (func1)
				{
					case 0b000://BEQ
						if (x[rs1] == x[rs2])
							pc = (pc-4) + B_I_D(ir);
                                                        #if DEBUG
                                                        cout << hex << pc_old << dec <<  '\t' << "beq" << '\t' 
                                                                << "x" << rs1 << '(' << x[rs1] << ')'
                                                                << ", x" << rs2 << '(' << x[rs2] << ')'
                                                                << ", " << (s32)B_I_D(ir)<< endl;
                                                        #endif
						break;
                                        case 0b001://BNE
                                                if (x[rs1] != x[rs2])
                                                        pc = (pc-4) + B_I_D(ir);
                                                        #if DEBUG
                                                        cout << hex << pc_old << dec <<  '\t' << "bne" << '\t' 
                                                                << "x" << rs1 << '(' << x[rs1] << ')'
                                                                << ", x" << rs2 << '(' << x[rs2] << ')'
                                                                << ", " << (s32)B_I_D(ir)<< endl;
                                                        #endif
                                                break;
                                        case 0b100://BLT
                                                if ((s32)x[rs1] < (s32)x[rs2])
                                                        pc = (pc-4) + B_I_D(ir);
                                                        #if DEBUG
                                                        cout << hex << pc_old << dec <<  '	' << "blt" << '\t' 
								<< "x" << rs1 << '(' << x[rs1] << ')' 
								<< ", x" << rs2 << '(' << x[rs2] << ')' 
								<< ", " << (s32)B_I_D(ir)<< endl;
                                                        #endif
                                                break;
                                        case 0b101://BGE
                                                if ((s32)x[rs1] >= (s32)x[rs2])
                                                        pc = (pc-4) + B_I_D(ir);
                                                        #if DEBUG
                                                        cout << hex << pc_old << dec <<  '	' << "bge" << '\t' 
								<< "x" << rs1 << '(' << x[rs1] << ')' 
								<< ", x" << rs2 << '(' << x[rs2] << ')' 
								<< ", " << (s32)B_I_D(ir)<< endl;
                                                        #endif
                                                break;
                                        case 0b110://BLTU
                                                if (x[rs1] < x[rs2])
                                                        pc = (pc-4) + B_I_D(ir);
                                                        #if DEBUG
                                                        cout << hex << pc_old << dec <<  '	' << "bltu" << '\t' 
								<< "x" << rs1 << '(' << x[rs1] << ')' 
								<< ", x" << rs2 << '(' << x[rs2] << ')' 
								<< ", " << (s32)B_I_D(ir)<< endl;
                                                        #endif
                                                break;
                                        case 0b111://BGEU
                                                if (x[rs1] >= x[rs2])
                                                        pc = (pc-4) + B_I_D(ir);
                                                        #if DEBUG
                                                        cout << hex << pc_old << dec <<  '\t' << "bgeu" << '\t' 
                                                                << "x" << rs1 << '(' << x[rs1] << ')'
                                                                << ", x" << rs2 << '(' << x[rs2] << ')'
                                                                << ", " << (s32)B_I_D(ir)<< endl;
                                                        #endif
                                                break;
					default:
						cout << "6-Error:Unknown Instruction" << endl;
						exit(0);
						break;
				};
				break;
			case 0b0000011://访存指令（I类型-load）
				switch(func1)
				{
					case 0b000://lb
						sim_mem_read(x[rs1]+I_I_D(ir), 1, &x[rd]);
						//带符号扩展
						x[rd] = (s32)(x[rd] << 24) >> 24;
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "lb" << '\t' 
							<< "x" << rd << '(' << x[rd] << ')' 
							<< ", " << (s32)I_I_D(ir) 
							<< '(' << "x" << rs1 << '(' << x[rs1] << ')' << ')' << endl;
                                                #endif
						break;
                                        case 0b001://lh
                                                sim_mem_read(x[rs1]+I_I_D(ir), 2, &x[rd]);
                                                x[rd] = (s32)(x[rd] << 16) >> 16;
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "lh" << '\t' 
							<< "x" << rd << '(' << x[rd] << ')' 
							<< ", " << (s32)I_I_D(ir) 
							<< '(' << "x" << rs1 << '(' << x[rs1] << ')' << ')' << endl;
                                                #endif
                                                break;
                                        case 0b010://lw
						sim_mem_read(x[rs1]+I_I_D(ir), 4, &x[rd]);
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "lw" << '\t'  
							<< "x" << rd << '(' << x[rd] << ')' 
							<< ", " << (s32)I_I_D(ir) 
							<< '(' << "x" << rs1 << '(' << x[rs1] << ')' << ')' << endl;
                                                #endif
                                                break;
                                        case 0b100://lbu
                                                sim_mem_read(x[rs1]+I_I_D(ir), 1, &x[rd]);
						//不带符号扩展
                                                x[rd] = (x[rd] << 24) >> 24;
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "lbu"<< '\t' 
							<< "x" << rd << '(' << x[rd] << ')' 
							<< ", " << (s32)I_I_D(ir) 
							<< '(' << "x" << rs1 << '(' << x[rs1] << ')'<< ')' << endl;
                                                #endif
                                                break;
                                        case 0b101://lhu
                                                sim_mem_read(x[rs1]+I_I_D(ir), 2, &x[rd]);
                                                x[rd] = (x[rd] << 16) >> 16;
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "lhu" << '\t'
							<< "x" << rd << '(' << x[rd] << ')' 
							<< ", " << (s32)I_I_D(ir) 
							<< '(' << "x" << rs1 << '(' << x[rs1] << ')' << ')' << endl;
                                                #endif
                                                break;
					default:
                                                cout << "7-Error:Unknown Instruction" << endl;
                                                exit(0);
						break;
				};
				break;
			case 0b0100011://访存指令（S类型-store）
				switch(func1)
				{
                                        case 0b000://sb
						sim_mem_write(x[rs1]+S_I_D(ir), 1, &x[rs2]);
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "sb" << '\t'
							<< "x" << rs2 << '(' << x[rs2] << ')' 
							<< ", " << (s32)S_I_D(ir) 
							<< '(' << "x" << rs1 << '(' << x[rs1] << ')' << ')' << endl;
                                                #endif
                                                break;
                                        case 0b001://sh
						sim_mem_write(x[rs1]+S_I_D(ir), 2, &x[rs2]);
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "sh" << '\t'
							<< "x" << rs2 << '(' << x[rs2] << ')' 
							<< ", " << (s32)S_I_D(ir) 
							<< '(' << "x" << rs1 << '(' << x[rs1] << ')' << ')' << endl;
                                                #endif
                                                break;
                                        case 0b010://sw
						sim_mem_write(x[rs1]+S_I_D(ir), 4, &x[rs2]);
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "sw" << '\t'
							<< "x" << rs2 << '(' << x[rs2] << ')' 
							<< ", " << (s32)S_I_D(ir) 
							<< '(' << "x" << rs1 << '(' << x[rs1] << ')' << ')' << endl;
                                                #endif
                                                break;
					default:
                                                cout << "8-Error:Unknown Instruction" << endl;
                                                exit(0);
						break;
				};
				break;

			case 0b0110111://整数计算指令(U类)——LUI
				x[rd] = U_I_D(ir);
                                #if DEBUG
                                        cout << hex << pc_old << dec <<  '	' << "lui" << '\t' 
						<< "x" << rd << '(' << x[rd] << ')' << '(' << x[rd] << ')' 
						<< ", "<< (s32)U_I_D(ir)<< endl;
                                #endif
				break;
			case 0b0010111://整数计算指令(U类)——AUIPC
				x[rd] = (pc-4) + U_I_D(ir);
                                #if DEBUG
                                        cout << hex << pc_old << dec <<  '	' << "auipc" << '\t' 
						<< "x" << rd << '(' << x[rd] << ')' 
						<< ", "<< (s32)U_I_D(ir)<< endl;
                                #endif
				break;
			case 0b1101111://非条件跳转指令（UJ类）——JAL
				x[rd] = pc;
				pc = (pc-4) + UJ_I_D(ir);
				
				if(pc==addr_time)
				{
					pc=pc-UJ_I_D(ir)+4;
					x[10]=time((time_t *)(unsigned long)(x[10]));
				}
				else if(pc==addr_clock)
				{
					pc=pc-UJ_I_D(ir)+4;
					x[10]=clock();
				}
                                #if DEBUG
                                        cout << hex << pc_old << dec <<  '	' << "jal" << '\t' 
						<< "x" << rd << '(' << x[rd] << ')' 
						<< ", "<< (s32)UJ_I_D(ir)<< '\t' << hex << "pc=" << pc << dec <<endl;
                                #endif
				break;
			case 0b1100111://非条件跳转指令（I类型）——JALR
				x[rd] = pc;
				pc = (x[rs1] + I_I_D(ir)) & 0xfffffffe;
                                #if DEBUG
                                        cout << hex << pc_old << dec <<  '	' << "jalr" << '\t' 
						<< "x" << rd << '(' << x[rd] << ')' 
						<< ", x" << rs1 << '(' << x[rs1] << ')' 
						<< ", " << (s32)I_I_D(ir)<< '\t' << hex << "pc=" << pc << dec <<endl;
                                #endif
				break;
			case 0b0000111://浮点访存指令
                                if (func1 == 0b011)//FLD
                                {
                                                sim_mem_read(x[rs1]+I_I_D(ir), 8, (u32 *)&f[rd]);
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "FLD" << '\t' 
                                                        << "x" << rd << '(' << f[rd] << ')'
                                                        << ", " << (s32)I_I_D(ir) 
                                                        << '(' << "x" << rs1 << '(' << x[rs1] << ')' << ')' << endl;
                                                #endif
                                                break;
                                }
                                if (func1 == 0b010)//FLW
                                {
                                                sim_mem_read(x[rs1]+I_I_D(ir), 4, (u32 *)&f[rd]);
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "FLW" << '\t' 
                                                        << "x" << rd << '(' << f[rd] << ')'
                                                        << ", " << (s32)I_I_D(ir) 
                                                        << '(' << "x" << rs1 << '(' << x[rs1] << ')' << ')' << endl;
                                                #endif
                                                break;
                                }

			case 0b0100111://浮点访存指令
				if (func1 == 0b011)//FSD
				{
                                             	sim_mem_write(x[rs1]+S_I_D(ir), 8, (u32 *)&f[rs2]);
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "FSD" << '\t'
                                                        << "x" << rs2 << '(' << f[rs2] << ')'
                                                        << ", " << (s32)S_I_D(ir) 
                                                        << '(' << "x" << rs1 << '(' << x[rs1] << ')' << ')' << endl;
                                                #endif
                                                break;					
				}
                                if (func1 == 0b010)//FSW
                                {
                                                sim_mem_write(x[rs1]+S_I_D(ir), 4, (u32 *)&f[rs2]);
                                                #if DEBUG
                                                cout << hex << pc_old << dec <<  '	' << "FSW" << '\t'
                                                        << rs2 << '(' << f[rs2] << ')'
                                                        << "x" << ", " << (s32)S_I_D(ir) 
                                                        << '(' << "x" << rs1 << '(' << x[rs1] << ')' << ')' << endl;
                                                #endif
                                                break;
                                }
			case 0b1010011://跳过其他浮点指令
				break;
			case 0b1110011://scall系统调用指令
				if ((ir >> 7) == 0)
				{
					switch(x[17])
					{
						case RISCV_write://write(),printf会调用
							x[10] = write(x[10], (const void*)(x[11]+MEM), x[12]);
							#if DEBUG
							cout << hex << pc_old << dec << '	' << "write" << '\t' 
								<< x[10] << "," << x[11] << "," << x[12] << endl;
							#endif
							break;
						case RISCV_read://read(),scanf会调用
							x[10] = read(x[10], (void*)(x[11]+MEM), x[12]);
							#if DEBUG
							cout << hex << pc_old << dec << '	' << "read" << '\t'
								<< x[10] << "," << x[11] << "," << x[12] << endl;
							#endif
							break;
						case RISCV_time://time()
							x[10] = time((time_t*)(x[10]+MEM));
							#if DEBUG
							 cout << hex << pc_old << dec << '	' << "time" << '\t'
								<< x[10] << endl;
							#endif
							break;
						case RISCV_exit://结束
							#if DEBUG
							 cout << hex << pc_old << dec << '	' << "exit" << endl;
							#endif
							return;
					};
					break;
				}
			case 0b0000000://停机指令
                                #if DEBUG
                                	cout << hex << pc_old << dec <<  '	' << "stop" << endl;
                                #endif
				return;
			default:
                                cout << "9-Error:Unknown Instruction" << endl;
                                exit(0);
				break;
		};
		if (x[0] != 0)	x[0] = 0;
	}
}

