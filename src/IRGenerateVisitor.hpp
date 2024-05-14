#pragma once
#include <cassert>
#include <memory>

#include "AST.hpp"
#include "IR.hpp"

class IRGenerateVisitor {
 private:
  BasicBlockIR* curr_basic_block;
  ProgramIR* program_ir;

 public:
  void PrintResult() {
    assert(program_ir);
    program_ir->PrintIR();
  }
  void Visit(CompUnit* ast);
  void Visit(FuncDef* ast);
  void Visit(Block* ast);
  void Visit(Stmt* ast);
};