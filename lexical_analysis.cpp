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

void readSourceFile()//��Դ����
{
    sourceNum = 0;
    while(getline(source,line))
    {
        sourceFile.push_back(line);
        sourceNum ++;
    }
}

void outSourceFile()//���Դ����
{
    for(int i=0;i<sourceNum;i++){
        cout<<sourceFile[i]<<endl;
    }
}

void lex_init()//�ʷ�������ʼ��
{
    indexInFile = -1;
    indexInLine = 0;
    lenOfLine = 0;
    fileEnd = false;
}

void lex_run()//���дʷ�����
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

void chRecord()//��¼��ǰλ��
{
    preIndexInFile = indexInFile;
    preIndexInLine = indexInLine;
    preLenOfLine = lenOfLine;
    preCh = ch;
    //strcpy(preLine,line);
    preLine = line;
    preFileEnd = fileEnd;
}

void chRollBack()//�ع�����¼��
{
    indexInFile = preIndexInFile;
    indexInLine = preIndexInLine;
    lenOfLine = preLenOfLine;
    ch = preCh;
    //strcpy(line,preLine);
    line = preLine;
    fileEnd = preFileEnd;
}

void get_char()//��һ���ַ�
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

bool isSpace()//�ж��ǲ��ǿո�
{

    if(ch ==' ')
    {
        //lexical<<"isSpace"<<endl ;
        return true;
    }
    return false;
}

bool isTab()//�ж��ǲ���tab
{

    if(ch == '\t')
    {
        //lexical<<"isTab"<<endl;
        return true;
    }
    return false;
}

bool isEnter()//�ж��ǲ��ǻس�
{

    if(ch == '\n')
    {
        //lexical<<"isEnter"<<endl;
        return true;
    }
    return false;
}

bool isDigit()//�ж��ǲ�������
{
    if(ch >= '0' && ch <= '9') return true;
    return false;
}

bool isZeroDigit()//�ж��ǲ���0
{
    if(ch == '0') return true;
    return false;
}

bool isNotZeroDigit()//�ж��ǲ��Ƿ�������
{
    if(ch >= '1' && ch <= '9') return true;
    return false;
}

bool isLetter()//�ж��ǲ�����ĸ
{
    if((ch >= 'a' && ch <= 'z') ||
       (ch >= 'A' && ch <= 'Z') ||
        ch == '_')
       return true;
    return false;
}

bool isCharacter() /// no " //�ж��ǲ����ַ�
{
    if(ch == 32 || ch == 33 ||
       (ch >= 35 && ch <= 126))
        return true;
    return false;
}

bool isAdd()//�ж��ǲ��ǼӺ�
{
    if(ch == '+') return true;
    return false;
}

bool isSubtract()//�ж��ǲ��Ǽ���
{
    if(ch == '-') return true;
    return false;
}

bool isMultiply()//�ж��ǲ��ǳ˺�
{
    if(ch == '*') return true;
    return false;
}

bool isDivide()//�ж��ǲ��ǳ���
{
    if(ch == '/') return true;
    return false;
}

bool isMod()//�ж��ǲ��ǰٷֺ�
{
    if(ch == '%') return true;
    return false;
}

bool isLess()//�ж��ǲ���С�ں�
{
    if(ch == '<') return true;
    return false;
}

bool isGreater()//�ж��ǲ��Ǵ��ں�
{
    if(ch == '>') return true;
    return false;
}

bool isNot()//�ж��ǲ��ǷǺ�
{
    if(ch == '!') return true;
    return false;
}

bool isAssign()//�ж��ǲ��ǵȺ�
{
    if(ch == '=') return true;
    return false;
}

bool isComma()//�ж��ǲ��Ƕ���
{
    if(ch == ',') return true;
    return false;
}

bool isSemicolon()//�ж��ǲ��Ƿֺ�
{
    if(ch == ';') return true;
    return false;
}

bool isColon()//�ж��ǲ���ð��
{
    if(ch == ':') return true;
    return false;
}

bool isSingleQuotion()//�ж��ǲ��ǵ�����
{
    if(ch == '\'') return true;
    return false;
}

bool isDoubleQuotion()//�ж��ǲ���˫����
{
    if(ch == '\"') return true;
    return false;
}

bool isLeftSmallBracket()//�ж��ǲ�����С����
{
    if(ch == '(') return true;
    return false;
}

bool isRightSmallBracket()//�ж��ǲ�����С����
{
    if(ch == ')') return true;
    return false;
}

bool isLeftMiddleBracket()//�ж��ǲ�����������
{
    if(ch == '[') return true;
    return false;
}

bool isRightMiddleBracket()//�ж��ǲ�����������
{
    if(ch == ']') return true;
    return false;
}

bool isLeftBigBracket()//�ж��ǲ����������
{
    if(ch == '{') return true;
    return false;
}

bool isRightBigBracket()//�ж��ǲ����Ҵ�����
{
    if(ch == '}') return true;
    return false;
}

bool isExclamation()//�ж��ǲ���̾��
{
    if(ch == '!') return true;
    return false;
}


bool isDot()//�ж��ǲ���С����
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

void get_symbol_lex()//��һ������
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
