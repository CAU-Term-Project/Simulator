#include <stdio.h>
#include "defines.h"
#define _CRT_SECURE_NO_WARNINGS

unsigned int PC, IR;
union instructionRegister inst;

extern int ALU(int X, int Y, int C, int* Z);
extern int MEM(unsigned int A, int V, int nRW, int S);
extern void resetMem(void);
extern unsigned int invertEndian(unsigned int data);
extern void instructionDecode(void);

void showRegister(void) {
    //int i;
    //cout << "[REGISTER]" << endl;
    //for (i = 0; i < REG_SIZE; i++) {
    //    cout << "R" << i << "=";
    //    cout << R[i] << endl;
    //}
    //cout << "PC" << PC << endl;
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
        char command;
        printf("command: ");
        scanf("%c", &command);

        switch (command) {
        case 'l':
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

            break;
        case 'j':
            unsigned int jaddr;
            printf("Enter Jump Address: ");
            scanf("%d", &jaddr);
            setPC(jaddr);

            break;
        case 'g':
            while (IR != SYSCALL) step();
            break;
        case 's':
            step();
            break;
        case 'm':
            break;
        case'r':
            break;
        case'x':
            return;
        }
    }
}