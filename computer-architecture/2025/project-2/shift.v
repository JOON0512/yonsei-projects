module shift (input_64, output_64);
       input [63:0] input_64;
		 output [63:0] output_64;
		 reg [63:0] output_64;
		 
		 
		 always @(input_64)
		 begin 
		       output_64 <= input_64 << 1 ;
		 end
		 
endmodule