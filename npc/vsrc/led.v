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
  // always @(posedge clk) begin
  //   if (rst) begin led <= 1; count <= 0; end
  //   else begin
  //     if (count == 0) led <= {led[14:0], led[15]};
  //     count <= (count >= 5000000 ? 32'b0 : count + 1);
  //   end
  // end
  //4-1优先编码器
  //led[1:0]为信号输出端，0123为信号输入,
  wire [1:0]choose;
  wire [3:0]data;
  integer i;
  assign data=sw[3:0];
  always @(data) begin
    choose[1:0]=2'b00;
    for(i=0;i<4;i=i+1)begin
      if(data[i]==1)begin
        choose[1:0]=i[1:0];
        i=4;
      end
    end
  r_led[1:0]=choose[1:0];    
  end
  assign ledr = {8'b11111111,r_led};
endmodule
