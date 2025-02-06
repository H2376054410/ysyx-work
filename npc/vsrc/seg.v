module seg(
  input clk,
  input rst,
  input [15:0] data,
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
wire [7:0] segs [7:0];
assign segs[0] = 8'b11111101;//the first one,right
assign segs[1] = 8'b01100000;
assign segs[2] = 8'b11011010;
assign segs[3] = 8'b11110010;
assign segs[4] = 8'b01100110;
assign segs[5] = 8'b10110110;
assign segs[6] = 8'b10111110;
assign segs[7] = 8'b11100000;

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

assign o_seg0 = ~segs[{1'b0,1'b0,data[0]}];
assign o_seg1 = ~segs[{1'b0,1'b0,data[1]}];
assign o_seg2 = ~segs[{1'b0,1'b0,data[2]}];
assign o_seg3 = ~segs[{1'b0,1'b0,data[3]}];
assign o_seg4 = ~segs[{1'b0,1'b0,data[4]}];
assign o_seg5 = ~segs[{1'b0,1'b0,data[5]}];
assign o_seg6 = ~segs[{1'b0,1'b0,data[6]}];
assign o_seg7 = 8'b11111111;


endmodule
