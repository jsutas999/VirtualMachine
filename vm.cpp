#include <iostream>
#include <fstream>
using namespace std;

class VM
{
public:
    VM(char* fileName,char* readFileName);
    ~VM();
    void LoadProgramMemory(char* fileName,char* readFileName);

    private:

    void Run();
    char* prog_mem;
    unsigned char* regs;
    int ieof_flag;
    char* _p;
    ifstream readFile;

};

VM::VM(char* fileName,char* readFileName)
{
    prog_mem = new char[255];
    regs = new unsigned char[16];
    LoadProgramMemory(fileName,readFileName);
    ieof_flag = 0;
}
VM::~VM()
{
    delete[] prog_mem;
    delete[] regs;
    readFile.close();
}

void VM::LoadProgramMemory(char* fileName,char* readFileName)
{
    ifstream in(fileName, ios::binary | ios::in);
    readFile.open(readFileName,ios::binary | ios::in);

    _p = &prog_mem[0];

    char t;
    while(in.get(t))
        *_p++ = t;

    _p = &prog_mem[0];
    Run();
}

void VM::Run()
{
    while(_p != &prog_mem[255])
    {

        switch(*_p)
        {
            case 1: {// INC Reg
                regs[*_p++]++;break;}
            case 2:{ // Dec Reg
                    regs[*_p++]--;break;}
            case 3:{ // Mov r1 r2
                _p++;
                regs[*_p & 0x0F] = regs[ (( *_p & 0xF0 ) >> 4) ];break;}
            case 4:{ // MOVC byte const
                    _p++;
                    regs[0] = *_p;break;}
            case 5:{ //LSL Reg
                    _p++;
                    regs[*_p]= regs[*_p] << 1;break;}
            case 6: //LSR Reg
                {
                    _p++;
                    regs[*_p]= regs[*_p] >> 1;
                    break;
                }
            case 7: // JMp addr
                {

                 _p++;
                 if(*_p <= 127)
                    _p += *_p-2;
                 else _p += ((*_p) - 256);
                 break;
                }
            case 8: // JZ addr ZF = 1
                {
                 _p++;
                 _p += *_p;
                 break;
                }
            case 9: //JNZ addr ZF = 0
                {
                    _p++;
                    _p += *_p;
                    break;
                }
            case 10:
                {
                    _p++;
                    if(ieof_flag == 1)
                        _p+=(*_p-1);
                    break;
                }
            case 11: // RET
                {
                    return;
                }
            case 12: // ADD reg1,reg2
                {
                    _p++;
                    regs[*_p & 0x0F] += regs[ (( *_p & 0xF0 ) >> 4) ];
                    break;

                }
            case 13: // SUB reg1, reg2
                {
                    _p++;
                    regs[*_p & 0x0F] -= regs[ (( *_p & 0xF0 ) >> 4) ];
                    break;
                }
            case 14: // XOR r1,r2
                {
                    _p++;
                    regs[*_p & 0x0F] = regs[*_p & 0x0F] ^ regs[ (( *_p & 0xF0 ) >> 4) ];
                    break;
                }
            case 15: // OR r1,r2
                {
                    _p++;
                    regs[*_p & 0x0F] = regs[*_p & 0x0F] | regs[ (( *_p & 0xF0 ) >> 4) ];
                    break;
                }
            case 16: // IN r1
                {
                    _p++;
                    char t = '!';
                    if(ieof_flag == 1)
                        break;

                    if(readFile.get(t))
                        {
                            regs[*_p] = t;
                            ieof_flag = 0;
                        } else ieof_flag = 1;

                    break;
                }
            case 17: // OUT reg1
                {
                    _p++;
                    cout << (char)regs[*_p];
                    break;
                }
        }
        _p++;
    }
    readFile.close();
}


int main()
{
    VM vm("decryptor.bin","q1_encr.txt");
}
