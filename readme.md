# 设计目标
开发一个C语言编译器，实现将C语言的源代码转换成mips汇编语言的目标代码。

一个简单编译器大概可以分为以下几个部分：词法分析，语法分析，中间代码生成，汇编语言生成四个部分，如果有时间可以再加上优化。

# 词法分析
##  介绍
词法分析的目的就是让程序知道哪些字符组合成一个词，并且知道这个词是什么意思。

因为源代码对于编译器来说就是一堆乱七八糟的字符，编译器并不知道哪些字符是组成一个单词，哪些字符应该分开，词法分析就是教给你的编译器如何去分词，并且把分好的词存在一张表中，给语法分析使用。

举个例子，比如源程序中有一段代码是这样子：int a;

那么编译器读入的时候识别到的是这样子的：○1i ○2n ○3t ○4(空格) ○5a ○6; 共六个字符，但是它并不知道该怎么划分，所以你要制定一些规则来告诉它怎么去划分，比如你可以规定：当i n t (空格)四个字符出现的时候，这就是一个整形变量声明的标识符，当一个字符a单独出现的时候这就可能是一个变量名或者函数名等等。

## 词法设计
#define INT_SYM 1 // int
#define CHAR_SYM 2 // char
#define FLOAT_SYM 3 // float
#define DOUBLE_SYM 4 // double
#define BOOL_SYM 5 // bool
#define VOID_SYM 6 // void
#define CONST_SYM 7 // const
#define IF_SYM 8 // if
#define ELSE_SYM 9 // else
#define FOR_SYM 10 // for
#define WHILE_SYM 11 // while
#define DO_SYM 12 // do
#define PRINTF_SYM 13 // print
#define SCANF_SYM 14 // scanf
#define RETURN_SYM 15 // return
#define MAIN_SYM 16 // main
#define IDENTIFIER_SYM 17 // indentifier
#define ADD_SYM 18 // +
#define SUBTRACT_SYM 19 // -
#define MULTIPLY_SYM 20 // *
#define DIVIDE_SYM 21 // /
#define LESS_SYM 22 // <
#define GREATER_SYM 23 // >
#define LESSEQUAL_SYM 24 // <=
#define GREATEREQUAL_SYM 25 // >=
#define EQUAL_SYM 26 // ==
#define NOTEQUAL_SYM 27 // !=
#define ASSIGN_SYM 28 // =
#define COMMA_SYM 29 // ,
#define SEMICOLON_SYM 30 // ;
#define COLON_SYM 31 // :
#define SINGALQUOTATION_SYM 32 // '
#define DOUBLEQUOTATION_SYM 33 // "
#define LEFTSMALLBRACKET_SYM 34 // (
#define RIGHTSMALLBRACKET_SYM 35 // )
#define LEFTMIDDLEBRACKET_SYM 36 // [
#define RIGHTMIDDLEBRACKET_SYM 37 // ]
#define LEFTBIGBRACKET_SYM 38 // {
#define RIGHTBIGBRACKET_SYM 39 // }
#define NUMBER_SYM 40 // number
#define STRING_SYM 41 // string
#define CHARACTER_SYM 42 // character
#define ERROR 0 // error

PS：词法设计就是给每一个划分好的词打上一个标签，方便之后语法分析时识别和使用。

