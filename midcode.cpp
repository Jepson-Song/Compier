#include "global.h"

//#define cout midcode
using namespace std;
ofstream midcoude;
int varNum,labelNum;
int quaternaryNum = 0;
quaternary quaternaryList[maxn];
map<string,string> stringTabel;
int stringNum;
int varWidth = 8;
void genMidCode(string op, string src1, string src2, string res)//生成四元式
{
    midcode<<"< ";
    midcode<<setw(varWidth)<<left<<op<<", ";
    midcode<<setw(varWidth)<<left<<src1<<", ";
    midcode<<setw(varWidth)<<left<<src2<<", ";
    midcode<<setw(varWidth)<<left<<res;
    midcode<<" >"<<endl;

    debug<<"< ";
    debug<<setw(varWidth)<<left<<op<<", ";
    debug<<setw(varWidth)<<left<<src1<<", ";
    debug<<setw(varWidth)<<left<<src2<<", ";
    debug<<setw(varWidth)<<left<<res;
    debug<<" >"<<endl;

    ++quaternaryNum;
    quaternaryList[quaternaryNum].op = op;
    quaternaryList[quaternaryNum].src1 = src1;
    quaternaryList[quaternaryNum].src2 = src2;
    quaternaryList[quaternaryNum].res = res;

    if(src1 == SPACE_OP || src2 == SPACE_OP || res == SPACE_OP)
    {
        quaternaryList[quaternaryNum].isLabelOrFunc = true;
    }

    //asm_run();

}

string genVar()//生成临时变量
{
    string var = "_Var_"+to_string(varNum);
    varNum ++;

    string op,src1,src2,res;
    op = INT_OP;
    src1 = SPACE_OP;
    src2 = SPACE_OP;
    res = var;

    midcode<<"< ";
    midcode<<setw(varWidth)<<left<<op<<", ";
    midcode<<setw(varWidth)<<left<<src1<<", ";
    midcode<<setw(varWidth)<<left<<src2<<", ";
    midcode<<setw(varWidth)<<left<<res;
    midcode<<" >"<<endl;

    debug<<"< ";
    debug<<setw(varWidth)<<left<<op<<", ";
    debug<<setw(varWidth)<<left<<src1<<", ";
    debug<<setw(varWidth)<<left<<src2<<", ";
    debug<<setw(varWidth)<<left<<res;
    debug<<" >"<<endl;

    ++quaternaryNum;
    quaternaryList[quaternaryNum].op = op;
    quaternaryList[quaternaryNum].src1 = src1;
    quaternaryList[quaternaryNum].src2 = src2;
    quaternaryList[quaternaryNum].res = res;

    //asm_run();

    return var;
}

string genLabel()//生成label
{
    string label = "_Label_"+to_string(labelNum);
    labelNum ++;
    return label;
}

string genString()//生成全局字符串
{
    string strVar = "_String_"+to_string(stringNum);
    stringNum ++;
    return strVar;
}
