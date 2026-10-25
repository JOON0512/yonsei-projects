module EXMEMreg(EX_RegWrite, EX_MemtoReg, EX_PCSrc, EX_MemRead, EX_Memwrite,
					EX_RefPC, EX_Zero, EX_ALUOut, EX_Data, EX_RegRd,
					MEM_RegWrite, MEM_MemtoReg, MEM_PCSrc, MEM_MemRead, MEM_Memwrite,
					MEM_BranchPC, MEM_Zero, MEM_ALUOut, MEM_Data, MEM_RegRd,
				clk);
	input EX_RegWrite, EX_MemtoReg, EX_PCSrc, EX_MemRead, EX_Memwrite, EX_Zero;
	input [63:0] EX_RefPC, EX_ALUOut, EX_Data;
	input [4:0] EX_RegRd;
	
	output MEM_RegWrite, MEM_MemtoReg, MEM_PCSrc, MEM_MemRead, MEM_Memwrite, MEM_Zero;
   output [63:0] MEM_BranchPC, MEM_ALUOut, MEM_Data;
   output [4:0] MEM_RegRd;
   
	reg MEM_RegWrite, MEM_MemtoReg, MEM_PCSrc, MEM_MemRead, MEM_Memwrite, MEM_Zero;
   reg [63:0] MEM_BranchPC, MEM_ALUOut, MEM_Data;
   reg [4:0] MEM_RegRd;
	
   input clk;
    
    initial
		begin
            MEM_RegWrite 	<=	0;
				MEM_MemtoReg	<=	0;
				MEM_PCSrc		<=	0;
				MEM_MemRead		<=	0;
				MEM_Memwrite	<=	0;
				MEM_Zero			<=	0;
				MEM_BranchPC	<=	0;
				MEM_ALUOut		<=	0;
				MEM_Data			<=	0;
				MEM_RegRd		<=	0;
        end
    
always @(posedge clk)		// EX/MEM Pipeline Register
    begin
            MEM_RegWrite 	<=	EX_RegWrite;
				MEM_MemtoReg	<=	EX_MemtoReg;
				MEM_PCSrc		<=	EX_PCSrc;
				MEM_MemRead		<=	EX_MemRead;
				MEM_Memwrite	<=	EX_Memwrite;
				MEM_Zero			<=	EX_Zero;
				MEM_BranchPC	<=	EX_RefPC;
				MEM_ALUOut		<=	EX_ALUOut;
				MEM_Data			<=	EX_Data;
				MEM_RegRd		<=	EX_RegRd;
    end   
endmodule
    