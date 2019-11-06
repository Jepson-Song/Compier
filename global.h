
#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <queue>
#include <iomanip>
#include <map>

using namespace std;

extern int state;
const int lex_sta = 1;
const int syn_sta = 2;
const int mid_sta = 3;
const int asm_sta = 4;
const int opt_sta = 5;

/*****************[main]******************/

extern ifstream source;
extern ofstream lexical;
extern ofstream debug;

/*****************************************/


/*****************[lex]*******************/

/**
±êÊ¶·û£º
int char float double bool void
+ - * / <  > <= >= == != =
, ; : ' "
( ) [ ] { }
if else for while do printf scanf return main identifier
number string
const
**/

#define INT_SYMID 1 // int
#define CHAR_SYMID 2 // char
#define FLOAT_SYMID 3 // float
#define DOUBLE_SYMID 4 // double
#define BOOL_SYMID 5 // bool
#define VOID_SYMID 6 // void
#define CONST_SYMID 7 // const
#define IF_SYMID 8 // if
#define ELSE_SYMID 9 // else
#define FOR_SYMID 10 // for
#define WHILE_SYMID 11 // while
#define DO_SYMID 12 // do
#define SWITCH_SYMID 13 // switch
#define CASE_SYMID 14 // case
#define DEFAULT_SYMID 15 // default
#define PRINTF_SYMID 16 // print
#define SCANF_SYMID 17 // scanf
#define RETURN_SYMID 18 // return
#define MAIN_SYMID 19 // main
#define IDENTIFIER_SYMID 20 // indentifier
#define ADD_SYMID 21 // +
#define SUBTRACT_SYMID 22 // -
#define MULTIPLY_SYMID 23 // *
#define DIVIDE_SYMID 24 // /
#define MOD_SYMID 25 // %
#define LESS_SYMID 26 // >
#define GREATER_SYMID 27 // >
#define LESSEQUAL_SYMID 28 // <=
#define GREATEREQUAL_SYMID 29 // >=
#define EQUAL_SYMID 30 // ==
#define NOTEQUAL_SYMID 31 // !=
#define ASSIGN_SYMID 32 // =
#define COMMA_SYMID 33 // ,
#define SEMICOLON_SYMID 34 // ;
#define COLON_SYMID 35 // :
#define DOT_SYMID 36 // .
#define SINGALQUOTATION_SYMID 37 // '
#define DOUBLEQUOTATION_SYMID 38 // "
#define LEFTSMALLBRACKET_SYMID 39 // (
#define RIGHTSMALLBRACKET_SYMID 40 // )
#define LEFTMIDDLEBRACKET_SYMID 41 // [
#define RIGHTMIDDLEBRACKET_SYMID 42 // ]
#define LEFTBIGBRACKET_SYMID 43 // {
#define RIGHTBIGBRACKET_SYMID 44 // }
#define NUMBER_SYMID 45 // not 0 head number
#define STRING_SYMID 46 // string
#define CHARACTER_SYMID 47 // character
#define LETTER_SYMID 48 // letter
#define DIGIT_SYMID 49 // 0~9
#define ZERODIGIT_SYMID 50 // 0
#define NOTZERODIGIT_SYMID 51 // 1~9
#define ZEROHEADNUMBER_SYMID 52 // 0 head number
#define STRUCT_SYMID 53 // struct

#define ERROR 0 // error

typedef struct node
{
    int symID;
    string symbol;
    int indexInFile;
    int indexInLine;
}NODE;
extern vector<NODE> symbolTabel;
extern string keyWord[];
extern string keyWordSym[];
void lex_run();

/******************************************/



/*******************[syn]******************/

