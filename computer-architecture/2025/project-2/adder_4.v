module adder_4 (OldValue, Newaddress);
       
		 input[63:0] OldValue;
		 output[63:0] Newaddress;
		 reg[63:0] Newaddress;
		 
		 
		 always @ (OldValue)
		 begin
		       Newaddress <= OldValue + 4;
		
		 end
	  endmodule