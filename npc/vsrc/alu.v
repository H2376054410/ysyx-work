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