## 函数说明
void readSourceFile()//读源代码  
void outSourceFile()//输出源代码  
void lex_init()//词法分析初始化  
void lex_run()//运行词法分析  
void chRecord()//记录当前位置  
void chRollBack()//回滚到记录处  
void get_char()//读一个字符  
bool isSpace()//判断是不是空格  
bool isTab()//判断是不是tab  
bool isEnter()//判断是不是回车  
bool isDigit()//判断是不是数字  
bool isZeroDigit()//判断是不是0  
bool isNotZeroDigit()//判断是不是非零数字  
bool isLetter()//判断是不是字母  
bool isCharacter() //判断是不是字符  
bool isAdd()//判断是不是加号  
bool isSubtract()//判断是不是减号  
bool isMultiply()//判断是不是乘号  
bool isDivide()//判断是不是除号  
bool isMod()//判断是不是百分号  
bool isLess()//判断是不是小于号  
bool isGreater()//判断是不是大于号  
bool isNot()//判断是不是非号  
bool isAssign()//判断是不是等号  
bool isComma()//判断是不是逗号  
bool isSemicolon()//判断是不是分号  
bool isColon()//判断是不是冒号  
bool isSingleQuotion()//判断是不是单引号  
bool isDoubleQuotion()//判断是不是双引号  
bool isLeftSmallBracket()//判断是不是左小括号  
bool isRightSmallBracket()//判断是不是右小括号  
bool isLeftMiddleBracket()//判断是不是左中括号  
bool isRightMiddleBracket()//判断是不是右中括号  
bool isLeftBigBracket()//判断是不是左大括号  
bool isRightBigBracket()//判断是不是右大括号  
bool isExclamation()//判断是不是叹号  
bool isDot()//判断是不是小数点  
void get_symbol_lex()//读一个单词  
# 语法分析
## 介绍
语法分析的输入就是词法分析生成的那张表，如果说词法分析的目的是让编译器知道哪些字符组成一个词并且知道这个词是什么意思，那么语法分析的作用就是让编译器知道哪些词组成一句话并且知道这句话是什么意思。  
依然举上面那个例子，原程序中有一段代码是：int a;
这段代码经过词法分析后就变成了：
- int（整形变量声明的标识符）
- a（变量名或者函数名）
- ；（分号）  

