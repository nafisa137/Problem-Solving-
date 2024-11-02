#include "vole.h"

int main()
{
    Machine machine(256);
    machine.loadProgram("ins.txt");
    machine.executeProgram();
    machine.displayStatus();

    return 0;
}
