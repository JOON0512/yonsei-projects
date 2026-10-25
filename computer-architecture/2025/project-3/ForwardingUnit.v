module ForwardingUnit(ID_EXRS1, IDEXRS2, EX_MEMRd, MEM_WBRd,
						EX_MEMRegWrite, MEM_WBRegWrite,
						ForwardA, ForwardB);
						
	input [4:0] ID_EXRS1, IDEXRS2, EX_MEMRd, MEM_WBRd;
	input EX_MEMRegWrite, MEM_WBRegWrite;
	
	output [1:0] ForwardA, ForwardB;
	
		//decide whether the register declarations are needed
	reg [1:0] ForwardA, ForwardB;
	
	// fill in the missing code
always @(ID_EXRS1 or IDEXRS2)
   begin
      if(EX_MEMRegWrite == 1 && EX_MEMRd != 0 && ID_EXRS1 == EX_MEMRd)
         ForwardA <= 2'b10;
      else if ((MEM_WBRegWrite == 1 && MEM_WBRd != 0 && ID_EXRS1 == MEM_WBRd)
				&& ~(EX_MEMRegWrite == 1 && EX_MEMRd != 0 && ID_EXRS1 == EX_MEMRd))
         ForwardA <= 2'b01;
      else
         ForwardA <= 0;
         
      if (EX_MEMRegWrite == 1 && EX_MEMRd != 0 && IDEXRS2 == EX_MEMRd)
         ForwardB <= 2'b10;
      else if ((MEM_WBRegWrite == 1 && MEM_WBRd != 0 && IDEXRS2 == MEM_WBRd)
				&& ~(EX_MEMRegWrite == 1 && EX_MEMRd != 0 && IDEXRS2 == EX_MEMRd))
         ForwardB <= 2'b01;
      else
         ForwardB <= 0;
   end

	
endmodule 

