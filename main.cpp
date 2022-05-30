#include <stdio.h>
#include "defines.h"
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

unsigned int PC, IR;
union instructionRegister inst;

extern int MEM(unsigned int A, int V, int nRW, int S);
extern unsigned int REG(unsigned int A, unsigned int V, unsigned int nRW);
extern void resetMem(void);
extern unsigned int invertEndian(unsigned int data);
extern void instructionDecode(void);

unsigned char progMEM[M_SIZE];
unsigned char dataMEM[M_SIZE];
unsigned char stakMEM[M_SIZE];

unsigned int data;
unsigned int addr;

unsigned int iCount;	// # of instructions
unsigned int dCount;	// # of data

void showRegister(void) {
    printf("PC: 0x%.8x\n", PC);
    printf("HI: 0x%.8x, LO: 0x%.8x\n", HI, LO);
    for (int i = 0; i < REG_SIZE; i++) {
        printf("R[%d]: 0x%.8x\n", i, REG(i, 0, 0));
    }
}

void setPC(unsigned int val) {
    PC = val;
    return;
}

void step() {
    inst.I = MEM(PC, 0, 0, 2);
    PC += 4;

    instructionDecode();
}

void main()
{
    FILE* fp;
    
    while (1) {
        // Get command line
        char command[2];
        printf("command: ");
        scanf("%s", command);
        getchar();

        if (strcmp(command, "sr") == 0) {
            unsigned int registerNumber, value;
            printf("Register Number: ");
            scanf("%d", &registerNumber);
            printf("Value: ");
            scanf("%d", &value);
            printf("R[%d] = %d\n", registerNumber, value);
            REG(registerNumber, value, 1);
        }
        else if (strcmp(command, "sm") == 0) {
            unsigned int location, value;
            printf("Location: 0x");
            scanf("%x", &location);
            printf("Value: ");
            scanf("%d", &value);
            printf("MEM[%.8x] = %d\n", location, value);
            MEM(location, value, 1, 2);
        }
        else if (command[0] == 'l') {
            resetMem();

            char* fileName;
            printf("Enter FileName: ");
            scanf("%s", &fileName);
            errno_t err;

            err = fopen_s(&fp, fileName, "rb");
            if (err) {
                printf("Cannot open file: %s\n", fileName);
                return;
            }

            // read instruction and data numbers
            fread(&data, sizeof(data), 1, fp);
            iCount = invertEndian(data);
            fread(&data, sizeof(data), 1, fp);
            dCount = invertEndian(data);
            printf("Number of Instructions: %d, Number of Data: %d\n", iCount, dCount);

            // Load to memory
            addr = 0;
            for (int i = 0; i < (int)iCount; i++) {
                fread(&data, sizeof(unsigned int), 1, fp);
                data = invertEndian(data);
                MEM(PROG_START + addr, data, 1, 2);
                addr += 4;
            }

            addr = 0;
            for (int i = 0; i < (int)dCount; i++) {
                fread(&data, sizeof(unsigned int), 1, fp);
                data = invertEndian(data);
                MEM(DATA_START + addr, data, 1, 2);
                addr += 4;
            }
            setPC(PROG_START);

            fclose(fp);
        }
        else if (command[0] == 'j') {
            unsigned int jaddr;
            printf("Enter Jump Address: ");
            scanf("%d", &jaddr);
            setPC(jaddr);
        }
        else if(command[0] == 'g') while (inst.RI.opc != SYSCALL) step();
        else if(command[0] == 's') step();
        else if (command[0] == 'm') {
            unsigned int start, end;
            printf("Start: ");
            scanf("%d", &start);
            printf("End: ");
            scanf("%d", &end);

            for (unsigned int addr = start; addr <= end; addr += 4) {
                printf("MEM[%d]: 0x%.8x\n", addr, MEM(addr, 0, 0, 2));
            }
        }
        else if(command[0] =='r') showRegister();
        else if(command[0] =='x') return;
    }
}