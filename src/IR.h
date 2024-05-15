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
  OP_MUL,
  OP_DIV,
  OP_MOD,
  OP_LE,
  OP_GE,
  OP_LT,
  OP_GT,
  OP_AND,
  OP_OR,
  OP_NEQ,
} OpType;

class ValueIR : public BaseIR {
 public:
  ValueIR(ValueTag t) : tag(t){};
  ValueTag tag;
  int reg;
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
  ValueIR* left;
  ValueIR* right;
  void PrintName() const override { out_file << name; }
  void PrintIR() const override {
    switch (op_type) {
      case OpType::OP_ADD:
        out_file << name << " = add ";
        break;
      case OpType::OP_SUB:
        out_file << name << " = sub ";
        break;
      case OpType::OP_EQU:
        out_file << name << " = eq ";
        break;
      case OpType::OP_MUL:
        out_file << name << " = mul ";
        break;
      case OpType::OP_DIV:
        out_file << name << " = div ";
        break;
      case OpType::OP_MOD:
        out_file << name << " = mod ";
        break;
      case OpType::OP_LT:
        out_file << name << " = lt ";
        break;
      case OpType::OP_GT:
        out_file << name << " = gt ";
        break;
      case OpType::OP_LE:
        out_file << name << " = le ";
        break;
      case OpType::OP_GE:
        out_file << name << " = ge ";
        break;
      case OpType::OP_NEQ:
        out_file << name << " = ne ";
        break;
      case OpType::OP_OR:
        out_file << name << " = or ";
        break;
      case OpType::OP_AND:
        out_file << name << " = and ";
        break;
      default:
        break;
    }
    left->PrintName();
    out_file << ", ";
    right->PrintName();
  };
};

class ReturnValueIR : public InstrIR {
 public:
  ReturnValueIR() { this->tag = ValueTag::IRV_RETURN; }
  ValueIR* ret_value;
  void PrintIR() const override {
    out_file << "ret ";
    if (ret_value->tag == IRV_INTEGER) {
      ret_value->PrintIR();
    } else {
      out_file << ((BinaryOpInstrIR*)ret_value)->name;
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
      if (value->tag == IRV_INTEGER) {
        continue;
      }
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
