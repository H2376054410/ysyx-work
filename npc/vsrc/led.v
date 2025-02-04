module led(
  input clk,
  input rst,
  input [4:0] btn,
  input [7:0] sw,
  output [15:0] ledr
);
  reg [31:0] count;
  reg [7:0] led;
  reg [7:0] r_led;
  always @(posedge clk) begin
    if (rst) begin led <= 1; count <= 0; end
    else begin
      if (count == 0) led <= {led[6:0], led[7]};
      count <= (count >= 5000000 ? 32'b0 : count + 1);
    end
  end
assign r_led[0]=sw[1]^sw[0];
assign r_led[1]=sw[1]^sw[0];
assign r_led[2]=sw[1]^sw[0];
assign r_led[3]=sw[1]^sw[0];
assign r_led[4]=sw[1]^sw[0];
assign r_led[5]=sw[1]^sw[0];
assign r_led[6]=sw[1]^sw[0];
assign r_led[7]=sw[1]^sw[0];


  assign ledr = {led[7:5], led[4:0] ^ btn, r_led};
endmodule
