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

void record() /// ��¼��ǰλ��
{
    preIndexInSymbolTabel = indexInSymbolTabel;
    preSymbol = symbol;
    preSymID = symID;
    preErrorLine = errorLine;
    preErrorColumn = errorColumn;
}

void rollBack() /// ���˵�֮ǰ��λ��
{
    cout<<"***[rollBack]***"<<endl;
    //debug<<"***[rollBack]***"<<endl;
    indexInSymbolTabel = preIndexInSymbolTabel;
    symbol = preSymbol;
    symID = preSymID;
    errorLine = preErrorLine;
    errorColumn = preErrorColumn;
}

void readAhead() /// Ԥ��һ��symbol
{
    cout<<"***[readAhead]***"<<"   ";
    //debug<<"***[readAhead]***"<<"   ";
    get_symbol();
}

void syn_init()//�﷨������ʼ��
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

void syn_run()//�����﷨����
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
    structVarDefinition_lex(); /// ��<��������>
    if(symID != SEMICOLON_SYMID)
    {
        errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn);
        return ;
    }
    get_symbol();// int | float | char

    while(symID == INT_SYMID || symID == CHAR_SYMID)
    {

        record(); /// ��¼��ǰsymbol

        readAhead(); /// Ԥ�ȶ�һ��symbol
        readAhead(); /// ��Ԥ�ȶ�һ��symbol

        if(symID == LEFTSMALLBRACKET_SYMID)
        {
            rollBack(); /// ���������˵���Ǻ������壬�Ȼ��ˣ�Ȼ�����<����˵��>
            break;
        }

        /// �����Ȼ��ˣ�������<��������>
        rollBack();
        structVarDefinition_lex();
        if(symID != SEMICOLON_SYMID)
        {
            errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn); /// ���ǣ���������
            return;
        }
        get_symbol();
    }

    /// TODO {���������壾;}
    cout<<"*out structVarDeclaration_lex"<<endl;
}

