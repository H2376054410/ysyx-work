module top(
    input clk,
    input rst,
    input [4:0] btn,
    input [7:0] sw,
    input ps2_clk,
    input ps2_data,
    input uart_rx,
    output uart_tx,
    output [15:0] ledr,
    output VGA_CLK,
    output VGA_HSYNC,
    output VGA_VSYNC,
    output VGA_BLANK_N,
    output [7:0] VGA_R,
    output [7:0] VGA_G,
    output [7:0] VGA_B,
    output [7:0] seg0,
    output [7:0] seg1,
    output [7:0] seg2,
    output [7:0] seg3,
    output [7:0] seg4,
    output [7:0] seg5,
    output [7:0] seg6,
    output [7:0] seg7
);
reg putdown;
wire [15:0]data;
wire [3:0]alu_out;
wire [2:0]alu_flag;
reg [7:0] ps2_out,ps2_scanout; 
assign data[7:0]=ps2_scanout;
assign data[15:8]=ps2_out;
led my_led(
    .clk(clk),
    .rst(rst),
    .btn(btn),
    .sw(sw),
    .ledr(ledr)
);

assign VGA_CLK = clk;

wire [9:0] h_addr;
wire [9:0] v_addr;
wire [23:0] vga_data;

vga_ctrl my_vga_ctrl(
    .pclk(clk),
    .reset(rst),
    .vga_data(vga_data),
    .h_addr(h_addr),
    .v_addr(v_addr),
    .hsync(VGA_HSYNC),
    .vsync(VGA_VSYNC),
    .valid(VGA_BLANK_N),
    .vga_r(VGA_R),
    .vga_g(VGA_G),
    .vga_b(VGA_B)
);

ps2_keyboard my_keyboard(
    .clk(clk),
    .resetn(~rst),
    .ps2_clk(ps2_clk),
    .ps2_data(ps2_data),
    .ps2_out(ps2_out),
    .ps2_scanout(ps2_scanout),
    .putdown(putdown)
);
// always @(data) begin
//     $display("high %x low %x ", data[15:8],data[7:0]);
// end
seg my_seg(
    .clk(clk),
    .rst(rst),
    .data(data),
    .btn(putdown),
    .o_seg0(seg0),
    .o_seg1(seg1),
    .o_seg2(seg2),
    .o_seg3(seg3),
    .o_seg4(seg4),
    .o_seg5(seg5),
    .o_seg6(seg6),
    .o_seg7(seg7)
);

vmem my_vmem(
    .h_addr(h_addr),
    .v_addr(v_addr[8:0]),
    .vga_data(vga_data)
);

uart my_uart(
  .tx(uart_tx),
  .rx(uart_rx)
);
ALU my_alu(
    .A(sw[7:4]),
    .B(sw[3:0]),
    .rst(btn[4]),
    .op(btn[2:0]),
    .out(alu_out[3:0]),
    .zero(alu_flag[2]),
    .overflow(alu_flag[1]),
    .carry(alu_flag[0])
    
);
endmodule
module ALU(
    input [3:0] A, B,       // 4-bit signed numbers (2's complement)
    input rst,
    input [2:0] op,         // 3-bit operation selector
    output reg [3:0] out,   // 4-bit ALU result
    output reg zero,        // Zero flag
    output reg overflow,    // Overflow flag
    output reg carry        // Carry flag
);
reg [2:0]option;
reg reset;
    always @(*) begin
        if(rst==1)
        begin
            option=3'b000;
        end
        else begin
         case (op)
            3'b001:option[0]=1;
            3'b010:option[1]=1;
            3'b100:option[2]=1;
            default: ;
        endcase
    end
    always @(posedge rst) begin
        option=3'b000;
    end
    wire [4:0] extended_A;
    wire [4:0] extended_B;

    assign extended_A = {1'b0, A}; // Extend A to 8 bits
    assign extended_B = {1'b0, B}; // Extend B to 8 bits
    always @(*) begin
        case (option)
            3'b000: begin // 加法 A + B
                {carry, out} = extended_A + extended_B;  // 计算加法
                overflow = (A[3] == B[3] && A[3] != out[3]);  // 溢出检测
            end
            3'b001: begin // 减法 A - B (A + (-B))
                {carry, out} = extended_A + ~extended_B + 1;  // 计算A - B
                overflow = (A[3] != B[3] && A[3] != out[3]);  // 溢出检测
            end
            3'b010: begin // 取反 ~A
                out = ~A;  // 按位取反
                overflow = 0;  // 取反不会产生溢出
                carry = 0;  // 取反操作没有进位
            end
            3'b011: begin // 与 A & B
                out = A & B;  // 按位与
                overflow = 0;  // 与操作不会溢出
                carry = 0;  // 与操作没有进位
            end
            3'b100: begin // 或 A | B
                out = A | B;  // 按位或
                overflow = 0;  // 或操作不会溢出
                carry = 0;  // 或操作没有进位
            end
            3'b101: begin // 异或 A ^ B
                out = A ^ B;  // 按位异或
                overflow = 0;  // 异或操作不会溢出
                carry = 0;  // 异或操作没有进位
            end
            3'b110: begin // 比较大小 A < B
                {carry, out} = extended_A + (~extended_B + 1);  // A - B 结果
                overflow = (A[3] != B[3] && A[3] != out[3]);  // 溢出判断
                if (out[3] == 1)  // 如果符号位为1，表示A < B
                    out = 4'b0001;
                else
                    out = 4'b0000;
            end
            3'b111: begin // 判断相等 A == B
                {carry, out} = extended_A + (~extended_B + 1);  // A - B
                if (out == 4'b0000)  // 如果A == B，输出1
                    out = 4'b0001;
                else
                    out = 4'b0000;
                overflow = 0;  // 相等判断没有溢出
                carry = 0;  // 相等判断没有进位
            end
            default: begin
                out = 4'b0000;
                overflow = 0;
                carry = 0;
            end
        endcase
        // Zero flag 判断是否为零
        zero = (out == 4'b0000);
    end
endmodule

module vmem(
    input [9:0] h_addr,
    input [8:0] v_addr,
    output [23:0] vga_data
);

reg [23:0] vga_mem [524287:0];

initial begin
    $readmemh("resource/picture.hex", vga_mem);
end

assign vga_data = vga_mem[{h_addr, v_addr}];

endmodule
