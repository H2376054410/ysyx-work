#include <verilated.h>
#include <Vtop.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <verilated_vcd_c.h>
int main(int argc, char** argv) {
	Verilated::commandArgs(argc,argv);
    VerilatedContext* contextp = new VerilatedContext;
    //contextp->commandArgs(argc, argv);
    Vtop* top = new Vtop{contextp};
    VerilatedVcdC* tfp = new VerilatedVcdC;
    contextp->traceEverOn(true);
    top->trace(tfp,99);
    tfp->open("wave.vcd");
    int i=10;
    while (i--) {
      int a = rand() & 1;
      int b = rand() & 1;
      top->a = a;
      top->b = b;
      top->eval();
      printf("a = %d, b = %d, f = %d\n", a, b, top->f);
      assert(top->f == (a ^ b));
      tfp->dump(contextp->time());
      contextp->timeInc(1);
    }
    delete top;
    delete contextp;
    tfp->close();
    return 0;
}

