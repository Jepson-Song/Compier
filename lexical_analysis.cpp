#include "global.h"
using namespace std;

#define LexError 1
//#define cout lexical


const int keyWordNum = 54 ;
string keyWord[keyWordNum] = {"error",
                    "int", "char", "float", "double", "bool",
                    "void", "const", "if", "else", "for",
                    "while", "do", "switch", "case", "default",
                    "printf", "scanf", "return", "main", "indentifier",
                    "+", "-", "*", "/", "%",
                    ">", "<", "<=", ">=", "==",
                    "!=", "=", ",", ";", ":",
                    ".", "\'", "\"", "(", ")",
                    "[", "]", "{", "}", "123456789",
                    "string", "#", "_", "2", "0",
                    "1", "0001", "struct"};


string keyWordSym[keyWordNum] = {"ERROR_SYMID",
                   "INT_SYMID", "CHAR_SYMID", "FLOAT_SYMID", "DOUBLE_SYMID", "BOOL_SYMID",
                   "VOID_SYMID", "CONST_SYMID", "IF_SYMID", "ELSE_SYMID", "FOR_SYMID",
                   "WHILE_SYMID", "DO_SYMID", "SWITCH_SYMID", "CASE_SYMID", "DEFAULT_SYMID",
                   "PRINTF_SYMID", "SCANF_SYMID", "RETURN_SYMID", "MAIN_SYMID", "IDENTIFIER_SYMID",
                   "ADD_SYMID", "SUBTRACT_SYMID", "MULTIPLY_SYMID", "DIVIDE_SYMID", "MOD_SYMID",
                   "LESS_SYMID", "GREATER_SYMID", "LESSEQUAL_SYMID", "GREATEREQUAL_SYMID", "EQUAL_SYMID",
                   "NOTEQUAL_SYMID", "ASSIGN_SYMID", "COMMA_SYMID", "SEMICOLON_SYMID", "COLON_SYMID",
                   "DOT_SYMID", "SINGALQUOTATION_SYMID", "DOUBLEQUOTATION_SYMID", "LEFTSMALLBRACKET_SYMID", "RIGHTSMALLBRACKET_SYMID",
                   "LEFTMIDDLEBRACKET_SYMID", "RIGHTMIDDLEBRACKET_SYMID", "LEFTBIGBRACKET_SYMID", "RIGHTBIGBRACKET_SYMID", "NUMBER_SYMID",
                   "STRING_SYMID", "CHARACTER_SYMID", "LETTER_SYMID", "DIGIT_SYMID", "ZERODIGIT_SYMID",
                   "NOTZERODIGIT_SYMID", "ZEROHEADNUMBER_SYMID", "STRUCT_SYMID"};

const int reservedWordNum = 21;

string reservedWord[reservedWordNum] = {
                    "int", "char", "float", "double", "bool",
                    "void", "const", "if", "else", "for",
                    "while", "do", "switch", "case", "default",
                    "printf", "scanf", "return", "main", "indentifier",
                    "struct"};

string reservedWordSym[reservedWordNum] = {
                   "INT_SYMID", "CHAR_SYMID", "FLOAT_SYMID", "DOUBLE_SYMID", "BOOL_SYMID",
                   "VOID_SYMID", "CONST_SYMID", "IF_SYMID", "ELSE_SYMID", "FOR_SYMID",
                   "WHILE_SYMID", "DO_SYMID", "SWITCH_SYMID", "CASE_SYMID", "DEFAULT_SYMID",
                   "PRINTF_SYMID", "SCANF_SYMID", "RETURN_SYMID", "MAIN_SYMID", "IDENTIFIER_SYMID",
                   "STRUCT_SYMID"};

vector<NODE> symbolTabel;
int indexInFile,preIndexInFile;
int indexInLine,preIndexInLine;
int lenOfLine,preLenOfLine;
char ch,preCh;
//char line[522],preLine[522];
bool fileEnd,preFileEnd;

string symbol_lex;
int symID_lex;

void get_char();
void lex_init();
void lex_run();
void get_symbol_lex();
void chRecord();
void chRollBack();

vector<string> sourceFile;
int sourceNum;
string line,preLine;

