#include "global.h"
#include <map>
#include <stack>

#define cout debug

using namespace std;

void get_symbol();
void syn_init();
void sym_run();

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
void structVarDefinition_lex();
void structVarDeclaration_lex();


ofstream sytax_analysis;

string glbtmp;

map<string, bool> constName;
map<string, bool> locVarName;
map<string, bool> glbVarName;
map<string, bool> funWithReturnName;
map<string, bool> funWithoutReturnName;
string functionName;

//int preIndexInFile,preIndexInLine,preSymID;
int symbolNum;
int indexInSymbolTabel,preIndexInSymbolTabel;
int symID,preSymID;
string symbol,preSymbol;
int errorLine, preErrorLine;
int errorColumn, preErrorColumn;

map<string,structVar> structVarSiz;

void get_symbol()
{
    symID = symbolTabel[indexInSymbolTabel].symID;
    symbol = symbolTabel[indexInSymbolTabel].symbol;
    errorLine = symbolTabel[indexInSymbolTabel].indexInFile;
    errorColumn = symbolTabel[indexInSymbolTabel].indexInLine;
    indexInSymbolTabel ++;
    cout<<setw(25)<<left<<keyWordSym[symID];
    cout<<setw(10)<<left<<symID<<symbol<<endl;
    //debug<<setw(25)<<left<<keyWordSym[symID];
    //debug<<setw(10)<<left<<symID<<symbol<<endl;

}

void record() /// 记录当前位置
{
    preIndexInSymbolTabel = indexInSymbolTabel;
    preSymbol = symbol;
    preSymID = symID;
    preErrorLine = errorLine;
    preErrorColumn = errorColumn;
}

void rollBack() /// 回退到之前的位置
{
    cout<<"***[rollBack]***"<<endl;
    //debug<<"***[rollBack]***"<<endl;
    indexInSymbolTabel = preIndexInSymbolTabel;
    symbol = preSymbol;
    symID = preSymID;
    errorLine = preErrorLine;
    errorColumn = preErrorColumn;
}

void readAhead() /// 预读一个symbol
{
    cout<<"***[readAhead]***"<<"   ";
    //debug<<"***[readAhead]***"<<"   ";
    get_symbol();
}

void syn_init()//语法分析初始化
{
    constName.clear();
    locVarName.clear();
    glbVarName.clear();
    functionName = "";
    funWithoutReturnName.clear();
    funWithReturnName.clear();
    symbolNum = symbolTabel.size();
    indexInSymbolTabel = 0;
    preIndexInSymbolTabel = 0;
    symbol = "";
    symID = 0;
    errorLine = -1;
    errorColumn = -1;
}

void syn_run()//运行语法分析
{
    state = syn_sta;

    syn_init();
    get_symbol();
    program_lex();
}

void struct_lex()
{

}

void structVarDeclaration_lex()
{
    cout<<"*in structVarDeclaration_lex"<<endl;
    get_symbol();
    structVarDefinition_lex(); /// 进<变量定义>
    if(symID != SEMICOLON_SYMID)
    {
        errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn);
        return ;
    }
    get_symbol();// int | float | char

    while(symID == INT_SYMID || symID == CHAR_SYMID)
    {

        record(); /// 记录当前symbol

        readAhead(); /// 预先读一个symbol
        readAhead(); /// 再预先读一个symbol

        if(symID == LEFTSMALLBRACKET_SYMID)
        {
            rollBack(); /// 如果读到（说明是函数定义，先回退，然后结束<变量说明>
            break;
        }

        /// 否则先回退，继续进<变量定义>
        rollBack();
        structVarDefinition_lex();
        if(symID != SEMICOLON_SYMID)
        {
            errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn); /// 不是；结束报错
            return;
        }
        get_symbol();
    }

    /// TODO {＜变量定义＞;}
    cout<<"*out structVarDeclaration_lex"<<endl;
}

string structName;
int arraySiz;
void structVarDefinition_lex()
{
    cout<<"*in structVarDefinition_lex"<<endl;
    string op,src1,src2,res; /// 四元式

    int remSymID = symID; /// 记录变量类型
    typeIdentifier_lex();

    structName = symbol;

        /// 如果全局变量已经定义过，则报错
        if(constName.find(symbol) != constName.end() || glbVarName.find(symbol) != glbVarName.end())
        {
//            cout<<0<<endl;
            errorContent(VARREDEF_ERR, errorLine, errorColumn);
            return ;
        }
        /// 如果未定义过，则记录下来
        else
        {
            glbVarName[symbol] = true;
        }


    res = symbol;
    identifier_lex();

    /// 如果读到 [ 则说明是数组
    if(symID == LEFTMIDDLEBRACKET_SYMID) // array
    {
        if(remSymID == INT_SYMID)
        {
            op = INTA_OP;
        }
        else
        {
            op = CHARA_OP;
        }

        get_symbol();
        src1 = symbol;
        unsignedInt_lex();

        if(symID == RIGHTMIDDLEBRACKET_SYMID)
        {
            get_symbol();
        }
        else
        {
            errorContent(RIGHTMIDDLEBRACKET_ERR, errorLine, errorColumn);
            return;
        }
        src2 = SPACE_OP;

        structVarSiz[structName].siz[++structVarSiz[structName].num] = arraySiz;

    }
    else // not array
    {
        if(remSymID == INT_SYMID)
        {
            op = INT_OP;
            structVarSiz[structName].siz[++structVarSiz[structName].num] = intSize;
        }
        else
        {
            op = CHAR_OP;
            structVarSiz[structName].siz[++structVarSiz[structName].num] = charSize;
        }
        src1 = SPACE_OP;
        src2 = SPACE_OP;
        //genMidCode(op, src1, src2, res);

    }

    /// 如果是 , 则重复上述操作，继续变量定义
    while(symID == COMMA_SYMID)
    {
        get_symbol();
        structName = symbol;


            /// 如果全局变量已经定义过，则报错
            if(constName.find(symbol) != constName.end() || glbVarName.find(symbol) != glbVarName.end())
            {
                errorContent(VARREDEF_ERR, errorLine, errorColumn);
                return ;
            }
            /// 如果未定义过，则记录下来
            else
            {
                glbVarName[symbol] = true;
            }

        res = symbol;
        identifier_lex();

        if(symID == LEFTMIDDLEBRACKET_SYMID)
        {

            if(remSymID == INT_SYMID)
            {
                op = INTA_OP;
            }
            else
            {
                op = CHARA_OP;
            }

            get_symbol();
            src1 = symbol;
            unsignedInt_lex();

            if(symID == RIGHTMIDDLEBRACKET_SYMID)
            {
                get_symbol();
            }
            else
            {

                errorContent(RIGHTMIDDLEBRACKET_ERR, errorLine, errorColumn);;
                return ;
            }
            src2 = SPACE_OP;

            structVarSiz[structName].siz[++structVarSiz[structName].num] = arraySiz;
        }
        else
        {
            if(remSymID == INT_SYMID)
            {
                op = INT_OP;
                structVarSiz[structName].siz[++structVarSiz[structName].num] = intSize;
            }
            else
            {
                op = CHAR_OP;
                structVarSiz[structName].siz[++structVarSiz[structName].num] = charSize;
            }
            src1 = SPACE_OP;
            src2 = SPACE_OP;
            //genMidCode(op, src1, src2, res);
        }
    }

    cout<<"*out structVarDefinition_lex"<<endl;
}

/**
    ＜程序＞    ::= ［＜常量说明＞］［＜变量说明＞］[<结构体说明>] {＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
**/
void program_lex()//＜程序＞
{
    cout<<"*in program_lex"<<endl;

    /// ［＜常量说明＞］
    if(symID == CONST_SYMID)
    {
        constDeclaration_lex();
    }

    /// ［＜变量说明＞］
    if(symID == INT_SYMID || symID == CHAR_SYMID)
    {
        record();
        readAhead();
        readAhead();
        if(symID != LEFTSMALLBRACKET_SYMID)
        {
            rollBack();
            varDeclaration_lex();
        }
        else
        {
            rollBack();
        }
    }

    /// [<结构体说明>]
    if(symID == STRUCT_SYMID)
    {

        get_symbol();
        if(symID != IDENTIFIER_SYMID)
        {
            /// error
        }

        get_symbol();
        if(symID != LEFTBIGBRACKET_SYMID)
        {
            ///
        }

        get_symbol();
        structVarDeclaration_lex();
    }

    /// {＜有返回值函数定义＞|＜无返回值函数定义＞}
    record();
    readAhead();
    int remSymID = symID;
    readAhead();
    while(remSymID != MAIN_SYMID && symID == LEFTSMALLBRACKET_SYMID)
    {
        rollBack();

        if(symID == INT_SYMID || symID == CHAR_SYMID)
        {
            funWithReturnDefinition_lex();
        }
        else
        {
            funWithoutReturnDefinition_lex();
        }

        record();
        readAhead();
        remSymID = symID;
        readAhead();

    }
    rollBack();

    /// ＜主函数＞
    record();
    readAhead();
    if(symID != MAIN_SYMID)
    {
        errorContent(MAIN_ERR, errorLine, errorColumn);
        return ;
    }
    else
    {
        rollBack();
        mainFun_lex();
    }

    cout<<"*out program_lex"<<endl;
}

