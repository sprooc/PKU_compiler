#include "IRGenVisitor.h"

#include <memory>

#include "AST.h"
#include "IR.h"

ValueIR* IRGenerateVisitor::Visit(CompUnit* ast) {
  program_ir = new ProgramIR();
  return Visit((FuncDef*)ast->func_def.get());
}

ValueIR* IRGenerateVisitor::Visit(FuncDef* ast) {
  FunctionIR* function_ir = new FunctionIR();
  function_ir->name = ast->ident;
  curr_basic_block = new BasicBlockIR();
  curr_basic_block->name = "entry";
  function_ir->basic_blocks.push_back(
      std::unique_ptr<BasicBlockIR>(curr_basic_block));
  Visit((Block*)ast->block.get());
  program_ir->functions.push_back(std::unique_ptr<FunctionIR>(function_ir));
  return nullptr;
}

ValueIR* IRGenerateVisitor::Visit(Block* ast) {
  Visit((Stmt*)ast->stmt.get());
  return nullptr;
}

ValueIR* IRGenerateVisitor::Visit(Stmt* ast) {
  ReturnValueIR* ret_ir = new ReturnValueIR();
  ret_ir->ret_value =
      std::unique_ptr<ValueIR>(Visit((Number*)ast->number.get()));
  curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(ret_ir));
  return nullptr;
}
ValueIR* IRGenerateVisitor::Visit(Number* ast) {
  IntegerValueIR* integer_value = new IntegerValueIR();
  integer_value->number = ast->int_const;
  return integer_value;
}