void readSourceFile()//读源代码
{
    sourceNum = 0;
    while(getline(source,line))
    {
        sourceFile.push_back(line);
        sourceNum ++;
    }
}

void outSourceFile()//输出源代码
{
    for(int i=0;i<sourceNum;i++){
        cout<<sourceFile[i]<<endl;
    }
}

void lex_init()//词法分析初始化
{
    indexInFile = -1;
    indexInLine = 0;
    lenOfLine = 0;
    fileEnd = false;
}

void lex_run()//运行词法分析
{
    state = lex_sta;

    readSourceFile();
    //outSourceFile();
    lex_init();
    get_char();
    while(!fileEnd)
    {
        get_symbol_lex();
    }
}

void chRecord()//记录当前位置
{
    preIndexInFile = indexInFile;
    preIndexInLine = indexInLine;
    preLenOfLine = lenOfLine;
    preCh = ch;
    //strcpy(preLine,line);
    preLine = line;
    preFileEnd = fileEnd;
}

void chRollBack()//回滚到记录处
{
    indexInFile = preIndexInFile;
    indexInLine = preIndexInLine;
    lenOfLine = preLenOfLine;
    ch = preCh;
    //strcpy(line,preLine);
    line = preLine;
    fileEnd = preFileEnd;
}

void get_char()//读一个字符
{
//    cout<<indexInFile<<endl;
//    cout<<line<<endl;
//    cout<<lenOfLine<<endl;
    //indexInLine ++;
    if(indexInLine >= lenOfLine)
    {
        indexInFile ++;
        if(indexInFile == sourceNum)
        {
            fileEnd = true;
            return ;
        }
        line = sourceFile[indexInFile];
        lenOfLine = line.length();
        indexInLine = 0;
        ch = '\n';
    }
    else
    {
        ch = line[indexInLine];
        indexInLine ++;
    }
    //lexical<<"***   "<<ch<<"   ***"<<endl;
    //test_write();
}

void write()
{
    lexical<<setw(25)<<left<<keyWordSym[symID_lex];
    lexical<<setw(10)<<left<<symID_lex<<symbol_lex<<endl;

    //debug<<setw(25)<<left<<keyWordSym[symID_lex];
    //debug<<setw(10)<<left<<symID_lex<<symbol_lex<<endl;
}

bool isSpace()//判断是不是空格
{

    if(ch ==' ')
    {
        //lexical<<"isSpace"<<endl ;
        return true;
    }
    return false;
}

bool isTab()//判断是不是tab
{

    if(ch == '\t')
    {
        //lexical<<"isTab"<<endl;
        return true;
    }
    return false;
}

bool isEnter()//判断是不是回车
{

    if(ch == '\n')
    {
        //lexical<<"isEnter"<<endl;
        return true;
    }
    return false;
}

bool isDigit()//判断是不是数字
{
    if(ch >= '0' && ch <= '9') return true;
    return false;
}

bool isZeroDigit()//判断是不是0
{
    if(ch == '0') return true;
    return false;
}

bool isNotZeroDigit()//判断是不是非零数字
{
    if(ch >= '1' && ch <= '9') return true;
    return false;
}

bool isLetter()//判断是不是字母
{
    if((ch >= 'a' && ch <= 'z') ||
       (ch >= 'A' && ch <= 'Z') ||
        ch == '_')
       return true;
    return false;
}

bool isCharacter() /// no " //判断是不是字符
{
    if(ch == 32 || ch == 33 ||
       (ch >= 35 && ch <= 126))
        return true;
    return false;
}

bool isAdd()//判断是不是加号
{
    if(ch == '+') return true;
    return false;
}

bool isSubtract()//判断是不是减号
{
    if(ch == '-') return true;
    return false;
}

bool isMultiply()//判断是不是乘号
{
    if(ch == '*') return true;
    return false;
}

bool isDivide()//判断是不是除号
{
    if(ch == '/') return true;
    return false;
}

bool isMod()//判断是不是百分号
{
    if(ch == '%') return true;
    return false;
}

bool isLess()//判断是不是小于号
{
    if(ch == '<') return true;
    return false;
}

bool isGreater()//判断是不是大于号
{
    if(ch == '>') return true;
    return false;
}

