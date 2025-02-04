#include <verilated.h>
#include <Vtop.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <verilated_vcd_c.h>
static TOP_NAME dut;

void nvboard_bind_all_pins(TOP_NAME* top);

static void single_cycle() {
  dut.clk = 0; dut.eval();
  dut.clk = 1; dut.eval();
}

static void reset(int n) {
  dut.rst = 1;
  while (n -- > 0) single_cycle();
  dut.rst = 0;
}
int main(int argc, char** argv) {
	nvboard_bind_pin( &dut->sw, 8, SW7, SW6, SW5, SW4, SW3, SW2, SW1, SW0);
  nvboard_bind_pin( &top->ledr, 16, LD15, LD14, LD13, LD12, LD11, LD10, LD9, LD8, LD7, LD6, LD5, LD4, LD3, LD2, LD1, LD0);
  nvboard_init();

  reset(10);

    while (true) {
    nvboard_update();
    single_cycle();
    }

    return 0;
}

