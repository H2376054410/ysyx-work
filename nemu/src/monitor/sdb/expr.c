/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <memory/paddr.h>
enum {
    TK_NOTYPE = 50, TK_EQ, TK_DTYPE, TK_HEX, TK_REG,
    TK_NEQ, TK_AND,TK_DEREF,
    /* TODO: Add more token types */
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"\\*0x[0-9a-fA-F]+", TK_DEREF},            // 指针解引用
  {"0x[0-9a-fA-F]+", TK_HEX},   // 十六进制整数
  {" +", TK_NOTYPE},    // spaces
  {"[0-9]+", TK_DTYPE},    // 十进制整数
  {"\\$.+", TK_REG},              // 寄存器名
  {"\\+", '+'},         // plus
  {"\\-", '-'},         // sub
  {"\\*", '*'},         // mul
  {"\\/", '/'},         // div
  {"\\(", '('},         // left
  {"\\)", ')'},         // div
  {"==", TK_EQ},        // equal
  {"!=", TK_NEQ},               // 不等于
  {"&&", TK_AND},               // 逻辑与

};

#define NR_REGEX ARRLEN(rules)

static int eval(int p,int q);
static bool check_parentheses(int p,int q);
static int find_main_operator(int p,int q);
static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;
int data_len=0;
static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;
        if(rules[i].token_type==TK_NOTYPE)
        {
          break;
        }

        tokens[nr_token].type=rules[i].token_type;
        char match[pmatch.rm_eo - pmatch.rm_so + 1];
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_DTYPE:
            // 提取匹配的整数
            snprintf(match, sizeof(match), "%.*s", pmatch.rm_eo - pmatch.rm_so, e + position-pmatch.rm_eo);
            strcpy(tokens[nr_token].str, match);
            printf("length is %d\n",substr_len);
          break;
          case TK_HEX: 
            snprintf(match, sizeof(match), "%.*s", pmatch.rm_eo - pmatch.rm_so, e + position-pmatch.rm_eo);
            printf("Found hex: %s\n", match); 
            // 处理十六进制数
            strcpy(tokens[nr_token].str, match);
          break;  
          case TK_DEREF: 
            snprintf(match, sizeof(match), "%.*s", pmatch.rm_eo - pmatch.rm_so-1, e + position-pmatch.rm_eo+1);
            printf("Found deref: %s\n", match); 
            // 处理指针解引用
            strcpy(tokens[nr_token].str, match);
          break; 
          case TK_REG: 
            snprintf(match, sizeof(match), "%.*s", pmatch.rm_eo - pmatch.rm_so-1, e + position-pmatch.rm_eo+1);
            printf("Found reg: %s\n", match); 
            // 处理指针解引用
            strcpy(tokens[nr_token].str, match);
          break;                
          default: break;
        }
        nr_token++;
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("input data is %s\n",e);
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  data_len=nr_token;
  return true;
}

char temp[32]={0};
word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  int j=0;
  printf("into the expr\n");
  word_t result=eval(0,data_len-1);
  printf("the answer is %x\n",result);

  for(j=0;j<=31;j++)
  {
    tokens[j].type=0;
    strcpy(tokens[j].str, temp);
  }

  return result;
}

