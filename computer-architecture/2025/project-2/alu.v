module alu(ctl, op1, op2, zero, result);
	input [3:0] ctl;
	input [63:0] op1, op2;
	output [63:0] result;
	output zero;
	
	reg [63:0] result;
	reg zero;
	
	// symbolic constants for ALU operations
	parameter ALU_add = 4'b0010;
	parameter ALU_sub = 4'b0110;
	parameter ALU_and = 4'b0000;
	parameter ALU_or  = 4'b0001;

	always @(op1 or op2 or ctl)
	begin
		case (ctl)
		
		ALU_add :
		          result <= op1+op2;
		ALU_sub :
		          result <= op1-op2;
		ALU_or  :
		          result <= op1|op2;
		ALU_and :
		          result <= op1&op2;
					 
		default : result <= result;
		endcase
		
		if (result == 63'd0)
		begin
			zero <= 1;
		end
		else
		begin
			zero <= 0;
		end
	end
endmodule

