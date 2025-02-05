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
  //2-4译码器
  //45为信号选择端，0123为信号输出端，选择一位输出1
  wire [5:4]choose;
  wire [3:0]data;
  assign choose=sw[5:4];
  always @(*) begin
    case(sw[5:4])
    2'b00:begin
      data[0]=1;
      data[1]=0;
      data[2]=0;
      data[3]=0;
          end
    2'b01:begin
      data[0]=0;
      data[1]=1;
      data[2]=0;
      data[3]=0;
          end
    2'b10:begin
      data[0]=0;
      data[1]=0;
      data[2]=1;
      data[3]=0;
          end
    2'b11:begin
      data[0]=0;
      data[1]=0;
      data[2]=0;
      data[3]=1;
          end
  endcase
  led[3:0]=data[3:0];    
  end
  assign ledr = led;
endmodule
