#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

extern std::ofstream out_file;

class BaseIR {
 public:
  virtual ~BaseIR() = default;
  virtual void PrintIR() const {};
};

typedef enum {
  IRV_RETURN,
  IRV_INTEGER,
  IRV_CONSTANT,
  IRV_INSTR,
  IRV_BOP,
} ValueTag;

typedef enum {
  OP_ADD,
  OP_SUB,
  OP_EQU,
} OpType;

class ValueIR : public BaseIR {
 public:
  ValueIR(ValueTag t) : tag(t){};
  ValueTag tag;
  void PrintIR() const override {};
  virtual void PrintName() const {};
};

class ConstantIR : public ValueIR {
 public:
  ConstantIR() : ValueIR(ValueTag::IRV_CONSTANT){};
  void PrintIR() const override {};
  void PrintName() const override {};
};

class InstrIR : public ValueIR {
 public:
  InstrIR() : ValueIR(ValueTag::IRV_INSTR){};
  void PrintIR() const override {};
  void PrintName() const override {};
};

class IntegerValueIR : public ConstantIR {
 public:
  IntegerValueIR() { this->tag = IRV_INTEGER; }
  int number;
  void PrintIR() const override { out_file << number; }
  void PrintName() const override { out_file << number; };
};

class BinaryOpInstrIR : public InstrIR {
 public:
  BinaryOpInstrIR() { this->tag = ValueTag::IRV_BOP; }
  std::string name;
  OpType op_type;
  std::unique_ptr<ValueIR> left;
  std::unique_ptr<ValueIR> right;
  void PrintName() const override { out_file << name; }
  void PrintIR() const override {
    out_file << name << " = ";
    switch (op_type) {
      case OpType::OP_ADD:
        out_file << name << " = add ";
        left->PrintName();
        out_file << ", ";
        right->PrintName();
        break;
      case OpType::OP_SUB:
        out_file << name << " = sub ";
        left->PrintName();
        out_file << ", ";
        right->PrintName();
        break;
      case OpType::OP_EQU:
        out_file << name << " = equ ";
        left->PrintName();
        out_file << ", ";
        right->PrintName();
        break;
      default:
        break;
    }
  };
};

class ReturnValueIR : public InstrIR {
 public:
  ReturnValueIR() { this->tag = ValueTag::IRV_RETURN; }
  std::unique_ptr<ValueIR> ret_value;
  void PrintIR() const override {
    out_file << "ret ";
    if (ret_value->tag == IRV_INTEGER) {
      ret_value->PrintIR();
    } else {
      out_file << ((BinaryOpInstrIR*)ret_value.get())->name;
    }
  }
};

class BasicBlockIR : public BaseIR {
 public:
  std::string name;
  std::vector<std::unique_ptr<ValueIR>> values;

  void PrintIR() const override {
    out_file << "%" << name << ":" << std::endl;
    for (auto& value : values) {
      out_file << "  ";
      value->PrintIR();
      out_file << std::endl;
    }
  }
};

/*-------------- Type --------------------*/
typedef enum {
  IRT_INT32,
  IRT_ARRAY,
  IRT_POINTER,
  IRT_FUNCTION,
} TypeTag;

class Type {
 public:
  TypeTag tag;
};

class ArrayType : public Type {
 public:
  std::unique_ptr<Type> elem_type;
  size_t len;
};

class PointerType : public Type {
 public:
  std::unique_ptr<Type> elem_type;
};

class FunctionType : public Type {
 public:
  std::unique_ptr<Type> ret_type;
  std::vector<std::unique_ptr<Type>> params;
};
/*----------------- Type end -------------------------*/

class FunctionIR : public BaseIR {
 public:
  // FunctionIR& operator=(FunctionIR&&) = default;
  // ~FunctionIR() override = default;
  // std::unique_ptr<Type> ret_type;
  std::string name;
  // std::vector<std::unique_ptr<struct Type>> params;
  std::vector<std::unique_ptr<BasicBlockIR>> basic_blocks;

  void PrintIR() const override {
    if (name != "main") {
      std::cerr << "error: syntax error";
      exit(1);
    }
    out_file << "fun @" << name << "(): i32 {" << std::endl;
    for (auto& basic_block : basic_blocks) {
      basic_block->PrintIR();
    }
    out_file << "}" << std::endl;
  }
};

class ProgramIR : public BaseIR {
 public:
  std::vector<std::unique_ptr<FunctionIR>> functions;

  void PrintIR() const override {
    for (auto& function : functions) {
      function->PrintIR();
    }
  }
};

// class ConstIntIR :public BaseIR {}
