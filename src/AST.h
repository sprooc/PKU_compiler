#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class BaseAST {
 public:
  virtual ~BaseAST() = default;
  virtual void Dump() const {};
};

class CompUnit : public BaseAST {
 public:
  std::unique_ptr<BaseAST> func_def;

  void Dump() const override {
    std::cout << "CompUnit { ";
    func_def->Dump();
    std::cout << " }";
  }
};

class FuncDef : public BaseAST {
 public:
  std::unique_ptr<BaseAST> func_type;
  std::string ident;
  std::unique_ptr<BaseAST> block;

  void Dump() const override {
    std::cout << "FuncDef { ";
    func_type->Dump();
    std::cout << ", " << ident << ", ";
    block->Dump();
    std::cout << " }";
  }
};

class FuncType : public BaseAST {
 public:
  void Dump() const override { std::cout << "FuncType { INT }"; }
};

class Stmt : public BaseAST {
 public:
  std::unique_ptr<BaseAST> exp;

  void Dump() const override {
    std::cout << "Stmt { ";
    std::cout << "RETURN, ";
    exp->Dump();
    std::cout << ", ;}";
  }
};

class Exp : public BaseAST {
 public:
  std::unique_ptr<BaseAST> lor_exp;
};

class MulExp : public BaseAST {
 public:
  std::unique_ptr<BaseAST> unary_exp;
  std::unique_ptr<BaseAST> mul_exp;
  std::string op;
};

class AddExp : public BaseAST {
 public:
  std::unique_ptr<BaseAST> mul_exp;
  std::unique_ptr<BaseAST> add_exp;
  std::string op;
};


class UnaryExp : public BaseAST {
 public:
  std::unique_ptr<BaseAST> primary_exp;
  std::unique_ptr<BaseAST> unary_op;
  std::unique_ptr<BaseAST> unary_exp;
};

class UnaryOp : public BaseAST {
 public:
  std::string op;
};

class Number : public BaseAST {
 public:
  int int_const;

  void Dump() const override { std::cout << "Number { " << int_const << " }"; }
};

class RelExp : public BaseAST {
 public:
  std::unique_ptr<BaseAST> add_exp;
  std::unique_ptr<BaseAST> rel_exp;
  std::string op;
};

class EqExp : public BaseAST {
 public:
  std::unique_ptr<BaseAST> rel_exp;
  std::unique_ptr<BaseAST> eq_exp;
  std::string op;
};

class LAndExp : public BaseAST {
 public:
  std::unique_ptr<BaseAST> eq_exp;
  std::unique_ptr<BaseAST> land_exp;
  std::string op;
};

class LOrExp : public BaseAST {
 public:
  std::unique_ptr<BaseAST> land_exp;
  std::unique_ptr<BaseAST> lor_exp;
  std::string op;
};

class Decl : public BaseAST {
 public:
  std::unique_ptr<BaseAST> const_decl;
};

class ConstDecl : public BaseAST {
 public:
  std::unique_ptr<BaseAST> b_type;
  std::vector<std::unique_ptr<BaseAST>> const_def;
};

class ConstInitVal : public BaseAST {
 public:
  std::unique_ptr<BaseAST> const_exp;
};

class Block : public BaseAST {
 public:
  std::vector<std::unique_ptr<BaseAST>> block_item;
};

class BlockInte : public BaseAST {
 public:
  std::unique_ptr<BaseAST> decl;
  std::unique_ptr<BaseAST> stmt;
};

class LVal : public BaseAST {
 public:
  std::unique_ptr<BaseAST> ident;
};

class PrimaryExp : public BaseAST {
 public:
  std::unique_ptr<BaseAST> exp;
  std::unique_ptr<BaseAST> lval;
  std::unique_ptr<BaseAST> number;
};

class ConstExp : public BaseAST {
 public:
  std::unique_ptr<BaseAST> exp;
};
