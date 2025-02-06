module seg(
  input clk,
  input rst,
  input [15:0] data,
  input [4:0] btn,
  output [7:0] o_seg0,
  output [7:0] o_seg1,
  output [7:0] o_seg2,
  output [7:0] o_seg3,
  output [7:0] o_seg4,
  output [7:0] o_seg5,
  output [7:0] o_seg6,
  output [7:0] o_seg7
);
//data[3:0]is out
//data[6:4]is flag
reg [7:0]b_out;
initial begin
  b_out[7:0]=8'b10000000;
end
wire [7:0] segs [15:0];
assign segs[0] = 8'b11111101;//the first one,right，zero is noshine
assign segs[1] = 8'b01100000;
assign segs[2] = 8'b11011010;
assign segs[3] = 8'b11110010;
assign segs[4] = 8'b01100110;
assign segs[5] = 8'b10110110;
assign segs[6] = 8'b10111110;
assign segs[7] = 8'b11100000;
assign segs[8] = 8'b11111110;
assign segs[9] = 8'b11100110;
assign segs[10] = 8'b11101110;
assign segs[11] = 8'b00111110;
assign segs[12] = 8'b10011100;
assign segs[13] = 8'b01111010;
assign segs[14] = 8'b10011110;
assign segs[15] = 8'b10001110;
parameter CLK_NUM = 5000000;

reg [31:0] count;
reg [2:0] offset;
// always @(posedge clk) begin
//   if(rst) begin count <= 0; offset <= 0; end
//   else begin
//     if(count == CLK_NUM) begin offset <= offset + 1; end
//     count <= (count == CLK_NUM) ? 0 : count + 1;
//   end
// end
//伪随机数生成部分
always @(btn) begin
  if(btn==0)
  b_out={(b_out[0]^b_out[4]^b_out[3]^b_out[2]),b_out[7:1]};
end
assign o_seg0 = 8'b11111111;
assign o_seg1 = ~segs[b_out[3:0]];
assign o_seg2 = ~segs[b_out[7:4]];
assign o_seg3 = 8'b11111111;
assign o_seg4 = 8'b11111111;
assign o_seg5 = 8'b11111111;
assign o_seg6 = 8'b11111111;
assign o_seg7 = 8'b11111111;


endmodule
