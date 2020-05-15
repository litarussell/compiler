/**
 * 语法分析器 parser
 */
#include "type.h"
#include "lexer.h"
#include "parser.h"
#include "vm.h"
#include <stdio.h>

// #include <string.h>

int basetype;
int expr_type;

/**
 * 语句解析
 * 目前支持一下6种语句:
 * 1, if (...) <statement> [else <statement>]
 * 2, while (...) <statement>
 * 3, { <statement> }
 * 4, return ...
 * 5, <empty statement>
 * 6, expression
 */
void statement () {
  int_ *a, *b;
  if (token == If) {
    match(If);
    match('(');
    expression(Assign);
    match(')');
    *++text = JZ;
    b = ++text;

    statement();
    if (token == Else) {
      match(Else);
      *b = (int_)(text + 3);
      *++text = JMP;
      b = ++text;

      statement();
    }

    *b = (int)(text + 1);
  }
  else if (token == While) {
    match(While);
    a = text + 1;
    match('(');
    expression(Assign);
    match(')');

    *++text = JZ;
    b = ++text;

    statement();

    *++text = JMP;
    *++text = (int_)a;
    *b = (int_)(text + 1);
  }
  else if (token == '{') {
    match('{');
    while (token != '}') {
      statement();
    }

    match('}');
  }
  else if (token == Return) {
    match(Return);

    if (token != ';') {
      expression(Assign);
    }

    match(';');

    *++text = LEV;
  }
  else if (token == ';') {
    match(';');
  }
  else {
    expression(Assign);
    match(';');
  }
}

// 表达式解析
void expression (int level) {
  int_ *id;
  int tmp;
  int *addr;
  if (token == Num) {
    match(Num);
    *++text = IMM;
    *++text = token_val;
    expr_type = INT;
  }
  else if (token == '"') {
    *++text = IMM;
    *++text = token_val;

    match('"');
    while (token == '"') {
      match('"');
    }

    data = (char *)(((int_)data + sizeof(int_)) & (-sizeof(int_)));
    expr_type = PTR;
  }
  else if (token == Sizeof) {
    match(Sizeof);
    match('(');
    expr_type = INT;

    if (token == Int) {
      match(Int);
    } else if (token == Char) {
      match(Char);
      expr_type = CHAR;
    }

    while (token == Mul) {
      match(Mul);
      expr_type = expr_type + PTR;
    }

    match(')');

    *++text = IMM;
    *++text = (expr_type == CHAR) ? sizeof(char) : sizeof(int);

    expr_type = INT;
  }
  else if (token == Id) {
    match(Id);
    id = current_id;
    if (token == '(') { // 函数调用
      match('(');

      tmp = 0;
      while (token != ')') {
        expression(Assign);
        *++text = PUSH;
        tmp++;
        if (token == ',') {
          match(',');
        }
      }
      match(')');

      if (id[Class] == Sys) {
        *++text = CALL;
        *++text = id[Value];
      }
      else if (id[Class] == Fun) {
        *++text = CALL;
        *++text = id[Value];
      }
      else {
        printf("%ld: 错误的函数调用\n", line);
        exit(-1);
      }

      if (tmp > 0) {
        *++text = ADJ;
        *++text = tmp;
      }
      expr_type = id[Type];
    }
    else if (id[Class] == Num) {
      *++text = IMM;
      *++text = id[Value];
      expr_type = INT;
    }
    else {
      if (id[Class] == Loc) {
        *++text = LEA;
        *++text = index_of_bp - id[Value];
      }
      else if (id[Class] == Glo) {
        *++text = IMM;
        *++text = id[Value];
      }
      else {
        printf("%ld: 变量未定义\n", line);
        exit(-1);
      }

      expr_type = id[Type];
      *++text = (expr_type == Char) ? LC : LI;
    }
  }
  else if (token == '(') {
    match('(');
    if (token == Int || token == Char) {
      tmp = (token == Char) ? CHAR : INT;
      match(token);
      while (token == Mul) {
        match(Mul);
        tmp = tmp + PTR;
      }
      match(')');
      expression(Inc);
      expr_type = tmp;
    } else {
      expression(Assign);
      match(')');
    }
  }
  else if (token == Mul) {
    match(Mul);
    expression(Inc);

    if (expr_type >= PTR) {
      expr_type = expr_type - PTR;
    } else {
      printf("%ld: 错误的引用\n", line);
      exit(-1);
    }

    *++text = (expr_type == CHAR) ? LC : LI;
  }
  else if (token == And) {
    match(And);
    expression(Inc);
    if (*text == LC || *text == LI) {
      text--;
    } else {
      printf("%ld: 错误的地址\n", line);
      exit(-1);
    }

    expr_type = expr_type + PTR;
  }
  else if (token == '!') {
    match('!');
    expression(Inc);
    *++text = PUSH;
    *++text = IMM;
    *++text = 0;
    *++text = EQ;

    expr_type = INT;
  }
  else if (token == '~') {
    match('~');
    expression(Inc);
    *++text = PUSH;
    *++text = IMM;
    *++text = -1;
    *++text = XOR;

    expr_type = INT;
  }
  else if (token = 'ADD') {
    match(Add);
    expression(Inc);
    expr_type = INT;
  }
  else if (token == Sub) {
    match(Sub);
    if (token == Num) {
      *++text = IMM;
      *++text = -token_val;
      match(Num);
    } else {
      *++text = IMM;
      *++text = -1;
      *++text = PUSH;
      expression(Inc);
      *++text = MUL;
    }
    expr_type = INT;
  }
  else if (token == Inc || token == Dec) { // 自增 自减
    tmp = token;
    match(token);
    expression(Inc);
    if (*text == LC) {
      *text = PUSH;
      *++text = LC;
    } else if (*text == LI) {
      *text = PUSH;
      *++text = LI;
    } else {
      printf("%ld: ", line);
      exit(-1);
    }
    *++text = PUSH;
    *++text = IMM;
    *++text = (expr_type > PTR) ? sizeof(int) : sizeof(char);
    *++text = (tmp == Inc) ? ADD : SUB;
    *++text = (expr_type == CHAR) ? SC : SI;
  }
  else {
    printf("%ld: 表达式解析错误\n", line);
    exit(-1);
  }
}

