module led(
  input clk,
  input rst,
  input [4:0] btn,
  input [7:0] sw,
  output [15:0] ledr
);
  reg [3:0] fsm_out; 
  reg  r_led;

always @(btn[0]) begin
  if(btn[0]==0)begin
    fsm_out={fsm_out[2:0],sw[1]};   
  end
end
  assign r_led=(fsm_out==4'b1111)?1:0;
  assign ledr = {8'b0000000,fsm_out[3:0],3'b000,r_led};
  //assign ledr = {led[7:5], led[4:0] ^ btn, sw};
endmodule
