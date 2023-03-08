#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

//E = F ([+-]F)*
//F = [0-9] | (E)

int tokenIdx = 0;
char *tokens;

int E();
int F();

void error(char *msg) {
  printf("%s", msg);
  assert(0);
}

// 取得目前字元
char ch() {
  char c = tokens[tokenIdx];
  return c;
}

// 取得目前字元，同時進到下一格
char next() {
  char c = ch();
  tokenIdx++;
  return c;
}

// ex: isNext("+-") 用來判斷下一個字元是不是 + 或 -
int isNext(char *set) {
  char c = ch();
  return (c!='\0' && strchr(set, c)!=NULL); // 透過strchr代入set以及c可以判斷在c這個字串當中是否有我們要尋找的set也就是運算元
}

// 產生下一個臨時變數的代號， ex: 3 代表 t3。
//請仔細注意老師使用nextTemp的時機，他在使用變數t的時候會先透過nextTemp把變數t加一之後才會開始做編譯動作
int nextTemp() {
  static int tempIdx = 0;
  return tempIdx++;
}

// F =  Number | '(' E ')'
int F() {
  int f;
  char c = ch();

  if (isdigit(c)/*檢查C字元是否為十進制整數(只能輸入9以下的整數)*/) { 
    next(); // 回傳現在機器正在讀取的字元，並且把IDX加一
    f = nextTemp(); //因為nextTemp在第38行使用了static，所以tempIDX只會在第一次執行的時候為0。
                    //而nextTemp的目的是為了可以記錄現在使用第幾個變數(執行程式時可以看到t0 t1 t2 ... 就是依靠nexTemp做紀錄的)
    printf("t%d=%c\n", f, c);
  } 
  
  else if (c=='('/*當字元為單括號的時候*/) { // '(' E ')'
    next(); //回傳現在機器正在讀取的字元，並且把IDX加一
    f = E();  //如果遇到單括號的時候，表示括號內必定有一串數字，所以透過E()的遞迴方式讓機器一直讀取直到檢查到另一個單邊括號
    assert(ch()==')');  //如果ch()讀取到的字元是左括號的時候代表我們讀取完一組完整的括號，此時才會執行下面的57行程式碼
    next(); //讀取左括號的下一個字元
  } 
  
  //如果不是上述任一種(+ - 十進位數字)的話。。。
  else {   
    error("F = (E) | Number fail!");
  }
  return f; 
}

// E = F ([+-] F)*
int E() {
  int i1 = F(); //i1紀錄變數t的開始，所以i1在一開始的時候都是t0直到後來77行執行以後i1才開始改變t變數
  while (isNext("+-")/*isNetx可以判斷下一個字元是否是運算元*/) {
    char op=next(); //讓機器開始讀取運算元後面的字元，透過next可以回傳運算元的同時又讓機器讀取到下一個字元
    int i2 = F(); //i2紀錄運算元右邊的數字屬於變數t幾
    int i = nextTemp(); //透過i計算現在使用到哪一個變數(t1 t2 。。。 tn)
    printf("t%d=t%d%ct%d\n", i, i1, op, i2);  //先把第幾個變數t先印出來，再把剛才左邊的數字 + 運算元 + 右邊的數字寫下來
    i1 = i; //如果在做完左邊數字與右邊數字的運算以後還有其他運算要做，i1就會變成下一個變數t(n+1)做為下一次運算時左邊的數字
  }
  return i1;   //之所以要return i1是因為透過56行讓下一次變數t加一
}

//讀取使用主輸入的程式碼
void parse(char *str) {
  tokens = str; //因為token是全域變數並且是指標型態，所以token存著str也就是使用者輸入的字串位址
  E();  //開始進行字串讀取
}

int main(int argc, char * argv[]) {
  printf("argv[0]=%s argv[1]=%s\n", argv[0], argv[1]);
  printf("=== EBNF Grammar =====\n");
  printf("E=F ([+-] F)*\n");
  printf("F=Number | '(' E ')'\n");
  printf("==== parse:%s ========\n", argv[1]);
  parse(argv[1]);
}
