#include <memory>
#include <string>

class BaseAST {
 public:
  virtual ~BaseAST() = default;
};

class CompUnit : public BaseAST {
 public:
  std::unique_ptr<BaseAST> func_def;
};

class FuncDef : public BaseAST {
 public:
  std::unique_ptr<BaseAST> func_type;
  std::string ident;
  std::unique_ptr<BaseAST> block;
};

class FuncType : public BaseAST {};

class Block : public BaseAST {
 public:
  std::unique_ptr<BaseAST> stmt;
};

class Stmt : public BaseAST {
 public:
  std::unique_ptr<BaseAST> number;
};

class Number : public BaseAST {
 public:
  int int_const;
};