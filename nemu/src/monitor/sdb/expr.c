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

enum {
  TK_NOTYPE = 256, TK_EQ,
  TK_DTYPE,
  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"[0-9]+", TK_DTYPE},    // 十进制整数
  {"\\+", '+'},         // plus
  {"\\-", '-'},         // sub
  {"\\*", '*'},         // mul
  {"\\/", '/'},         // div
  {"\\(", '('},         // left
  {"\\)", ')'},         // div
  {"==", TK_EQ},        // equal
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
            for(int i=0;i<=31;i++)
            tokens[nr_token].str[i]=match[i];
            printf("Found integer: %s\n", match); 
            printf("length is %d\n",substr_len);
          break;
          default: break;
        }
        nr_token++;
        break;
      }
    }

    if (i == NR_REGEX) {
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
  // make_token(e);
  // for(j=0;j<=31&&tokens[j].type!=0;j++)
  // {
  //     printf("tokens[j].type is %d\n",tokens[j].type);
  //     printf("tokens[j].str is %s\n",tokens[j].str);
  // }
  printf("the answer is %d\n",eval(0,data_len-1));

  for(j=0;j<=31;j++)
  {
    tokens[j].type=0;
    strcpy(tokens[j].str, temp);
  }

  return 0;
}
static int eval(int p,int q)
{
  if (p > q) {
    return -1;
  }
  else if (p == q) {
      return atoi(tokens[p].str);
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