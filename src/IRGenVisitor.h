#pragma once
#include <cassert>
#include <memory>

#include "AST.h"
#include "IR.h"

class IRGenerateVisitor {
 private:
  BasicBlockIR* curr_basic_block;
  ProgramIR* program_ir;
  int tmp = 0;
  std::string GetTmp() {
    return "%" + std::to_string(tmp++);
  }

 public:
  void PrintResult() {
    assert(program_ir);
    program_ir->PrintIR();
  }
  ProgramIR* GetProgramIR() { return program_ir; }
  ValueIR* Visit(CompUnit* ast);
  ValueIR* Visit(FuncDef* ast);
  ValueIR* Visit(Block* ast);
  ValueIR* Visit(Stmt* ast);
  ValueIR* Visit(Number* ast);
  ValueIR* Visit(Exp* ast);
  ValueIR* Visit(PrimaryExp* ast);
  ValueIR* Visit(UnaryExp* ast);
  ValueIR* Visit(UnaryOp* ast);
};