然后你就要制定一些文法来告诉编译器当它遇到这些单词的时候该怎样去分析，怎去去理解这些单词的含义，以及这句话的含义。比如你可以规定当语法分析读到一个int的时候就说明这句话可能是变量声明也可能是函数声明，然后又读到一个a，因为a既可能是函数名也可能是变量名所以这句话的含义还是没办法确定，所以只能继续往后再读一个词，这时编译器读到了一个分号，这样就把函数声明给否定了，只有可能是变量声明。于是编译器就知道了这句话的意思：声明一个int类型的变量，变量名是a。  
换言之，如果a后面跟着的不是分号而是一个左小括号，那么这句话就应该是函数声明而不是变量声明了。  
然后就进入编译器开发一个很关键的地方——文法设计。  
文法设计就是制定规则，告诉编译器该怎么分析这些单词。  
那么问题来了，一个程序可能有成千上万行代码，每行代码都不一样，甚至就一句简单的变量声明int a; int b; int ;c等等无数种可能，那么我们该怎样去制定规则呢？  
这时就要采用递归下降的语法分析方法。每一条规则不是死板的对应一个语句，而是对应一类语句，并且规则之间还可以通过相互嵌套调用，从而可以衍生出更多规则，实现有限的规则包含无数种语句可能。  
举个例子：  
我们可以先设计一条文法：＜标识符＞=＜字母＞｛＜字母＞｜＜数字＞｝
这样<标识符>就可以用来表示所有可能的名称  
然后再设计一条文法：<变量声明> = int <标识符>;  
于是所有的整形变量声明就可以被包含在上述文法之中了。  
PS：实际文法设计可能要更复杂一些，详见下附的我的文法设计。同时关于文法设计的粒度的问题也是因人而异，比如我的<标识符>就是在词法分析过程中完成的，语法分析时是看不到单个字符的，它得到的最小单位就是标识符了。  
PPS：因为词法分析跟语法分析很像，甚至可以说词法分析就是语法分析的基础版，所以把二者结合起来也是ok的。  
## 文法设计
＜加法运算符＞  ::= +｜-  
＜乘法运算符＞  ::= *｜/  
＜关系运算符＞  ::=  <｜<=｜>｜>=｜!=｜==  
＜字母＞   ::= ＿｜a｜．．．｜z｜A｜．．．｜Z  
＜数字＞   ::= ０｜＜非零数字＞  
＜非零数字＞  ::= １｜．．．｜９  
＜字符＞    ::=  '＜加法运算符＞'｜'＜乘法运算符＞'｜'＜字母  ＞'｜'＜数字＞  '  
＜字符串＞   ::=  "｛十进制编码为32,33,35-126的ASCII字符｝"  
＜程序＞    ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值  
＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}  
＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}  | float＜标识符＞＝＜实数＞{,＜标识符＞＝＜实数＞} | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}  
＜无符号整数＞  ::= ＜非零数字＞｛＜数字＞｝    
//＜整数＞        ::= ［＋｜－］＜无符号整数＞｜０  
＜整数＞        ::= number  
＜小数部分＞    ::= ＜数字＞｛＜数字＞｝｜＜空＞  
＜实数＞        ::= ［＋｜－］＜整数＞[.＜小数部分＞]  
＜标识符＞    ::=  ＜字母＞｛＜字母＞｜＜数字＞｝  
＜声明头部＞   ::=  int＜标识符＞ |float ＜标识符＞|char＜标识符＞  
＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}  
＜变量定义＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’ )}  
＜可枚举常量＞   ::=  ＜整数＞|＜字符＞  
＜类型标识符＞      ::=  int | float | char  
＜有返回值函数定义＞  ::=  ＜声明头部＞‘(’＜参数＞‘)’ ‘{’＜复合语句＞‘}’  
＜无返回值函数定义＞  ::= void＜标识符＞‘(’＜参数＞‘)’‘{’＜复合语句＞‘}’  
＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞  
＜参数＞    ::= ＜参数表＞  
＜参数表＞    ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞  
＜主函数＞    ::= void main‘(’‘)’‘{’＜复合语句＞‘}’  
＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}  
＜项＞     ::= ＜因子＞{＜乘法运算符＞＜因子＞}  
＜因子＞    ::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]|<标识符>.<标识符>｜＜有返回值函数调用语句＞|‘(’＜表达式＞‘)’｜＜整数＞|＜实数＞|＜字符＞  
＜语句＞    ::= ＜条件语句＞｜＜循环语句＞|＜情况语句＞| ‘{’＜语句列＞‘}’｜＜读语句＞;｜＜写语句＞;｜＜返回语句＞;｜＜赋值语句＞;｜＜有返回值函数调用语句＞;  |＜无返回值函数调用语句＞;｜＜空＞;  
＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞  
＜条件语句＞  ::=  if ‘(’＜条件＞‘)’＜语句＞［else＜语句＞］  
＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为  
＜循环语句＞   ::=  while ‘(’＜条件＞‘)’＜语句＞  
＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞＜缺省＞ ‘}’  
＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}  
＜情况子语句＞  ::=  case＜可枚举常量＞：＜语句＞  
＜缺省＞   ::=  default : ＜语句＞|＜空＞  
＜有返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’  
＜无返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’  
＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞  
＜语句列＞   ::= ｛＜语句＞｝  
＜读语句＞    ::=  scanf ‘(’＜标识符＞{,＜标识符＞}‘)’  
＜写语句＞    ::= printf ‘(’ ＜字符串＞,＜表达式＞ ‘)’ | printf ‘(’＜字符串＞ ‘)’| printf ‘(’＜表达式＞‘)’  
＜返回语句＞   ::=  return[‘(’＜表达式＞‘)’]   
PS：文法大部分都是借鉴别人的博客，然后自己进行了一点儿修改。  
以上文法基本可以实现C语言的大部分功能，除了指针、结构体、多维数组、位运算、三目运算符。  
这些没实现的功能理论上也可以搞出来，我最后其实想把结构体和多维数组弄出来的，大概思路也有了，但是因为进入考试月并且暑假要去实习了，就没时间搞了。现在刚刚实习回来再回头看三四个月前自己写的代码，一脸懵逼这都是些啥。。。  
PPS：语法分析应该算是整个编译器开发过程中最困难的一处了，因为文法设计中的每条文法映射到编译器开发中都是一个函数，一个函数短则十几行，长则一二百行，实际开发过程中，文法的嵌套就是函数的调用，而且是可能多达十几层的递归调用，要让函数间顺利调用而不出现bug是非常非常难的一件事，很多时候某个函数多读了一个括号就会让你头疼一下午。。。  
整个编译器开发完了之后，包括优化部分都写完了，我还是能时不时地发现一两个语法分析的bug，要想尽量减少bug除了好好设计文法之外，只有使用大量地测试用例去测试，并且要在程序的各个地方都加上打印以便输出编译器运行日志。  
## 函数说明
void get_symbol()//读一个symbol  
void record() /// 记录当前位置  
void rollBack() /// 回退到之前的位置  
void readAhead() /// 预读一个symbol  
void syn_init()//语法分析初始化  
void syn_run()//运行语法分析  
void program_lex()//＜程序＞  
void constDeclaration_lex()//＜常量说明＞  
void constDefinition_lex()///TODO//＜常量定义＞  
void headDeclaration_lex()///TODO//＜声明头部＞  
void varDeclaration_lex()//＜变量说明＞  
void varDefinition_lex()//＜变量定义＞  
void parameter_lex()//＜参数＞  
void parameterTable_lex()//＜参数表＞  
void funWithReturnDefinition_lex() /// TODO : midcode//＜有返回值函数定义＞  
void funWithoutReturnDefinition_lex() /// TODO :midcode//＜无返回值函数定义＞  
void compound_lex()//＜复合语句＞  
void mainFun_lex()//＜主函数＞  
void expression_lex()///TODO [+|-]//＜表达式＞  
void factor_lex()/// TODO//＜因子＞  
void sentence_lex()//＜语句＞  
void sentenceColumn_lex()//＜语句列＞  
void assignSentence_lex()//＜赋值语句＞  
void conditionSentence_lex()//＜条件语句＞  
void condition_lex()//＜条件＞  
void forAssignSentence_lex()//＜for赋值语句＞  
void loopSentence_lex()//＜循环语句＞  
void situationSentence_lex()//＜情况语句＞  
void situationSubSentence_lex()//＜情况子语句＞  
void default_lex()//＜缺省＞  
void funWithReturnCall_lex()//＜有返回值函数调用语句＞  
void funWithoutReturnCall_lex()//＜无返回值函数调用语句＞  
void valueParameterTable_lex()//＜值参数表＞  
void scanf_lex()//＜读语句＞  
void printf_lex()//＜写语句＞  
void return_lex()//＜返回语句＞  
void add_lex()//＜加法运算符＞  
void multiply_lex()//＜乘法运算符＞  
void relation_lex()//＜关系运算符＞  
void letter_lex()//＜字母＞  
void digit_lex()//＜数字＞  
void notZeroDigit_lex()// ＜非零数字＞  
void character_lex()//＜字符＞  
void string_lex()//＜字符串＞  
void unsignedInt_lex()//＜无符号整数＞  
void int_lex()//＜整数＞  
void fractionalPart_lex()//＜小数部分＞  
void realNumber_lex()//＜实数＞  
void identifier_lex()//＜标识符＞  
void enumerableConst_lex()///TODO ＜可枚举常量＞??//＜可枚举常量＞  
void typeIdentifier_lex()//＜类型标识符＞  
# 中间代码生成
## 介绍
当编译器语法分析能成功跑通时，整个编译器的开发工作已经可以说是完成一大半了，后面的中间代码生成跟汇编语言生成相对来说比较简单。  
中间代码生成的作用就是把语法分析的结果展示出来，这里我使用的是四元式的形式。四元式的作用就是在C语言的源代码和汇编语言的目标代码之间架一座桥梁，使之翻译起来更容易一点，其实理论上跳过四元式部分，直接在语法分析的过程中生成汇编应该也是可以的，但是难度可能就会大一些。  
中间代码生成这一部分的代码量很少，因为大部分内容都是结合语法分析完成的。当你把语法分析写完后，只需要在语法分析代码的每一个函数中加上一两句输出打印一下四元式就行。  
## 四元式设计  
源代码 | 四元式 
- | -
int a;                 | < int, , ,a >  
char a;				   | < char, , ,a >  
int a[10];			    |< int, 10, , a >
char a[10];				|< char, 10, , a >
const int a = 1;		|< const, int, 1, a >
const char a = 2;		|< const, char, 2, a >
int a();				|< func, int, , a >
char a();				|< func, char, , a >
void a();				|< func, void, , a >
fun(int a,)				|< para, int, , a >
fun(char a,)			|< para, char, , a >
a = b;					|< =, b, , a >
a[i] = b;				|< []=, b, i, a >
a = b[i];				|< =[], b, i, a >
(int|char) fun();		|< call, , , fun >
(void) fun();			|< call, , , >
fun(a);					|< callpara, int|char, , a >
if(a < b)				|< blt, a, b, label >
if(a <= b)				|< ble, a, b, label >
if(a > b)				|< bgt, a, b, label >
if(a >= b)				|< bge, a, b, label >
if(a == b)				|< beq, a, b, label >
if(a != b)				|< bne, a, b, label >
jmp	label		 		|< j, , , label>
set label				|< lab, , , label >
return ;				|< ret, , , >
return a;				|< ret, , , a >
scanf(a);				|< scf, , , a >
print("a",b);			|< prf, a, b,  >
a + b					|< +, a, b, >
a - b					|< -, a, b, >
a * b					|< *, a, b, >
a / b					|< /, a, b, >  
PS：这里说的比较抽象，若不理解建议直接阅读源代码，我当时就是通过读别人源码搞清楚的。同时这部分还是有一些理论知识要去学习的，比如抽象语法树，因为语法分析的过程其实就是走语法树的过程，四元式就是语法树中一些关键节点的输出，再比如双重for循环该如何拆解，if else语句该怎么转化等等。  
## 函数说明
void genMidCode(string op, string src1, string src2, string res)//生成四元式  
string genVar()//生成临时变量  
string genLabel()//生成label  
string genString()//生成全局字符串  
# 汇编语言生成
## 介绍
汇编语言生成的输入就是四元式，这时编译器已经不管源码是什么样子了，它能看到的只有四元式。该部分就是将四元式转化成汇编语言，这里相对来说也是比较容易的，因为四元式已经是很接近汇编语言的存在了，这部分工作的难点主要在于要搞懂操作系统中汇编语言是怎样运行的，尤其是内存堆栈分配的问题。  
以mips汇编语言举例，每一个函数的每次调用都一个属于自己的栈帧，栈帧的边界由sp和fp两个寄存器确定，其中sp存的是栈顶（内存低地址），fp存的是栈底（内存高地址），栈空间的分配是从高地址到低地址来分配的。但是因为寄存器数量很少，以mips为例只有32个，不可能每次函数调用时都给它分配两个寄存器来存储边界，所以函数调用时只能共享这sp和fp这两个寄存器。但是每调用一个新的函数，sp和fp就被赋予了一个新值，这样的话上一个函数栈帧边界的值就丢失了，新函数调用完后返回就函数程序运行就会出错。为了解决这个问题，每次在调用新函数时就要把sp和fp的值存到内存中，当新函数调用完后再从内存中将旧的sp和fp的值取回来，这样理论上就可以实现无数次函数的递归调用而不会带来内存冲突。  
但是实际上每次调用函数时只需要将旧的fp的值存起来就够了，因为新的fp的值就是就得sp的值，所以新函数调用完之后就直接将fp寄存器的值赋值给sp即可将sp恢复。  
同时，每次调用新函数要存入内存的不仅仅有fp，还有旧函数的局部变量和新函数的参数以及ra寄存器的值都要通过这种方式存入内存，至于存到新函数的栈帧内还是存入旧函数的栈帧内就可以按照自己的想法来了，只要恢复时按照存入的规则规则恢复就可以了。  
至于局部变量和参数为什么也要存入，这是因为汇编语言在执行时是通过fp的偏移量来存取内存中变量的值，对于每个函数，他能使用的只有自己栈帧里的变量和全局变量，所以当调用新函数时，必须将他能使用的旧函数的局部变量和他自己的参数存入他的栈帧，不然新函数就找不到这些变量。而全局变量是存在.data内，使用时可以直接通过la命令和变量名获取到他在内存中的地址，所以就不需要入栈了。  
 