void match (int tk) {
  if (token == tk) {
    next();
  } else {
    printf("%ld: 非预期的token: %ld\n", line, tk);
    exit(-1);
  }
}

int index_of_bp = 0;

void function_parameter () {
  int type;
  int params = 0;
  while (token != ')') {
    type = INT;
    if (token == Int) {
      match(Int);
    } else if (token == Char) {
      type = CHAR;
      match(Char);
    }

    while (token == Mul) { // 指针类型
      match(Mul);
      type = type + PTR;
    }

    if (token != Id) {
      printf("%ld: 错误的函数参数声明\n", line);
      exit(-1);
    }
    
    if (current_id[Class] == Loc) {
      printf("%ld: 重复的参数声明\n", line);
      exit(-1);
    }
    
    match(Id);

    current_id[BClass] = current_id[Class];
    current_id[BType] = current_id[Type];
    current_id[BValue] = current_id[Value];

    current_id[Class] = Loc;
    current_id[Type] = type;
    current_id[Value] = params++;

    if (token == ',') {
      match(',');
    }
  }

  index_of_bp = params + 1;
}

void function_body () {
  int pos_local;
  int type;
  pos_local = index_of_bp;

  while (token == Int || token == Char) {
    basetype = (token == Int) ? INT : CHAR;
    match(token);

    while (token != ';') {
      type = basetype;
      while (token == Mul)  {
        match(Mul);
        type = type + PTR;
      }

      if (token != Id) {
        printf("%ld: 错误的局部变量声明\n", line);
        exit(-1);
      }

      if (current_id[Class] == Loc) {
        printf("%ld: 重复的局部变量声明\n", line);
        exit(-1);
      }

      match(Id);
      
      current_id[BClass] = current_id[Class];
      current_id[BType] = current_id[Type];
      current_id[BValue] = current_id[Value];

      current_id[Class] = Loc;
      current_id[Type] = type;
      current_id[Value] = ++pos_local;

      if (token == ',') {
        match(',');
      }
    }

    match(';');
  }
  
  // *++text = ENT;
  // *++text = pos_local - index_of_bp;

  while (token != '}') {
    statement();
  }

  // *++text = LEV;
}

void function_declaration () {
  // type fun_name (...) {...}
  match('(');
  function_parameter();
  match(')');
  match('{');
  function_body();
  // match('}');

  // 将符号表中的信息恢复成全局符号的信息
  current_id = symbols;
  while (current_id[Token]) {
    if (current_id[Class] == Loc) {
      current_id[Class] = current_id[BClass];
      current_id[Type] = current_id[BType];
      current_id[Value] = current_id[BValue];
    }
    current_id = current_id + IdSize;
  }
  
}

// 解析枚举类型
void enum_declaration () {
  // enum [id] {a = 1, b = 3, ...}
  int i = 0;
  while (token != '}') {
    if (token != Id) {
      printf("%ld: 错误的枚举类型声明 %d\n", line, token);
      exit(-1);
    }
    next();
    if (token == Assign) {
      next();
      if (token != Num) {
        printf("%ld: 错误的枚举变量初始化\n", line);
        exit(-1);
      }
      i = token_val;
      next();
    }

    current_id[Class] = Num;
    current_id[Type] = INT;
    current_id[Value] = i++;

    if (token == ',') {
      next();
    }
  }
}

void global_declaration () {
  int type; // 变量类型
  int i;

  basetype = INT;

  if (token == Enum) { // 分析enum关键字的语法
    match(Enum);
    if (token != '{') {
      match(Id);
    }
    if (token == '{') {
      match('{');
      enum_declaration();
      match('}');
    }

    match(';');
    return;
  }

  if (token == Int) {
    match(Int);
  } else if (token == Char) {
    match(Char);
    basetype = CHAR;
  }

  while (token != ';' && token != '}') {
    type = basetype;
    while (token == Mul) {
      match(Mul);
      type = type + PTR;
    }
    if (token != Id) {
      printf("%ld: 错误的全局变量声明\n", line);
      exit(-1);
    }
    if (current_id[Class]) {
      printf("%ld: 重复的全局变量声明\n", line);
      exit(-1);
    }

    match(Id);
    current_id[Type] = type;

    if (token == '(') {
      current_id[Class] = Fun;
      // current_id[Value] = (int_)(text + 1)
      function_declaration();
    } else {
      current_id[Class] = Glo;
      // current_id[Value] = (int_)data;
      // data = data + sizeof(int_);
    }

    if (token == ',') {
      match(',');
    }
  }
  
}