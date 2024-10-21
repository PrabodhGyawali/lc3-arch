#ifdef TRAP_CODES_H
#define TRAP_CODES_H

enum trap {
	TRAP_GETC = 0x20, 	/* get char from keyboard */
	TRAP_OUT = 0x21, 	/* output a char */
	TRAP_PUTS = 0x22, 	/* output a word string */
	TRAP_IN = 0x23, 	/* get char from keyboard, echoed to terminal */
	TRAP_PUTSP = 0x24, 	/* output a byte string */
	TRAP_HALT = 0x25, 	/* halt the program */
};

#endif // TRAP_CODES_H

/**
 * Not in instructions, already built in LC-3 
 * Similar to OS system calls
 * Trap routines are written in assembly
 * 
 * Trap Routine address begines from: 0x0
 * 		Reason for programs starting at 0x3000
 */
