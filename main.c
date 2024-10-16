#include "opcodes.h"
#include <stdint.h>

/* allowing negative numbers */
uint16_t sign_extend(uint16_t x, int bit_count) {
	if ((x >> (bit_count -1)) & 1) {
		x |= (0xFFFF << bit_count);
	}
}

/* when value is written to register need updating to indicate sign */
void update_flags(uint16_t r) {
	if (reg[r] == 0) {
		reg[R_COND] = FL_ZERO;
	}
	else if (reg[r] >> 15) /* a 1 in the left-most bit indicates negative */ {
		reg[R_COND] = FL_NEG;
	} else {
		reg[R_COND] = FL_POS;
	}
}

int main(int argc, char* argv[]) {
	// Load Arguments
	if (argc < 2) {
		fprintf(stderr, "lc3 [image-file1] ...\n");
		exit(2);
	}
	for (int j = 1; j < argc; j++) {
		if (!read_image(argv[j])) {
			fprintf(stderr, "failed to load image: %s\n", argv[j]);
			exit(1);
		}
	}
	// Setup
	
	/* one COND flag at a time */
	reg[R_COND] = FL_ZRO;
	
	/* set the PC to starting position */
	/* 0x300 is the default */
	enum { PC_START = 0x3000 };
	reg[R_PC] = PC_START;
	
	int running = 1;
	while (running) {
		/* FETCH */
		uint16_t instr = mem_read(reg[R_PC]++);
		uint16_t op = instr >> 12;
		
		switch (op) {
			case OP_ADD;
				// ENCODING (Big Endian) bit 5 is mode
				// 0001	DR	SR1	0	00	SR2
				// 0001 DR	SR1	1	imm5
				
				/* DR */
				uint16_t r0 = (instr >> 9) & 0x7;
				/* SR1 */
				uint16_t r1 = (instr >> 6) & 0x7;
				/* mode check */
				uint16_t imm_flag = (instr >> 5) & 0x1;
				if (imm_flag) {
					uint16_t imm5 = sign_extend(instr & 0x1F, 5);
					reg[r0] = reg[r1] + imm5;
				}
				else {
					uint16_t r2 = instr & 0x7;
					reg[r0] = reg[r1] + reg[r2];
				}
				
				update_flags(r0);
				
				break;
			case OP_NOT;
				// ENCODING (Big Endian)
				// 1001 DR SR 111111
				
				/* DR */
				uint16_t r0 = (instr >> 9) & 0x7;
				/* SR1 */
				uint16_t r1 = (instr >> 6) & 0x7;
				reg[r0] = !reg[r1];
				
				update_flags(r0);
				break;
		
			case OP_BR;
				// 0000 nzp PCoffset9
				uint16_t pc_offset = sign_extend((instr & 1FFF), 9);			
	
				uint16_t n = (instr >> 11) & 0x1;
				uint16_t z = (instr >> 10) & 0x1;
				uint16_t p = (instr >> 11) & 0x1;
				if (n or z or p) {
					reg[R_PC] = mem_read(mem_read(R_PC) + pc_offset);	
				}
				break;
			case OP_JMP;
				break;
			
			case OP_JSR;
				break;
			case OP_LD;
				break;
			case OP_LDI;
				// ENCODING (Big Endian)
				// 1010 DR PCoffset9
				uint16_t r0 = instr >> 9 & 0x7;
				uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
				/* add pc_offset to current PC */
				reg[r0] = mem_read(mem_read(reg[R_PC]) + pc_offset));
				update_flags(r0);
				break;
			case OP_LDR;
				break;
		
			case OP_LEA;
				break;
			case OP_ST;
				break;
			
			case OP_STI;
				break;
			case OP_STR;
				break;
			case OP_TRAP;
				break;
			case OP_RES;
				break;
		
			case OP_RTI;
				break;
			default:
				break;	
		}
	}
	// Shutdown
}
