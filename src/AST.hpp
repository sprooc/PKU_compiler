#pragma once
#include <iostream>
#include <memory>
#include <string>

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

class Block : public BaseAST {
 public:
  std::unique_ptr<BaseAST> stmt;

  void Dump() const override {
    std::cout << "Block { ";
    stmt->Dump();
    std::cout << " }";
  }
};

class Stmt : public BaseAST {
 public:
  std::unique_ptr<BaseAST> number;

  void Dump() const override {
    std::cout << "Stmt { ";
    std::cout << "RETURN, ";
    number->Dump();
    std::cout <<", ;}";
  }
};

class Number : public BaseAST {
 public:
  int int_const;

  void Dump() const override {
    std::cout << "Number { " << int_const << " }";
  }
};