/*
add_lex                             ＜加法运算符＞  ::= +｜-
multiply_lex                         ＜乘法运算符＞  ::= *｜/
relation_lex                        ＜关系运算符＞  ::=  <｜<=｜>｜>=｜!=｜==
letter_lex                          ＜字母＞   ::= ＿｜a｜．．．｜z｜A｜．．．｜Z
digit_lex                           ＜数字＞   ::= ０｜＜非零数字＞
notZeroDigit_lex                    ＜非零数字＞  ::= １｜．．．｜９
character_lex                       ＜字符＞    ::=  '＜加法运算符＞'｜'＜乘法运算符＞'｜'＜字母＞'｜'＜数字＞'
string_lex                          ＜字符串＞   ::=  "｛十进制编码为32,33,35-126的ASCII字符｝"

program_lex                         ＜程序＞    ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
constDeclaration_lex                ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
constDefinition_lex                 ＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
                                                         | float＜标识符＞＝＜实数＞{,＜标识符＞＝＜实数＞}
                                                         | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
unsignerInt_lex                     ＜无符号整数＞  ::= ＜非零数字＞｛＜数字＞｝
int_lex                             //＜整数＞        ::= ［＋｜－］＜无符号整数＞｜０
									＜整数＞        ::= number
fractionalPart_lex                  ＜小数部分＞    ::= ＜数字＞｛＜数字＞｝｜＜空＞
realNumber_lex                      ＜实数＞        ::= ［＋｜－］＜整数＞[.＜小数部分＞]
identifier_lex                      ＜标识符＞    ::=  ＜字母＞｛＜字母＞｜＜数字＞｝
headDeclaration_lex                 ＜声明头部＞   ::=  int＜标识符＞ |float ＜标识符＞|char＜标识符＞
varDeclaration_lex                  ＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}
varDefinition_lex                   ＜变量定义＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’ )}
enumerableConst_lex                 ＜可枚举常量＞   ::=  ＜整数＞|＜字符＞
typeIdentifier_lex                  ＜类型标识符＞      ::=  int | float | char
funWithReturnDefinition_lex         ＜有返回值函数定义＞  ::=  ＜声明头部＞‘(’＜参数＞‘)’ ‘{’＜复合语句＞‘}’
funWithoutReturnDefinition_lex      ＜无返回值函数定义＞  ::= void＜标识符＞‘(’＜参数＞‘)’‘{’＜复合语句＞‘}’
compound_lex                        ＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
parameter_lex                       ＜参数＞    ::= ＜参数表＞
parameterTable_lex                  ＜参数表＞    ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
mainFun_lex                         ＜主函数＞    ::= void main‘(’‘)’‘{’＜复合语句＞‘}’
expression_lex                      ＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
term_lex                            ＜项＞     ::= ＜因子＞{＜乘法运算符＞＜因子＞}
factor_lex                          ＜因子＞    ::= ＜标识符＞
											 ｜＜标识符＞‘[’＜表达式＞‘]’
											 ｜＜有返回值函数调用语句＞
											  |‘(’＜表达式＞‘)’
						 			   		 ｜＜整数＞
											  |＜实数＞
											  |＜字符＞
sentence_lex                        ＜语句＞    ::= ＜条件语句＞
												｜＜循环语句＞
												|＜情况语句＞
											   	| ‘{’＜语句列＞‘}’
												｜＜读语句＞;
												｜＜写语句＞;
												｜＜返回语句＞;
												｜＜赋值语句＞;
												｜＜有返回值函数调用语句＞;
                                                |＜无返回值函数调用语句＞;
												｜＜空＞;
assignSentence_lex                  ＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
conditionSentence_lex               ＜条件语句＞  ::=  if ‘(’＜条件＞‘)’＜语句＞［else＜语句＞］
condition_lex                       ＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
loopSentence_lex                    ＜循环语句＞   ::=  while ‘(’＜条件＞‘)’＜语句＞
situationSentence_lex               ＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞＜缺省＞ ‘}’
situationTable_lex                  ＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}
situationSubSentence_lex            ＜情况子语句＞  ::=  case＜可枚举常量＞：＜语句＞
default_lex                         ＜缺省＞   ::=  default : ＜语句＞|＜空＞
funWithReturnCall_lex               ＜有返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
funWithoutReturnCall_lex            ＜无返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
valueParameterTable_lex             ＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
sentenceColumn_lex                  ＜语句列＞   ::= ｛＜语句＞｝
scanf_lex                           ＜读语句＞    ::=  scanf ‘(’＜标识符＞{,＜标识符＞}‘)’
printf_lex                          ＜写语句＞    ::= printf ‘(’ ＜字符串＞,＜表达式＞ ‘)’
													| printf ‘(’＜字符串＞ ‘)’
													| printf ‘(’＜表达式＞‘)’
return_lex                          ＜返回语句＞   ::=  return[‘(’＜表达式＞‘)’]

*/

