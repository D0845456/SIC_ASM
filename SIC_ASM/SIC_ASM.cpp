// SICASM.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <stdio.h>
#include <stdlib.h>
/************************STRUCTURE**********************/
typedef struct OperationCodeTable
{
    char Mnemonic[8];
    char Format;
    unsigned short int  ManchineCode;
}SIC_OPTAB;

typedef struct SymbolTable
{
    char Label[10];
    int Address;
}SIC_SYMTAB;

typedef struct instruct
{
    int Loc;  // location
    char Label[10]; // 標籤
    char Operator[6];  // 運算子
    char Operand[20]; // 運算元
    char ObjCode[7];
} instruct;
int progLen = 0;
int SymTabLen = 0;
int ObjLen = 0;
int LOC[1000];
char buf[100][100];
char ObjCode[100][7];
SIC_SYMTAB SymTab[100];
char filename[100] = { "" };

static SIC_OPTAB OPTAB[] =
{
    {   "ADD",  '3',  0x18},
    {   "AND",  '3',  0x40},
    {  "COMP",  '3',  0x28},
    {   "DIV",  '3',  0x24},
    {     "J",  '3',  0x3C},
    {   "JEQ",  '3',  0x30},
    {   "JGT",  '3',  0x34},
    {   "JLT",  '3',  0x38},
    {  "JSUB",  '3',  0x48},
    {   "LDA",  '3',  0x00},
    {  "LDCH",  '3',  0x50},
    {   "LDL",  '3',  0x08},
    {   "LDX",  '3',  0x04},
    {   "MUL",  '3',  0x20},
    {    "OR",  '3',  0x44},
    {    "RD",  '3',  0xD8},
    {  "RSUB",  '3',  0x4C},
    {   "STA",  '3',  0x0C},
    {  "STCH",  '3',  0x54},
    {   "STL",  '3',  0x14},
    {  "STSW",  '3',  0xE8},
    {   "STX",  '3',  0x10},
    {   "SUB",  '3',  0x1C},
    {    "TD",  '3',  0xE0},
    {   "TIX",  '3',  0x2C},
    {    "WD",  '3',  0xDC},
};
/**************** FUNCTION **************/
void fileInput(instruct* tptr)
{
    // t[0].label = {"FIRST"};
    // t[0].instr = {"LDA"};
    // t[0].operand = {"ZERO"};
    int row = 0;
    FILE* fptr = NULL;
    char filename_asm[50] = { "COPY.asm" };
    //char filename_asm[50];
    printf("Please enter your file:");
    //fgets(filename_asm, 50, stdin);
    printf("%s\n", filename_asm);
    for (unsigned int i = 0; i < strlen(filename_asm); i++) {
        if (filename_asm[i] != '.') {
            filename[i] = filename_asm[i];
        }
        else break;
    }
    //printf("filename = %s\n", filename);
    //fptr = fopen("SystemProgram\\SIC_C\\SIC_20210502\\HW1A.asm", "r");
    fptr = fopen(filename_asm, "r");
    //fptr = fopen("test.asm", "r");
    if (!fptr)
        printf("File Open Failed\n");
    else
        printf("File Open successed!\n");
    while (!feof(fptr))
    {
        fgets(buf[row], 100, fptr);

        if (buf[row][0] == '.')
        {
            continue;
        }
        int count = 0, i = 0;
        while (count < 3)
        {
            // printf("count = %d\n",count);
            if (count == 0)
                i = 0;
            else if (count == 1)
                i = 9;
            else
                i = 17;
            int len = 0;
            char str[10] = { "" };
            while (buf[row][i] != ' ' && buf[row][i] != '\n' && buf[row][i] != '\t')
            {
                // printf("%c",buf[row][i]);
                str[len] = buf[row][i];
                len++;
                i++;
            }
            // printf("str = %s\n",str);
            if (count == 0)
                sprintf(tptr[row].Label, "%s", str);
            else if (count == 1)
                sprintf(tptr[row].Operator, "%s", str);
            else
                sprintf(tptr[row].Operand, "%s", str);
            count++;
        }
        // printf("row = %d\n",row);
        //printf("%-9s%-8s%s\n", tptr[row].Label, tptr[row].Operator, tptr[row].Operand);
        row++;
    }
    //puts("");
    progLen = row;
    fclose(fptr);
    //printf("%d\n", progLen);
}
void listFile(instruct* tPtr) {
    FILE* lstPtr = NULL;
    char list[5] = ".lst";
    char filename_lst[100] = { "" };
    strcpy(filename_lst, filename);
    strcat(filename_lst, list);
    lstPtr = fopen(filename_lst, "w");
    for (int i = 0; i < progLen; i++) {
        fprintf(lstPtr, "%-6X%-8s%-8s%-9s%-6s\n", tPtr[i].Loc, tPtr[i].ObjCode, tPtr[i].Label, tPtr[i].Operator, tPtr[i].Operand);//%-6s
        //printf("%-6X%-8s%-8s%-9s%-6s\n", tPtr[i].Loc, tPtr[i].Label, tPtr[i].Operator, tPtr[i].Operand, tPtr[i].ObjCode);
    }
    fprintf(lstPtr, "\0");
    fclose(lstPtr);
    printf("listFIle finished!!\n");
}
int HexToDec(int hex) {
    int dec = 0;
    int pow = 1;
    int tmp = hex;
    while (tmp) {
        dec += (tmp % 10) * pow;
        tmp /= 10;
        pow *= 16;
    }
    return dec;
}
void makeSymTable(instruct* tPtr) {
    int i = 0;
    for (i = 0; i < 100; i++) {
        strcpy(SymTab[i].Label, "\0");
        SymTab[i].Address = 0;
    }
    for (i = 0; i < progLen; i++) {
        if (tPtr[i].Label[0] != '\0') {
            strcpy(SymTab[SymTabLen].Label, tPtr[i].Label);
            SymTab[SymTabLen].Address = tPtr[i].Loc;
            SymTabLen++;
        }
    }
    //printf("Symble Table Len = %d\n", SymTabLen);
    /*for (i = 0; i < SymTabLen; i++) {
        printf("%s\t", SymTab[i].Label);
        printf("%X\n", SymTab[i].Address);
    }*/
}
int findOpTable(char* Oper) {
    int i = 0, MCCode = 0;
    int OpTabLen = sizeof(OPTAB) / sizeof(OPTAB[0]);
    for (i = 0; i < OpTabLen; i++) {
        if (strcmp(Oper, OPTAB[i].Mnemonic) == 0) {
            MCCode = OPTAB[i].ManchineCode;
            return MCCode;
        }
        else if (strcmp(Oper, "WORD") == 0 || strcmp(Oper, "BYTE") == 0) {
            return 0xfe;
        }
        else if (strcmp(Oper, "RESW") == 0 || strcmp(Oper, "RESB") == 0) {
            return 0xfd;
        }
    }
    return 0xff;
}
int findSymTable(char* Lable) {
    int i = 0, LabAdd = 0;
    //printf("Lable = %s\n", Lable);
    if (Lable[0] == '\0') {
        return 0x00;
    }
    for (i = 0; i < SymTabLen; i++) {
        if (strcmp(Lable, SymTab[i].Label) == 0) {
            LabAdd = SymTab[i].Address; // Lable Address
            //printf("%s LabAdd = %X\n", Lable,LabAdd);
            return LabAdd;
        }
    }
    int Lable_len = strlen(Lable);
    int index = 0;
    char tmp[10] = { "" };
    //printf("Lable_len = %d\n", Lable_len);
    for (index = 0; index < Lable_len; index++) {
        //printf("[%c]", Lable[index]);
        if (Lable[index] == ',') {
            strncpy(tmp, Lable, index);
            //printf("tmp = %s\n", tmp);
            return findSymTable(tmp) + 0x8000;
        }
    }
    //printf("\n");
    return 0x00;
}
void makeObjCode(instruct* tPtr) {
    int i = 0;
    for (i = 0; i < 100; i++) {
        strcpy(tPtr[i].ObjCode, "\0");
    }
    findOpTable(tPtr[i].Operator);
    for (i = 0; i < progLen; i++) {
        if (findOpTable(tPtr[i].Operator) != 0xff) {
            if (findOpTable(tPtr[i].Operator) == 0xfe) {
                if (strcmp(tPtr[i].Operator, "WORD") == 0) {
                    int num = atoi(tPtr[i].Operand);
                    memset(tPtr[i].ObjCode, num, 6);
                    //printf("ObjCode1 = %06s\n", tPtr[i].ObjCode);
                    sprintf(tPtr[i].ObjCode, "%06X", num);
                    //printf("num = %d\n", num);
                    //printf("ObjCode2 = %06s\n", tPtr[i].ObjCode);
                    //printf("tPtr[i].ObjCode = %d\n", tPtr[i].Operand);
                }
                else if (strcmp(tPtr[i].Operator, "BYTE") == 0) {
                    int size = strlen(tPtr[i].Operand);
                    //printf("size = %d\n", strlen(tPtr[i].Operand));
                    strcpy(tPtr[i].ObjCode, "");
                    if (tPtr[i].Operand[0] == 'c') {     // 字元
                        for (int index = 2; index < size - 1; index++) {
                            /*printf("%c ", tPtr[i].Operand[index]);
                            printf("%X\n", tPtr[i].Operand[index]);*/
                            char tmp[3];
                            sprintf(tmp, "%X", tPtr[i].Operand[index]);
                            strcat(tPtr[i].ObjCode, tmp);
                            //printf("%s", tPtr[i].ObjCode);
                            //printf("tmp = %s\n", tmp);
                        }
                    }
                    else {  // Hex
                        char tmp[10];
                        int tmp_len = 0;
                        for (int index = 2; index < size - 1; index++) {
                            //printf("%c ", tPtr[i].Operand[index]);
                            tmp[tmp_len++] = tPtr[i].Operand[index];
                            //printf("%s", tPtr[i].ObjCode);
                        }
                        tmp[tmp_len++] = '\0';
                        //printf("%s\n", tmp);
                        strcat(tPtr[i].ObjCode, tmp);
                    }
                    /*printf("ObjCode1 = %06s\n", tPtr[i].ObjCode);
                    sprintf(tPtr[i].ObjCode,"%d", tPtr[i].Operand[0]);
                    printf("ObjCode2 = %06s\n", tPtr[i].ObjCode);*/
                    //printf("{%s}\n", tPtr[i].ObjCode);
                }
                else {
                    strcpy(tPtr[i].ObjCode, "\0");
                }
            }
            else if (findOpTable(tPtr[i].Operator) == 0xfd) {
                strcpy(tPtr[i].ObjCode, "      ");
            }
            else {
                //printf("[%02X]\n", inOpTable(tPtr[i].Operator));
                // invert int to str
                sprintf(tPtr[i].ObjCode, "%02X%04X", findOpTable(tPtr[i].Operator), findSymTable(tPtr[i].Operand));
            }
        }
        else {
            sprintf(tPtr[i].ObjCode, "      ");
        }
    }
}
void displayOpTable(instruct* tPtr) {
    int i = 0;
    for (i = 0; i < progLen; i++) {
        printf("[%-6s]\n", tPtr[i].ObjCode);
    }
}
void pass_1(instruct* tPtr) {
    int LOCCTR = 0;
    int row = 0;
    for (row = 0; row < progLen; row++) {
        if (strcmp(tPtr[row].Operator, "START") == 0) {
            int Operand = atoi(tPtr[row].Operand);
            Operand = HexToDec(Operand);
            //Operand = 0x4096;
            LOCCTR = Operand;
            printf("START LOCCTR = %x\n", LOCCTR);
            tPtr[row].Loc = LOCCTR;
        }
        else {
            tPtr[row].Loc = LOCCTR;
            char Operator[20] = { "\0" };
            strncpy(Operator, tPtr[row].Operator, sizeof(tPtr[row].Operator));
            //printf("Operator = %s\n", Operator);
            if (strcmp(Operator, "WORD") == 0) {
                LOCCTR += 3;
            }
            else if (strcmp(Operator, "BYTE") == 0) {
                int len = strlen(tPtr[row].Operand) - 3; // 去掉X''或c''
                /*printf("\nlen = %d\n", len);
                printf("%s\n", tPtr[row].Operand);*/
                if (tPtr[row].Operand[0] == 'X') {
                    len /= 2; // 2 digit equal 1 byte
                }
                // and 1 char equal 1 byte
                LOCCTR += len;
            }
            else if (strcmp(Operator, "RESW") == 0) {
                int diff = atoi(tPtr[row].Operand);
                LOCCTR += (3 * diff);
            }
            else if (strcmp(Operator, "RESB") == 0) {
                int diff = atoi(tPtr[row].Operand);
                LOCCTR += diff;
            }
            else {
                LOCCTR += 3;
            }
        }
    }
    makeSymTable(tPtr);
}
void displayTable(instruct* tPtr) {
    int i = 0;
    printf("\nListFile:\n");
    for (i = 0; i < progLen; i++) {
        //printf("%4X  %-6s  %-6s  %-6s  %s\n", tPtr[i].Loc, tPtr[i].ObjCode, tPtr[i].Label, tPtr[i].Operator, tPtr[i].Operand);
        printf("%-6X%-8s%-8s%-8s%-9s\n", tPtr[i].Loc, tPtr[i].ObjCode, tPtr[i].Label, tPtr[i].Operator, tPtr[i].Operand);
    }
}
void objFile(instruct* tPtr) {
    int max_byte = 30;
    printf("tPtr[progLen].Operand = %s\n", tPtr[progLen - 1].Operand);
    printf("tPtr[0].Loc = %X\n", tPtr[0].Loc);
    int ProgLen_Loc = tPtr[progLen - 1].Loc - tPtr[0].Loc;
    printf("ProgLen_Loc = %X\n", ProgLen_Loc);
    int i = 0;
    printf("\nObjFile:\n");
    // H record
    printf("H %06s %06X\n", tPtr[i++].Operand, ProgLen_Loc);
    // T record
    printf("T ");
    int tRec_num = 0;
    int tRec_Len[10] = { 0 };
    int tRec_START[10] = { 0 };
    for (i = 1; i < progLen - 1; i++) {
        if (tRec_Len[tRec_num] < 0x1F) {
            if (strcmp(tPtr[i].Operator, "RESW") == 0 || strcmp(tPtr[i].Operator, "RESB") == 0) {
                continue;
            }
            tRec_Len[tRec_num]++;
        }
        tRec_num++;
    }
    for (i = 0; i < tRec_num; i++) {
        printf("tRec_Len[%d] = %d\n", i, tRec_Len[i]);
    }
    /*for (i=1; i < progLen-1; i++) {
        printf("%6s ", tPtr[i].ObjCode);
        if((i % 10)+1 == 1) {
            printf("\nT ");
        }
    }*/
    // E record
    printf("\nE %06X\n", findSymTable(tPtr[progLen - 1].Operand));
}
int main(void)
{
    printf("This is a SIC assembler\n");
    instruct table[100];
    instruct* t = table;
    fileInput(t);

    printf("Pass 1 Processing...\n");
    pass_1(t);
    //makeSymTable(t);
    makeObjCode(t);
    listFile(t);
    objFile(t);
    //displayOpTable(t);
    //displayTable(t);
    return 0;
}
/*
    Edited by D0845456
    2021/5/2:: 建好資料結構+讀檔並去掉註解
    2021/5/16:: Object Code Part I
    2021/5/17:: SIC Assembler completed
*/


// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
