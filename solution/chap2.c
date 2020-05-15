#include <stdio.h>

char ch;
int token;
int row_num = 1;
int col_num = 0;

FILE *fin = NULL;

enum e_TokenCode {
	TK_PLUS,		  // +
  TK_MINUS,		  // -
  TK_STAR,		  // *
  TK_DIVIDE,	  // /
  TK_MOD,			  // %
  TK_EQ,			  // ==
  TK_NEQ,		  	// !=
  TK_LT,			  // <
  TK_LEQ,			  // <=
  TK_GT,			  // >
  TK_GEQ,			  // >=
  TK_ASSIGN,	  // =
  TK_POINTSTO,  // ->
  TK_DOT,			  // .
	TK_AND,       // &
	TK_OPENPA,		// (
	TK_CLOSEPA,		// )
	TK_OPENBR,		// [
	TK_CLOSEBR,		// ]
	TK_BEGIN,		  // {
	TK_END,			  // }
  TK_SEMICOLON,	// ;  
  TK_COMMA,		  // ,
	TK_ELLIPSIS,	// ...
	TK_EOF,			  // 文件结束标识符

  TK_CINT,		  // 
  TK_CCHAR,		  // 
  TK_CSTR,		  //

	KW_CHAR,		  // char
	KW_SHORT,		  // short
	KW_INT,			  // int
  KW_VOID,		  // void
  KW_STRUCT,		// struct 
	KW_IF,			  // if
	KW_ELSE,		  // else
	KW_FOR,			  // for
	KW_CONTINUE,	// continue
  KW_BREAK,		  // break
  KW_RETURN,		// return
  KW_SIZEOF,		// sizeof
	
	TK_IDENT      // 标识符
};

// 读取源文件的下一个字符
void getch () {
  ch = getc(fin);
  if (ch != '\n') col_num++;
  else {
    row_num++;
    col_num = 0;
  }
}

// 忽略空格
void skip_white_space () {
  while (ch == ' ' || ch == '\t' || ch == '\r') {
    if (ch == '\r') {
      getch();
      if (ch != '\n') return;
    }
    getch();
  }
}

typedef struct DynString {
  int count;
  int cap;
  char *val;
} DynString;

void realloc_string (DynString *p, int new_size) {
  int cap = p->cap;
  char *val;
  while (cap < new_size) cap*=2;
  val = realloc(p->val, cap);
  if (!val) {
    printf("内存分配失败!");
    return;
  }
  p->cap = cap;
  p->val = val;
}

void add_char (DynString *pstr, int ch) {
    int count;
    count = pstr->count + 1;
    if (count > pstr->cap)
        realloc_string(pstr, count);
    ((char *)pstr->val)[count - 1] = ch;
    pstr->count = count;
}

// parse identifier 标识符
int parse_identifier () {
  int i = row_num;
  int j = col_num;
  DynString str;
  DynString* p = &str;
  p->val = (char*)malloc(sizeof(char) * 8);
  p->count = 0;
  p->cap = 8;
  while ((ch >= 'a' && ch <= 'z')
        || (ch >= 'A' && ch <= 'Z')
        || (ch >= '0' && ch <= '9')
        || ch == '_') {
    add_char(p, ch);
    getch();
  }
  if (p->count == 2 && *(p->val) == 'i' && *(p->val+1) == 'f') {
    printf("IF  (%d, %d)\n", i, j);
    return KW_IF;
  } else {
    printf("ID(");
    int n = 0;
    while (n++ < p->count) printf("%c", *(p->val + n - 1));
    printf(")   (%d, %d)\n", i, j);
    return TK_IDENT;
  }
  
  free(p->val);
}

// parse number 数字
int parse_num () {
  int i = row_num;
  int j = col_num;
  DynString str;
  DynString* p = &str;
  p->val = (char*)malloc(sizeof(char) * 8);
  p->count = 0;
  p->cap = 8;
  while (ch >= '0' && ch <= '9') {
    add_char(p, ch);
    getch();
  }
  if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
    while ((ch >= 'a' && ch <= 'z')
        || (ch >= 'A' && ch <= 'Z')
        || (ch >= '0' && ch <= '9')
        || ch == '_') {
      add_char(p, ch);
      getch();
    }
    printf("ID(");
    int n = 0;
    while (n++ < p->count) printf("%c", *(p->val + n - 1));
    printf(")   (%d, %d)\n", i, j);
    return TK_IDENT;
  }

  add_char(p, '\0');
  printf("NUM(%d)   (%d, %d)\n", atoi(p->val), i, j);
  return KW_INT;
}

void get_token () {
  getch();
  skip_white_space();
  if ((ch >= 'a' && ch <= 'z')
      || (ch >= 'A' && ch <= 'Z')
      || ch == '_') {
    token = parse_identifier();
  }
  else if (ch >= '0' && ch <= '9') {
    token = parse_num();
  }
  else if (ch == EOF) {
    token = TK_EOF;
  }
}

int main (int argc, char** argv) {
  // fin = fopen(argv[1], "rb");
  fin = fopen("/Users/lita/_work/project/compiler/solution/chap2-test.txt", "r");
  if (!fin) {
    printf("无法读取源文件!\n");
    return 0;
  }

  do {
    get_token();
  } while (token != TK_EOF);

  return 0;
}