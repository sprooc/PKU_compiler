#include "CodeGenVisitor.h"

#include <cassert>

void CodeGenVisitor::Visit(const ProgramIR* program) {
  for (auto& function : program->functions) {
    out_file << "  .test" << std::endl;
    Visit((FunctionIR*)function.get());
  }
}
void CodeGenVisitor::Visit(const FunctionIR* function) {
  out_file << " .global " << function->name << std::endl;
  out_file << function->name << ":" << std::endl;
  for (auto& bb : function->basic_blocks) {
    Visit((BasicBlockIR*)bb.get());
  }
}
void CodeGenVisitor::Visit(const BasicBlockIR* basic_block) {
  for (auto& value : basic_block->values) {
    Visit((ValueIR*)value.get());
  }
}
void CodeGenVisitor::Visit(const ValueIR* value) {
  switch (value->tag) {
    case ValueTag::IRV_RETURN:
      Visit((ReturnValueIR*)value);
      break;
    case ValueTag::IRV_INTEGER:
      Visit((IntegerValueIR*)value);
      break;
    default:
      assert(0);
      break;
  }
}
void CodeGenVisitor::Visit(const ReturnValueIR* return_value) {
  int ret_value = ((IntegerValueIR*)(return_value->ret_value.get()))->number;
  out_file << "  li a0, " << ret_value << std::endl;
  out_file << "  ret" << std::endl;
}
void CodeGenVisitor::Visit(const IntegerValueIR* integer_value) {}