void add_lex();
void multiply_lex();
void relation_lex();
void letter_lex();
void digit_lex();
void notZeroDigit_lex();
void character_lex();
void string_lex();
void program_lex();
void constDeclaration_lex();
void constDefinition_lex();
void unsignedInt_lex();
void int_lex();
void fractionalPart_lex();
void realNumber_lex();
void identifier_lex();
void head_eclaration_lex();
void varDeclaration_lex();
void varDefinition_lex();
void enumerableConst_lex();
void typeIdentifier_lex();
void funWithReturnDefinition_lex();
void funWithoutReturnDefinition_lex();
void compound_lex();
void parameter_lex();
void parameterTable_lex();
void mainFun_lex();
void expression_lex();
void term_lex();
void factor_lex();
void factor_lex();
void sentence_lex();
void assignSentence_lex();
void conditionSentence_lex();
void condition_lex();
void loopSentence_lex();
void situationSentence_lex();
void situationTable_lex();
void situationSubSentence_lex();
void default_lex();
void funWithReturnCall_lex();
void funWithoutReturnCall_lex();
void valueParameterTable_lex();
void sentenceColumn_lex();
void scanf_lex();
void printf_lex();
void return_lex();

void syn_run();

extern ofstream syntax;

typedef struct node7
{
    int num;
    int siz[100];
}structVar;
extern map<string,structVar> structVarSiz;

/******************************************/


/******************[error]*****************/

#define UNCERTAIN_ERR 0
#define CONSTREDEF_ERR 1
#define VARREDEF_ERR 2
#define FUNREDEF_ERR 3
#define LACKSEMICOLON_ERR 4
#define VARUNDEF_ERR 5
#define FUNUNDEF_ERR 6
#define CONSTREASSIGN_ERR 7
#define ADDORSUB_OPE_ERR 8
#define MULORDIV_OPE_ERR 9
#define RELATION_OPE_ERR 10
#define CONST_SYMID_ERR 11
#define INTORCHAR_SYMID_ERR 12
#define ASS_SYMID_ERR 13
#define NUMBER_ERR 14
#define INT_ERR 15
#define IDENTIFIER_ERR 16
#define LEFTSMALLBRACKET_ERR 17
#define RIGHTSMALLBRACKET_ERR 18
#define LEFTMIDDLEBRACKET_ERR 19
#define RIGHTMIDDLEBRACKET_ERR 20
#define LEFTBIGBRACKET_ERR 21
#define RIGHTBIGBRACKET_ERR 22
#define VOID_ERR 23
#define MAIN_ERR 24
#define PRINTF_ERR 25
#define SCANF_ERR 26
#define COLON_ERR 27
#define CASE_ERR 28
#define SWITCH_ERR 29
#define WHILE_ERR 30
#define IF_ERR 31
#define DO_ERR 32
#define FOR_ERR 33
#define ASSIGN_ERR 34
#define SEMICOLON_ERR 35
#define RETURN_ERR 36
#define LETTER_ERR 37
#define DIGIT_ERR 38
#define NOTZERO_ERR 39
#define SINGALQUOTATION_ERR 40
#define DOUBLEQUOTATION_ERR 41
#define CHARACTER_ERR 42
#define STRING_ERR 43
#define ENUMERABEL_ERR 44
//void errorLine(int line);
void errorContent(int errorID, int errorLine, int errorColumn);

