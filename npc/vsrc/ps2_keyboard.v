module ps2_keyboard(clk,resetn,ps2_clk,ps2_data,putdown,ps2_out,ps2_scanout);
    input clk,resetn,ps2_clk,ps2_data;
    output reg[7:0] ps2_out,ps2_scanout; 
    output reg putdown;
    reg [7:0]ps2_lastscanout;
    reg [7:0] reg_ps2out,reg_ps2scanout,regps2_lastscanout;
    reg [9:0] buffer;        // ps2_data bits
    reg [3:0] count;  // count ps2_data bits
    reg [2:0] ps2_clk_sync;
    reg [7:0] lastlastscanout;
    reg startconvert;
    initial begin
      reg_ps2out=8'b0;
      reg_ps2scanout=8'b0;

    end

    always @(posedge clk) begin
        ps2_clk_sync <=  {ps2_clk_sync[1:0],ps2_clk};
    end

    wire sampling = ps2_clk_sync[2] & ~ps2_clk_sync[1];

    always @(posedge clk) begin
        if (resetn == 0) begin // reset
            count <= 0;
        end
        else begin
            if (sampling) begin
              if (count == 4'd10) begin
                if ((buffer[0] == 0) &&  // start bit
                    (ps2_data)       &&  // stop bit
                    (^buffer[9:1])) begin      // odd  parity
                    lastlastscanout=regps2_lastscanout;
                    regps2_lastscanout=reg_ps2scanout;
                    reg_ps2scanout=buffer[8:1];
                    ps2_out=reg_ps2out;//dont change
                    ps2_scanout=reg_ps2scanout;
                    ps2_lastscanout=regps2_lastscanout;
                    startconvert=1;
                    if (lastlastscanout==8'hf0&&regps2_lastscanout==reg_ps2scanout) begin
                    putdown=1;                        
                    end
                    else begin
                        putdown=0;
                    end

                    // $display("ps2_scanout %x", ps2_scanout[7:0]);
                    // $display("ps2_lastscanout %x", ps2_lastscanout[7:0]);
                    // $display("reg_ps2scanout %x", reg_ps2scanout[7:0]);
                    // $display("regps2_lastscanout %x", regps2_lastscanout[7:0]);
                end
                count <= 0;     // for next
              end else begin
                buffer[count] <= ps2_data;  // store ps2_data
                count <= count + 3'b1;
                putdown = 0;
              end
            end
        end
    end
always @(count) begin
  if(count==4'd10||startconvert==1)
  begin
   case (buffer[8:1])
        8'h1C: reg_ps2out = 8'h61; // 'a' -> 'A'
        8'h1B: reg_ps2out = 8'h73; // 's'
        8'h23: reg_ps2out = 8'h64; // 'd'
        8'h2B: reg_ps2out = 8'h66; // 'f'
        8'h24: reg_ps2out = 8'h65;  //'e
        8'h43: reg_ps2out = 8'h69;  //'i'
        8'h44: reg_ps2out = 8'h6F;  //'o'
        8'h4D: reg_ps2out = 8'h70;  //'p'
        8'h15: reg_ps2out = 8'h71;  //'q'
        8'h2D: reg_ps2out = 8'h72;  //'r'
        8'h2C: reg_ps2out = 8'h74;  //'t'
        8'h3C: reg_ps2out = 8'h75;  //'u'
        8'h1D: reg_ps2out = 8'h77;  //'w'
        8'h35: reg_ps2out = 8'h79;  //'y'
        8'h34: reg_ps2out = 8'h66; // 'g'
        8'h33: reg_ps2out = 8'h68; // 'h'
        8'h3B: reg_ps2out = 8'h6A; // 'j'
        8'h42: reg_ps2out = 8'h6B; // 'k'
        8'h4B: reg_ps2out = 8'h6C; // 'l'
        8'h1A: reg_ps2out = 8'h7A; // 'z'
        8'h22: reg_ps2out = 8'h78; // 'x'
        8'h21: reg_ps2out = 8'h63; // 'c'
        8'h2A: reg_ps2out = 8'h76; // 'v'
        8'h32: reg_ps2out = 8'h62; // 'b'
        8'h31: reg_ps2out = 8'h6E; // 'n'
        8'h3A: reg_ps2out = 8'h6D; // 'm'
        // 数字键
        8'h69: reg_ps2out = 8'h31; // '1'
        8'h72: reg_ps2out = 8'h32; // '2'
        8'h7A: reg_ps2out = 8'h33; // '3'
        8'h6B: reg_ps2out = 8'h34; // '4'
        8'h73: reg_ps2out = 8'h35; // 
        8'h74: reg_ps2out = 8'h36;
        8'h6C: reg_ps2out = 8'h37;
        8'h75: reg_ps2out = 8'h38;
        8'h7D: reg_ps2out = 8'h39;
        8'h70: reg_ps2out = 8'h30;
        //其他键
        8'h29: reg_ps2out = 8'h20;  // Space
        8'h66: reg_ps2out = 8'h08;  // Backspace
        8'h5A: reg_ps2out = 8'h0D;  // Enter
        8'hF0: reg_ps2out = 8'h88;

        default: reg_ps2out = 8'h00; // 默认返回0表示未定义或错误
    endcase

        startconvert=0;
    end
end
endmodule
