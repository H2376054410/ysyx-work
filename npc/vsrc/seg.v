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
wire [3:0]ddata;
assign ddata=data[3:0];
reg [3:0]ddata_reg;
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
reg [7:0] oo_seg6;
// always @(posedge clk) begin
//   if(rst) begin count <= 0; offset <= 0; end
//   else begin
//     if(count == CLK_NUM) begin offset <= offset + 1; end
//     count <= (count == CLK_NUM) ? 0 : count + 1;
//   end
// end
always @(ddata) begin
  ddata_reg=ddata;
  // if(ddata[3]==1)begin
  //   o_seg7 = ~segs[3'd1];
  // end
  // else begin
  //   o_seg7 = ~segs[3'd0];    
  // end
  oo_seg6=~segs[ddata_reg[2:0]];
end
// assign o_seg0 = ~segs[3'd0];
// assign o_seg1 = ~segs[3'd1];
// assign o_seg2 = ~segs[3'd2];
// assign o_seg3 = ~segs[3'd3];
// assign o_seg4 = ~segs[3'd4];
// assign o_seg5 = ~segs[3'd5];
// assign o_seg6 = ~segs[3'd6];
// assign o_seg7 = ~segs[3'd7];

assign o_seg0 = 8'b11111111;
assign o_seg1 = 8'b11111111;
assign o_seg2 = 8'b11111111;
assign o_seg3 = 8'b11111111;
assign o_seg4 = 8'b11111111;
assign o_seg5 = 8'b11111111;
assign o_seg6 = oo_seg6;
assign o_seg7 = 8'b11111111;

endmodule