/**
    ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
**/
void constDeclaration_lex()//＜常量说明＞
{
    cout<<"*in constDeclaration_lex"<<endl;

    //const
    if(symID != CONST_SYMID)
    {
        errorContent(CONST_SYMID_ERR, errorLine, errorColumn);
        // 第一个symbol必须是const
        return;
    }

    get_symbol();
    constDefinition_lex(); /// 进<常量定义>

    // ;
    get_symbol();//const

    while(symID == CONST_SYMID)
    {
        //debug<<"111111111111"<<endl;
        get_symbol();

        constDefinition_lex();
        // ;
        get_symbol();
    }

    //get_symbol();

    cout<<"*out constDeclaration_lex"<<endl;
}

/**
    ＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
                          | float＜标识符＞＝＜实数＞{,＜标识符＞＝＜实数＞}
                          | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
**/
void constDefinition_lex()///TODO//＜常量定义＞
{
    cout<<"*in constDefinition_lex"<<endl;

    string op,src1,src2,res; /// 四元式
    string type;

    op = CONST_OP;
    if(symID != INT_SYMID && symID != FLOAT_SYMID && symID != CHAR_SYMID)
    {
        errorContent(INTORCHAR_SYMID_ERR, errorLine, errorColumn);
        // 第一个必须是 int float char
        return ;
    }

    else//if(symID == INT_SYMID)///int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
    {
        if(symID == INT_SYMID)
        {
            type = INT_OP;
        }
        else
        {
            type = CHAR_OP;
        }
        get_symbol();

        if(constName.find(symbol) == constName.end())
        {
            constName[symbol] = true;
        }
        else
        {
            errorContent(CONSTREDEF_ERR, errorLine, errorColumn);
            return ;
        }

        res = symbol;
        identifier_lex();// =

        if(symID != ASSIGN_SYMID)
        {
            errorContent(ASS_SYMID_ERR, errorLine, errorColumn);
            return ;
        }

        op = CONST_OP;
        src1 = type;
        src2 = SPACE_OP;

        genMidCode(op, src1, src2, res); /// 生成四元式

        get_symbol();
        if(symID == SUBTRACT_SYMID)
        {
            op = SUB_OP;
            src1 = ZERO_OP;

            get_symbol();
            src2 = symbol;

            /// res = res

            genMidCode(op, src1, src2, res);
        }
        else
        {
            op = ASS_OP;
            src1 = symbol;
            src2 = SPACE_OP;
            /// res = res

            genMidCode(op, src1, src2, res);
        }
        get_symbol();

        //int_lex(); /// 进<整数>

        while(symID == COMMA_SYMID) // ,
        {

            get_symbol();

            if(constName.find(symbol) == constName.end())
            {
                constName[symbol] = true;
            }
            else
            {
                errorContent(CONSTREDEF_ERR, errorLine, errorColumn);
                return ;
            }

            res = symbol;
            identifier_lex();// =

            if(symID != ASSIGN_SYMID)
            {
                errorContent(ASS_SYMID_ERR, errorLine, errorColumn);
                return ;
            }

            op = CONST_OP;
            src1 = type;
            src2 = SPACE_OP;

            genMidCode(op, src1, src2, res); /// 生成四元式

            get_symbol();
            if(symID == SUBTRACT_SYMID)
            {
                op = SUB_OP;
                src1 = ZERO_OP;

                get_symbol();
                src2 = symbol;

                /// res = res

                genMidCode(op, src1, src2, res);
            }
            else
            {
                op = ASS_OP;
                src1 = symbol;
                src2 = SPACE_OP;
                /// res = res

                genMidCode(op, src1, src2, res);
            }
            get_symbol();

            //int_lex(); /// 进<整数>
        }
    }

    cout<<"*out constDefinition_lex"<<endl;
}

/**
    ＜声明头部＞   ::=  int＜标识符＞ |float ＜标识符＞|char＜标识符＞
**/
string funcName; /// 记录有返回值函数名
string funcType; /// 记录有返回值函数类型
void headDeclaration_lex()///TODO//＜声明头部＞
{
    cout<<"*in headDeclaration_lex"<<endl;

    if(symID == INT_SYMID || symID == FLOAT_SYMID || symID == CHARACTER_SYMID)
    {
        if(symID == INT_SYMID)
        {
            funcType = INT_OP;
        }
        else
        {
            funcType = CHAR_OP;
        }

        get_symbol();

        /// 检查函数是否已经定义过
        if(funWithReturnName.find(symbol) == funWithReturnName.end())
        {
            /// 如果还没定义过就记录下来
            funWithReturnName[symbol] = true;
            functionName = symbol;
        }
        else
        {
            /// 如果已经定义过了就报错重定义
            errorContent(FUNREDEF_ERR, errorLine, errorColumn);
            return ;
        }

        funcName = symbol;
        identifier_lex();
    }
    else
    {
        errorContent(INTORCHAR_SYMID_ERR, errorLine, errorColumn);;
        return ;
    }
    cout<<"*out headDeclaration_lex"<<endl;
}

/**
    ＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}
**/
void varDeclaration_lex()//＜变量说明＞
{
    cout<<"*in varDeclaration_lex"<<endl;
    varDefinition_lex(); /// 进<变量定义>
    if(symID != SEMICOLON_SYMID)
    {
        errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn);
        return ;
    }
    get_symbol();// int | float | char

    while(symID == INT_SYMID || symID == CHAR_SYMID)
    {

        record(); /// 记录当前symbol

        readAhead(); /// 预先读一个symbol
        readAhead(); /// 再预先读一个symbol

        if(symID == LEFTSMALLBRACKET_SYMID)
        {
            rollBack(); /// 如果读到（说明是函数定义，先回退，然后结束<变量说明>
            break;
        }

        /// 否则先回退，继续进<变量定义>
        rollBack();
        varDefinition_lex();
        if(symID != SEMICOLON_SYMID)
        {
            errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn); /// 不是；结束报错
            return;
        }
        get_symbol();
    }

    /// TODO {＜变量定义＞;}
    cout<<"*out varDeclaration_lex"<<endl;
}

