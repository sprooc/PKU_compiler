#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "AST.h"
#include "CodeGenVisitor.h"
#include "IRGenVisitor.h"

using namespace std;

// 声明 lexer 的输入, 以及 parser 函数
// 为什么不引用 sysy.tab.hpp 呢? 因为首先里面没有 yyin 的定义
// 其次, 因为这个文件不是我们自己写的, 而是被 Bison 生成出来的
// 你的代码编辑器/IDE 很可能找不到这个文件, 然后会给你报错 (虽然编译不会出错)
// 看起来会很烦人, 于是干脆采用这种看起来 dirty 但实际很有效的手段
extern FILE *yyin;
extern int yyparse(unique_ptr<BaseAST> &ast);

const char *koopa_mode = "-koopa";
const char *riscv_mod = "-riscv";

std::ofstream out_file;
int main(int argc, const char *argv[]) {
  // 解析命令行参数. 测试脚本/评测平台要求你的编译器能接收如下参数:
  // compiler 模式 输入文件 -o 输出文件
  assert(argc == 5);
  auto mode = argv[1];
  auto input = argv[2];
  auto output = argv[4];
  out_file.open(output);

  // std::ifstream file(input);
  // std::string line;
  // int i = 0;
  // while (std::getline(file, line)) {
  //   if (i++ == 1) out_file << line << std::endl;  // 输出每一行到标准输出
  // }
  // 打开输入文件, 并且指定 lexer 在解析的时候读取这个文件
  yyin = fopen(input, "r");
  assert(yyin);

  // 调用 parser 函数, parser 函数会进一步调用 lexer 解析输入文件的
  unique_ptr<BaseAST> ast;
  auto ret = yyparse(ast);
  assert(!ret);

  // 输出解析得到的 AST, 其实就是个字符串
  // ast->Dump();
  IRGenerateVisitor IR_visitor;
  IR_visitor.Visit((CompUnit *)ast.get());

  std::string mode_str = std::string(mode);
  if (mode_str == "-koopa") {
    IR_visitor.PrintResult();
  } else if (mode_str == "-riscv") {
    CodeGenVisitor cg_visitor;
    cg_visitor.Visit(IR_visitor.GetProgramIR());
  }
  return 0;
}