PS：不同人开发的编译器fp，局部变量，参数，ra等入栈的顺序可能会不太一样，但是只要恢复的顺序跟入栈的顺序匹配就可以了（一般来说顺序是刚好相反的）  
到此为止，只要你编译器转化出的汇编代码能成功运行并且结果还正确，那么编译器就算开发出来了。当然了，这种程度的编译器在功能、性能、可靠性等方面都还有很大的进步空间。  
## mips指令集
R-type | op | rs | rt | rd | shamt | func | ？ 
- | - | - | - | - | - | - | -
add | 000000 | rs | rt | rd | 00000 | 100000 |  rd <- rs + rt   ；其中rs＝$2，rt=$3, rd=$1
addu | 000000 | rs | rt | rd | 00000 | 100001 |  rd <- rs + rt   ；其中rs＝$2，rt=$3, rd=$1,无符号数
sub | 000000 | rs | rt | rd | 00000 | 100010 |  rd <- rs - rt   ；其中rs＝$2，rt=$3, rd=$1
subu | 000000 | rs | rt | rd | 00000 | 100011 |  rd <- rs - rt   ；其中rs＝$2，rt=$3, rd=$1,无符号数
and | 000000 | rs | rt | rd | 00000 | 100100 |  rd <- rs & rt   ；其中rs＝$2，rt=$3, rd=$1
or | 000000 | rs | rt | rd | 00000 | 100101 |  rd <- rs | rt   ；其中rs＝$2，rt=$3, rd=$1
xor | 000000 | rs | rt | rd | 00000 | 100110 |  rd <- rs xor rt   ；其中rs＝$2，rt=$3, rd=$1(异或）
nor | 000000 | rs | rt | rd | 00000 | 100111 |  rd <- not(rs | rt)   ；其中rs＝$2，rt=$3, rd=$1(或非）
slt | 000000 | rs | rt | rd | 00000 | 101010 |  if (rs < rt) rd=1 else rd=0 ；其中rs＝$2，rt=$3, rd=$1
sltu | 000000 | rs | rt | rd | 00000 | 101011 |  if (rs < rt) rd=1 else rd=0 ；其中rs＝$2，rt=$3, rd=$1  (无符号数）
sll | 000000 | 00000 | rt | rd | shamt | 000000 |  rd <- rt << shamt  ；shamt存放移位的位数，也就是指令中的立即数，其中rt=$2, rd=$1
srl | 000000 | 00000 | rt | rd | shamt | 000010 |  rd <- rt >> shamt ；(logical) ，其中rt=$2, rd=$1
sra | 000000 | 00000 | rt | rd | shamt | 000011 |  rd <- rt >> shamt  ；(arithmetic) 注意符号位保留 其中rt=$2, rd=$1
sllv | 000000 | rs | rt | rd | 00000 | 000100 |  rd <- rt << rs  ；其中rs＝$3，rt=$2, rd=$1
srlv | 000000 | rs | rt | rd | 00000 | 000110 |  rd <- rt >> rs  ；(logical)其中rs＝$3，rt=$2, rd=$1
srav | 000000 | rs | rt | rd | 00000 | 000111 |  rd <- rt >> rs  ；(arithmetic) 注意符号位保留 其中rs＝$3，rt=$2, rd=$1
jr | 000000 | rs | 00000 | 00000 | 00000 | 001000 |  PC <- rs

I-type | op | rs | rt | immediate | ？
- | - | - | - | - | -
addi | 001000 | rs | rt | immediate |  rt <- rs + (sign-extend)immediate ；其中rt=$1,rs=$2
addiu | 001001 | rs | rt | immediate |  rt <- rs + (zero-extend)immediate ；其中rt=$1,rs=$2
andi | 001100 | rs | rt | immediate |  rt <- rs & (zero-extend)immediate ；其中rt=$1,rs=$2
ori | 001101 | rs | rt | immediate |  rt <- rs | (zero-extend)immediate ；其中rt=$1,rs=$2
xori | 001110 | rs | rt | immediate |  rt <- rs xor (zero-extend)immediate ；其中rt=$1,rs=$2
lui | 001111 | 00000 | rt | immediate |  rt <- immediate*65536 ；将16位立即数放到目标寄存器高16 位，目标寄存器的低16位填0
lw | 100011 | rs | rt | immediate |  rt <- memory[rs + (sign-extend)immediate] ；rt=$1,rs=$2
sw | 101011 | rs | rt | immediate |  memory[rs + (sign-extend)immediate] <- rt ；rt=$1,rs=$2
beq | 000100 | rs | rt | immediate |  if (rs == rt) PC <- PC+4 + (sign-extend)immediate<<2 
bne | 000101 | rs | rt | immediate |  if (rs != rt) PC <- PC+4 + (sign-extend)immediate<<2 
slti | 001010 | rs | rt | immediate |  if (rs <(sign-extend)immediate) rt=1 else rt=0 ；   其中rs＝$2，rt=$1
sltiu | 001011 | rs | rt | immediate |  if (rs <(zero-extend)immediate) rt=1 else rt=0 ；  其中rs＝$2，rt=$1

J-type | op | address | ？ 
- | - | - | -
j | 000010 | address |  PC <- (PC+4)[31..28],address,0,0   ；address=10000/4
jal | 000011 | address |  $31<-PC+4；PC <- (PC+4)[31..28],address,0,0   ；address=10000/
## 函数说明
void asm_init()//汇编语言生成初始化  
void saveLocVar()//保存局部变量  
void recoverLocVar()//恢复局部变量  
void outLocVar()//输出局部变量  
void asm_out(string op_mips, string var1, string var2, string var3)//输出汇编语言  
bool isNumber(string str)//判断是否是数字  
bool isString(string str)//判断是否是字符串  
bool isReg(string str)//判断是否是寄存器  
void int_mips()//< int, , , a >  
void char_mips()//< char, , , a >  
void intArry_mips()//< int, 10, , a >  
void charArry_mips()//< char, 10, , a >  
string shamt(int a, string b) // 计算地址偏移  
void para_mips()//< para, int, , a >  
void func_mips()//< func, int, , a >  
void return_mips()//< ret, , , >  
void exits_mips()//<exit, , a>  
void add_mips()//< +, a, b, >  
void sub_mips()//< -, a, b, >  
void mul_mips()//< *, a, b, >  
void div_mips()//< /, a, b, >  
void ass_mips()//< =, b, , a >  
void assArry_mips()//< []=, b, i, a >  
void call_mips()//< call, , , fun >  
void callpara_mips()//< callpara, int|char, , a >  
void lab_mips()//< lab, , , label >  
void jmp_mips()//< j, , , label>  
void blt_mips()//< blt, a, b, label >  
void ble_mips()//< ble, a, b, label >  
void bgt_mips()//< bgt, a, b, label >  
void bge_mips()//< bge, a, b, label >  
void beq_mips()//< beq, a, b, label >  
void bne_mips()//< bne, a, b, label >  
void scf_mips()//< scf, , , a >  
void prf_mips()//< prf, a, b,  >  
void asm_run()//运行  
# 优化
## 介绍
现在的编译器在将源代码翻译成汇编时，对变量进行修改大概需要进行如下几步操作：  
若是全局变量：
获取全局变量的地址存到寄存器中 ->
通过地址获取该变量的值存到寄存器中 ->
修改该寄存器的值 ->
再次获取全局变量的地址存到寄存器中 ->
将修改后的值写回内存  
若是局部变量：
通过fp和偏移量取到该变量的值存到寄存器中 ->
修改该寄存器的值 ->
将修改后的值写回内存  
所以不管是全局变量还是局部变量每次修改时都要经过 读内存-修改-写内存 三个过程，当同一变量在短时间内多次使用时，就会导致大量地内存读写操作，会严重降低编译器性能，对于这个问题的一个解决方案是：在第一次修改变量时，将变量从内存读到寄存器后先不写回，之后再修改该变量时就不需要重复读内存而直接修改寄存器就好，知道最后一次修改该变量之后再写回内存。这样只进行了两次访存操作，会大大加开编译器速度。  
但是这种方法带来的问题就是，因为寄存器的数量有限而且很少，当一个函数内变量很多时没办法为每个变量都分配一个寄存器让他一直占用，在寄存器被占满但是新的变量又需要寄存器时就需要将已经占有寄存器的某个变量写回内存，然后将寄存器腾出来给新的变量。那么将哪个变量写回内存呢？这个问题需要一定的算法来帮助做决策，比如引用计数法和图着色法等。我使用的是引用计数法，这个方法简单概括就是每次使用变量时都给它记个数，当寄存器不够用而要写回某个变量时，每次都写回次数最少的哪一个。这虽然不一定是最优的选择，但总体来说还是比较优秀的一个方法。  
## 函数说明
void optimize_init()//优化初始化    
bool isTarget(int index)//判断是否是目标语句  
bool isTransfer(int index)//判断是否是转移语句  
bool isBlockEntrance(int index)//判断是否是基本块入口  
bool isBlockExit(int index)//判断是否是基本块出口  
void optimize_blcDiv()// 划分基本快  
void optimize_dataStream()//数据流分析  
bool cmp(const VAR a, const VAR b)//按引用次数排序  
bool isVar(string s)//判断石是否是变量  
void optimize_regAllocation()//寄存器分配  
void optimize_regGraint(int index)//寄存器授权  
void optimize_regRevoke(int index)//寄存器回收  
void optimize_varReplace(int index) // 用寄存器替换变量  
void genAsm(string op)//生成汇编语言  
void optimize_outAll()//输出分配后的结果  
void optimize_run()//运行优化  