bool isNot()//判断是不是非号
{
    if(ch == '!') return true;
    return false;
}

bool isAssign()//判断是不是等号
{
    if(ch == '=') return true;
    return false;
}

bool isComma()//判断是不是逗号
{
    if(ch == ',') return true;
    return false;
}

bool isSemicolon()//判断是不是分号
{
    if(ch == ';') return true;
    return false;
}

bool isColon()//判断是不是冒号
{
    if(ch == ':') return true;
    return false;
}

bool isSingleQuotion()//判断是不是单引号
{
    if(ch == '\'') return true;
    return false;
}

bool isDoubleQuotion()//判断是不是双引号
{
    if(ch == '\"') return true;
    return false;
}

bool isLeftSmallBracket()//判断是不是左小括号
{
    if(ch == '(') return true;
    return false;
}

bool isRightSmallBracket()//判断是不是右小括号
{
    if(ch == ')') return true;
    return false;
}

bool isLeftMiddleBracket()//判断是不是左中括号
{
    if(ch == '[') return true;
    return false;
}

bool isRightMiddleBracket()//判断是不是右中括号
{
    if(ch == ']') return true;
    return false;
}

bool isLeftBigBracket()//判断是不是左大括号
{
    if(ch == '{') return true;
    return false;
}

bool isRightBigBracket()//判断是不是右大括号
{
    if(ch == '}') return true;
    return false;
}

bool isExclamation()//判断是不是叹号
{
    if(ch == '!') return true;
    return false;
}


bool isDot()//判断是不是小数点
{
    if(ch == '.') return true;
    return false;
}

/*
void test_write()
{
    if(isSpace()) result<<"***"<<"/k"<<endl;
    else if(isTab()) result<<"***"<<"/t"<<endl;
    else if(isEnter()) result<<"***"<<"/n"<<endl;
    else result<<"***"<<ch<<endl;
}
*/

