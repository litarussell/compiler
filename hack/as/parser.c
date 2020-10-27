/**
 * 语法分析器
 * 读取、解析汇编程序文件
 */

#include "parser.h"
#include "debug.h"

int  code;            // 从A指令中提取的符号地址、或者立即数
int  pc = -1;         // 程序计数器 每遇到A或C指令+1

// 提取@xxx
// 或提取 (xxx) 中的xxx
// int symbol(c_type type) {
//   // getch;
//   if (type == A_COMMOND_ID) { // a指令符号

//   }
//   else if (type == L_COMMOND) { // 

//   }
// }

void parser() {
  while (get_token() != -1) {
    if (sym_type == A_COMMOND_ID) {  // 处理A指令-符号
      // 搜索符号地址
      // int addr = get_address(id);
      
      // if (addr == -1) {
      //   code = add_entry(id, -1);   // 创建符号
      // } else {
      //   code = addr;
      // }
      printf("A_COMMOND_ID: %d\n", id);
      pc++;
    }
    else if (sym_type == A_COMMOND_NUM) { // 处理A指令-立即数
      code = num;
      printf("A_COMMOND_NUM: %d\n", code);
      pc++;
    }
    else if (sym_type == L_COMMOND) {  // 处理伪命令
      printf("L_COMMOND: %s\n", id);
      // add_entry(id);
      // int code = symbol(L_COMMOND);
    }
    else if (sym_type == C_COMMOND) {  // 处理C指令
      printf("C_COMMOND: %s, %s, %s\n",
        DEST_MAP[dest_type], COMP_MAP[comp_type], JUMP_MAP[jump_type]);
      pc++;
    }
  }
  fclose(fin);
}