string structName;
int arraySiz;
void structVarDefinition_lex()
{
    cout<<"*in structVarDefinition_lex"<<endl;
    string op,src1,src2,res; /// ��Ԫʽ

    int remSymID = symID; /// ��¼��������
    typeIdentifier_lex();

    structName = symbol;

        /// ���ȫ�ֱ����Ѿ���������򱨴�
        if(constName.find(symbol) != constName.end() || glbVarName.find(symbol) != glbVarName.end())
        {
//            cout<<0<<endl;
            errorContent(VARREDEF_ERR, errorLine, errorColumn);
            return ;
        }
        /// ���δ����������¼����
        else
        {
            glbVarName[symbol] = true;
        }


    res = symbol;
    identifier_lex();

    /// ������� [ ��˵��������
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

    /// ����� , ���ظ�����������������������
    while(symID == COMMA_SYMID)
    {
        get_symbol();
        structName = symbol;


            /// ���ȫ�ֱ����Ѿ���������򱨴�
            if(constName.find(symbol) != constName.end() || glbVarName.find(symbol) != glbVarName.end())
            {
                errorContent(VARREDEF_ERR, errorLine, errorColumn);
                return ;
            }
            /// ���δ����������¼����
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
    ������    ::= �ۣ�����˵�����ݣۣ�����˵������[<�ṹ��˵��>] {���з���ֵ�������壾|���޷���ֵ�������壾}����������
**/
void program_lex()//������
{
    cout<<"*in program_lex"<<endl;

    /// �ۣ�����˵������
    if(symID == CONST_SYMID)
    {
        constDeclaration_lex();
    }

    /// �ۣ�����˵������
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

    /// [<�ṹ��˵��>]
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

    /// {���з���ֵ�������壾|���޷���ֵ�������壾}
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

    /// ����������
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
    ������˵���� ::=  const���������壾;{ const���������壾;}
**/
void constDeclaration_lex()//������˵����
{
    cout<<"*in constDeclaration_lex"<<endl;

    //const
    if(symID != CONST_SYMID)
    {
        errorContent(CONST_SYMID_ERR, errorLine, errorColumn);
        // ��һ��symbol������const
        return;
    }

    get_symbol();
    constDefinition_lex(); /// ��<��������>

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
    ���������壾   ::=   int����ʶ��������������{,����ʶ��������������}
                          | float����ʶ��������ʵ����{,����ʶ��������ʵ����}
                          | char����ʶ���������ַ���{,����ʶ���������ַ���}
**/
void constDefinition_lex()///TODO//���������壾
{
    cout<<"*in constDefinition_lex"<<endl;

    string op,src1,src2,res; /// ��Ԫʽ
    string type;

    op = CONST_OP;
    if(symID != INT_SYMID && symID != FLOAT_SYMID && symID != CHAR_SYMID)
    {
        errorContent(INTORCHAR_SYMID_ERR, errorLine, errorColumn);
        // ��һ�������� int float char
        return ;
    }

    else//if(symID == INT_SYMID)///int����ʶ��������������{,����ʶ��������������}
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

        genMidCode(op, src1, src2, res); /// ������Ԫʽ

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

        //int_lex(); /// ��<����>

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

            genMidCode(op, src1, src2, res); /// ������Ԫʽ

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

            //int_lex(); /// ��<����>
        }
    }

    cout<<"*out constDefinition_lex"<<endl;
}

/**
    ������ͷ����   ::=  int����ʶ���� |float ����ʶ����|char����ʶ����
**/
string funcName; /// ��¼�з���ֵ������
string funcType; /// ��¼�з���ֵ��������
void headDeclaration_lex()///TODO//������ͷ����
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

        /// ��麯���Ƿ��Ѿ������
        if(funWithReturnName.find(symbol) == funWithReturnName.end())
        {
            /// �����û������ͼ�¼����
            funWithReturnName[symbol] = true;
            functionName = symbol;
        }
        else
        {
            /// ����Ѿ�������˾ͱ����ض���
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
    ������˵����  ::= ���������壾;{���������壾;}
**/
void varDeclaration_lex()//������˵����
{
    cout<<"*in varDeclaration_lex"<<endl;
    varDefinition_lex(); /// ��<��������>
    if(symID != SEMICOLON_SYMID)
    {
        errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn);
        return ;
    }
    get_symbol();// int | float | char

    while(symID == INT_SYMID || symID == CHAR_SYMID)
    {

        record(); /// ��¼��ǰsymbol

        readAhead(); /// Ԥ�ȶ�һ��symbol
        readAhead(); /// ��Ԥ�ȶ�һ��symbol

        if(symID == LEFTSMALLBRACKET_SYMID)
        {
            rollBack(); /// ���������˵���Ǻ������壬�Ȼ��ˣ�Ȼ�����<����˵��>
            break;
        }

        /// �����Ȼ��ˣ�������<��������>
        rollBack();
        varDefinition_lex();
        if(symID != SEMICOLON_SYMID)
        {
            errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn); /// ���ǣ���������
            return;
        }
        get_symbol();
    }

    /// TODO {���������壾;}
    cout<<"*out varDeclaration_lex"<<endl;
}

/**
    ���������壾  ::= �����ͱ�ʶ����(����ʶ����|����ʶ������[�����޷�����������]��){,(����ʶ����|����ʶ������[�����޷�����������]�� )}
**/
void varDefinition_lex()//���������壾
{
    cout<<"*in varDefinition_lex"<<endl;
    string op,src1,src2,res; /// ��Ԫʽ

    int remSymID = symID; /// ��¼��������
    typeIdentifier_lex();

    /// ���ұ������Ƿ��Ѿ������

    if(functionName == "") /// ȫ�ֱ�������
    {
        /// ���ȫ�ֱ����Ѿ���������򱨴�
        if(constName.find(symbol) != constName.end() || glbVarName.find(symbol) != glbVarName.end())
        {
//            cout<<0<<endl;
            errorContent(VARREDEF_ERR, errorLine, errorColumn);
            return ;
        }
        /// ���δ����������¼����
        else
        {
            glbVarName[symbol] = true;
        }
    }
    else /// �ֲ���������
    {
        /// ����ֲ������Ѿ���������򱨴�
        if(locVarName.find(symbol) != locVarName.end())
        {
//            cout<<1<<endl;
            errorContent(VARREDEF_ERR, errorLine, errorColumn);
            return;
        }
        /// ����ֲ�����δ�����
        else
        {
            locVarName[symbol] = true;
        }
    }

    res = symbol;
    identifier_lex();

    /// ������� [ ��˵��������
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

    /// ����� , ���ظ�����������������������
    while(symID == COMMA_SYMID)
    {
        get_symbol();

        if(functionName == "") /// ȫ�ֱ�������
        {
            /// ���ȫ�ֱ����Ѿ���������򱨴�
            if(constName.find(symbol) != constName.end() || glbVarName.find(symbol) != glbVarName.end())
            {
                errorContent(VARREDEF_ERR, errorLine, errorColumn);
                return ;
            }
            /// ���δ����������¼����
            else
            {
                glbVarName[symbol] = true;
            }
        }
        else /// �ֲ���������
        {
            /// ����ֲ������Ѿ���������򱨴�
            if(locVarName.find(symbol) != locVarName.end())
            {
                errorContent(VARREDEF_ERR, errorLine, errorColumn);
                return;
            }
            /// ����ֲ�����δ�����
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
    ��������    ::= ��������
**/
void parameter_lex()//��������
{
    cout<<"*in parameter_lex"<<endl;

    parameterTable_lex(); /// ��<������>

    cout<<"*out parameter_lex"<<endl;
}

/**
    ��������    ::=  �����ͱ�ʶ��������ʶ����{,�����ͱ�ʶ��������ʶ����}| ���գ�
**/
void parameterTable_lex()//��������
{
    cout<<"*in parameterTable_lex"<<endl;

    string op,src1,src2,res; /// ��Ԫʽ
    op = PARA_OP;
    src1 = symbol;
    typeIdentifier_lex(); /// ��<���ͱ�ʶ��>

    res = symbol;
    /// ���ֲ������Ƿ����
    if(locVarName.find(res) != locVarName.end())
    {
        errorContent(VARREDEF_ERR, errorLine, errorColumn);
        return ;
    }
    else
    {
        locVarName[res] = true;
    }

    identifier_lex(); /// ��<��ʶ��>

    src2 = SPACE_OP;
    genMidCode(op, src1, src2, res); /// ������Ԫʽ

    //paraNum ++;

    /// ����� , ��˵�����в������ظ���������
    while(symID == COMMA_SYMID)
    {
        get_symbol();
        op = PARA_OP;
        src1 = symbol;
        typeIdentifier_lex();

        res = symbol;
        /// ���ֲ������Ƿ����
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
    ���з���ֵ�������壾  ::=  ������ͷ������(������������)�� ��{����������䣾��}��
**/
void funWithReturnDefinition_lex() /// TODO : midcode//���з���ֵ�������壾
{
    cout<<"*in funWithReturnDefinition_lex"<<endl;

    locVarName.clear();

    string op,src1,src2,res; /// ��Ԫʽ
    headDeclaration_lex(); /// ֱ�ӽ�<����ͷ��>

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
    parameter_lex(); /// ��<����>

    genMidCode(op,src1,src2,res);
    //get_symbol();
    if(symID != RIGHTSMALLBRACKET_SYMID) // ��
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
    compound_lex(); /// ��<�������>

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
    ���޷���ֵ�������壾  ::= void����ʶ������(������������)����{����������䣾��}��
**/
void funWithoutReturnDefinition_lex() /// TODO :midcode//���޷���ֵ�������壾
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
    parameter_lex(); /// ������

    genMidCode(op,src1,src2,res); /// ���ɺ���������Ԫʽ

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
    ��������䣾   ::=  �ۣ�����˵�����ݣۣ�����˵�����ݣ�����У�
**/
void compound_lex()//��������䣾
{
    cout<<"*in compound_lex"<<endl;

    /// ������� const ��� <����˵��>
    if(symID == CONST_SYMID)
    {
        constDeclaration_lex();
    }

    /// ������� int|float|char �������˵��
    if(symID == INT_SYMID || symID == FLOAT_SYMID || symID == CHAR_SYMID)
    {
        varDeclaration_lex(); // int float char
    }

    sentenceColumn_lex(); /// ��<�����>
    cout<<"*out compound_lex"<<endl;
}

/**
    ����������    ::= void main��(����)����{����������䣾��}��
**/
void mainFun_lex()//����������
{
    cout<<"*in mainFun_lex"<<endl;

    locVarName.clear();

    string op,src1,src2,res; /// ��Ԫʽ
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
    compound_lex(); /// ��<�������>

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
    genMidCode(op, src1, src2, res); ///������Ԫʽ

    cout<<"*out mainFun_lex"<<endl;
}

/**
    �����ʽ��    ::= �ۣ������ݣ��{���ӷ�����������}
**/
void expression_lex()///TODO [+|-]//�����ʽ��
{
    cout<<"*in expression_lex"<<endl;

    string op,src1,src2,res; // ��Ԫʽ
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
            genMidCode(op, src1, src2, res); /// ������Ԫʽ���� -x ��� 0-x
        }
    }
    else
    {
        term_lex(); /// ����<��>
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
        src1 = res; ///��һ�����ʽ�Ľ����Ϊ��һ�����ʽ�ĵ�һ������
        src2 = glbtmp;
        res = genVar(); /// ����һ����ʱ����
        genMidCode(op, src1, src2, res); /// ������Ԫʽ
    }
    glbtmp = res;
    cout<<"*out expression_lex"<<endl;
}

/**
    ���     ::= �����ӣ�{���˷�������������ӣ�}
**/
void term_lex()//���
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
    �����ӣ�    ::= ����ʶ����
                  |����ʶ������[�������ʽ����]��
                  |��(�������ʽ����)��
                  |��������
                  |��ʵ����
                  |���ַ���
                  |���з���ֵ����������䣾
**/
void factor_lex()/// TODO//�����ӣ�
{
    cout<<"*in factor_lex"<<endl;

    string op,src1,src2,res;
    if(symID == IDENTIFIER_SYMID)
    {
//        int preIndexInFile = indexInFile;
//        int preIndexInLine = indexInLine;
//        string preSymbol = symbol;
//        int preSymID = symID;
        record(); /// ��¼��ǰsymbol

        //get_symbol();
        readAhead(); /// Ԥ��һ��symbol

        if(symID == LEFTMIDDLEBRACKET_SYMID) /// ����ʶ������[�������ʽ����]��
        {
            //cout<<"����ʶ������[�������ʽ����]��"<<endl;
            rollBack(); /// ����

            /// �������Ƿ����
            if(constName.find(symbol) == constName.end() && locVarName.find(symbol) == locVarName.end() && glbVarName.find(symbol) == glbVarName.end())
            {
                /// ���δ�����򱨴�
                errorContent(VARUNDEF_ERR, errorLine, errorColumn);
                return ;
            }

            identifier_lex(); /// ��<��ʶ��>
            // [

            get_symbol();
            expression_lex(); /// ��<���ʽ>

            if(symID != RIGHTMIDDLEBRACKET_SYMID) // ]
            {
                errorContent(RIGHTMIDDLEBRACKET_ERR, errorLine, errorColumn);
                return ;
            }

            get_symbol();
        }
        else if(symID == LEFTSMALLBRACKET_SYMID) /// ���з���ֵ����������䣾
        {
            //cout<<"���з���ֵ����������䣾"<<endl;
            rollBack();

            /*op = CALL_OP;
            src1 = symbol;
            src2 = SPACE_OP;
            res = genVar();
            genMidCode(op,src1,src2,res);
            glbtmp = res;*/

            if(funWithReturnName.find(symbol) == funWithReturnName.end())
            {
                /// �������û������򱨴�δ����
                errorContent(FUNUNDEF_ERR, errorLine, errorColumn);
                return ;
            }
            funWithReturnCall_lex(); /// ��<�з���ֵ�����������>
        }
        else /// ����ʶ����
        {
            //cout<<"��ʶ��"<<endl;
            rollBack(); /// ����
            glbtmp = symbol;
            identifier_lex(); /// ��<��ʶ��>
        }
    }
    else if(symID == LEFTSMALLBRACKET_SYMID) /// ��(�������ʽ����)��
    {
        get_symbol();
        //(

        expression_lex(); /// ��<���ʽ>

        if(symID != RIGHTSMALLBRACKET_SYMID)
        {
            errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
    }
    else if(symID == CHARACTER_SYMID) /// ���ַ���
    {
        //cout<<"�ַ�"<<endl;
        glbtmp = symbol;
        character_lex();
    }
    else //if(symID == ADD_SYMID || symID == SUBTRACT_SYMID) /// ��������|��ʵ����
    {
        //cout<<"����"<<endl;
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
    ����䣾    ::= ��������䣾
                 ����ѭ����䣾
                 |�������䣾
                 ��������䣾;
                 ����д��䣾;
                 ����������䣾;
                 | ��{��������У���}��
                 ������ֵ��䣾;
                 �����з���ֵ����������䣾;
                 |���޷���ֵ����������䣾;
                 �����գ�;
**/
void sentence_lex()//����䣾
{
    cout<<"*in sentence_lex"<<endl;

    string op,src1,src2,res;
    if(symID == IF_SYMID) /// ��������䣾
    {
        conditionSentence_lex();
    }
    else if(symID == WHILE_SYMID || symID == DO_SYMID || symID == FOR_SYMID) /// ��ѭ����䣾
    {
        loopSentence_lex();
    }
    else if(symID == SWITCH_SYMID) /// �������䣾
    {
        situationSentence_lex();
    }
    else if(symID == LEFTBIGBRACKET_SYMID) /// ��{��������У���}��
    {

        if(symID != LEFTBIGBRACKET_SYMID)
        {
            errorContent(LEFTBIGBRACKET_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();

        sentenceColumn_lex(); /// ��<�����>

        if(symID != RIGHTBIGBRACKET_SYMID) // }
        {
            errorContent(RIGHTBIGBRACKET_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
    }
    else if(symID == SCANF_SYMID) /// ������䣾;
    {
        scanf_lex();

        if(symID != SEMICOLON_SYMID)
        {
            errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
    }
    else if(symID == PRINTF_SYMID) /// ��д��䣾;
    {
        printf_lex();

        if(symID != SEMICOLON_SYMID)
        {
            errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
    }
    else if(symID == RETURN_SYMID) /// ��������䣾;
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

        if(symID == ASSIGN_SYMID || symID == LEFTMIDDLEBRACKET_SYMID) /// ����ֵ��䣾;
        {
//            indexInFile = preIndexInFile;/// rollback
//            indexInLine = preIndexInLine;
//            symbol = preSymbol;
//            symID = preSymID;
            rollBack(); /// ����

            /// ����������ڱ�������ͳ��������ж�û���֣��򱨴�δ����
            if(constName.find(symbol) == constName.end() && locVarName.find(symbol) == locVarName.end() && glbVarName.find(symbol) == glbVarName.end())
            {
                errorContent(VARUNDEF_ERR, errorLine, errorColumn);
                return ;
            }

            /// �����������ֵ���򱨴�
            if(constName.find(symbol) != constName.end())
            {
                errorContent(CONSTREASSIGN_ERR, errorLine, errorColumn);
                return ;
            }

            assignSentence_lex(); /// ��<��ֵ���>

            if(symID != SEMICOLON_SYMID)
            {
                errorContent(LACKSEMICOLON_ERR, errorLine, errorColumn);
                return ;
            }

            get_symbol();
        }

        else if(funWithReturnName.find(symbol) != funWithReturnName.end()) /// ���з���ֵ����������䣾;
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
        else if(funWithoutReturnName.find(symbol) != funWithoutReturnName.end()) /// ���޷���ֵ����������䣾;
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
    ������У�   ::= ������䣾��
**/
void sentenceColumn_lex()//������У�
{
    cout<<"*in sentenceColumn_lex"<<endl;

    while(symID != RIGHTBIGBRACKET_SYMID)
    {
        //cout<<"in "<<fileEnd<<' ';
        sentence_lex();
        /// TODO : how to deal with ������䣾}
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
    ����ֵ��䣾   ::=  ����ʶ�����������ʽ��
                     |����ʶ������[�������ʽ����]��=�����ʽ��
**/
void assignSentence_lex()//����ֵ��䣾
{
    cout<<"*in assignSentence_lex"<<endl;

    string op,src1,src2,res;
    if(symID != IDENTIFIER_SYMID)
    {
        errorContent(IDENTIFIER_ERR, errorLine, errorColumn);
        return ;
    }

    /// ����������ڱ�������ͳ��������ж�û���֣��򱨴�δ����
    if(constName.find(symbol) == constName.end() && locVarName.find(symbol) == locVarName.end() && glbVarName.find(symbol) == glbVarName.end())
    {
        errorContent(VARUNDEF_ERR, errorLine, errorColumn);
        return ;
    }
    /// �����������ֵ�򱨴�
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
        genMidCode(op,src1,src2,res); /// ������Ԫʽ
    }
    else if(symID == LEFTMIDDLEBRACKET_SYMID) /// ��ֵ������
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
        genMidCode(op,src1,src2,res); /// ������Ԫʽ
    }

    cout<<"*out assignSentence_lex"<<endl;
}

/**
    ��������䣾  ::=  if ��(������������)������䣾��else����䣾��
**/
string cdtOp,cdtSrc1,cdtSrc2;//,cdtRes;
void conditionSentence_lex()//��������䣾
{
    cout<<"*in conditionSentence_lex"<<endl;

    string op,src1,src2,res; /// ��Ԫʽ
    string label1 = genLabel(); /// �����µ�label
    string label2 = genLabel(); /// �����µ�label
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
    condition_lex(); /// ��<����>

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
    ��������    ::=  �����ʽ������ϵ������������ʽ��
                  �������ʽ�� //���ʽΪ0����Ϊ�٣�����Ϊ��
**/
void condition_lex()//��������
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
    ��for��ֵ��䣾   ::=  ����ʶ�����������ʽ��
                     |����ʶ������[�������ʽ����]��=�����ʽ��
**/
string forOp,forSrc1,forSrc2,forRes;
void forAssignSentence_lex()//��for��ֵ��䣾
{
    cout<<"*in assignSentence_lex"<<endl;

    string op,src1,src2,res;
    if(symID != IDENTIFIER_SYMID)
    {
        errorContent(IDENTIFIER_ERR, errorLine, errorColumn);
        return ;
    }

    /// ����������ڱ�������ͳ��������ж�û���֣��򱨴�δ����
    if(constName.find(symbol) == constName.end() && locVarName.find(symbol) == locVarName.end() && glbVarName.find(symbol) == glbVarName.end())
    {
        errorContent(VARUNDEF_ERR, errorLine, errorColumn);
        return ;
    }
    /// �����������ֵ�򱨴�
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
        //genMidCode(op,src1,src2,res); /// ������Ԫʽ
    }
    else if(symID == LEFTMIDDLEBRACKET_SYMID) /// ��ֵ������
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
        //genMidCode(op,src1,src2,res); /// ������Ԫʽ
    }

    forOp = op;
    forSrc1 = src1;
    forSrc2 = src2;
    forRes = res;

    cout<<"*out assignSentence_lex"<<endl;
}

/**
    ��ѭ����䣾   ::=  while ��(������������)������䣾
                        |do����䣾while ��(������������)��
                        |for��(������ʶ�����������ʽ��;��������;����ʶ����������ʶ����(+|-)����������)������䣾
**/
void loopSentence_lex()//��ѭ����䣾
{
    cout<<"*in loopSentence_lex"<<endl;

    string op,src1,src2,res;
    string label1 = genLabel(); /// �����µ�label
    string label2 = genLabel(); /// �����µ�label

    /// while ��(������������)������䣾
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

        sentence_lex(); /// ��<���>

        /// jump to label2
        genMidCode(JMP_OP,SPACE_OP,SPACE_OP,label2);

        /// set label1
        genMidCode(LAB_OP,SPACE_OP,SPACE_OP,label1);

    }
    /// do����䣾while ��(������������)��
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
        condition_lex(); /// ����

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
    /// for��(������ʶ�����������ʽ��;��������;����ʶ����������ʶ����(+|-)����������)������䣾
    /// for��(������ֵ��䣾;��������;��for��ֵ��䣾��)������䣾
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
        assignSentence_lex(); /// ��ֵ���


        genMidCode(LAB_OP,SPACE_OP,SPACE_OP,label1); /// label1

        if(symID != SEMICOLON_SYMID) // ;
        {
            errorContent(SEMICOLON_ERR, errorLine, errorColumn);
            return ;
        }

        get_symbol();
        condition_lex(); /// ����

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
        forAssignSentence_lex(); /// for��ֵ���

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

        genMidCode(op,src1,src2,res); /// ����

        genMidCode(JMP_OP,SPACE_OP,SPACE_OP,label1); /// continue loop

        genMidCode(LAB_OP,SPACE_OP,SPACE_OP,label2); /// label2

    }
    cout<<"*out loopSentence_lex"<<endl;

}

/**
    �������䣾  ::=  switch ��(�������ʽ����)�� ��{�����������ȱʡ�� ��}��
**/
string switchTmp;
void situationSentence_lex()//�������䣾
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
    situationTable_lex(); /// ��<�����>

    if(symID == DEFAULT_SYMID)
    {
        default_lex(); /// ��<ȱʡ>
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
    �������   ::=  ���������䣾{���������䣾}
**/
void situationTable_lex()//�������
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
    ���������䣾  ::=  case����ö�ٳ�����������䣾
**/
void situationSubSentence_lex()//���������䣾
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
    sentence_lex(); /// ��<���>
    genMidCode(LAB_OP,SPACE_OP,SPACE_OP,label);

    cout<<"*out situationSubSentence_lex"<<endl;
}

/**
    ��ȱʡ��   ::=  default : ����䣾
                  |���գ�
**/
void default_lex()//��ȱʡ��
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
    sentence_lex(); /// ��<���>

    cout<<"*out default_lex"<<endl;
}

/**
    ���з���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
**/
void funWithReturnCall_lex()//���з���ֵ����������䣾
{
    cout<<"*in funWithReturnCall_lex"<<endl;

    string op,src1,src2,res; /// ��Ԫʽ
    if(symID != IDENTIFIER_SYMID)
    {
        errorContent(IDENTIFIER_ERR, errorLine, errorColumn);
        return ;
    }
    op = CALL_OP;
    src1 = symbol;
    src2 = SPACE_OP;
    res = genVar(); /// ������ʱ�����洢��������ֵ

    get_symbol();
    if(symID != LEFTSMALLBRACKET_SYMID) // (
    {
        errorContent(LEFTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    //valParaNum = 0;

    get_symbol();
    valueParameterTable_lex(); /// ��<ֵ������>

    if(symID != RIGHTSMALLBRACKET_SYMID) // )
    {
        errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();

    genMidCode(op,src1,src2,res); /// ������Ԫʽ
    glbtmp = res; /// ���������ȫ����ʱ����

    //genMidCode(op,src1,src2,res);
    cout<<"*out funWithReturnCall_lex"<<endl;
}

/**
    ���޷���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
**/
void funWithoutReturnCall_lex()//���޷���ֵ����������䣾
{
    cout<<"*in funWithoutReturnCall_lex"<<endl;

    string op,src1,src2,res; /// ��Ԫʽ
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
    valueParameterTable_lex(); /// ��<ֵ������>

    if(symID != RIGHTSMALLBRACKET_SYMID) // )
    {
        errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
        return ;
    }

    get_symbol();

    genMidCode(op,src1,src2,res); /// ������Ԫʽ

    cout<<"*out funWithoutReturnCall_lex"<<endl;
}

/**
    ��ֵ������   ::= �����ʽ��{,�����ʽ��}
                    �����գ�
**/

stack<string> paraStack;
void valueParameterTable_lex()//��ֵ������
{
    cout<<"*in valueParameterTable_lex"<<endl;

    string op ,src1, src2, res;
    /// TODO : how to deal with ���գ� ????
    //expression_lex();
    //valParaNum ++;

    op = CALPARA_OP;
    src1 = SPACE_OP;
    src2 = SPACE_OP;
    //get_symbol();
    expression_lex(); /// ��<���ʽ>
    res = glbtmp; /// ���������ȫ����ʱ����
    //genMidCode(op,src1,src2,res); /// ������Ԫʽ

    paraStack.push(res);


    while(symID == COMMA_SYMID) // ,
    {
        //valParaNum ++;

        get_symbol();
        expression_lex(); /// ��<���ʽ>
        res = glbtmp; /// ���������ȫ����ʱ����
        //genMidCode(op,src1,src2,res); /// ������Ԫʽ

        paraStack.push(res);

    }

    while(!paraStack.empty()) /// ������������ѹջ
    {
        res = paraStack.top();
        paraStack.pop();
        genMidCode(op,src1,src2,res); /// ������Ԫʽ
    }
    cout<<"*out valueParameterTable_lex"<<endl;
}

/**
    ������䣾    ::=  scanf ��(������ʶ����{,����ʶ����}��)��
**/
void scanf_lex()//������䣾
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

    /// �������Ƿ����
    /// �������û������򱨴�
    if(constName.find(symbol) == constName.end() && locVarName.find(symbol) == locVarName.end() && glbVarName.find(symbol) == glbVarName.end())
    {
        errorContent(VARUNDEF_ERR, errorLine, errorColumn);
        return ;
    }
    /// �����������ֵ�򱨴�
    if(constName.find(symbol) != constName.end())
    {
        errorContent(CONSTREASSIGN_ERR, errorLine, errorColumn);
        return ;
    }
    src1 = SPACE_OP;
    src2 = SPACE_OP;
    res = symbol;
    identifier_lex();

    genMidCode(op,src1,src2,res); /// ������Ԫʽ

    /// ����� , ��˵�������ж������
    while(symID == COMMA_SYMID) //��
    {
        get_symbol();
        res = symbol;
        identifier_lex();

        genMidCode(op,src1,src2,res); /// ������Ԫʽ

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
    ��д��䣾    ::= printf ��(�� ���ַ�����,�����ʽ�� ��)��
                  | printf ��(�����ַ����� ��)��
                  | printf ��(�������ʽ����)��
**/
void printf_lex()//��д��䣾
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
    genMidCode(op,src1,src2,res); /// ������Ԫʽ
    cout<<"*out printf_lex"<<endl;
}

/**
    ��������䣾   ::=  return[��(�������ʽ����)��]

    ��������䣾   ::=  return[�����ʽ��]
**/
void return_lex()//��������䣾
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
        res = glbtmp; /// ��ֵ����ȫ����ʱ����

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
        res = glbtmp; /// ��ֵ����ȫ����ʱ����

        if(symID != RIGHTSMALLBRACKET_SYMID) // )
        {
            //error
            errorContent(RIGHTSMALLBRACKET_ERR, errorLine, errorColumn);
            return ;
        }
        get_symbol();
    }
    /// return
    else if(symID != SEMICOLON_SYMID) // ��
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
    genMidCode(op,src1,src2,res); /// ������Ԫʽ
    cout<<"*out return_lex"<<endl;
}

/**
    ���ӷ��������  ::= +��-
**/
void add_lex()//���ӷ��������
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
    ���˷��������  ::= *��/
**/
void multiply_lex()//���˷��������
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
    ����ϵ�������  ::=  <��<=��>��>=��!=��==
**/
void relation_lex()//����ϵ�������
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
    ����ĸ��   ::= �ߣ�a����������z��A����������Z
**/
void letter_lex()//����ĸ��
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
    �����֣�   ::= �������������֣�
**/
void digit_lex()//�����֣�
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
    ���������֣�  ::= ��������������
**/
void notZeroDigit_lex()// ���������֣�
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
    ���ַ���    ::=  '���ӷ��������'��'���˷��������'��'����ĸ��'��'�����֣�'
**/
void character_lex()//���ַ���
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
    ���ַ�����   ::=  "��ʮ���Ʊ���Ϊ32,33,35-126��ASCII�ַ���"
**/
void string_lex()//���ַ�����
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
    ���޷���������  ::= ���������֣��������֣���
    ###���޷���������  ::= DIGIT_SYMID
**/
void unsignedInt_lex()//���޷���������
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
    ��������        ::= �ۣ������ݣ��޷�������������
**/
void int_lex()//��������
{
    cout<<"*in int_lex"<<endl;

    if(symID == ADD_SYMID || symID == SUBTRACT_SYMID)
    {
        get_symbol();
        unsignedInt_lex();
    }
    else if(symID == NUMBER_SYMID)
    {
        unsignedInt_lex(); /// ��<�޷�������>
    }
    else
    {
        errorContent(INT_ERR, errorLine, errorColumn);
        return ;
    }

    cout<<"*out int_lex"<<endl;
}
/**
    ��С�����֣�    ::= �����֣��������֣��������գ�
**/
void fractionalPart_lex()//��С�����֣�
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
    ��ʵ����        ::= �ۣ������ݣ�������[.��С�����֣�]
**/
void realNumber_lex()//��ʵ����
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
    ����ʶ����    ::=  ����ĸ��������ĸ���������֣���
**/
void identifier_lex()//����ʶ����
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
    ����ö�ٳ�����   ::=  ��������|���ַ���
**/
void enumerableConst_lex()///TODO ����ö�ٳ�����??//����ö�ٳ�����
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
    �����ͱ�ʶ����      ::=  int | float | char
**/
void typeIdentifier_lex()//�����ͱ�ʶ����
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
