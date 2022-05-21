#include <stdio.h>
#include "defines.h"
#define _CRT_SECURE_NO_WARNINGS

extern union instructionRegister inst;

// R-type instructin decoding
void decodeRtype(unsigned int fct)
{
	unsigned int fcth, fctl;

	fctl = fct & 0x7;
	fcth = (fct & 0x38) >> 3;

	if (fcth == 0) {
		if (fctl == 0)      executeShift(SLL, inst.RI.rd, inst.RI.rt, inst.RI.sht);
		else if (fctl == 2) executeShift(SRL, inst.RI.rd, inst.RI.rt, inst.RI.sht);
		else if (fctl == 3) executeShift(SRA, inst.RI.rd, inst.RI.rt, inst.RI.sht);
		else {
			printf("Undefined instruction\n");
			return;
		}
	}
	else if (fcth == 1) {
		if (fctl == 0)      executeJr(JR, inst.RI.rs);
		else if (fctl == 4) executeSyscall(SYSCALL);
		else printf("Undefined instruction\n");
	}
	else if (fcth == 2) {
		if (fctl == 0)      executeMfHiLo(MFHI, inst.RI.rd);
		else if (fctl == 2) executeMfHiLo(MFLO, inst.RI.rd);
		else printf("Undefined instruction\n");
	}
	else if (fcth == 3) {
		if (fctl == 0)      executeMul(MUL, inst.RI.rs, inst.RI.rt);
		else printf("Undefined instruction\n");
	}
	else if (fcth == 4) {
		if (fctl == 0)      executeALU(ADD, inst.RI.rd, inst.RI.rs, inst.RI.rt);
		else if (fctl == 2) executeALU(SUB, inst.RI.rd, inst.RI.rs, inst.RI.rt);
		else if (fctl == 4) executeALU(AND, inst.RI.rd, inst.RI.rs, inst.RI.rt);
		else if (fctl == 5) executeALU(OR, inst.RI.rd, inst.RI.rs, inst.RI.rt);
		else if (fctl == 6) executeALU(XOR, inst.RI.rd, inst.RI.rs, inst.RI.rt);
		else if (fctl == 7) executeALU(NOR, inst.RI.rd, inst.RI.rs, inst.RI.rt);
		else printf("Undefined instruction\n");
	}
	else if (fcth == 5) {
		if (fctl == 2)      executeALU(SLT, inst.RI.rd, inst.RI.rs, inst.RI.rt);
		else printf("Undefined instruction\n");
	}
	else printf("Undefined instruction\n");
}

// instruction decoding
void instructionDecode(void)
{
	unsigned int opc, fct;

	unsigned int opch, opcl;

	opc = inst.RI.opc;
	fct = inst.RI.fct;

	printf("Opc: %2x, Fct: %2x, Inst: ", opc, fct);

	opcl = opc & 0x7;
	opch = (opc & 0x38) >> 3;

	if (opch == 0) {
		if (opcl == 0) {

			decodeRtype(fct);
		}
		else if (opcl == 1) executeBranch(BLTZ, inst.II.rs, inst.II.rt, inst.II.offset);
		else if (opcl == 2) executeJump(J, inst.JI.jval);
		else if (opcl == 3) executeJump(JAL, inst.JI.jval);
		else if (opcl == 4) executeBranch(BEQ, inst.II.rs, inst.II.rt, inst.II.offset);
		else if (opcl == 5) executeBranch(BNE, inst.II.rs, inst.II.rt, inst.II.offset);
		else printf("Undefined instruction\n");
	}
	else if (opch == 1) {
		if (opcl == 0)      executeALUI(ADDI, inst.II.rt, inst.II.rs, inst.II.offset);
		else if (opcl == 2) executeALUI(SLTI, inst.II.rt, inst.II.rs, inst.II.offset);
		else if (opcl == 4) executeALUI(ANDI, inst.II.rt, inst.II.rs, inst.II.offset);
		else if (opcl == 5) executeALUI(ORI, inst.II.rt, inst.II.rs, inst.II.offset);
		else if (opcl == 6) executeALUI(XORI, inst.II.rt, inst.II.rs, inst.II.offset);
		else if (opcl == 7) executeALUI(LUI, inst.II.rt, inst.II.rs, inst.II.offset);
		else printf("Undefined instruction\n");
	}
	else if (opch == 4) {
		if (opcl == 0)      executeLoadStore(LB, inst.II.rt, inst.II.rs, inst.II.offset);
		else if (opcl == 3) executeLoadStore(LW, inst.II.rt, inst.II.rs, inst.II.offset);
		else if (opcl == 4) executeLoadStore(LBU, inst.II.rt, inst.II.rs, inst.II.offset);
		else printf("Undefined instruction\n");
	}
	else if (opch == 5) {
		if (opcl == 0)      executeLoadStore(SB, inst.II.rt, inst.II.rs, inst.II.offset);
		else if (opcl == 3) executeLoadStore(SW, inst.II.rt, inst.II.rs, inst.II.offset);
		else printf("Undefined instruction\n");
	}
	else printf("Undefined instruction\n");
}

void executeShift(unsigned int op, unsigned int rd, unsigned int rt, unsigned int sht)
{
	printf("%s  $%d, $%d, %d\n", instName[op], rd, rt, sht);
}

void executeJr(unsigned int op, unsigned int rs)
{
	printf("%s  $%d\n", instName[op], rs);
}

void executeSyscall(unsigned int op)
{
	printf("%s\n", instName[op]);
}

void executeMfHiLo(unsigned int op, unsigned int rd)
{
	printf("%s $%d\n", instName[op], rd);
}

void executeMul(unsigned int op, unsigned int rs, unsigned int rt)
{
	printf("%s  $%d, $%d\n", instName[op], rs, rt);
}

void executeALU(unsigned int op, unsigned int rd, unsigned int rs, unsigned int rt)
{
	printf("%s  $%d, $%d, $%d\n", instName[op], rd, rs, rt);
}

void executeALUI(unsigned int op, unsigned int rt, unsigned int rs, unsigned int immediate)
{
	printf("%s  $%d, $%d, %d\n", instName[op], rt, rs, immediate);
}

void executeBranch(unsigned int op, unsigned int rs, unsigned int rt, int offset)
{
	if (op == BLTZ)
		printf("%s  $%d, %d\n", instName[op], rs, offset << 2); // print shifted offset
	else
		printf("%s  $%d, $%d, %d\n", instName[op], rs, rt, offset << 2); // print shifted offset
}

void executeJump(unsigned int op, unsigned int offset)
{

	printf("%s 0x%08X\n", instName[op], offset << 2);	// print shifted offset
}

void executeLoadStore(unsigned int op, unsigned int rt, unsigned int rs, int offset)
{
	printf("%s  $%d, %d($%d)\n", instName[op], rt, offset, rs);
}