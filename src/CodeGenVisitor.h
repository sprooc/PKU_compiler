#include "IRGenVisitor.h"
class CodeGenVisitor {
  public:
    void Visit(const ProgramIR* program);
    void Visit(const FunctionIR* function);
    void Visit(const BasicBlockIR* BasicBlockIR);
    // void Visit
};