void get_symbol_lex()//读一个单词
{
    //lexical<<"in get_symbol_lex"<<endl;
    symbol_lex = "";
    symID_lex = 0;

    get_char();
    //lexical<<"---ch: "<<ch<<endl;
    if(fileEnd) return;
    //test_write();
    while(isSpace() || isTab() || isEnter())
    {
        get_char();
        //test_write();
    }

    if(isAdd()) // +
    {
        symbol_lex += ch;
        symID_lex = ADD_SYMID;
    }
    else if(isSubtract()) // -
    {
        symbol_lex += ch;
        symID_lex = SUBTRACT_SYMID;
    }
    else if(isMultiply()) // *
    {
        symbol_lex += ch;
        symID_lex = MULTIPLY_SYMID;
    }
    else if(isDivide()) // /
    {
        symbol_lex += ch;
        chRecord();
        get_char();
        if(isDivide())
        {
            while(!isEnter())
            {
                get_char();
            }
            return;
        }
        else if(isMultiply())
        {
            char remCh = ch;
            while(1)
            {
                get_char();
                if(remCh == '*' && isDivide())
                {
                    break;
                }
                remCh = ch;
            }
            return ;
        }
        else
        {
            chRollBack();
        }
        symID_lex = DIVIDE_SYMID;
    }
    else if(isMod())
    {
        symbol_lex += ch;
        symID_lex = MOD_SYMID;
    }
    else if(isLess())
    {
        symbol_lex += ch;
        get_char();
        if(isAssign()) // <=
        {
            symbol_lex += ch;
            symID_lex = LESSEQUAL_SYMID;
        }
        else // <
        {
            symID_lex = LESS_SYMID;
            indexInLine --;
        }
    }
    else if(isGreater())
    {
        symbol_lex += ch;
        get_char();
        if(isAssign()) // >=
        {
            symbol_lex += ch;
            symID_lex = GREATEREQUAL_SYMID;
        }
        else // >
        {
            symID_lex = GREATER_SYMID;
            indexInLine --;
        }
    }
    else if(isExclamation()) // !=
    {
        symbol_lex += ch;
        get_char();
        if(isAssign())
        {
            symbol_lex += ch;
            symID_lex = NOTEQUAL_SYMID;
        }
    }
    else if(isAssign())
    {
        symbol_lex += ch;
        get_char();
        if(isAssign()) // ==
        {
            symbol_lex += ch;
            symID_lex = EQUAL_SYMID;
        }
        else // =
        {
            symID_lex = ASSIGN_SYMID;
            indexInLine --;
        }
    }
    else if(isComma()) // ,
    {
        symbol_lex += ch;
        symID_lex = COMMA_SYMID;
    }
    else if(isSemicolon()) // ;
    {
        symbol_lex += ch;
        symID_lex = SEMICOLON_SYMID;
    }
    else if(isColon()) // :
    {
        symbol_lex += ch;
        symID_lex = COLON_SYMID;
    }
    else if(isSingleQuotion()) // '
    {
        //symbol_lex += ch;

        get_char();
//        symbol_lex += ch;
//        if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
//        {
//            symID_lex = LETTER_SYMID;
//        }
//        else if(ch >= '0' && ch <= '9')
//        {
//            symID_lex = DIGIT_SYMID;
//        }
//        else
//        {
//            symID_lex = CHARACTER_SYMID;
//        }
//        string tmp = "";
//        tmp += ch;
        symbol_lex = to_string(int(ch));
        symID_lex = NUMBER_SYMID;
        get_char();
        if(!isSingleQuotion())
        {
            //error
            symbol_lex = "error";
            symID_lex = ERROR;
        }
        else
        {
            //symbol_lex += ch;
        }


    }
    else if(isDoubleQuotion()) // "
    {
        symbol_lex += ch;

        while(1)
        {
            get_char();
            if(isDoubleQuotion()) break;
            symbol_lex += ch;
        }
        symbol_lex += ch;
        symID_lex = STRING_SYMID;
        //write();

    }
    else if(isLeftSmallBracket()) // (
    {
        symbol_lex += ch;
        symID_lex = LEFTSMALLBRACKET_SYMID;
    }
    else if(isRightSmallBracket()) // )
    {
        symbol_lex += ch;
        symID_lex = RIGHTSMALLBRACKET_SYMID;
    }
    else if(isLeftMiddleBracket()) // [
    {
        symbol_lex += ch;
        symID_lex = LEFTMIDDLEBRACKET_SYMID;
    }
    else if(isRightMiddleBracket()) // ]
    {
        symbol_lex += ch;
        symID_lex = RIGHTMIDDLEBRACKET_SYMID;
    }
    else if(isLeftBigBracket()) // {
    {
        symbol_lex += ch;
        symID_lex = LEFTBIGBRACKET_SYMID;
    }
    else if(isRightBigBracket()) // }
    {
        symbol_lex += ch;
        symID_lex = RIGHTBIGBRACKET_SYMID;
    }
    else if(isDot())
    {
        symbol_lex += ch;
        symID_lex = DOT_SYMID;
    }
    else if(isLetter()) // letter
    {
        symbol_lex += ch;
        while(1)
        {
            chRecord();
            get_char();
            if(!isLetter() && !isDigit())
            {
                chRollBack();
                break;
            }
            symbol_lex += ch;
        }
        for(int i=0; i<reservedWordNum; i++)
        {
            if(symbol_lex == reservedWord[i])
            {
                symID_lex = i+1;
                break;
            }
        }
        if(symID_lex == 0)
        {
            symID_lex = IDENTIFIER_SYMID;
        }
    }
    else if(isDigit()) // digit
    {
        if(isZeroDigit())
        {
            symID_lex = ZEROHEADNUMBER_SYMID;
        }
        else
        {
            symID_lex = NUMBER_SYMID;
        }
        symbol_lex += ch;
        while(1)
        {
            chRecord();
            get_char();
            if(!isDigit())
            {
                chRollBack();
                break;
            }
            symbol_lex += ch;
            //get_char();
        }
        if(symbol_lex.length()==1)
        {
            symID_lex = NUMBER_SYMID;
        }

    }

    write();

    NODE tmp;
    tmp.symbol = symbol_lex;
    tmp.symID = symID_lex;
    tmp.indexInFile = indexInFile;
    tmp.indexInLine = indexInLine;
    symbolTabel.push_back(tmp);
}
