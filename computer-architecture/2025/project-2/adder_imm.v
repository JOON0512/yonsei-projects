module adder_imm(OldValue, imm, Newaddress);
      
	    input [63:0] OldValue;
		 input [63:0] imm;
		 output [63:0] Newaddress;
		 reg [63:0] Newaddress;
		 
		 
		 always @ (OldValue or imm)
		         Newaddress <= OldValue + imm;

endmodule
