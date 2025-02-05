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
  //4-2编码器
  //led[1:0]为信号输出端，0123为信号输入,
  wire [1:0]choose;
  wire [3:0]data;
  assign data=sw[3:0];
  always @(*) begin
    case(data)
    4'b0001:begin
      choose[0]=0;
      choose[1]=0;
          end
    4'b0010:begin
      choose[0]=1;
      choose[1]=0;
          end
    4'b0100:begin
      choose[0]=0;
      choose[1]=1;
          end
    4'b1000:begin
      choose[0]=1;
      choose[1]=1;
          end
    default:begin
      choose[0]=0;
      choose[1]=0;      
    end

  endcase
  r_led[1:0]=choose[1:0];    
  end
  assign ledr = {8'b11111111,r_led};
endmodule
