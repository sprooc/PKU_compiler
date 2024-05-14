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

class InstrIR : public BaseIR {
 public:
  void PrintIR() const override {};
};

class RetInstrIR : public InstrIR {
 public:
  int ret_value;

  void PrintIR() const override {
    out_file << "ret ";
    out_file << ret_value;
  }
};

class BasicBlockIR : public BaseIR {
 public:
  std::string name;
  std::vector<std::unique_ptr<InstrIR>> instrs;

  void PrintIR() const override {
    out_file << "%" << name << ":" << std::endl;
    for (auto& instr : instrs) {
      out_file << "  ";
      instr->PrintIR();
      out_file << std::endl;
    }
  }
};

typedef enum {
  IRT_INT32,
  IRT_ARRAY,
  IRT_POINTER,
  IRT_FUNCTION,
} TypeTag;

// typedef struct Type {
//   TypeTag tag;
//   union {
//     struct {
//       std::unique_ptr<struct Type> elem_type;
//       size_t len;
//     } array;
//     struct {
//       std::unique_ptr<struct Type> elem_type;
//     } pointer;
//     struct {
//       std::unique_ptr<struct Type> ret_type;
//       std::vector<std::unique_ptr<struct Type>> params;
//     } function;
//   } type;
// } Type;

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
