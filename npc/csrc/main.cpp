#include <iostream>
#include "Vcpu.h"
#include <verilated.h>
#include <verilated_vcd_c.h>
int main(int argc,char** argv)
{
    Verilated::commandArgs(argc, argv);  // 解析命令行参数
    VerilatedContext* contextp = new VerilatedContext;
    Vcpu* cpu = new Vcpu{contextp};  // 实例化模块

    // 初始化波形跟踪（可选）
    VerilatedVcdC* tfp = new VerilatedVcdC;
    Verilated::traceEverOn(true);
    cpu->trace(tfp, 99);  // 设置跟踪层级
    tfp->open("cpuform.vcd");
    // 初始化信号
    cpu->rst = 1;
    cpu->clk = 0;

    // 复位操作
    for (int i = 0; i < 2; ++i) {
        cpu->clk = !cpu->clk;
        cpu->eval();
    }
    cpu->rst = 0;

    // 主仿真循环
    for (int cycle = 0; cycle < 10; ++cycle) {
        // 时钟驱动
        cpu->clk = !cpu->clk;
        cpu->eval();
        tfp->dump(contextp->time());
        contextp->timeInc(1);
    }

    // 清理资源
    tfp->close();
    delete cpu;
    return 0;
}