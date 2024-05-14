#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>

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

class FunctionIR : public BaseIR {
 public:
  std::string name;
  std::vector<std::unique_ptr<BasicBlockIR>> basic_blocks;

  void PrintIR() const override {
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
