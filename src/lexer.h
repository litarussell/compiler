#include "type.h"

void next ();

int_ poolsize;

char *src,
     *old_src;  // pointer to source code string;

int_ line;

int_ token,        // current token
    token_val;    // value of current token

int_ *current_id,  // current parsed ID
    *symbols;     // symbol table

int_ *idmain;      // the `main` function

// tokens and classes (operators last and in precedence order)
enum {
  Num = 128, Fun, Sys, Glo, Loc, Id,
  Char, Else, Enum, If, Int, Return, Sizeof, While,
  Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak
};

// fields of identifier
enum {Token, Hash, Name, Type, Class, Value, BType, BClass, BValue, IdSize};