static int eval(int p,int q)
{
  if (p > q) {
    return -1;
  }
  else if (p == q) {
    switch (tokens[p].type)
    {
    case TK_DTYPE:
          return atoi(tokens[p].str);
    break;
    case TK_HEX:
          return strtol(tokens[p].str, NULL, 16);  // 将十六进制字符串转换为整数
    break;
    case TK_DEREF:
         int result=strtol(tokens[p].str, NULL, 16);
         int result1=paddr_read((paddr_t)(result),4);
          printf("deref data is %x\n",result1);
         return   result1;// 将十六进制字符串转换为整数
    break;
    case TK_REG:
            extern const char* regs[];
            int found = 0;  // 标记是否找到匹配的字符串   
          // 遍历 str1 数组中的每个元素，使用 strcmp 进行比较
          for (int i = 0; i < 32; i++) {
              if (strcmp(regs[i], tokens[p].str) == 0) {
                  printf("Found match: %s == %s\n", regs[i], tokens[p].str);
                  found = 1;
                  return cpu.gpr[i];
                  break;  // 找到第一个匹配项，退出循环
              }
          }
          if (!found) {
              printf("No match found.\n");
          }
          return -1;
    break;

    default:
        return -1;
    break;
    }

  }
  else if(q-p==1&&tokens[p].type=='-')
  {
      return -atoi(tokens[q].str);
  }
  else if (check_parentheses(p, q) == true) {
    return eval(p + 1, q - 1);
  }
  else {
        // 查找主运算符的位置
        int op_position = find_main_operator(p, q);
        if (op_position == -1) {
            printf("No operator found\n");
            return 0;
        }
        
        // 递归计算左侧和右侧的值
        int val1 = eval(p, op_position - 1);
        int val2 = eval(op_position + 1, q);
        char op_type = tokens[op_position].type;
        // 根据操作符执行运算
        switch (op_type) {
            case '+': return val1 + val2;
            case '-': return val1 - val2;
            case '*': return val1 * val2;
            case '/': 
                if (val2 == 0) {
                    printf("Division by zero\n");
                    return 0;
                }
                return val1 / val2;
            case TK_EQ: return val1 == val2;  // 处理等于
            case TK_NEQ: return val1 != val2;  // 处理不等于
            case TK_AND: return val1 && val2;  // 处理逻辑与
            // case TK_DEREF: 
            // printf("in deref val1=%x val2=%x\n",val1,val2);
            //   word_t mem=paddr_read((paddr_t)(val2),4);
            //   return mem;   // 处理指针解引用
            default: 
                assert(0); // 如果是非法操作符，触发断言失败
        }    

  }
}
//finding the last
static bool check_parentheses(int p, int q) { 
    int count = 0;
    for (int i = p; i <= q; ++i) {
        if (tokens[i].type == '(') count++;
        else if (tokens[i].type == ')') count--;
        if (count < 0) return false; // 括号不匹配
    }
    if(tokens[p].type=='('&&tokens[q].type==')')
    {
      return true; // 括号匹配
    }
    else
    {
      return false;
    }
    
}

static int find_main_operator(int p,int q)
{
  //find + and -
    int op_position = -1;
    int parenthesis_level = 0;
  for(int i=p;i<=q;++i)
  {
    if(tokens[i].type=='(')
    {
      parenthesis_level++;
    }
    else if(tokens[i].type==')')
    {
      parenthesis_level--;
    }

    if(parenthesis_level==0)
    {
      if((tokens[i].type=='+'||tokens[i].type=='-')&&i!=0)
      {
      op_position=i;
      break;
      }
    }
  }
    // 如果没有找到 + 或 -，查找位运算符
    if (op_position == -1) {
        for (int i = p; i <= q; ++i) {
            if (tokens[i].type == '(') {
                parenthesis_level++;
            } else if (tokens[i].type == ')') {
                parenthesis_level--;
            }

            if (parenthesis_level == 0) {
                // 查找位运算符 & | ^ << >>
                if (tokens[i].type == TK_AND || tokens[i].type == TK_EQ
                    || tokens[i].type == TK_NEQ ) {
                    op_position = i;
                    break;
                }
            }
        }
    }
  if(op_position==-1)
  {
  for(int i=p;i<=q;++i)
  {
    if(tokens[i].type=='(')
    {
      parenthesis_level++;
    }
    else if(tokens[i].type==')')
    {
      parenthesis_level--;
    }

    if(parenthesis_level==0)
    {
      if(tokens[i].type=='*'||tokens[i].type=='/')
      {
      op_position=i;
      break;
      }
    }
  }    
  }
  return op_position;
}