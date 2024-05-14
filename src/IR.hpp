#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>

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
    std::cout << "ret ";
    std::cout << ret_value;
  }
};

class BasicBlockIR : public BaseIR {
 public:
  std::string name;
  std::vector<std::unique_ptr<InstrIR>> instrs;

  void PrintIR() const override {
    std::cout << "%" << name << ":" << std::endl;
    for (auto& instr : instrs) {
      std::cout << "  ";
      instr->PrintIR();
      std::cout << std::endl;
    }
  }
};

class FunctionIR : public BaseIR {
 public:
  std::string name;
  std::vector<std::unique_ptr<BasicBlockIR>> basic_blocks;

  void PrintIR() const override {
    std::cout << "fun @" << name << "(): i32 {" << std::endl;
    for (auto& basic_block : basic_blocks) {
      basic_block->PrintIR();
    }
    std::cout << "}" << std::endl;
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
