#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

class Register
{
public:
    int value;
    Register() : value(0) {}
};

class Memory
{
public:
    vector<int> data;
    Memory(int size) : data(size, 0) {}
};

class Instruction
{
public:
    int opcode;
    int operand1;
    int operand2;

};

class Machine
{
public:
    Register registers[16];
    Memory memory;
    int programCounter;

    Machine(int memSize) : memory(memSize), programCounter(0)
    {
        for (int i = 0; i < 16; i++)
        {
            registers[i].value = 0;
        }
    }

    void loadProgram(const string& filename)
    {
        ifstream file(filename);
        string line;

        while (getline(file, line))
        {
            cout << "Instruction : " << line << endl;
            istringstream iss(line);
            int opcode, operand1, operand2;
            if (!(iss >> hex >> opcode >> operand1 >> operand2))
            {
                cerr << "Error: Invalid instruction format in the program file." << endl;
                exit(1);
            }


            Instruction instruction;
            instruction.opcode = opcode;
            instruction.operand1 = operand1;
            instruction.operand2 = operand2;
            program.push_back(instruction);
        }

        file.close();
    }

    void executeProgram()
    {
        while (programCounter < program.size())
        {
            Instruction& instruction = program[programCounter];
            int opcode = instruction.opcode;
            int operand1 = instruction.operand1;
            int operand2 = instruction.operand2;


            // Implement instruction execution logic
            switch (opcode)
            {
            case 1:
                loadRegister(operand1, operand2);
                break;
            case 2:
                loadValue(operand1,operand2);
                break;
            case 3:
                if (operand1 == 0 && operand2 == 0)
                {
                    storeScreen(operand1);  // Special case for screen
                }
                else
                {
                    storeRegister(operand1, operand2);
                }
                break;
            case 4:
                moveRegister(operand1, operand2);
                break;
            case 5:
                addRegisters(operand1, operand2);
                break;
            case 6:
                addFloatRegisters(operand1, operand2);
                break;
            case 11:
                jumpIfEqual(operand1, operand2);
                break;
            case 12:
                haltExecution();
                break;
            default:
                cerr << "Error: Invalid opcode " << opcode << endl;
                exit(1);
            }

            programCounter++;
        }
    }

    void displayStatus()
    {
        cout << "Register Status:" << endl;
        for (int i = 0; i < 16; i++)
        {
            cout << "R" << hex << i << ": " << setfill('0') << setw(2) << hex << uppercase << registers[i].value << dec << endl;
        }

        cout << "Program Counter: " << setfill('0') << setw(2) << programCounter << dec << endl;

        cout << "Memory Status:" << endl;
        for (int i = 0; i < memory.data.size(); i++)
        {
            cout << "Memory[" << setfill('0') << setw(2) << hex << i << "]: " << setfill('0') << setw(2) << hex << uppercase << memory.data[i] << dec << endl;
        }
    }


private:
    vector<Instruction> program;

    void loadRegister(int operand1, int operand2)
    {
        int r = operand1 & 0xF;
        int address = operand2 & 0xFF;
        registers[r].value = memory.data[address];
    }


    void loadValue(int operand1, int operand2)
    {
        int r = operand1 & 0xFF;

        int value = operand2 & 0xFF;

        registers[r].value = value;

    }
    void storeRegister(int operand1, int operand2)
    {
        int r = operand1 & 0xFF;
        int address = operand2 & 0xFF;
        memory.data[address] = registers[r].value;
    }

    void storeScreen(int operand1)
    {

        cout << "Screen: " << hex << uppercase << registers[0].value << dec << endl;
    }

    void moveRegister(int operand1, int operand2)
    {
        int  r = operand1 & 0xF; ;
        int s = (operand2 >> 4) & 0x7;
        registers[r].value = registers[s].value;
    }

    void addRegisters(int operand1, int operand2)
    {
        // Extract destination register (r), and two source registers (s and t)
        int r = operand1 & 0x7;  // Bits 0-2 for destination register
        int s = (operand2 >> 4) & 0xF;  // Bits 4-7 for source register s
        int t = operand2 & 0xF;  // Bits 0-3 for source register t


        registers[r].value = registers[s].value + registers[t].value;


        cout << "Adding values in registers " << s << " and " << t << " and storing the sum in register " << r << "." << endl;
        cout << "Value in register " << r << ": " << hex << uppercase << registers[r].value << dec << endl;
    }

    void addFloatRegisters(int operand1, int operand2)
    {
        int R = operand1 & 0x7;
        int S = (operand2 >> 4) & 0xF;
        int T = operand2 & 0xF;


        float result = (float)(registers[S].value) + (float)(registers[T].value);

        // Store the result back in the destination register (R)
        registers[R].value = (float)(result);

        // Optional: Print the values for verification
        cout << "Adding floating-point values in registers " << S << " and " << T << " and storing the result in register " << R << "." << endl;
        cout << "Value in register " << R << ": " << hex << uppercase << registers[R].value << dec << endl;
    }

    void jumpIfEqual(int operand1, int operand2)
    {
        int  r = operand1 & 0xF; ;
        int address = operand2 & 0xFF;
        if (registers[r].value == registers[0].value)
        {
            programCounter = address;
        }
    }

    void haltExecution()
    {
        programCounter = program.size();
    }
};
