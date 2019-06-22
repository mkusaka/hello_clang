#include "tim.h"

char *user_input; // 入力プログラム

// エラー報告のための関数
void error_at(char *loc, char *msg) {
  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, ""); // pos個の空白表示
  fprintf(stderr, "^ %s\n", msg);
  exit(1);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "引数の個数が正しくありません。\n");
    return 1;
  }

  if (strncmp(argv[1], "-test", 5) == 0) {
    runtest();
    return 0;
  }

  tokens = new_vector();

  tokenize(argv[1]);
  Node *node = expr();

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  gen(node);

  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}

// gcc -o tim tim.c