/******************************************/


/***************[midcode]******************/

/*
int a;                  < int, , ,a >
char a;				    < char, , ,a >
int a[10];				< int, 10, , a >
char a[10];				< char, 10, , a >
const int a = 1;		< const, int, 1, a >
const char a = 2;		< const, char, 2, a >
int a();				< func, int, , a >
char a();				< func, char, , a >
void a();				< func, void, , a >
fun(int a,)				< para, int, , a >
fun(char a,)			< para, char, , a >
a = b;					< =, b, , a >
a[i] = b;				< []=, b, i, a >
a = b[i];				< =[], b, i, a >
(int|char) fun();		< call, , , fun > #
(void) fun();			< call, , , >
fun(a);					< callpara, int|char, , a >
if(a < b)				< blt, a, b, label >#
if(a <= b)				< ble, a, b, label >
if(a > b)				< bgt, a, b, label >
if(a >= b)				< bge, a, b, label >
if(a == b)				< beq, a, b, label >
if(a != b)				< bne, a, b, label >
jmp	label		 		< j, , , label>
set label				< lab, , , label >
return ;				< ret, , , >
return a;				< ret, , , a >
scanf(a);				< scf, , , a >
print("a",b);			< prf, a, b,  >
a + b					< +, a, b, >
a - b					< -, a, b, >
a * b					< *, a, b, >
a / b					< /, a, b, >
*/

extern ofstream midcode;
extern ofstream debug;

#define CONST_OP "const"
#define INT_OP "int"
#define CHAR_OP "char"
#define INTA_OP "inta"
#define CHARA_OP "chara"
#define END_OP "end"
#define ADD_OP "+"
#define SUB_OP "-"
#define MUL_OP "*"
#define DIVS_OP "/"
#define FUNC_OP "func"
#define PRTF_OP "prtf"
#define SCF_OP "scf"
#define RET_OP "ret"
#define LAB_OP "lab"
#define PARA_OP "para"
#define CALPARA_OP "callpara"
#define CALL_OP "call"
//#define JNE_OP "jne"
#define JMP_OP "jmp"
#define ASS_OP "="
#define ARRYASS_OP "=[]"
#define ASSARRY_OP "[]="
#define BT_OP ">"
#define ST_OP "<"
#define EQL_OP "=="
#define NEQ_OP "!="
#define BET_OP ">="
#define SET_OP "<="
#define EXITS_OP "exit"
#define VOID_OP "void"
#define VAL_OP "value"
#define SPACE_OP ""
#define ZERO_OP "0"

#define maxn 1000010
typedef struct node1
{
    string op;
    string src1;
    string src2;
    string res;
    int blockNum;
    bool isEntrance;
    bool isExit;
    bool isLabelOrFunc;
}quaternary;
extern quaternary quaternaryList[maxn];
extern int quaternaryNum;
extern int varWidth;

void genMidCode(string op, string src1, string src2, string res);
string genVar();
string genLabel();

extern map<string,string> stringTabel;
extern int stringNum;
string genString();
/******************************************/



/******************[asm]*******************/


