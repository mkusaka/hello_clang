#include "tim.h"

int pos = 0;

int consume(int ty) {
  Token *token = tokens->data[pos];
  if (token->ty != ty)
    return 0;
  pos++;
  return 1;
}

Vector *tokens;

// prototype宣言
// 関数は使用する前に宣言されている必要がある
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *term();

// nodeを新規作成する
Node *new_node(int ty, Node *lhs,Node *rhs) {
  Node *node = malloc(sizeof(Node));
  node->ty = ty;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

// numのnodeを新規作成する
Node *new_node_num(int val) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_NUM;
  node->val = val;
  return node;
}

// expr = equality と対応
Node *expr() {
  return equality();
}

// 等号
// equality = relational ("==" relational | "!=" relational)* と対応
Node *equality() {
  Node *node = relational();

  for (;;) {
    if (consume(TK_EQ))
      node = new_node(TK_EQ, node, relational());
    else if (consume(TK_NE))
      node = new_node(TK_NE, node, relational());
    else
      return node;
  }
}

// 大小関係
// relational = add ("<" add | "<=" add | ">" add | ">=" add)* と対応
Node *relational() {
  Node *node = add();

  for (;;) {
    if (consume(TK_LE))
      node = new_node(TK_LE, node, add());
    else if (consume(TK_GE))
      // genで TK_LE の逆操作を使用するために順序を変更
      node = new_node(TK_GE, add(), node);
    else if (consume('<'))
      node = new_node('<', node, add());
    else if (consume('>'))
      // genで TK_LE の逆操作を使用するために順序を変更
      node = new_node('>', add(), node);
    else
      return node;
  }
}

// 足し算、引き算
// add = mul ("+" mul | "-" mul)* と対応
Node *add() {
  Node *node = mul();

  for (;;) {
    if (consume('+'))
      node = new_node('+', node, mul());
    else if (consume('-'))
      node = new_node('-', node, mul());
    else
      return node;
  }
}

// 乗算、除算
// mul = unary ("*" unary | "/" unary)* と対応
Node *mul() {
  Node *node = unary();

  for (;;) {
    if (consume('*'))
      node = new_node('*', node, unary());
    else if (consume('/'))
      node = new_node('/', node, unary());
    else
      return node;
  }
}

// 単項演算子
// unary = ("+" | "-")? term と対応
Node *unary() {
  if (consume('+'))
    return term();
  if (consume('-'))
    return new_node('-', new_node_num(0), term());
  return term();
}

// 括弧、数字
// term = num | "(" expr ")" と対応
Node *term() {
  Token *token = tokens->data[pos];

  if (consume('(')) {
    Node *node = expr();
    if (!consume(')'))
      error_at(token->input, "閉じカッコに対応するカッコがありません");

    return node;
  }

  // カッコでなければ数値
  if (token->ty == TK_NUM) {
    Token *next_token = tokens->data[pos++];
    return new_node_num(next_token->val);
  }

  error_at(token->input, "数値でもカッコでもないトークン");
}

void tokenize(char *user_input) {
  char *p = user_input;

  while (*p) {
    // 空白はスキップ
    if (isspace(*p)) {
      p++;
      continue;
    }

    Token *token = malloc(sizeof(Token));

    if (strncmp(p, "==", 2) == 0) {
      token->ty = TK_EQ;
      token->input = p;
      vec_push(tokens, token);
      p += 2;
      continue;
    }

    if (strncmp(p, "!=", 2) == 0) {
      token->ty = TK_NE;
      token->input = p;
      vec_push(tokens, token);
      p += 2;
      continue;
    }

    if (strncmp(p, "<=", 2) == 0) {
      token->ty = TK_LE;
      token->input = p;
      vec_push(tokens, token);
      p += 2;
      continue;
    }

    if (strncmp(p, ">=", 2) == 0) {
      token->ty = TK_GE;
      token->input = p;
      vec_push(tokens, token);
      p += 2;
      continue;
    }

    // `+` または `-` であればtokensに格納する
    if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')' || *p == '<' || *p == '>') {
      token->ty = *p;
      token->input = p;
      vec_push(tokens, token);
      p++;
      continue;
    }

    if (isdigit(*p)) {
      token->ty = TK_NUM;
      token->input = p;
      token->val = strtol(p, &p, 10);
      vec_push(tokens, token);
      continue;
    }

    error_at(p, "トークナイズできません");
  }

  Token *token = malloc(sizeof(Token));
  token->ty = TK_EOF;
  token->input = p;
  vec_push(tokens, token);
}
// utils
// 型をチェックして、期待した型であればトークンを一つ進めて真を返す