/**
    ＜变量定义＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’ )}
**/
void varDefinition_lex()//＜变量定义＞
{
    cout<<"*in varDefinition_lex"<<endl;
    string op,src1,src2,res; /// 四元式

    int remSymID = symID; /// 记录变量类型
    typeIdentifier_lex();

    /// 查找变量名是否已经定义过

    if(functionName == "") /// 全局变量定义
    {
        /// 如果全局变量已经定义过，则报错
        if(constName.find(symbol) != constName.end() || glbVarName.find(symbol) != glbVarName.end())
        {
//            cout<<0<<endl;
            errorContent(VARREDEF_ERR, errorLine, errorColumn);
            return ;
        }
        /// 如果未定义过，则记录下来
        else
        {
            glbVarName[symbol] = true;
        }
    }
    else /// 局部变量定义
    {
        /// 如果局部变量已经定义过，则报错
        if(locVarName.find(symbol) != locVarName.end())
        {
//            cout<<1<<endl;
            errorContent(VARREDEF_ERR, errorLine, errorColumn);
            return;
        }
        /// 如果局部变量未定义过
        else
        {
            locVarName[symbol] = true;
        }
    }

    res = symbol;
    identifier_lex();

    /// 如果读到 [ 则说明是数组
    if(symID == LEFTMIDDLEBRACKET_SYMID) // array
    {
        if(remSymID == INT_SYMID)
        {
            op = INTA_OP;
        }
        else
        {
            op = CHARA_OP;
        }

        get_symbol();
        src1 = symbol;
        unsignedInt_lex();

        if(symID == RIGHTMIDDLEBRACKET_SYMID)
        {
            get_symbol();
        }
        else
        {
            errorContent(RIGHTMIDDLEBRACKET_ERR, errorLine, errorColumn);
            return;
        }
        src2 = SPACE_OP;

        genMidCode(op, src1, src2, res);
    }
    else // not array
    {
        if(remSymID == INT_SYMID)
        {
            op = INT_OP;
        }
        else
        {
            op = CHAR_OP;
        }
        src1 = SPACE_OP;
        src2 = SPACE_OP;
        genMidCode(op, src1, src2, res);
    }

    /// 如果是 , 则重复上述操作，继续变量定义
    while(symID == COMMA_SYMID)
    {
        get_symbol();

        if(functionName == "") /// 全局变量定义
        {
            /// 如果全局变量已经定义过，则报错
            if(constName.find(symbol) != constName.end() || glbVarName.find(symbol) != glbVarName.end())
            {
                errorContent(VARREDEF_ERR, errorLine, errorColumn);
                return ;
            }
            /// 如果未定义过，则记录下来
            else
            {
                glbVarName[symbol] = true;
            }
        }
        else /// 局部变量定义
        {
            /// 如果局部变量已经定义过，则报错
            if(locVarName.find(symbol) != locVarName.end())
            {
                errorContent(VARREDEF_ERR, errorLine, errorColumn);
                return;
            }
            /// 如果局部变量未定义过
            else
            {
                locVarName[symbol] = true;
            }
        }

        res = symbol;
        identifier_lex();

        if(symID == LEFTMIDDLEBRACKET_SYMID)
        {

            if(remSymID == INT_SYMID)
            {
                op = INTA_OP;
            }
            else
            {
                op = CHARA_OP;
            }

            get_symbol();
            src1 = symbol;
            unsignedInt_lex();

            if(symID == RIGHTMIDDLEBRACKET_SYMID)
            {
                get_symbol();
            }
            else
            {

                errorContent(RIGHTMIDDLEBRACKET_ERR, errorLine, errorColumn);;
                return ;
            }
            src2 = SPACE_OP;
            genMidCode(op, src1, src2, res);
        }
        else
        {
            if(remSymID == INT_SYMID)
            {
                op = INT_OP;
            }
            else
            {
                op = CHAR_OP;
            }
            src1 = SPACE_OP;
            src2 = SPACE_OP;
            genMidCode(op, src1, src2, res);
        }
    }

    cout<<"*out varDefinition_lex"<<endl;
}

/**
    ＜参数＞    ::= ＜参数表＞
**/
void parameter_lex()//＜参数＞
{
    cout<<"*in parameter_lex"<<endl;

    parameterTable_lex(); /// 进<参数表>

    cout<<"*out parameter_lex"<<endl;
}

/**
    ＜参数表＞    ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
**/
void parameterTable_lex()//＜参数表＞
{
    cout<<"*in parameterTable_lex"<<endl;

    string op,src1,src2,res; /// 四元式
    op = PARA_OP;
    src1 = symbol;
    typeIdentifier_lex(); /// 进<类型标识符>

    res = symbol;
    /// 检查局部变量是否定义过
    if(locVarName.find(res) != locVarName.end())
    {
        errorContent(VARREDEF_ERR, errorLine, errorColumn);
        return ;
    }
    else
    {
        locVarName[res] = true;
    }

    identifier_lex(); /// 进<标识符>

    src2 = SPACE_OP;
    genMidCode(op, src1, src2, res); /// 生成四元式

    //paraNum ++;

    /// 如果是 , 则说明还有参数，重复上述操作
    while(symID == COMMA_SYMID)
    {
        get_symbol();
        op = PARA_OP;
        src1 = symbol;
        typeIdentifier_lex();

        res = symbol;
        /// 检查局部变量是否定义过
        if(locVarName.find(res) != locVarName.end())
        {
            errorContent(VARREDEF_ERR, errorLine, errorColumn);
            return ;
        }
        else
        {
            locVarName[res] = true;
        }
        identifier_lex();

        src2 = SPACE_OP;
        genMidCode(op, src1, src2, res);

        //paraNum ++;
    }

    ///TODO kong
    cout<<"*out parameterTable_lex"<<endl;
}

