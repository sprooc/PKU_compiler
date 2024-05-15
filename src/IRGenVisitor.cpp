#include "IRGenVisitor.h"

#include <cassert>
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
  ret_ir->ret_value = std::unique_ptr<ValueIR>(Visit((Exp*)ast->exp.get()));
  curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(ret_ir));
  return nullptr;
}

ValueIR* IRGenerateVisitor::Visit(Number* ast) {
  IntegerValueIR* integer_value = new IntegerValueIR();
  integer_value->number = ast->int_const;
  return integer_value;
}

ValueIR* IRGenerateVisitor::Visit(Exp* ast) {
  return Visit((UnaryExp*)ast->unary_exp.get());
}

ValueIR* IRGenerateVisitor::Visit(PrimaryExp* ast) {
  if (ast->exp) {
    return Visit((Exp*)ast->exp.get());
  } else if (ast->number) {
    return Visit((Number*)ast->number.get());
  }
  assert(0);
}

ValueIR* IRGenerateVisitor::Visit(UnaryExp* ast) {
  if (ast->primary_exp) {
    return Visit((PrimaryExp*)ast->primary_exp.get());
  } else {
    std::string op = ((UnaryOp*)ast->unary_op.get())->op;
    if (op == "+") {
      return Visit((UnaryExp*)ast->unary_exp.get());
    }
    if (op == "-") {
      BinaryOpInstrIR* binstr = new BinaryOpInstrIR();
      binstr->name = GetTmp();
      binstr->op_type = OP_SUB;
      auto zero = new IntegerValueIR();
      zero->number = 0;
      binstr->left = std::unique_ptr<ValueIR>(zero);
      binstr->right =
          std::unique_ptr<ValueIR>(Visit((UnaryExp*)ast->unary_exp.get()));
      curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(binstr));
      return binstr;
    } else if (op == "!") {
      BinaryOpInstrIR* equ_ir = new BinaryOpInstrIR();
      auto zero = new IntegerValueIR();
      zero->number = 0;
      equ_ir->name = GetTmp();
      equ_ir->left = std::unique_ptr<ValueIR>(zero);
      equ_ir->right =
          std::unique_ptr<ValueIR>(Visit((UnaryExp*)ast->unary_exp.get()));
      curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(equ_ir));
      return equ_ir;
    }
  }
}
ValueIR* IRGenerateVisitor::Visit(UnaryOp* ast){}