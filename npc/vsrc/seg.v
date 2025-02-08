module seg(
  input clk,
  input rst,
  input [15:0] data,
  input  btn,
  output [7:0] o_seg0,
  output [7:0] o_seg1,
  output [7:0] o_seg2,
  output [7:0] o_seg3,
  output [7:0] o_seg4,
  output [7:0] o_seg5,
  output [7:0] o_seg6,
  output [7:0] o_seg7
);

wire [7:0] segs [15:0];
reg [7:0] oo_segs[7:0];
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
reg lastis;
initial begin
lastis=0;
end
reg [7:0]count;
always @(data[7:0] or btn) begin
  if(count<8'hff)
  count=count+1;
  else
  begin
   count=0;   
  end
  $display("count is %d data is %x",count[7:0],data[7:0]);


if(data[7:0]!=8'hF0)begin

  if(lastis==1)begin
      $display("last is f0");
      oo_segs[0]=8'b11111111;
      oo_segs[1]=8'b11111111;
      oo_segs[2]=8'b11111111;
      oo_segs[3]=8'b11111111;
      oo_segs[4]=8'b11111111;
      oo_segs[5]=~segs[count[3:0]];
      oo_segs[6]=~segs[count[7:4]];  
      oo_segs[7]= 8'b11111111;
      lastis=0;     
  end
      else begin

        oo_segs[0]=~segs[data[3:0]];
        oo_segs[1]=~segs[data[7:4]];
        oo_segs[2]=~segs[data[11:8]];
        oo_segs[3]=~segs[data[15:12]];
        oo_segs[4]=8'b11111111;
        oo_segs[5]=~segs[count[3:0]];
        oo_segs[6]=~segs[count[7:4]]; 
        oo_segs[7]= 8'b11111111;       
    end
end 
else begin
         $display("this is f0");
  oo_segs[0]=8'b11111111;
  oo_segs[1]=8'b11111111;
  oo_segs[2]=8'b11111111;
  oo_segs[3]=8'b11111111;
  oo_segs[4]=8'b11111111;
  oo_segs[5]=~segs[count[3:0]];
  oo_segs[6]=~segs[count[7:4]];  
  oo_segs[7]= 8'b11111111;
  lastis=1;
end
end
assign o_seg0 = oo_segs[0];
assign o_seg1 = oo_segs[1];
assign o_seg2 = oo_segs[2];
assign o_seg3 = oo_segs[3];
assign o_seg4 = 8'b11111111;
assign o_seg5 = oo_segs[5];
assign o_seg6 = oo_segs[6];
assign o_seg7 = 8'b11111111;


endmodule
