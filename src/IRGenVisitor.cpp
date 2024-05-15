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
  ret_ir->ret_value = Visit((Exp*)ast->exp.get());
  curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(ret_ir));
  return nullptr;
}

ValueIR* IRGenerateVisitor::Visit(Number* ast) {
  IntegerValueIR* integer_value = new IntegerValueIR();
  integer_value->number = ast->int_const;
  // when output, it should be emit; but to avoid memory leak, it should be
  // owned to block
  curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(integer_value));
  return integer_value;
}

ValueIR* IRGenerateVisitor::Visit(Exp* ast) {
  return Visit((LOrExp*)ast->lor_exp.get());
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
    auto zero = new IntegerValueIR();
    zero->number = 0;
    curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(zero));
    if (op == "-") {
      BinaryOpInstrIR* binstr = new BinaryOpInstrIR();
      binstr->op_type = OP_SUB;
      binstr->left = zero;
      binstr->right = Visit((UnaryExp*)ast->unary_exp.get());
      binstr->name = GetTmp();
      curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(binstr));
      return binstr;
    } else if (op == "!") {
      BinaryOpInstrIR* equ_ir = new BinaryOpInstrIR();
      equ_ir->left = zero;
      equ_ir->right = Visit((UnaryExp*)ast->unary_exp.get());
      equ_ir->name = GetTmp();
      equ_ir->op_type = OP_EQU;
      curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(equ_ir));
      return equ_ir;
    }
  }

  assert(0);
}
ValueIR* IRGenerateVisitor::Visit(UnaryOp* ast) { assert(0); }

ValueIR* IRGenerateVisitor::Visit(AddExp* ast) {
  if (ast->add_exp) {
    BinaryOpInstrIR* add_ir = new BinaryOpInstrIR();
    switch (ast->op[0]) {
      case '+':
        add_ir->op_type = OP_ADD; /* code */
        break;
      case '-':
        add_ir->op_type = OP_SUB;
        break;
      default:
        assert(0);
        break;
    }

    add_ir->left = Visit((AddExp*)ast->add_exp.get());
    add_ir->right = Visit((MulExp*)ast->mul_exp.get());
    add_ir->name = GetTmp();
    curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(add_ir));
    return add_ir;
  } else {
    return Visit((MulExp*)ast->mul_exp.get());
  }
}

ValueIR* IRGenerateVisitor::Visit(MulExp* ast) {
  if (ast->mul_exp) {
    BinaryOpInstrIR* mul_ir = new BinaryOpInstrIR();
    switch (ast->op[0]) {
      case '*':
        mul_ir->op_type = OP_MUL;
        break;
      case '/':
        mul_ir->op_type = OP_DIV;
        break;
      case '%':
        mul_ir->op_type = OP_MOD;
        break;
      default:
        assert(0);
        break;
    }
    mul_ir->left = Visit((MulExp*)ast->mul_exp.get());
    mul_ir->right = Visit((UnaryExp*)ast->unary_exp.get());
    mul_ir->name = GetTmp();
    curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(mul_ir));
    return mul_ir;
  } else {
    return Visit((UnaryExp*)ast->unary_exp.get());
  }
}

ValueIR* IRGenerateVisitor::Visit(RelExp* ast) {
  if (ast->rel_exp) {
    BinaryOpInstrIR* rel_ir = new BinaryOpInstrIR();
    if (ast->op == "<") {
      rel_ir->op_type = OP_LT;
    } else if (ast->op == ">") {
      rel_ir->op_type = OP_GT;
    } else if (ast->op == ">=") {
      rel_ir->op_type = OP_GE;
    } else if (ast->op == "<=") {
      rel_ir->op_type = OP_LE;
    }
    rel_ir->left = Visit((RelExp*)ast->rel_exp.get());
    rel_ir->right = Visit((AddExp*)ast->add_exp.get());
    rel_ir->name = GetTmp();
    curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(rel_ir));
    return rel_ir;
  } else {
    return Visit((AddExp*)ast->add_exp.get());
  }
}
ValueIR* IRGenerateVisitor::Visit(EqExp* ast) {
  if (ast->eq_exp) {
    BinaryOpInstrIR* eq_ir = new BinaryOpInstrIR();
    if (ast->op == "==") {
      eq_ir->op_type = OP_EQU;
    } else if (ast->op == "!=") {
      eq_ir->op_type = OP_NEQ;
    }
    eq_ir->left = Visit((EqExp*)ast->eq_exp.get());
    eq_ir->right = Visit((RelExp*)ast->rel_exp.get());
    eq_ir->name = GetTmp();
    curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(eq_ir));
    return eq_ir;
  } else {
    return Visit((RelExp*)ast->rel_exp.get());
  }
}
ValueIR* IRGenerateVisitor::Visit(LAndExp* ast) {
  if (ast->land_exp) {
    auto zero = new IntegerValueIR();
    zero->number = 0;
    curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(zero));
    BinaryOpInstrIR* ne_ir1 = new BinaryOpInstrIR();
    ne_ir1->left = zero;
    ne_ir1->right = Visit((EqExp*)ast->eq_exp.get());
    ne_ir1->name = GetTmp();
    ne_ir1->op_type = OP_NEQ;
    curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(ne_ir1));

    BinaryOpInstrIR* ne_ir2 = new BinaryOpInstrIR();
    ne_ir2->left = zero;
    ne_ir2->right = Visit((LAndExp*)ast->land_exp.get());
    ne_ir2->name = GetTmp();
    ne_ir2->op_type = OP_NEQ;
    curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(ne_ir2));

    BinaryOpInstrIR* land_ir = new BinaryOpInstrIR();
    land_ir->op_type = OP_AND;
    land_ir->left = ne_ir1;
    land_ir->right = ne_ir2;
    land_ir->name = GetTmp();
    curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(land_ir));
    return land_ir;
  } else {
    return Visit((EqExp*)ast->eq_exp.get());
  }
}

ValueIR* IRGenerateVisitor::Visit(LOrExp* ast) {
  if (ast->lor_exp) {
    BinaryOpInstrIR* lor_ir = new BinaryOpInstrIR();
    lor_ir->op_type = OP_OR;
    lor_ir->left = Visit((LOrExp*)ast->lor_exp.get());
    lor_ir->right = Visit((LAndExp*)ast->land_exp.get());
    lor_ir->name = GetTmp();
    curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(lor_ir));

    BinaryOpInstrIR* ne_ir = new BinaryOpInstrIR();
    auto zero = new IntegerValueIR();
    zero->number = 0;
    curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(zero));
    ne_ir->left = zero;
    ne_ir->right = lor_ir;
    ne_ir->name = GetTmp();
    ne_ir->op_type = OP_NEQ;
    curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(ne_ir));
    return ne_ir;
  } else {
    return Visit((LAndExp*)ast->land_exp.get());
  }
}