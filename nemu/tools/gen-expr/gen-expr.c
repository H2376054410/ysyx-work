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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
int choose(int a);
char gen_num();
char gen_rand_op();
char gen(char c);
static struct parentheses find_parentheses(int p);

static bool check_parentheses(int p, int q);
static int find_main_operator(int p,int q);
static int eval(int p,int q);
// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";
static char op_station[4]=
{'+','-','*','/'};
int i=0;
int out_limit=0;
int div_flag=0;
struct parentheses
{
  int start;
  int end;
};
int div_all[255]={};
int fdiv_all=0;
static void gen_rand_expr() {
  div_flag=i-1;
  switch (buf[div_flag])
  {
  case '/':
  //      printf("into (");
        buf[i]=gen('(');
        i++; 
        div_flag=0;
        gen_rand_expr();
        buf[i]=gen(')');
  //      printf("out (");
        i++;

  break;
    default:
        switch (choose(3)) {
        case 0: 
          buf[i]=gen_num();
          i++; 
        break;
        case 1:
        buf[i]=gen('(');
        i++; 
        gen_rand_expr();
        buf[i]=gen(')');
        i++;
          break;
        default: 
        gen_rand_expr(); 
        buf[i]=gen_rand_op();
        if(buf[i]='/')
        {
        div_all[fdiv_all]=i;
        fdiv_all++; 
        }
        i++;
        gen_rand_expr(); 
        break;
      }
  
    break;
  }

}
static struct parentheses find_parentheses(int p)
{
    struct parentheses pa1;
    pa1.start=p;
    printf("left is %c\n",buf[p]);
    assert(buf[p]=='(');
    int count = 0;
    for (int i = p; ; ++i) {
        if (buf[i] == '(') count++;
        else if (buf[i] == ')') count--;

        if (count == 0) 
        {
          pa1.end=i;  
          return pa1; 
          break;
        }
    }

}
int choose(int a)
{
  assert(a>0);
  return (rand()%a);
}
char gen_num()
{
  return (0x30+(rand()%10));
}
char gen_rand_op()
{
  int i=rand()%4;
  // if(i==3)
  // div_flag=1;
  return op_station[i];
}
char gen(char c)
{
  return c;
}
void replace(int p,int q)
{
  for(int i=p;i<=q;i++)
  {
    if(buf[i]==0x30)
    {
      buf[i]=0x31;
    }
  }
}
int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }

  int zero_flag=0;
  for (int s = 0; s < loop||zero_flag!=0; s ++) {
    gen_rand_expr();
    out_limit=i;
    while(out_limit>100)
    {
      i=0;
      memset(div_all, 0, sizeof(div_all)); // 将arr的全部10个元素设置为0
      memset(buf, 0, sizeof(buf)); 
      fdiv_all=0;
      gen_rand_expr();
      out_limit=i;
    }
    int find_div_mid=0;
    struct parentheses pa_div;
    for(int j=0;j<=fdiv_all;j++)
    {
      printf("check the %d division op:%c\n",div_all[j],buf[div_all[j]]);
    }
    printf("break,and the fdiv_all is %d\n",fdiv_all);
    printf("buf is %s\n",buf);
    while(fdiv_all!=0)
    {
      int pos=div_all[find_div_mid];
      printf("the data in %d is %c,and the next is %c\n",pos,buf[pos],buf[pos+1]);
      find_div_mid++;
      printf("the next data is %c\n",buf[div_all[find_div_mid]]);
      pa_div=find_parentheses(pos+1);
      replace(pa_div.start,pa_div.end);
      int getdata=eval(pa_div.start,pa_div.end);
      if(getdata<1)
      {
        zero_flag=1;
        printf("get the value is 0\n");
        break;

      }
      else{
        zero_flag=0;
      }
      if(find_div_mid>(fdiv_all-1))
      break;
    }

  if(zero_flag==0)
  {
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);
    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    ret = fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  else
  {
    printf("------------------------------------------------------------------\n");
  }
  }
//      printf("the buf is %s\n",buf);
  return 0;
}
static int eval(int p,int q)
{
  if (p > q) {
    return -1;
  }
  else if (p == q) {
      return (int)(buf[p]-0x30);
  }
  else if(q-p==1&&buf[p]=='-')
  {
      return -(int)(buf[p]-0x30);
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
        char op_type = buf[op_position];
        
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
        if (buf[i] == '(') count++;
        else if (buf[i] == ')') count--;
        if (count < 0) return false; // 括号不匹配
    }
    if(buf[p]=='('&&buf[q]==')')
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
    if(buf[i]=='(')
    {
      parenthesis_level++;
    }
    else if(buf[i]==')')
    {
      parenthesis_level--;
    }

    if(parenthesis_level==0)
    {
      if((buf[i]=='+'||buf[i]=='-')&&i!=0)
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
    if(buf[i]=='(')
    {
      parenthesis_level++;
    }
    else if(buf[i]==')')
    {
      parenthesis_level--;
    }

    if(parenthesis_level==0)
    {
      if(buf[i]=='*'||buf[i]=='/')
      {
      op_position=i;
      break;
      }
    }
  }    
  }
  return op_position;
}