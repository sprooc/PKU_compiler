#include "IRGenVisitor.h"
extern std::ofstream out_file;
class CodeGenVisitor {
 public:
  void Visit(const ProgramIR* program);
  void Visit(const FunctionIR* function);
  void Visit(const BasicBlockIR* basic_block);
  void Visit(const ValueIR* value);
  void Visit(const ReturnValueIR* return_value);
  void Visit(const IntegerValueIR* integer_value);
};