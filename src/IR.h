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
} ValueTag;

class ValueIR : public BaseIR {
 public:
  ValueTag tag;
  void PrintIR() const override {};
};

class IntegerValueIR : public ValueIR {
 public:
  int number;
  void PrintIR() const override { out_file << number; }
};

class ReturnValueIR : public ValueIR {
 public:
  std::unique_ptr<ValueIR> ret_value;
  void PrintIR() const override {
    out_file << "ret ";
    ret_value->PrintIR();
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
