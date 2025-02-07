module led(
  input clk,
  input rst,
  input [4:0] btn,
  input [7:0] sw,
  output [15:0] ledr
);
reg [3:0]state,next_state;
reg out_z;
    // 状态定义
    localparam A = 4'b0000,
               B = 4'b0001,
               C = 4'b0010,
               D = 4'b0011,
               E = 4'b0100,
               F = 4'b0101,
               G = 4'b0110,
               H = 4'b0111,
               I = 4'b1000;
  //组合逻辑状态转移规则
  always @(*) begin
    case (state)
        A:next_state=(sw[1])?F:B; 
        B:next_state= (sw[1])?F:C;
        C:next_state= (sw[1])?F:D;
        D:next_state= (sw[1])?F:E;
        E:next_state= (sw[1])?F:E;
        F:next_state= (sw[1])?G:B;
        G:next_state= (sw[1])?H:B;
        H:next_state= (sw[1])?I:B;
        I:next_state= (sw[1])?I:B;
      default: next_state=A;
    endcase
  end
  always @(negedge btn[0] or posedge sw[0]) begin
    if(sw[0]==1)
    begin
      state<=A;
    end
    else 
    begin
      state<=next_state;
    end
  end

  always @(*) begin
    if(state==4'b1111)
    out_z=1;
    else
    out_z=0;
  end
  assign ledr = {8'b0000000,state[3:0],3'b000,out_z};
  //assign ledr = {led[7:5], led[4:0] ^ btn, sw};
endmodule
