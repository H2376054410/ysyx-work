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

#include <common.h>
#include "monitor/sdb/sdb.h"
void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();
// #define test 1
int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif
// int *ptr = NULL;
// *ptr = 42;  // 对空指针解引用，触发段错误
  /* Start engine. */
  #ifdef test
  FILE *file;
  char buffer[256];
  bool *suc=malloc(sizeof(bool));
  // 打开文件
  file = fopen("/home/hwx/ysyx-workbench/nemu/tools/gen-expr/input", "r");
  if (file == NULL) {
      perror("Failed to open file");
  }
  // 逐行读取文件内容
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    char *token;
    printf("read line data is %s\n",buffer);
    // 第一次调用 strtok(),读取计算结果
    token = strtok(buffer," ");
    assert(token!=NULL);
    int res=atoi(token);//转化计算结果
    //第二次转化
    token = strtok(NULL," ");
    char *cmd=token+strlen(token)+1;
    *cmd='\0';
    printf("expr is %s\n",token);
    int result=(unsigned)expr(token,suc);
    printf("res is %d result is %d\n",res,result);
    assert((res-result)<=1||(res-result)>=-1);
  }
  // 关闭文件
  fclose(file);
  printf("over\n");
#endif
  engine_start();

  return is_exit_status_bad();
}