#define NOP_MIPS "nop"
#define MOVE_MIPS "move"
#define MUL_MIPS "mul"
#define DIV_MIPS "div"
#define ADD_MIPS "add"
#define SUB_MIPS "sub"
#define ADDI_MIPS "addi"
#define SUBI_MIPS "subi"
#define LI_MIPS "li"
#define LA_MIPS "la"
#define LW_MIPS "lw"
#define SW_MIPS "sw"
#define JAL_MIPS "jal"
#define JR_MIPS "jr"
#define JMP_MIPS "j"
#define BEQ_MIPS "beq"
#define BNE_MIPS "bne"
#define BLT_MIPS "blt"
#define BGT_MIPS "bgt"
#define BLE_MIPS "ble"
#define BGE_MIPS "bge"
#define SYSCALL_MIPS "syscall"
#define ADDU_MIPS "addu"
#define ADDIU_MIPS "addiu"
#define SUBU_MIPS "subu"
#define SUBIU_MIPS "subiu"
#define AND_MIPS "and"
#define OR_MIPS "or"
#define XOR_MIPS "xor"
#define NOR_MIPS "nor"
#define SLT_MIPS "slt"
#define SLTU_MIPS "sltu"
#define SLL_MIPS "sll"
#define SHAMT_MIPS "shamt"
#define SRA_MIPS "sra"
#define SLLV_MIPS "sllv"
#define SRLV_MIPS "srlv"
#define SRAV_MIPS "srav"
#define ORI_MIPS "ori"
#define XORI_MIPS "xori"
#define LUI_MIPS "lui"
#define SLTI_MIPS "slti"
#define SLTIU_MIPS "sltiu"

#define SPACE_MIPS ""

#define SP_REG "$sp"
#define FP_REG "$fp"
#define V0_REG "$v0"
#define V1_REG "$v1"
#define RA_REG "$ra"
#define A0_REG "$a0"
#define A1_REG "$a1"
#define A2_REG "$a2"
#define A3_REG "$a3"
#define T0_REG "$t0"
#define T1_REG "$t1"
#define T2_REG "$t2"
#define T3_REG "$t3"
#define T4_REG "$t4"
#define T5_REG "$t5"
#define T6_REG "$t6"
#define T7_REG "$t7"
#define T8_REG "$t8"
#define T9_REG "$t9"
#define S0_REG "$s0"
#define S1_REG "$s1"
#define S2_REG "$s2"
#define S3_REG "$s3"
#define S4_REG "$s4"
#define S5_REG "$s5"
#define S6_REG "$s6"
#define S7_REG "$s7"
#define K0_REG "$k0"
#define K1_REG "$k1"

#define PRFINT "1"
#define PRFCHAR "11"
#define PRFSTR "4"
#define SCFINT "5"
#define SCFCHAR "12"

extern ofstream asmcode;
extern int paraNum;
extern int valParaNum;
void asm_run();
extern string res,src1,src2;
extern map<string, string> reg2var;
extern const int intSize;
extern const int charSize;


typedef struct node2
{
    int offset;
    string type;
}varNode;
extern map<string, varNode> glbVar;
extern map<string, varNode> locVar;

string shamt(int a, string b);

void asm_init();
void stringDecl();
void asm_out(string op, string var1, string var2, string var3);

void int_mips();
void char_mips();
void add_mips();
void sub_mips();
void mul_mips();
void div_mips();
void ass_mips();
void para_mips();
void func_mips();
void call_mips();
void return_mips();
void callpara_mips();
void lab_mips();
void jmp_mips();
void blt_mips();
void ble_mips();
void bgt_mips();
void bge_mips();
void beq_mips();
void bne_mips();
void exits_mips();
void scf_mips();
void prf_mips();
void assArry_mips();
void intArry_mips();
void charArry_mips();

void outLocVar();

//typedef struct node3
//{
//    string name;
//    string type;
//    string value;
//    string sizee;
//}glbDeclNode;
//extern vector<glbNode> glbDeclTabel;
/******************************************/



/****************[optimize]****************/

typedef struct node3
{
    string op;
    string var1;
    string var2;
    string var3;
    bool isText;
    bool isEntrance;
    bool isExit;
    int blockNum;
    string line;
}ASMSENTENCE;

extern ASMSENTENCE asmSent[maxn];
extern int asmSentNum;

extern ofstream optimize;

void optimize_run();

/******************************************/
#endif // GLOBAL_H_INCLUDED
