module IDEXreg(ID_ALUOp, ID_ALUSrc, ID_RegWrite, ID_MemtoReg, ID_PCSrc, ID_MemRead, ID_MemWrite, ID_pc,
			ID_Rs1value, ID_Rs2value, ID_RegRs1, ID_RegRs2, ID_RegRd, ID_IMM, ID_funct7, ID_funct3,
			EX_ALUOp, EX_ALUSrc, EX_RegWrite, EX_MemtoReg, EX_PCSrc, EX_MemRead, EX_MemWrite, 
			EX_pc, EX_Rs1value, EX_Rs2value,EX_RegRs1,EX_RegRs2, EX_RegRd, EX_IMM, EX_funct7,EX_funct3,
				clk);
	input	[1:0] ID_ALUOp;
	input ID_ALUSrc, ID_RegWrite, ID_MemtoReg, ID_PCSrc, ID_MemRead, ID_MemWrite; // Control Signals
	input [63:0] ID_pc, ID_Rs1value, ID_Rs2value, ID_IMM;
	input [4:0] ID_RegRs1, ID_RegRs2, ID_RegRd;
	input ID_funct7;
	input [2:0] ID_funct3;

	output [1:0] EX_ALUOp;
	output EX_ALUSrc, EX_RegWrite, EX_MemtoReg, EX_PCSrc, EX_MemRead, EX_MemWrite;  // Control Signals
   output [63:0] EX_pc, EX_Rs1value, EX_Rs2value, EX_IMM;
   output [4:0] EX_RegRs1,EX_RegRs2, EX_RegRd;
	output EX_funct7;
	output [2:0] EX_funct3;
    
	reg [1:0] EX_ALUOp;
	reg EX_ALUSrc, EX_RegWrite, EX_MemtoReg, EX_PCSrc, EX_MemRead, EX_MemWrite;  // Control Signals
   reg [63:0] EX_pc, EX_Rs1value, EX_Rs2value, EX_IMM;
   reg [4:0] EX_RegRs1,EX_RegRs2, EX_RegRd;
	reg EX_funct7;
	reg [2:0] EX_funct3;
   
   input clk;
    
    initial
		begin
            EX_ALUOp			<= 0;
				EX_ALUSrc		<=	0;
				EX_RegWrite		<=	0;
				EX_MemtoReg		<=	0;
				EX_PCSrc			<=	0;
				EX_MemRead		<=	0;
				EX_MemWrite		<=	0;
				EX_pc				<=	0;
				EX_Rs1value		<=	0;
				EX_Rs2value		<=	0;
				EX_RegRs1		<=	0;
				EX_RegRs2		<=	0;
				EX_RegRd			<=	0;
				EX_IMM			<=	0;
				EX_funct7		<=	0;
				EX_funct3		<=	0;
        end
    
    
always @(posedge clk)		// ID/EX Pipeline Register
    begin
            EX_ALUOp			<= 	ID_ALUOp;
				EX_ALUSrc		<=		ID_ALUSrc;
				EX_RegWrite		<=		ID_RegWrite;
				EX_MemtoReg		<=		ID_MemtoReg;
				EX_PCSrc			<=		ID_PCSrc;
				EX_MemRead		<=		ID_MemRead;
				EX_MemWrite		<=		ID_MemWrite;
				EX_pc				<=		ID_pc;
				EX_Rs1value		<=		ID_Rs1value;
				EX_Rs2value		<=		ID_Rs2value;
				EX_RegRs1		<=		ID_RegRs1;
				EX_RegRs2		<=		ID_RegRs2;
				EX_RegRd			<=		ID_RegRd;
				EX_IMM			<=		ID_IMM;
				EX_funct7		<=		ID_funct7;
				EX_funct3		<=		ID_funct3;
    end   


endmodule
    