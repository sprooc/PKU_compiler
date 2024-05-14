#include "IRGenerateVisitor.hpp"

#include <memory>

#include "AST.hpp"
#include "IR.hpp"

ProgramIR* IRGenerateVisitor::visitCompUnit(CompUnit* ast) {
  ProgramIR* program_ir = new ProgramIR();
  program_ir->functions.push_back(std::unique_ptr<FunctionIR>(
      visitFuncDef((FuncDef*)(ast->func_def.get()))));
  return program_ir;
}

FunctionIR* IRGenerateVisitor::visitFuncDef(FuncDef* ast) {
  FunctionIR* function_ir = new FunctionIR();
  function_ir->name = ast->ident;
  curr_basic_block = new BasicBlockIR();
  curr_basic_block->name = "entry";
  function_ir->basic_blocks.push_back(
      std::unique_ptr<BasicBlockIR>(curr_basic_block));
  visitBlock((Block*)ast->block.get());
  return function_ir;
}

BaseIR* IRGenerateVisitor::visitBlock(Block* ast) {
  visitStmt((Stmt*)ast->stmt.get());
  return nullptr;
}

BaseIR* IRGenerateVisitor::visitStmt(Stmt* ast) {
  RetInstrIR* ret_it = new RetInstrIR();
  ret_it->ret_value = ((Number*)ast->number.get())->int_const;
  curr_basic_block->instrs.push_back(std::unique_ptr<InstrIR>(ret_it));
  return nullptr;
}
