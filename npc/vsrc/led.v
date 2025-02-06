module led(
  input clk,
  input rst,
  input [4:0] btn,
  input [7:0] sw,
  output [15:0] ledr
);
  reg [31:0] count;
  reg [15:0] led;
  reg [7:0] r_led;
  //8-3优先编码器
  //led[3:0]为信号输出端，012为信号输出,3为标志位，有1则亮，否则灭
  //首先保证高位的优先级要大于低位的，所以是i从大变小检测，然后因为得到的i就是位的值，所以只需将i输出即可
  wire [2:0]choose;
  wire [7:0]data;
  reg flag;
  integer i;
  assign data=sw[7:0];
  always @(data) begin
    choose[2:0]=3'b000;
    flag=0;
    for(i=7;i>-1;i=i-1)begin
      if(data[i]==1)begin
        choose[2:0]=i[2:0];
        flag=1;
        i=-1;
      end
    end
  r_led[3:0]={flag,choose[2:0]};    
  end
  assign ledr = {3'b111,btn[4:0],r_led};
  //assign ledr = {led[7:5], led[4:0] ^ btn, sw};
endmodule
