module imm_gen (input_32, output_64);
	input [31:0] input_32;
	output reg [63:0] output_64;
	
	always @ (input_32)
	 begin
	  case (input_32[6:0])
	   7'b0000011 :
		begin 
		  output_64[11:0] <= input_32[31:20];
		end
	   
		7'b0100011 :
		begin
		  output_64[4:0] <= input_32[11:7];
		  output_64[11:5] <= input_32[31:25];
		end
		
		7'b0010011 :
		begin
		  output_64[11:0] <= input_32[31:20];
		end
		
		7'b1100111 :
		begin
		  output_64[10] <= input_32[7];
		  output_64[3:0] <= input_32[11:8];
		  output_64[9:4] <= input_32[30:25];
		  output_64[11] <= input_32[31];
		end
	endcase
	 
	 if (input_32[31]==1'b1)
	   output_64[63:12] <= 52'b1;
	  
	 else 
	   output_64[63:12] <= 52'b0;
	  
	end

	endmodule
