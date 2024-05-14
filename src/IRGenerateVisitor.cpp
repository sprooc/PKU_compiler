#include "IRGenerateVisitor.hpp"

#include <memory>

#include "AST.hpp"
#include "IR.hpp"

void IRGenerateVisitor::Visit(CompUnit* ast) {
  program_ir = new ProgramIR();
  Visit((FuncDef*)ast->func_def.get());
}

void IRGenerateVisitor::Visit(FuncDef* ast) {
  FunctionIR* function_ir = new FunctionIR();
  function_ir->name = ast->ident;
  curr_basic_block = new BasicBlockIR();
  curr_basic_block->name = "entry";
  function_ir->basic_blocks.push_back(
      std::unique_ptr<BasicBlockIR>(curr_basic_block));
  Visit((Block*)ast->block.get());
  program_ir->functions.push_back(std::unique_ptr<FunctionIR>(function_ir));
}

void IRGenerateVisitor::Visit(Block* ast) { Visit((Stmt*)ast->stmt.get()); }

void IRGenerateVisitor::Visit(Stmt* ast) {
  RetInstrIR* ret_it = new RetInstrIR();
  ret_it->ret_value = ((Number*)ast->number.get())->int_const;
  curr_basic_block->instrs.push_back(std::unique_ptr<InstrIR>(ret_it));
}