/**
    ＜有返回值函数定义＞  ::=  ＜声明头部＞‘(’＜参数＞‘)’ ‘{’＜复合语句＞‘}’
**/
void funWithReturnDefinition_lex() /// TODO : midcode//＜有返回值函数定义＞
{
    cout<<"*in funWithReturnDefinition_lex"<<endl;

    locVarName.clear();

    string op,src1,src2,res; /// 四元式
    headDeclaration_lex(); /// 直接进<声明头部>

    op = FUNC_OP;
    res = funcName;
    src1 = funcType;
    src2 = SPACE_OP;

    if(symID != LEFTSMALLBRACKET_SYMID) // (
    {
        errorContent(LEFTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    parameter_lex(); /// 进<参数>

    genMidCode(op,src1,src2,res);
    //get_symbol();
    if(symID != RIGHTSMALLBRACKET_SYMID) // ）
    {
        errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    if(symID != LEFTBIGBRACKET_SYMID) //{
    {
        //cout<<"1"<<endl;
        errorContent(LEFTBIGBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    compound_lex(); /// 进<复合语句>

    //get_symbol();
    if(symID != RIGHTBIGBRACKET_SYMID) // }
    {

        errorContent(RIGHTBIGBRACKET_ERR, errorLine, errorColumn);;
        return ;
    }
    get_symbol();

    //genMidCode(RET_OP,SPACE_OP,SPACE_OP,SPACE_OP);
    genMidCode(EXITS_OP,SPACE_OP,SPACE_OP,res);
    //locVarName.clear();
    cout<<"*out funWithReturnDefinition_lex"<<endl;
}

/**
    ＜无返回值函数定义＞  ::= void＜标识符＞‘(’＜参数＞‘)’‘{’＜复合语句＞‘}’
**/
void funWithoutReturnDefinition_lex() /// TODO :midcode//＜无返回值函数定义＞
{
    cout<<"*in funWithoutReturnDefinition_lex"<<endl;

    locVarName.clear();

    string op,src1,src2,res;
    if(symID != VOID_SYMID)
    {
        errorContent(VOID_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    if(funWithoutReturnName.find(symbol) == funWithoutReturnName.end())
    {
        funWithoutReturnName[symbol] = true;
        functionName = symbol;
    }
    else
    {
        errorContent(FUNREDEF_ERR, errorLine, errorColumn);
        return ;
    }

    op = FUNC_OP;
    res = symbol;
    src1 = src2 = SPACE_OP;

    identifier_lex();

    get_symbol();
    if(symID != LEFTSMALLBRACKET_SYMID)
    {
        errorContent(LEFTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    parameter_lex(); /// 参数表

    genMidCode(op,src1,src2,res); /// 生成函数定义四元式

    //get_symbol();
    if(symID != RIGHTSMALLBRACKET_SYMID)
    {
        errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    if(symID != LEFTBIGBRACKET_SYMID)
    {

        errorContent(LEFTBIGBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    compound_lex();

    get_symbol();
    if(symID != RIGHTBIGBRACKET_SYMID)
    {

        errorContent(RIGHTBIGBRACKET_ERR, errorLine, errorColumn);;
        return ;
    }
    genMidCode(RET_OP,SPACE_OP,SPACE_OP,SPACE_OP);
    genMidCode(EXITS_OP,SPACE_OP,SPACE_OP,res);
    //locVarName.clear();
    cout<<"*out funWithoutReturnDefinition_lex"<<endl;
}

/**
    ＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
**/
void compound_lex()//＜复合语句＞
{
    cout<<"*in compound_lex"<<endl;

    /// 如果读到 const 则进 <常量说明>
    if(symID == CONST_SYMID)
    {
        constDeclaration_lex();
    }

    /// 如果读到 int|float|char 则进变量说明
    if(symID == INT_SYMID || symID == FLOAT_SYMID || symID == CHAR_SYMID)
    {
        varDeclaration_lex(); // int float char
    }

    sentenceColumn_lex(); /// 进<语句列>
    cout<<"*out compound_lex"<<endl;
}

/**
    ＜主函数＞    ::= void main‘(’‘)’‘{’＜复合语句＞‘}’
**/
void mainFun_lex()//＜主函数＞
{
    cout<<"*in mainFun_lex"<<endl;

    locVarName.clear();

    string op,src1,src2,res; /// 四元式
    if(symID != VOID_SYMID) // void
    {
        errorContent(VOID_ERR, errorLine, errorColumn);
        //cout<<"!"<<endl;
        return ;
    }

    get_symbol();
    if(symID != MAIN_SYMID) // main
    {
        errorContent(MAIN_ERR, errorLine, errorColumn);
        return ;
    }

    string remSymbol = symbol;
    op = FUNC_OP;
    res = symbol;
    src1 = SPACE_OP;
    src2 = SPACE_OP;
    genMidCode(op, src1, src2, res);

    get_symbol();
    if(symID != LEFTSMALLBRACKET_SYMID) // (
    {
        errorContent(LEFTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    if(symID != RIGHTSMALLBRACKET_SYMID) // }
    {
        errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    if(symID != LEFTBIGBRACKET_SYMID) // {
    {
        errorContent(LEFTBIGBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    compound_lex(); /// 进<复合语句>

    if(symID !=RIGHTBIGBRACKET_SYMID) // }
    {
        errorContent(RIGHTBIGBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    //get_symbol();
    op = EXITS_OP;
    src1 = SPACE_OP;
    src2 = SPACE_OP;
    res = remSymbol;
    genMidCode(op, src1, src2, res); ///生成四元式

    cout<<"*out mainFun_lex"<<endl;
}

/**
    ＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
**/
void expression_lex()///TODO [+|-]//＜表达式＞
{
    cout<<"*in expression_lex"<<endl;

    string op,src1,src2,res; // 四元式
    if(symID == ADD_SYMID || symID == SUBTRACT_SYMID) // +|-
    {
        int remSymID = symID;
        add_lex();
        term_lex();
        if(remSymID == ADD_SYMID)
        {
            res = glbtmp;
        }
        else
        {
            op = SUB_OP;
            src1 = ZERO_OP;
            src2 = glbtmp;
            res = genVar();
            genMidCode(op, src1, src2, res); /// 生成四元式，将 -x 变成 0-x
        }
    }
    else
    {
        term_lex(); /// 进入<项>
        res = glbtmp;
    }

    while(symID == ADD_SYMID || symID == SUBTRACT_SYMID)
    {
        if(symID == ADD_SYMID)
        {
            op = ADD_OP;
        }
        else
        {
            op = SUB_OP;
        }
        add_lex();
        term_lex();
        src1 = res; ///上一个表达式的结果作为这一个表达式的第一个参数
        src2 = glbtmp;
        res = genVar(); /// 生成一个临时变量
        genMidCode(op, src1, src2, res); /// 生成四元式
    }
    glbtmp = res;
    cout<<"*out expression_lex"<<endl;
}

/**
    ＜项＞     ::= ＜因子＞{＜乘法运算符＞＜因子＞}
**/
void term_lex()//＜项＞
{
    cout<<"*in term_lex"<<endl;

    string op,src1,src2,res;
    factor_lex();
    res = glbtmp;

    while(symID == MULTIPLY_SYMID || symID == DIVIDE_SYMID)
    {
        if(symID == MULTIPLY_SYMID)
        {
            op = MUL_OP;
        }
        else
        {
            op = DIVS_OP;
        }
        multiply_lex();
        factor_lex();
        src1 = res;
        src2 = glbtmp;
        res = genVar();
        genMidCode(op,src1,src2,res);
    }
    glbtmp = res;
    cout<<"*out term_lex"<<endl;
}

/**
    ＜因子＞    ::= ＜标识符＞
                  |＜标识符＞‘[’＜表达式＞‘]’
                  |‘(’＜表达式＞‘)’
                  |＜整数＞
                  |＜实数＞
                  |＜字符＞
                  |＜有返回值函数调用语句＞
**/
void factor_lex()/// TODO//＜因子＞
{
    cout<<"*in factor_lex"<<endl;

    string op,src1,src2,res;
    if(symID == IDENTIFIER_SYMID)
    {
//        int preIndexInFile = indexInFile;
//        int preIndexInLine = indexInLine;
//        string preSymbol = symbol;
//        int preSymID = symID;
        record(); /// 记录当前symbol

        //get_symbol();
        readAhead(); /// 预读一个symbol

        if(symID == LEFTMIDDLEBRACKET_SYMID) /// ＜标识符＞‘[’＜表达式＞‘]’
        {
            //cout<<"＜标识符＞‘[’＜表达式＞‘]’"<<endl;
            rollBack(); /// 回退

            /// 检查变量是否定义过
            if(constName.find(symbol) == constName.end() && locVarName.find(symbol) == locVarName.end() && glbVarName.find(symbol) == glbVarName.end())
            {
                /// 如果未定义则报错
                errorContent(VARUNDEF_ERR, errorLine, errorColumn);
                return ;
            }

            identifier_lex(); /// 进<标识符>
            // [

            get_symbol();
            expression_lex(); /// 进<表达式>

            if(symID != RIGHTMIDDLEBRACKET_SYMID) // ]
            {
                errorContent(RIGHTMIDDLEBRACKET_ERR, errorLine, errorColumn);
                return ;
            }

            get_symbol();
        }
        else if(symID == LEFTSMALLBRACKET_SYMID) /// ＜有返回值函数调用语句＞
        {
            //cout<<"＜有返回值函数调用语句＞"<<endl;
            rollBack();

            /*op = CALL_OP;
            src1 = symbol;
            src2 = SPACE_OP;
            res = genVar();
            genMidCode(op,src1,src2,res);
            glbtmp = res;*/

            if(funWithReturnName.find(symbol) == funWithReturnName.end())
            {
                /// 如果函数没定义过则报错未定义
                errorContent(FUNUNDEF_ERR, errorLine, errorColumn);
                return ;
            }
            funWithReturnCall_lex(); /// 进<有返回值函数调用语句>
        }
        else /// ＜标识符＞
        {
            //cout<<"标识符"<<endl;
            rollBack(); /// 回退
            glbtmp = symbol;
            identifier_lex(); /// 进<标识符>
        }
    }
    else if(symID == LEFTSMALLBRACKET_SYMID) /// ‘(’＜表达式＞‘)’
    {
        get_symbol();
        //(

        expression_lex(); /// 进<表达式>

        if(symID != RIGHTSMALLBRACKET_SYMID)
        {
            errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
    }
    else if(symID == CHARACTER_SYMID) /// ＜字符＞
    {
        //cout<<"字符"<<endl;
        glbtmp = symbol;
        character_lex();
    }
    else //if(symID == ADD_SYMID || symID == SUBTRACT_SYMID) /// ＜整数＞|＜实数＞
    {
        //cout<<"数字"<<endl;
        //get_symbol();
        if(symID == NUMBER_SYMID)
        {
            glbtmp = symbol;
            int_lex();
        }
        if(symID == DOT_SYMID)
        {
            glbtmp = symbol;
            fractionalPart_lex();
        }
    }
    /*
    else
    {
        errorContent(42);
        return ;
    }
*/

    cout<<"*out factor_lex"<<endl;
}

/**
    ＜语句＞    ::= ＜条件语句＞
                 ｜＜循环语句＞
                 |＜情况语句＞
                 ｜＜读语句＞;
                 ｜＜写语句＞;
                 ｜＜返回语句＞;
                 | ‘{’＜语句列＞‘}’
                 ｜＜赋值语句＞;
                 ｜＜有返回值函数调用语句＞;
                 |＜无返回值函数调用语句＞;
                 ｜＜空＞;
**/
void sentence_lex()//＜语句＞
{
    cout<<"*in sentence_lex"<<endl;

    string op,src1,src2,res;
    if(symID == IF_SYMID) /// ＜条件语句＞
    {
        conditionSentence_lex();
    }
    else if(symID == WHILE_SYMID || symID == DO_SYMID || symID == FOR_SYMID) /// ＜循环语句＞
    {
        loopSentence_lex();
    }
    else if(symID == SWITCH_SYMID) /// ＜情况语句＞
    {
        situationSentence_lex();
    }
    else if(symID == LEFTBIGBRACKET_SYMID) /// ‘{’＜语句列＞‘}’
    {

        if(symID != LEFTBIGBRACKET_SYMID)
        {
            errorContent(LEFTBIGBRACKET_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();

        sentenceColumn_lex(); /// 进<语句列>

        if(symID != RIGHTBIGBRACKET_SYMID) // }
        {
            errorContent(RIGHTBIGBRACKET_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
    }
    else if(symID == SCANF_SYMID) /// ＜读语句＞;
    {
        scanf_lex();

        if(symID != SEMICOLON_SYMID)
        {
            errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
    }
    else if(symID == PRINTF_SYMID) /// ＜写语句＞;
    {
        printf_lex();

        if(symID != SEMICOLON_SYMID)
        {
            errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
    }
    else if(symID == RETURN_SYMID) /// ＜返回语句＞;
    {
        return_lex();

        if(symID != SEMICOLON_SYMID)
        {
            errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
    }
    else if(symID == IDENTIFIER_SYMID)
    {
//        int preIndexInFile = indexInFile;
//        int preIndexInLine = indexInLine;
//        string preSymbol = symbol;
//        int preSymID = symID;
        record();
        //get_symbol();
        readAhead();

        if(symID == ASSIGN_SYMID || symID == LEFTMIDDLEBRACKET_SYMID) /// ＜赋值语句＞;
        {
//            indexInFile = preIndexInFile;/// rollback
//            indexInLine = preIndexInLine;
//            symbol = preSymbol;
//            symID = preSymID;
            rollBack(); /// 回退

            /// 如果变量名在变量定义和常量定义中都没出现，则报错未定义
            if(constName.find(symbol) == constName.end() && locVarName.find(symbol) == locVarName.end() && glbVarName.find(symbol) == glbVarName.end())
            {
                errorContent(VARUNDEF_ERR, errorLine, errorColumn);
                return ;
            }

            /// 如果给常量赋值，则报错
            if(constName.find(symbol) != constName.end())
            {
                errorContent(CONSTREASSIGN_ERR, errorLine, errorColumn);
                return ;
            }

            assignSentence_lex(); /// 进<赋值语句>

            if(symID != SEMICOLON_SYMID)
            {
                errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn);
                return ;
            }

            get_symbol();
        }

        else if(funWithReturnName.find(symbol) != funWithReturnName.end()) /// ＜有返回值函数调用语句＞;
        {
            /*op = CALL_OP;
            src1 = symbol;
            src2 = SPACE_OP;
            res = genVar();
            genMidCode(op,src1,src2,res);
            glbtmp = res;*/

            funWithReturnCall_lex();
            if(symID != SEMICOLON_SYMID)
            {
                errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn);
                return ;
            }
            get_symbol();
        }
        else if(funWithoutReturnName.find(symbol) != funWithoutReturnName.end()) /// ＜无返回值函数调用语句＞;
        {
            /*op = CALL_OP;
            src1 = symbol;
            src2 = SPACE_OP;
            res = SPACE_OP;
            genMidCode(op,src1,src2,res);*/

            funWithoutReturnCall_lex();
            if(symID != SEMICOLON_SYMID)
            {
                errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn);
                return ;
            }
            get_symbol();
        }
        else /// kong
        {

        }

    }

    cout<<"*out sentence_lex"<<endl;
}

/**
    ＜语句列＞   ::= ｛＜语句＞｝
**/
void sentenceColumn_lex()//＜语句列＞
{
    cout<<"*in sentenceColumn_lex"<<endl;

    while(symID != RIGHTBIGBRACKET_SYMID)
    {
        //cout<<"in "<<fileEnd<<' ';
        sentence_lex();
        /// TODO : how to deal with ｛＜语句＞}
        //cout<<fileEnd<<endl;
//        if(symID == RIGHTBIGBRACKET_SYMID)
//        {
//            //get_symbol();
//            break;
//        }
        //cout<<"filend = "<<fileEnd<<endl;
    }
    //get_symbol();
    cout<<"*out sentenceColumn_lex"<<endl;
}

/**
    ＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞
                     |＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
**/
void assignSentence_lex()//＜赋值语句＞
{
    cout<<"*in assignSentence_lex"<<endl;

    string op,src1,src2,res;
    if(symID != IDENTIFIER_SYMID)
    {
        errorContent(IDENTIFIER_ERR, errorLine, errorColumn);
        return ;
    }

    /// 如果变量名在变量定义和常量定义中都没出现，则报错未定义
    if(constName.find(symbol) == constName.end() && locVarName.find(symbol) == locVarName.end() && glbVarName.find(symbol) == glbVarName.end())
    {
        errorContent(VARUNDEF_ERR, errorLine, errorColumn);
        return ;
    }
    /// 如果给常量赋值则报错
    if(constName.find(symbol) != constName.end())
    {
        errorContent(CONSTREASSIGN_ERR, errorLine, errorColumn);
        return ;
    }

    res = symbol;
    identifier_lex();

    if(symID == ASSIGN_SYMID)
    {
        op = ASS_OP;
        get_symbol();
        expression_lex();
        src1 = glbtmp;
        src2 = SPACE_OP;
        genMidCode(op,src1,src2,res); /// 生成四元式
    }
    else if(symID == LEFTMIDDLEBRACKET_SYMID) /// 赋值给数组
    {
        op = ASSARRY_OP;
        get_symbol();
        expression_lex();
        src2 = glbtmp;

        if(symID != RIGHTMIDDLEBRACKET_SYMID)
        {
            errorContent(RIGHTMIDDLEBRACKET_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
        if(symID != ASSIGN_SYMID)
        {
            errorContent(ASS_SYMID_ERR, errorLine, errorColumn);
            return;
        }

        get_symbol();
        expression_lex();
        src1 = glbtmp;
        genMidCode(op,src1,src2,res); /// 生成四元式
    }

    cout<<"*out assignSentence_lex"<<endl;
}

/**
    ＜条件语句＞  ::=  if ‘(’＜条件＞‘)’＜语句＞［else＜语句＞］
**/
string cdtOp,cdtSrc1,cdtSrc2;//,cdtRes;
void conditionSentence_lex()//＜条件语句＞
{
    cout<<"*in conditionSentence_lex"<<endl;

    string op,src1,src2,res; /// 四元式
    string label1 = genLabel(); /// 生成新的label
    string label2 = genLabel(); /// 生成新的label
    if(symID != IF_SYMID) // if
    {
        errorContent(IF_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    if(symID != LEFTSMALLBRACKET_SYMID) // {
    {
        errorContent(LEFTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    condition_lex(); /// 进<条件>

    //debug<<"00000000000000000000"<<endl;

    /// skip if
    op = cdtOp;
    src1 = cdtSrc1;
    src2 = cdtSrc2;

    res = label1;

    genMidCode(op,src1,src2,res);

    if(symID != RIGHTSMALLBRACKET_SYMID)
    {
        errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    sentence_lex();

    /// skip else
    op = JMP_OP;
    src1 = src2 = SPACE_OP;
    res = label2;

    genMidCode(op,src1,src2,res);

    /// set label1
    op = LAB_OP;
    src1 = src2 = SPACE_OP;
    res = label1;

    genMidCode(op,src1,src2,res);

    //while(symID == ELSE_SYMID){
    if(symID == ELSE_SYMID){
        ////debug<<"000000000000000"<<symbol<<endl;
        get_symbol();
        ////debug<<"111111111111111"<<symbol<<endl;
        sentence_lex();
        ////debug<<"222222222222222"<<symbol<<endl;
    }

    /// set label2
    op = LAB_OP;
    src1 = src2 = SPACE_OP;
    res = label2;

    genMidCode(op,src1,src2,res);

    cout<<"*out conditionSentence_lex"<<endl;
}

/**
    ＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞
                  ｜＜表达式＞ //表达式为0条件为假，否则为真
**/
void condition_lex()//＜条件＞
{
    cout<<"*in condition_lex"<<endl;

    string op,src1,src2,res;
    expression_lex();
    src1 = glbtmp;

    if(symID == LESS_SYMID || symID == LESSEQUAL_SYMID
       || symID == GREATER_SYMID || symID == GREATEREQUAL_SYMID
       || symID == EQUAL_SYMID || symID == NOTEQUAL_SYMID)
    {

        if(symID == LESS_SYMID){ // <
            //op = ST_OP;
            op = BET_OP;
            relation_lex();
            expression_lex();
            src2 = glbtmp;
            res = SPACE_OP;
            //genMidCode(op,src1,src2,res);
        }
        else if(symID == LESSEQUAL_SYMID){ // <=
            //op = SET_OP;
            op = BT_OP;
            relation_lex();
            expression_lex();
            src2 = glbtmp;
            res = SPACE_OP;
            //genMidCode(op,src1,src2,res);
        }
        else if(symID == GREATER_SYMID){ // >
            //op = BT_OP;
            op = SET_OP;
            relation_lex();
            expression_lex();
            src2 = glbtmp;
            res = SPACE_OP;
            //genMidCode(op,src1,src2,res);
        }
        else if(symID == GREATEREQUAL_SYMID){ // >=
            //op = BET_OP;
            op = ST_OP;
            relation_lex();
            expression_lex();
            src2 = glbtmp;
            res = SPACE_OP;
            //genMidCode(op,src1,src2,res);
        }
        else if(symID == NOTEQUAL_SYMID){ // !=
            //op = NEQ_OP;
            op = EQL_OP;
            relation_lex();
            expression_lex();
            src2 = glbtmp;
            res = SPACE_OP;
            //genMidCode(op,src1,src2,res);
        }
        else if(symID == EQUAL_SYMID){ // ==
            //op = EQL_OP;
            op = NEQ_OP;
            relation_lex();
            expression_lex();
            src2 = glbtmp;
            res = SPACE_OP;
            //genMidCode(op,src1,src2,res);
        }
    }
    else
    {
        op = BEQ_MIPS;
        src2 = ZERO_OP;
        res = SPACE_OP;
    }

    cdtOp = op;
    cdtSrc1 = src1;
    cdtSrc2 = src2;
    //cdtRes = res;

    cout<<"*out condition_lex"<<endl;
}

/**
    ＜for赋值语句＞   ::=  ＜标识符＞＝＜表达式＞
                     |＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
**/
string forOp,forSrc1,forSrc2,forRes;
void forAssignSentence_lex()//＜for赋值语句＞
{
    cout<<"*in assignSentence_lex"<<endl;

    string op,src1,src2,res;
    if(symID != IDENTIFIER_SYMID)
    {
        errorContent(IDENTIFIER_ERR, errorLine, errorColumn);
        return ;
    }

    /// 如果变量名在变量定义和常量定义中都没出现，则报错未定义
    if(constName.find(symbol) == constName.end() && locVarName.find(symbol) == locVarName.end() && glbVarName.find(symbol) == glbVarName.end())
    {
        errorContent(VARUNDEF_ERR, errorLine, errorColumn);
        return ;
    }
    /// 如果给常量赋值则报错
    if(constName.find(symbol) != constName.end())
    {
        errorContent(CONSTREASSIGN_ERR, errorLine, errorColumn);
        return ;
    }

    res = symbol;
    identifier_lex();

    if(symID == ASSIGN_SYMID)
    {
        op = ASS_OP;
        get_symbol();
        expression_lex();
        src1 = glbtmp;
        src2 = SPACE_OP;
        //genMidCode(op,src1,src2,res); /// 生成四元式
    }
    else if(symID == LEFTMIDDLEBRACKET_SYMID) /// 赋值给数组
    {
        op = ASSARRY_OP;
        get_symbol();
        expression_lex();
        src2 = glbtmp;

        if(symID != RIGHTMIDDLEBRACKET_SYMID)
        {
            errorContent(RIGHTMIDDLEBRACKET_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
        if(symID != ASSIGN_SYMID)
        {
            errorContent(ASS_SYMID_ERR, errorLine, errorColumn);
            return;
        }

        get_symbol();
        expression_lex();
        src1 = glbtmp;
        //genMidCode(op,src1,src2,res); /// 生成四元式
    }

    forOp = op;
    forSrc1 = src1;
    forSrc2 = src2;
    forRes = res;

    cout<<"*out assignSentence_lex"<<endl;
}

/**
    ＜循环语句＞   ::=  while ‘(’＜条件＞‘)’＜语句＞
                        |do＜语句＞while ‘(’＜条件＞‘)’
                        |for‘(’＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞‘)’＜语句＞
**/
void loopSentence_lex()//＜循环语句＞
{
    cout<<"*in loopSentence_lex"<<endl;

    string op,src1,src2,res;
    string label1 = genLabel(); /// 生成新的label
    string label2 = genLabel(); /// 生成新的label

    /// while ‘(’＜条件＞‘)’＜语句＞
    if(symID == WHILE_SYMID){ /// while loop

        if(symID != WHILE_SYMID)
        {
            errorContent(WHILE_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
        if(symID != LEFTSMALLBRACKET_SYMID)
        {
            errorContent(LEFTSMALLBRACKET_ERR, errorLine, errorColumn);
            return ;
        }

        /// set label2
        genMidCode(LAB_OP,SPACE_OP,SPACE_OP,label2);

        get_symbol();
        condition_lex();

        op = cdtOp;
        src1 = cdtSrc1;
        src2 = cdtSrc2;
        res = label1;

        /// jump to label1
        genMidCode(op,src1,src2,res);

        if(symID != RIGHTSMALLBRACKET_SYMID)
        {
            errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();

        sentence_lex(); /// 进<语句>

        /// jump to label2
        genMidCode(JMP_OP,SPACE_OP,SPACE_OP,label2);

        /// set label1
        genMidCode(LAB_OP,SPACE_OP,SPACE_OP,label1);

    }
    /// do＜语句＞while ‘(’＜条件＞‘)’
    else if(symID == DO_SYMID) /// do loop
    {
        if(symID != DO_SYMID)
        {
            errorContent(DO_ERR, errorLine, errorColumn);
            return ;
        }

        genMidCode(LAB_OP,SPACE_OP,SPACE_OP,label1); /// label1

        get_symbol();
        sentence_lex();

        if(symID != WHILE_SYMID)
        {
            errorContent(WHILE_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
        if(symID != LEFTSMALLBRACKET_SYMID)
        {
            errorContent(LEFTSMALLBRACKET_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
        condition_lex(); /// 条件

        op = cdtOp;
        src1 = cdtSrc1;
        src2 = cdtSrc2;
        res = label2;
        genMidCode(op,src1,src2,res); /// break

        genMidCode(JMP_OP,SPACE_OP,SPACE_OP,label1); /// continue loop

        genMidCode(LAB_OP,SPACE_OP,SPACE_OP,label2); /// label2


        if(symID != RIGHTSMALLBRACKET_SYMID)
        {
            errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
    }
    /// for‘(’＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞‘)’＜语句＞
    /// for‘(’＜赋值语句＞;＜条件＞;＜for赋值语句＞‘)’＜语句＞
    else if(symID == FOR_SYMID) /// for loop
    {
        if(symID != FOR_SYMID) // for
        {
            errorContent(FOR_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
        if(symID != LEFTSMALLBRACKET_SYMID) // (
        {
            errorContent(LEFTSMALLBRACKET_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
        assignSentence_lex(); /// 赋值语句


        genMidCode(LAB_OP,SPACE_OP,SPACE_OP,label1); /// label1

        if(symID != SEMICOLON_SYMID) // ;
        {
            errorContent(SEMICOLON_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
        condition_lex(); /// 条件

        op = cdtOp;
        src1 = cdtSrc1;
        src2 = cdtSrc2;
        res = label2;
        genMidCode(op,src1,src2,res);

        if(symID != SEMICOLON_SYMID)
        {
            errorContent(SEMICOLON_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
        forAssignSentence_lex(); /// for赋值语句

        op = forOp;
        src1 = forSrc1;
        src2 = forSrc2;
        res = forRes;

        if(symID != RIGHTSMALLBRACKET_SYMID)
        {
            errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
        sentence_lex();

        genMidCode(op,src1,src2,res); /// 步长

        genMidCode(JMP_OP,SPACE_OP,SPACE_OP,label1); /// continue loop

        genMidCode(LAB_OP,SPACE_OP,SPACE_OP,label2); /// label2

    }
    cout<<"*out loopSentence_lex"<<endl;

}

/**
    ＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞＜缺省＞ ‘}’
**/
string switchTmp;
void situationSentence_lex()//＜情况语句＞
{
    cout<<"*in situationSentence_lex"<<endl;

    if(symID != SWITCH_SYMID) // switch
    {
        errorContent(SWITCH_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    if(symID != LEFTSMALLBRACKET_SYMID) // (
    {
        errorContent(LEFTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    expression_lex();
    switchTmp = glbtmp;

    if(symID != RIGHTSMALLBRACKET_SYMID) // )
    {
        errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    if(symID != LEFTBIGBRACKET_SYMID) // {
    {
        errorContent(LEFTBIGBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    situationTable_lex(); /// 进<情况表>

    if(symID == DEFAULT_SYMID)
    {
        default_lex(); /// 进<缺省>
    }

    if(symID != RIGHTBIGBRACKET_SYMID) // }
    {
        errorContent(RIGHTBIGBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    cout<<"*out situationSentence_lex"<<endl;
}

/**
    ＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}
**/
void situationTable_lex()//＜情况表＞
{
    cout<<"*in situationTable_lex"<<endl;

    situationSubSentence_lex();

    while(1)
    {
        situationSubSentence_lex();
        /// TODO: when to break???
        if(symID == DEFAULT_SYMID || symID == RIGHTBIGBRACKET_SYMID) break;
    }
    cout<<"*out situationTable_lex"<<endl;
}

/**
    ＜情况子语句＞  ::=  case＜可枚举常量＞：＜语句＞
**/
void situationSubSentence_lex()//＜情况子语句＞
{
    cout<<"*in situationSubSentence_lex"<<endl;

    if(symID != CASE_SYMID) // case
    {
        errorContent(CASE_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();

    string label = genLabel();
    genMidCode(NEQ_OP,symbol,switchTmp,label);

    /// TODO : enumerableConst ?????
    enumerableConst_lex();


    if(symID != COLON_SYMID) // :
    {
        errorContent(COLON_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    sentence_lex(); /// 进<语句>
    genMidCode(LAB_OP,SPACE_OP,SPACE_OP,label);

    cout<<"*out situationSubSentence_lex"<<endl;
}

/**
    ＜缺省＞   ::=  default : ＜语句＞
                  |＜空＞
**/
void default_lex()//＜缺省＞
{
    cout<<"*in default_lex"<<endl;

    if(symID != DEFAULT_SYMID) // default
    {
        // no error
        return ;
    }

    get_symbol();
    if(symID != COLON_SYMID) // :
    {
        errorContent(COLON_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    sentence_lex(); /// 进<语句>

    cout<<"*out default_lex"<<endl;
}

/**
    ＜有返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
**/
void funWithReturnCall_lex()//＜有返回值函数调用语句＞
{
    cout<<"*in funWithReturnCall_lex"<<endl;

    string op,src1,src2,res; /// 四元式
    if(symID != IDENTIFIER_SYMID)
    {
        errorContent(IDENTIFIER_ERR, errorLine, errorColumn);
        return ;
    }
    op = CALL_OP;
    src1 = symbol;
    src2 = SPACE_OP;
    res = genVar(); /// 生成临时变量存储函数返回值

    get_symbol();
    if(symID != LEFTSMALLBRACKET_SYMID) // (
    {
        errorContent(LEFTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    //valParaNum = 0;

    get_symbol();
    valueParameterTable_lex(); /// 进<值参数表>

    if(symID != RIGHTSMALLBRACKET_SYMID) // )
    {
        errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();

    genMidCode(op,src1,src2,res); /// 生成四元式
    glbtmp = res; /// 将结果赋给全局临时变量

    //genMidCode(op,src1,src2,res);
    cout<<"*out funWithReturnCall_lex"<<endl;
}

/**
    ＜无返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
**/
void funWithoutReturnCall_lex()//＜无返回值函数调用语句＞
{
    cout<<"*in funWithoutReturnCall_lex"<<endl;

    string op,src1,src2,res; /// 四元式
    if(symID != IDENTIFIER_SYMID)
    {
        errorContent(IDENTIFIER_ERR, errorLine, errorColumn);
        return ;
    }
    op = CALL_OP;
    src1 = symbol;
    src2 = SPACE_OP;
    res = SPACE_OP;

    get_symbol();
    if(symID != LEFTSMALLBRACKET_SYMID) // (
    {
        errorContent(LEFTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    //valParaNum = 0;

    get_symbol();
    valueParameterTable_lex(); /// 进<值参数表>

    if(symID != RIGHTSMALLBRACKET_SYMID) // )
    {
        errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();

    genMidCode(op,src1,src2,res); /// 生成四元式

    cout<<"*out funWithoutReturnCall_lex"<<endl;
}

/**
    ＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}
                    ｜＜空＞
**/

stack<string> paraStack;
void valueParameterTable_lex()//＜值参数表＞
{
    cout<<"*in valueParameterTable_lex"<<endl;

    string op ,src1, src2, res;
    /// TODO : how to deal with ＜空＞ ????
    //expression_lex();
    //valParaNum ++;

    op = CALPARA_OP;
    src1 = SPACE_OP;
    src2 = SPACE_OP;
    //get_symbol();
    expression_lex(); /// 进<表达式>
    res = glbtmp; /// 将结果赋给全局临时变量
    //genMidCode(op,src1,src2,res); /// 生成四元式

    paraStack.push(res);


    while(symID == COMMA_SYMID) // ,
    {
        //valParaNum ++;

        get_symbol();
        expression_lex(); /// 进<表达式>
        res = glbtmp; /// 将结果赋给全局临时变量
        //genMidCode(op,src1,src2,res); /// 生成四元式

        paraStack.push(res);

    }

    while(!paraStack.empty()) /// 参数从右往左压栈
    {
        res = paraStack.top();
        paraStack.pop();
        genMidCode(op,src1,src2,res); /// 生成四元式
    }
    cout<<"*out valueParameterTable_lex"<<endl;
}

/**
    ＜读语句＞    ::=  scanf ‘(’＜标识符＞{,＜标识符＞}‘)’
**/
void scanf_lex()//＜读语句＞
{
    cout<<"*in scanf_lex"<<endl;

    string op,src1,src2,res;
    if(symID != SCANF_SYMID) // scanf
    {
        errorContent(SCANF_ERR, errorLine, errorColumn);
        return ;
    }
    op = SCF_OP;
    get_symbol();
    if(symID != LEFTSMALLBRACKET_SYMID) //(
    {
        errorContent(LEFTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();

    /// 检查变量是否定义过
    /// 如果变量没定义过则报错
    if(constName.find(symbol) == constName.end() && locVarName.find(symbol) == locVarName.end() && glbVarName.find(symbol) == glbVarName.end())
    {
        errorContent(VARUNDEF_ERR, errorLine, errorColumn);
        return ;
    }
    /// 如果给常量赋值则报错
    if(constName.find(symbol) != constName.end())
    {
        errorContent(CONSTREASSIGN_ERR, errorLine, errorColumn);
        return ;
    }
    src1 = SPACE_OP;
    src2 = SPACE_OP;
    res = symbol;
    identifier_lex();

    genMidCode(op,src1,src2,res); /// 生成四元式

    /// 如果是 , 则说明输入有多个变量
    while(symID == COMMA_SYMID) //，
    {
        get_symbol();
        res = symbol;
        identifier_lex();

        genMidCode(op,src1,src2,res); /// 生成四元式

        //get_symbol();
    }

    if(symID != RIGHTSMALLBRACKET_SYMID) // )
    {
        errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
         return ;
    }

    get_symbol();

    cout<<"*out scanf_lex"<<endl;
}

/**
    ＜写语句＞    ::= printf ‘(’ ＜字符串＞,＜表达式＞ ‘)’
                  | printf ‘(’＜字符串＞ ‘)’
                  | printf ‘(’＜表达式＞‘)’
**/
void printf_lex()//＜写语句＞
{
    cout<<"*in printf_lex"<<endl;

    string op,src1,src2,res;
    if(symID != PRINTF_SYMID) // printf
    {
        errorContent(PRINTF_ERR, errorLine, errorColumn);
        return ;
    }
    op = PRTF_OP;
    get_symbol();
    if(symID != LEFTSMALLBRACKET_SYMID) // (
    {
        errorContent(LEFTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    if(symID == STRING_SYMID) // string
    {
        src1 = symbol;

        if(stringTabel.find(symbol) == stringTabel.end())
        {
            stringTabel[symbol] = genString();
        }

        string_lex();

        if(symID == COMMA_SYMID)
        {
            get_symbol();
            expression_lex();
            src2 = glbtmp;

            if(symID != RIGHTSMALLBRACKET_SYMID)
            {
                errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
                return ;
            }

            get_symbol();
        }
        else if(symID == RIGHTSMALLBRACKET_SYMID)
        {
            src2 = SPACE_OP;
            get_symbol();
        }
        else
        {
            errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
            return ;
        }
    }
    else
    {
        src1 = SPACE_OP;
        expression_lex();
        src2 = glbtmp;
        if(symID != RIGHTSMALLBRACKET_SYMID)
        {
            errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
            return ;
        }
        get_symbol();
    }
    genMidCode(op,src1,src2,res); /// 生成四元式
    cout<<"*out printf_lex"<<endl;
}

/**
    ＜返回语句＞   ::=  return[‘(’＜表达式＞‘)’]

    ＜返回语句＞   ::=  return[＜表达式＞]
**/
void return_lex()//＜返回语句＞
{
    cout<<"*in return_lex"<<endl;

    string op,src1,src2,res;
    if(symID != RETURN_SYMID) // return
    {
        errorContent(RETURN_ERR, errorLine, errorColumn);
        return ;
    }

    op = RET_OP;
    src1 = SPACE_OP;
    src2 = SPACE_OP;
    get_symbol();
    //cout<<symbol<<endl;

    /// return (expr);
    if(symID == LEFTSMALLBRACKET_SYMID) // (
    {

        get_symbol();
        expression_lex();
        res = glbtmp; /// 将值赋给全局临时变量

        if(symID != RIGHTSMALLBRACKET_SYMID) // )
        {
            //error
            errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
            return ;
        }
        get_symbol();
    }
    /// return expr;
    else if(symID == IDENTIFIER_SYMID || symID == NUMBER_SYMID || symID == CHARACTER_SYMID)
    {
        expression_lex();
        res = glbtmp; /// 将值赋给全局临时变量

        if(symID != RIGHTSMALLBRACKET_SYMID) // )
        {
            //error
            errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
            return ;
        }
        get_symbol();
    }
    /// return
    else if(symID != SEMICOLON_SYMID) // ；
    {
        errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn);
        return ;
    }
    /// return ;
    else
    {
        res = SPACE_OP;
        //cout<<"*in 1"<<endl;
        //return;
    }
    genMidCode(op,src1,src2,res); /// 生成四元式
    cout<<"*out return_lex"<<endl;
}

/**
    ＜加法运算符＞  ::= +｜-
**/
void add_lex()//＜加法运算符＞
{
    cout<<"*in add_lex"<<endl;

    if(symID != ADD_SYMID && symID != SUBTRACT_SYMID)
    {
        //errorLine(indexInFile);
        errorContent(ADDORSUB_OPE_ERR, errorLine, errorColumn);
        return ;
    }
    get_symbol();
    cout<<"*out add_lex"<<endl;
}

/**
    ＜乘法运算符＞  ::= *｜/
**/
void multiply_lex()//＜乘法运算符＞
{
    cout<<"*in multiply_lex"<<endl;

    if(symID != MULTIPLY_SYMID && symID != DIVIDE_SYMID)
    {
        errorContent(MULORDIV_OPE_ERR, errorLine, errorColumn);
        return ;
    }
    get_symbol();
    cout<<"*out multiply_lex"<<endl;
}

/**
    ＜关系运算符＞  ::=  <｜<=｜>｜>=｜!=｜==
**/
void relation_lex()//＜关系运算符＞
{
    cout<<"*in relation_lex"<<endl;

    if(symID != LESS_SYMID && symID != LESSEQUAL_SYMID &&
       symID != GREATER_SYMID && symID != GREATEREQUAL_SYMID &&
       symID != NOTEQUAL_SYMID && symID != EQUAL_SYMID)
    {
        errorContent(RELATION_OPE_ERR, errorLine, errorColumn);
        return ;
    }
    get_symbol();
    cout<<"*out relation_lex"<<endl;
}

/**
    ＜字母＞   ::= ＿｜a｜．．．｜z｜A｜．．．｜Z
**/
void letter_lex()//＜字母＞
{
    cout<<"*in letter_lex"<<endl;
    if(symID != LETTER_SYMID)
    {
        errorContent(LETTER_ERR, errorLine, errorColumn);
    }
    get_symbol();
    cout<<"*out letter_lex"<<endl;
}

/**
    ＜数字＞   ::= ０｜＜非零数字＞
**/
void digit_lex()//＜数字＞
{
    cout<<"*in digit_lex"<<endl;

    if(symID != DIGIT_SYMID)
    {
        errorContent(DIGIT_ERR, errorLine, errorColumn);
        return ;
    }

    if(symbol == "0")
    {
        get_symbol();
    }
    else {
        //get_symbol();
        notZeroDigit_lex();
    }

    cout<<"*out digit_lex"<<endl;
}

/**
    ＜非零数字＞  ::= １｜．．．｜９
**/
void notZeroDigit_lex()// ＜非零数字＞
{
    cout<<"*in notZeroDigit_lex"<<endl;

    if(symID != DIGIT_SYMID)
    {
        errorContent(DIGIT_ERR,errorLine, errorColumn);
        return ;
    }

    if(!(symbol >= "1" && symbol <= "9"))
    {
        errorContent(NOTZERO_ERR, errorLine, errorColumn);
        return ;
    }
    get_symbol();
    cout<<"*out notZeroDigit_lex"<<endl;
}

/**
    ＜字符＞    ::=  '＜加法运算符＞'｜'＜乘法运算符＞'｜'＜字母＞'｜'＜数字＞'
**/
void character_lex()//＜字符＞
{
    cout<<"*in character_lex"<<endl;

    if(symID != SINGALQUOTATION_SYMID)
    {
        errorContent(SINGALQUOTATION_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    if(symID == ADD_SYMID)
    {
        add_lex();
    }
    else if(symID == MULTIPLY_SYMID)
    {
        multiply_lex();
    }
    else if(symID == LETTER_SYMID)
    {
        letter_lex();
    }
    else if(symID == DIGIT_SYMID)
    {
        digit_lex();
    }
    else
    {
        errorContent(CHARACTER_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    if(symID != SINGALQUOTATION_SYMID)
    {
        errorContent(SINGALQUOTATION_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();
    cout<<"*out character_lex"<<endl;
}

/**
    ＜字符串＞   ::=  "｛十进制编码为32,33,35-126的ASCII字符｝"
**/
void string_lex()//＜字符串＞
{
//    while(symbol == 32 || symbol == 33 || symbol >= 35 && symbol <= 126)
//    {
//
//    }
    cout<<"*in string_lex"<<endl;

    /*if(symID != DOUBLEQUOTATION_SYMID)
    {
        errorContent(10);
        return ;
    }

    get_symbol();*/
    if(symID != STRING_SYMID)
    {
        errorContent(STRING_ERR, errorLine, errorColumn);
        return ;
    }
/*
    get_symbol();
    if(symID != DOUBLEQUOTATION_SYMID)
    {
        errorContent(12);
        return ;
    }
*/
    get_symbol();
    cout<<"*out string_lex"<<endl;
}

/**
    ＜无符号整数＞  ::= ＜非零数字＞｛＜数字＞｝
    ###＜无符号整数＞  ::= DIGIT_SYMID
**/
void unsignedInt_lex()//＜无符号整数＞
{
    cout<<"*in unsignedInt_lex"<<endl;

    if(symID == NUMBER_SYMID)
    {
        get_symbol();
    }
    else
    {
        errorContent(NUMBER_ERR, errorLine, errorColumn);
        return ;
    }

    cout<<"*out unsignedInt_lex"<<endl;
}
/**
    ＜整数＞        ::= ［＋｜－］＜无符号整数＞｜０
**/
void int_lex()//＜整数＞
{
    cout<<"*in int_lex"<<endl;

    if(symID == ADD_SYMID || symID == SUBTRACT_SYMID)
    {
        get_symbol();
        unsignedInt_lex();
    }
    else if(symID == NUMBER_SYMID)
    {
        unsignedInt_lex(); /// 进<无符号整数>
    }
    else
    {
        errorContent(INT_ERR, errorLine, errorColumn);
        return ;
    }

    cout<<"*out int_lex"<<endl;
}
/**
    ＜小数部分＞    ::= ＜数字＞｛＜数字＞｝｜＜空＞
**/
void fractionalPart_lex()//＜小数部分＞
{
    cout<<"*in fractionalPart_lex"<<endl;
    if(symID == ZEROHEADNUMBER_SYMID)
    {

        get_symbol();
    }
    else if(symID == NUMBER_SYMID)
    {
        get_symbol();
    }
    else ///TODO : kong
    {
        get_symbol();
    }
    cout<<"*out fractionalPart_lex"<<endl;
}
/**
    ＜实数＞        ::= ［＋｜－］＜整数＞[.＜小数部分＞]
**/
void realNumber_lex()//＜实数＞
{
    cout<<"*in realNumber_lex"<<endl;
    if(symID == ADD_SYMID)
    {
        add_lex();
    }

    int_lex();

    if(symID == DOT_SYMID)
    {
        get_symbol();
        fractionalPart_lex();
    }
    cout<<"*out realNumber_lex"<<endl;
}
/**
    ＜标识符＞    ::=  ＜字母＞｛＜字母＞｜＜数字＞｝
**/
void identifier_lex()//＜标识符＞
{
    cout<<"*in identifier_lex"<<endl;
    if(symID == IDENTIFIER_SYMID)
    {
        get_symbol();
    }
    else
    {
        errorContent(IDENTIFIER_ERR, errorLine, errorColumn);
        return ;
    }
    cout<<"*out identifier_lex"<<endl;
}

/**
    ＜可枚举常量＞   ::=  ＜整数＞|＜字符＞
**/
void enumerableConst_lex()///TODO ＜可枚举常量＞??//＜可枚举常量＞
{
    cout<<"*in enumerableConst_lex"<<endl;

    if(symID == NUMBER_SYMID)
    {
        int_lex();
    }
    else if(symID == ADD_SYMID || symID == MULTIPLY_SYMID || symID == LETTER_SYMID || symID == DIGIT_SYMID)
    {
        letter_lex();
        /// TODO character?
    }
    else
    {
        errorContent(ENUMERABEL_ERR, errorLine, errorColumn);
    }
    cout<<"*out enumerableConst_lex"<<endl;
}
/**
    ＜类型标识符＞      ::=  int | float | char
**/
void typeIdentifier_lex()//＜类型标识符＞
{
    cout<<"*in typeIdentifier_lex"<<endl;

    if(symID == INT_SYMID || symID == FLOAT_SYMID || symID == CHAR_SYMID)
    {
        get_symbol();
    }
    else
    {
        errorContent(INTORCHAR_SYMID_ERR, errorLine, errorColumn);
        return ;
    }
    cout<<"*out typeIdentifier_lex"<<endl;
}
