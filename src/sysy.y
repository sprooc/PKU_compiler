%code requires {
  #include <memory>
  #include <string>
  #include "AST.h"
}

%{

#include <iostream>
#include <memory>
#include <string>
#include "AST.h"

// 声明 lexer 函数和错误处理函数
int yylex();
void yyerror(std::unique_ptr<BaseAST> &ast, const char *s);

using namespace std;

%}


%parse-param { std::unique_ptr<BaseAST> &ast }


%union {
  std::string *str_val;
  int int_val;
  BaseAST *ast_val;
}


%token INT RETURN LT GT LE GE EQ NEQ AND OR
%token <str_val> IDENT 
%token <int_val> INT_CONST

// 非终结符的类型定义
%type <ast_val> FuncDef FuncType Block Stmt Number Exp 
%type <ast_val> PrimaryExp UnaryExp UnaryOp MulExp AddExp
%type <ast_val> RelExp EqExp LAndExp LOrExp
%type <str_val> RelOp EqOp LAndOp LOrOp
%%


CompUnit
  : FuncDef {
    auto comp_unit = make_unique<CompUnit>();
    comp_unit->func_def = unique_ptr<BaseAST>($1);
    ast = move(comp_unit);
  }
  ;


FuncDef
  : FuncType IDENT '(' ')' Block {
    auto ast = new FuncDef();
    ast->func_type = unique_ptr<BaseAST>($1);
    ast->ident = *unique_ptr<string>($2);
    ast->block = unique_ptr<BaseAST>($5);
    $$ = ast;
  }
  ;


FuncType
  : INT {
    $$ = new FuncType();
  }
  ;

Block
  : '{' Stmt '}' {
    auto ast = new Block();
    ast->stmt = unique_ptr<BaseAST>($2);
    $$ = ast;
  }
  ;

Stmt
  : RETURN Exp ';' {
    auto ast = new Stmt();
    ast->exp = unique_ptr<BaseAST>($2);
    $$ = ast;
  }
  ;

Exp
  : LOrExp {
    auto ast = new Exp();
    ast->lor_exp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  ;



PrimaryExp
  : '(' Exp ')' {
    auto ast = new PrimaryExp();
    ast->exp = unique_ptr<BaseAST>($2);
    $$ = ast;
  }
  | Number {
    auto ast = new PrimaryExp();
    ast->number = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  ;

Number
  : INT_CONST {
    auto ast = new Number();
    ast->int_const = $1;
    $$ = ast;
  }
  ;

UnaryExp
  : PrimaryExp {
    auto ast = new UnaryExp();
    ast->primary_exp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | UnaryOp UnaryExp {
    auto ast = new UnaryExp();
    ast->unary_op = unique_ptr<BaseAST>($1);
    ast->unary_exp = unique_ptr<BaseAST>($2);
    $$ = ast;
  }
  ;

UnaryOp
  : '+' {
    auto ast = new UnaryOp();
    ast->op = "+";
    $$ = ast;
  }
  | '-' {
    auto ast = new UnaryOp();
    ast->op = "-";
    $$ = ast;
  }
  | '!' {
    auto ast = new UnaryOp();
    ast->op = "!";
    $$ = ast;
  }
  ;

MulExp
  : UnaryExp {
    auto ast = new MulExp();
    ast->unary_exp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | MulExp '*' UnaryExp {
    auto ast = new MulExp();
    ast->mul_exp = unique_ptr<BaseAST>($1);
    ast->op = "*";
    ast->unary_exp = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  | MulExp '/' UnaryExp {
    auto ast = new MulExp();
    ast->mul_exp = unique_ptr<BaseAST>($1);
    ast->op = "/";
    ast->unary_exp = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  | MulExp '%' UnaryExp {
    auto ast = new MulExp();
    ast->mul_exp = unique_ptr<BaseAST>($1);
    ast->op = "%";
    ast->unary_exp = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

AddExp
  : MulExp {
    auto ast = new AddExp();
    ast->mul_exp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | AddExp '-' MulExp {
    auto ast = new AddExp();
    ast->add_exp = unique_ptr<BaseAST>($1);
    ast->op = "-";
    ast->mul_exp = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  | AddExp '+' MulExp {
    auto ast = new AddExp();
    ast->add_exp = unique_ptr<BaseAST>($1);
    ast->op = "+";
    ast->mul_exp = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

RelExp
  : AddExp {
    auto ast = new RelExp();
    ast->add_exp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | RelExp RelOp AddExp {
    auto ast = new RelExp();
    ast->rel_exp = unique_ptr<BaseAST>($1);
    ast->op = *$2;
    ast->add_exp = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

EqExp
  : RelExp {
    auto ast = new EqExp();
    ast->rel_exp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | EqExp EqOp RelExp {
    auto ast = new EqExp();
    ast->eq_exp = unique_ptr<BaseAST>($1);
    ast->op = *$2;
    ast->rel_exp = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

LAndExp
  : EqExp {
    auto ast = new LAndExp();
    ast->eq_exp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | LAndExp LAndOp EqExp {
    auto ast = new LAndExp();
    ast->land_exp = unique_ptr<BaseAST>($1);
    ast->op = *$2;
    ast->eq_exp = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

LOrExp
  : LAndExp {
    auto ast = new LOrExp();
    ast->land_exp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | LOrExp LOrOp LAndExp {
    auto ast = new LOrExp();
    ast->lor_exp = unique_ptr<BaseAST>($1);
    ast->op = *$2;
    ast->land_exp = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

RelOp
  : LT { $$ = new string("<"); }
  | GT { $$ = new string(">"); }
  | LE { $$ = new string("<="); }
  | GE { $$ = new string(">="); }
  ;

EqOp
  : EQ { $$ = new string("=="); }
  | NEQ { $$ = new string("!="); }
  ;

LAndOp
  : AND { $$ = new string("&&"); }
  ;

LOrOp
  : OR { $$ = new string("||"); }
  ;




%%

// 定义错误处理函数, 其中第二个参数是错误信息
// parser 如果发生错误 (例如输入的程序出现了语法错误), 就会调用这个函数
void yyerror(unique_ptr<BaseAST> &ast, const char *s) {
  cerr << "error: " << s << endl;
}
