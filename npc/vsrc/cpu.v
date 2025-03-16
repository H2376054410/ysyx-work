module ysyx_25020051_RegisterFile #(ADDR_WIDTH = 5, DATA_WIDTH = 32) (
  input clk,
  input [DATA_WIDTH-1:0] wdata,
  input [ADDR_WIDTH-1:0] waddr,
  input wen,

  input [ADDR_WIDTH-1:0] raddr1, // 读取地址1
  output [DATA_WIDTH-1:0] rdata1// 读取数据1
);
  //定义寄存器数组
  reg [DATA_WIDTH-1:0] rf [2**ADDR_WIDTH-1:0];
  //异步读入数据
  assign rdata1 = (raddr1 != 0) ? rf[raddr1] : 0;
  //同步写入数据
  always @(posedge clk) begin
    if (wen && waddr !=0) rf[waddr] <= wdata;
    $display("the value of x1 is %d",rf[1]);
  end
    // 初始化寄存器（可选，用于仿真）
  initial begin
    rf[0] = 0;  // 强制$0寄存器为0
  end
endmodule
module ysyx_25020051_InstructionMemory (
  input  [31:0] addr,
  output [31:0] data
);
  // 假设存储4条指令（实际需扩展）
  reg [31:0] mem [0:3];
  
  initial begin
    // 示例指令：addi x1, x0, 5 （x0恒为0）
    mem[0] = 32'h00500093;  // addi x1, x0, 5
    mem[1] = 32'h00000013;  // nop（addi x0, x0, 0）
  end

  assign data = mem[addr[3:2]]; // 32位地址按字（4字节）寻址
endmodule
module ysyx_25020051_IFU #(DATA_WIDTH = 32)(
  input clk,
  input rst,
  input reg [DATA_WIDTH-1:0] pc_now,
  output reg [DATA_WIDTH-1:0] pc_next
);
always @(posedge clk) begin
  if(rst==1)
  begin
    pc_next<=32'h80000000;
  end
  else
  begin
    pc_next<=pc_now+32'h4;
  end
end
endmodule
module ysyx_25020051_IDU #(DATA_WIDTH = 32)(
  input clk,
  input [DATA_WIDTH-1:0] inst,
  output [4:0] rd,
  output [4:0] rs1,
  output [4:0] rs2,
  output [31:0] imm,
  output inst_type
);
  assign rd    = inst[11:7];   // RISC-V格式：addi rd, rs1, imm
  assign rs1   = inst[19:15];
  assign rs2   = inst[24:20];
  assign imm   = {{20{inst[31]}}, inst[31:20]}; // 符号扩展
  assign inst_type = (inst[6:0] == 7'b0010011); // opcode判断 
  always @(posedge clk) begin
      $display("the value of inst is %h",inst);
  end   
endmodule //ysyx_25020051_IDU

module cpu(
  input clk,
  input rst
);
  //取PC
  reg [31:0] pc=32'h80000000;      // 32位PC
  ysyx_25020051_IFU IFU(
    .clk(clk),
    .rst(rst),
    .pc_now(pc),
    .pc_next(pc)
  );
  //取指
  wire [31:0] instruction;
  ysyx_25020051_InstructionMemory imem (
    .addr(pc),
    .data(instruction)
  );
  //译码
  wire [4:0]  rd;     // 目标寄存器地址
  wire [4:0]  rs1;    // 源寄存器地址
  wire [4:0]  rs2;    // 源寄存器地址
  wire [31:0] imm;    // 立即数
  wire        inst_type;// 是否为addi指令 
  ysyx_25020051_IDU IDU(
    .clk(clk),
    .inst(instruction),
    .rd(rd),
    .rs1(rs1),
    .rs2(rs2),
    .imm(imm),
    .inst_type(inst_type)
  );
  //寄存器操作
  wire [31:0] rs1_value;
  wire [31:0] alu_result;
  ysyx_25020051_RegisterFile #(
    .ADDR_WIDTH(5),
    .DATA_WIDTH(32)
  )regdata(
    .clk(clk),
    .wdata(alu_result),
    .waddr(rd),
    .wen(inst_type),
    .raddr1(rs1),
    .rdata1(rs1_value)
  );
  //alu部分
  assign alu_result=rs1_value+imm;
endmodule //cpu
