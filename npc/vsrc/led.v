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
  //0123为信号输入端，45为信号选择端，输出信号为led[0]
  wire [1:0]choose;
  wire [3:0]data;
  assign choose=sw[5:4];
  assign data=sw[3:0];
  always @(*) begin
    led[0]=data[choose];
  end
  assign ledr = led;
endmodule
