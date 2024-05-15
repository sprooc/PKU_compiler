#pragma once
#include <cassert>
#include <memory>

#include "AST.h"
#include "IR.h"

class IRGenerateVisitor {
 private:
  BasicBlockIR* curr_basic_block;
  ProgramIR* program_ir;

 public:
  void PrintResult() {
    assert(program_ir);
    program_ir->PrintIR();
  }
  ValueIR* Visit(CompUnit* ast);
  ValueIR* Visit(FuncDef* ast);
  ValueIR* Visit(Block* ast);
  ValueIR* Visit(Stmt* ast);
  ValueIR* Visit(Number* ast);
};