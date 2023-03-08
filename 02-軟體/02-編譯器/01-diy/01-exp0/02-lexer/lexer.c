#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TMAX 10000000
#define SMAX 100000 //意義不明

//偵測宣告的東西屬於甚麼型態
enum { Id, Int, Keyword, Literal, Char };
char *typeName[5] = {"Id", "Int", "Keyword", "Literal", "Char"};

char code[TMAX];
char strTable[TMAX], *strTableEnd=strTable;
char *tokens[TMAX];
int tokenTop=0;
int types[TMAX];

//偵測宣告變數
#define isDigit(ch) ((ch) >= '0' && (ch) <='9')
#define isAlpha(ch) (((ch) >= 'a' && (ch) <='z') || ((ch) >= 'A' && (ch) <= 'Z'))

//讓text變數可以讀取File中的所有字，並且記住File的長度
int readText(char *fileName, char *text, int size) {
  FILE *file = fopen(fileName, "r");
  int len = fread(text, 1, size, file);
  text[len] = '\0';
  fclose(file);
  return len;
}

/* strTable =
#\0include\0"sum.h"\0int\0main\0.....
*/

//P是指向當前機器讀取到哪一個字元，根據規則判斷P必須讀取到哪邊才能停
char *next(char *p) {
  while (isspace(*p)/*isspace讀取該字是否為空白 如果是空白會回傳一個False值*/) p++;

  //start宣告意義為?  因為P會隨著機器的讀取改變位置，所以透過start紀錄P一開始是從哪裡讀取的
  char *start = p; //         include "sum.h"
                   //         ^      ^
                   //  start= p      p
  
  int type; //判斷讀取到的東西屬於甚麼型態

  if (*p == '\0') return NULL;  //如果是\0表示結尾就break

  //如果是雙引號代表內容是字串，所以P必須讀取直到遇到下一個雙引號為止
  if (*p == '"') {  
    p++;
    while (*p != '"') p++;
    p++;
    type = Literal; //讀取到的變數為字串型態
  } 
  
  //如果讀取到的是數字，就讀取直到沒有獨到數字，並取type變成Int
  else if (*p >='0' && *p <='9') {
    while (*p >='0' && *p <='9') p++;
    type = Int;
  } 
  
  //檢查是否為變數名稱，變數名稱根據規則第一個不可以是數字，所以63的if可以巧妙規過濾取名規則的問題
  else if (isAlpha(*p) || *p == '_') { // 變數名稱或關鍵字
    while (isAlpha(*p) || isDigit(*p) || *p == '_') p++;
    type = Id;  //將type改成"變數"型態
  } 
  
  //如果是ㄧ些特殊符號EX:+-*/這類型的，就判定為一個單一字元
  else { // 單一字元
    p++;
    type = Char;  //將變數改變成字元型態
  }

  int len = p-start;  //機器讀取資料的長度=P最後讀取的位置扣掉start一開始讀取的位置
  char *token = strTableEnd;  //token存取了strTableEnd這個變數，當strTableEnd變的時候token也會改變
  strncpy(strTableEnd, start, len); //透過strcpy的函式可以把start開始然後長度為len做一個讀取，並把讀取到的東西存在strTable
  strTableEnd[len] = '\0';  //在結尾加上\0
  strTableEnd += (len+1); //不懂，經過測試註解掉也沒差
  types[tokenTop] = type; //將P讀取到的資料的型態存在types陣列中
  tokens[tokenTop++] = token; //將讀取到的資料存在tokens陣列中，然後tokenstop變數再加一
  printf("token=%s\n", token); 
  return p; // 回傳P(一個記憶體位置)告知機器接下來要讀取到哪裡
}

void lex(char *code) {
  char *p = code; //讓P從File中的第一個字開始讀取
  while (1) {
    p = next(p);
    if (p == NULL) break;
  }
}

void dump(char *strTable[], int top) {
  for (int i=0; i<top; i++) {
    printf("%d:%s\n", i, strTable[i]);
  }
}

int main(int argc, char * argv[]) {
  readText(argv[1], code, sizeof(code)-1);
  puts(code);
  lex(code);
  dump(tokens, tokenTop);
}

