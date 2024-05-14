#pragma once
#include <memory>

#include "AST.hpp"
#include "IR.hpp"

class IRGenerateVisitor {
 private:
  BasicBlockIR* curr_basic_block;

 public:
  ProgramIR* visitCompUnit(CompUnit* ast);
  FunctionIR* visitFuncDef(FuncDef* ast);
  BaseIR* visitBlock(Block* ast);
  BaseIR* visitStmt(Stmt* ast);
};