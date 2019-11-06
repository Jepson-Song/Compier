#include "global.h"

#include <map>
//#define asmcode debug

/*
int funcNum = 0;
string funcName;
map<string,int> funcNumTabel;
int nowFuncNum;
*/
const int intSize = 4;
const int charSize = 4;
const int floatSize = 4;
const string intSym = "int";
const string charSym = "char";
const string floatSym = "float";

ASMSENTENCE asmSent[maxn];
int asmSentNum = 0;
bool isText = false;

map<string, varNode> locVar;
map<string, varNode> glbVar;
//map<string, int> funcPostion;
string src1,src2,res;
int offset = 0;
string nowFunc;

int paraNum = 0;
int preParaNum ;
int valParaNum = 0;

//map<string, int> glbVar;
//map<string, int> locVar;
map<string, varNode> preLocVar;

//void getFucNum()
//{
//    nowFuncNum = funcNumTabel[funcName];
//}
bool newFunc;

void asm_init()
{
    offset = 0;
    paraNum = 0;
    preParaNum = 0;
    valParaNum = 0;
    nowFunc = "";
    locVar.clear();
    glbVar.clear();
    preLocVar.clear();
    newFunc = false;
}

void saveLocVar()
{
    preLocVar.clear();

    map<string, varNode>::iterator iter;
    iter = locVar.begin();
    while(iter != locVar.end())
    {
        preLocVar[iter->first] = iter->second;
        iter ++;
    }

    locVar.clear();
}

void recoverLocVar()
{
    locVar.clear();

    map<string, varNode>::iterator iter;
    iter = preLocVar.begin();
    while(iter != preLocVar.end())
    {
        locVar[iter->first] = iter->second;
        iter ++;
    }

    preLocVar.clear();
}

void outLocVar()
{
    map<string, varNode>::iterator iter;
    iter = locVar.begin();
    while(iter != locVar.end())
    {
        asmcode<<iter->first<<":"<<iter->second.offset<<endl;
        iter ++;
    }
}

void asm_out(string op_mips, string var1, string var2, string var3)
{
    //outLocVar();
    if(state == asm_sta)
    {
        if(op_mips[op_mips.length()-1]==':') /// label or func
        {
            asmcode<<"\r";
        }
        else if(isText) /// text
        {
            asmcode<<"\t\t";
        }
        else /// data
        {
            asmcode<<"\t";
        }

        asmcode<<op_mips<<"\t"<<var1<<"\t"<<var2<<"\t"<<var3<<endl;
    }
    else if(state == opt_sta)
    {
        if(op_mips[op_mips.length()-1]==':') /// label or func
        {
            optimize<<"\r";
        }
        else if(isText) /// text
        {
            optimize<<"\t\t";
        }
        else /// data
        {
            optimize<<"\t";
        }

        optimize<<op_mips<<"\t"<<var1<<"\t"<<var2<<"\t"<<var3<<endl;


///****输出局部变量****/
//        map<string, varNode>::iterator iter;
//        iter = locVar.begin();
//        while(iter != locVar.end())
//        {
//            optimize<<"#"<<iter->first<<":"<<iter->second.offset<<endl;
//            iter ++;
//        }
    }



//    if(isText)
//    {
        asmSent[asmSentNum].op = op_mips;
        asmSent[asmSentNum].var1 = var1;
        asmSent[asmSentNum].var2 = var2;
        asmSent[asmSentNum].var3 = var3;
        asmSent[asmSentNum].isText = isText;
        asmSentNum ++;
//    }
//    else
//    {
//        asmSent[asmSentNum].line = "\t\t"+op_mips+"\t"+var1+"\t"+var2+"\t"+var3;
//        asmSent[asmSentNum].isText = isText;
//        asmSentNum ++;
//    }

    //debug<<"\t\t"<<op_mips<<"\t"<<var1<<"\t"<<var2<<"\t"<<var3<<endl;
}

bool isNumber(string str)
{
    if((str[0] >= '0' && str[0] <= '9') || str[0] == '-')
    {
        return true;
    }
    return false;
}

bool isString(string str)
{
    if(str[0] == '\"') return true;
    return false;
}

bool isReg(string str)
{
    if(str[0] == '$')
    {
        return true;
    }
    return false;
}

void int_mips()
{
    //asmcode<<"in int_mips  func="<<nowFunc<<endl;

//    asm_out(SUBI_MIPS,SP_REG,SP_REG,"4");
//    offset -= 4;
    //locVar[res] = offset;
    //asmcode<<"----"<<res<<"  "<<offset<<endl;
    //debug<<"----"<<res<<"\t"<<offset<<endl;

    if(nowFunc=="")
    {
        //offset -= 4;
        //glbVar[res] = offset;
        glbVar[res].type = intSym;
        //asm_out(res,".space","4");
        //asmcode<<res<<"\t"<<".space"<<"\t"<<"4"<<endl;
        //asmcode<<"global"<<endl;
    }
    else
    {
        offset -= intSize;
        locVar[res].offset = offset;
        locVar[res].type = intSym;
        asm_out(SUBI_MIPS,SP_REG,SP_REG,to_string(intSize));
        //asmcode<<"local"<<endl;
    }
    //asmcode<<"out int_mips"<<endl;
}

void char_mips()
{
    //asmcode<<endl;
    /*
    if(nowFunc=="")
    {
        //offset -= 4;
        //glbVar[res] = offset;
        glbVar[res].type = "char";
        //asmcode<<res<<"\t"<<".space"<<"\t"<<"4"<<endl;
        asm_out(res,".space","1");
    }
    else
    {
        offset -= 1;
        locVar[res].offset = offset;
        locVar[res].type = "char";
        asm_out(SUBI_MIPS,SP_REG,SP_REG,"1");
    }
    */
    if(nowFunc=="")
    {
        //offset -= 4;
        //glbVar[res] = offset;
        glbVar[res].type = charSym;
        //asmcode<<res<<"\t"<<".space"<<"\t"<<"4"<<endl;
        //asm_out(res,".space","4");
    }
    else
    {
        offset -= charSize;
        locVar[res].offset = offset;
        locVar[res].type = charSym;
        asm_out(SUBI_MIPS,SP_REG,SP_REG,to_string(charSize));
    }
    //asmcode<<endl;
}

/**

int a[10];				< int, 10, , a >
**/
void intArry_mips()
{
    if(nowFunc == "")
    {
        glbVar[res].type = intSym;
        //asm_out(res,".array",to_string(atoi(src1.c_str())*4));
    }
    else
    {
        locVar[res].offset = offset - intSize;
        offset -= atoi(src1.c_str())*intSize;
        locVar[res].type = intSym;
        asm_out(SUBI_MIPS,SP_REG,SP_REG,to_string(atoi(src1.c_str())*intSize));
    }
}

void charArry_mips()
{
    /*
    if(nowFunc == "")
    {
        glbVar[res].type = "char";
        asm_out(res,".array",src1);
    }
    else
    {
        locVar[res].offset = offset - 1;
        offset -= atoi(src1.c_str());
        locVar[res].type = "int";
        asm_out(SUBI_MIPS,SP_REG,SP_REG,src1);
    }
    */
    if(nowFunc == "")
    {
        glbVar[res].type = charSym;
        //asm_out(res,".array",to_string(atoi(src1.c_str())*4));
    }
    else
    {
        locVar[res].offset = offset - charSize;
        offset -= atoi(src1.c_str())*charSize;
        locVar[res].type = charSym;
        asm_out(SUBI_MIPS,SP_REG,SP_REG,to_string(atoi(src1.c_str())*charSize));
    }
}

/// 计算地址偏移
string shamt(int a, string b)
{
    return to_string(a)+"("+b+")";
}

/**
fun(int a,)				< para, int, , a >
fun(char a,)			< para, char, , a >
**/
void para_mips()
{
    //asmcode<<"in para_mips"<<endl;

    if(!newFunc)
    {
        newFunc = true;
        saveLocVar();
    }

    paraNum ++;

    locVar[res].offset = (paraNum+1)*4;
    locVar[res].type = src1;

    //asmcode<<"out para_mips"<<endl;
}

/**
int a();				< func, int, , a >
char a();				< func, char, , a >
void a();				< func, void, , a >
**/
void func_mips()
{
    //asmcode<<"in func_mips"<<endl;
    //asmcode<<res<<":"<<endl;
    asm_out(res+":",SPACE_MIPS,SPACE_MIPS,SPACE_MIPS);
    //asm_out(SUBI_MIPS,SP_REG,SP_REG,to_string(paraNum*4)); /// para

    if(!newFunc)
    {
        newFunc = true;
        saveLocVar();
    }

    nowFunc = res;

    //asmcode<<"out func_mips func="<<nowFunc<<endl;
}

/**
return ;				< ret, , , >
return a;				< ret, , , a >
**/
void return_mips()
{
    //asmcode<<endl;

    if(res != SPACE_OP)
    {
        if(isNumber(res)) /// imm
        {
            asm_out(LI_MIPS,T1_REG,res,SPACE_MIPS);
        }
        else if(locVar.find(res) != locVar.end()) /// loc
        {
            asm_out(LW_MIPS,T1_REG,shamt(locVar[res].offset,FP_REG),SPACE_MIPS);
        }
        else /// glb
        {
            asm_out(LA_MIPS,T0_REG,res,SPACE_MIPS);
            asm_out(LW_MIPS,T1_REG,shamt(0,T0_REG),SPACE_MIPS);
        }

        asm_out(MOVE_MIPS,V0_REG,T1_REG,SPACE_MIPS);

    }

    //asm_out(JR_MIPS,RA_REG,SPACE_MIPS,SPACE_MIPS);

    if(res != "main")
    {
        asm_out(JR_MIPS,RA_REG,SPACE_MIPS,SPACE_MIPS);
    }

    //asmcode<<endl;
}

/**
    <exit, , a>
**/
void exits_mips()
{
    //locVar.clear();
    recoverLocVar();

    newFunc = false;

    paraNum = 0;

}

/**
a + b					< +, a, b, >
**/
void add_mips()
{
    //asmcode<<"add_mips()"<<endl;
    //asmcode<<endl;

    if(isNumber(src1)) /// imm
    {
        asm_out(LI_MIPS,T1_REG,src1,SPACE_MIPS);
    }
    else if(isReg(src1)) /// reg
    {
        asm_out(MOVE_MIPS,T1_REG,src1,SPACE_MIPS);
    }
    else /// var
    {
        if(locVar.find(src1) != locVar.end()) /// local
        {
            asm_out(LW_MIPS,T1_REG,shamt(locVar[src1].offset,FP_REG),SPACE_MIPS);
        }
        else /// global
        {
            asm_out(LA_MIPS,T0_REG,src1,SPACE_MIPS);
            asm_out(LW_MIPS,T1_REG,shamt(0,T0_REG),SPACE_MIPS);
        }
    }

    if(isNumber(src2))
    {
        asm_out(LI_MIPS,T2_REG,src2,SPACE_MIPS);
    }
    else if(isReg(src2)) /// reg
    {
        asm_out(MOVE_MIPS,T2_REG,src2,SPACE_MIPS);
    }
    else
    {
        if(locVar.find(src2) != locVar.end())
        {
            asm_out(LW_MIPS,T2_REG,shamt(locVar[src2].offset,FP_REG),SPACE_MIPS);
        }
        else
        {
            asm_out(LA_MIPS,T0_REG,src2,SPACE_MIPS);
            asm_out(LW_MIPS,T2_REG,shamt(0,T0_REG),SPACE_MIPS);
        }
    }

    asm_out(ADD_MIPS,T1_REG,T1_REG,T2_REG);


    if(isReg(res)) /// reh
    {
        asm_out(MOVE_MIPS,res,T2_REG,SPACE_MIPS);
    }
    else if(locVar.find(res) != locVar.end())
    {
        asm_out(SW_MIPS,T1_REG,shamt(locVar[res].offset,FP_REG),SPACE_MIPS);
    }
    else
    {
        asm_out(LA_MIPS,T0_REG,res,SPACE_MIPS);
        asm_out(SW_MIPS,T1_REG,shamt(0,T0_REG),SPACE_MIPS);
    }
    //asmcode<<endl;
}

/**
a - b					< -, a, b, >
**/
void sub_mips()
{
    //cout<<"sub_mips"<<endl;
    //asmcode<<endl;

    if(isNumber(src1)) /// imm
    {
        asm_out(LI_MIPS,T1_REG,src1,SPACE_MIPS);
    }
    else if(isReg(src1)) /// reg
    {
        asm_out(MOVE_MIPS,T1_REG,src1,SPACE_MIPS);
    }
    else /// var
    {
        if(locVar.find(src1) != locVar.end()) /// local
        {
            asm_out(LW_MIPS,T1_REG,shamt(locVar[src1].offset,FP_REG),SPACE_MIPS);
        }
        else /// global
        {
            asm_out(LA_MIPS,T0_REG,src1,SPACE_MIPS);
            asm_out(LW_MIPS,T1_REG,shamt(0,T0_REG),SPACE_MIPS);
        }
    }

    if(isNumber(src2))
    {
        asm_out(LI_MIPS,T2_REG,src2,SPACE_MIPS);
    }
    else if(isReg(src1)) /// reg
    {
        asm_out(MOVE_MIPS,T1_REG,src1,SPACE_MIPS);
    }
    else if(isReg(src2)) /// reg
    {
        asm_out(MOVE_MIPS,T2_REG,src2,SPACE_MIPS);
    }
    else
    {
        if(locVar.find(src2) != locVar.end()) /// loc
        {
            asm_out(LW_MIPS,T2_REG,shamt(locVar[src2].offset,FP_REG),SPACE_MIPS);
        }
        else /// glb
        {
            asm_out(LA_MIPS,T0_REG,src2,SPACE_MIPS);
            asm_out(LW_MIPS,T2_REG,shamt(0,T0_REG),SPACE_MIPS);
        }
    }

    asm_out(SUB_MIPS,T1_REG,T1_REG,T2_REG);

    if(isReg(res)) /// reh
    {
        asm_out(MOVE_MIPS,res,T2_REG,SPACE_MIPS);
    }
    else if(locVar.find(res) != locVar.end())
    {
        asm_out(SW_MIPS,T1_REG,shamt(locVar[res].offset,FP_REG),SPACE_MIPS);
    }
    else
    {
        asm_out(LA_MIPS,T0_REG,res,SPACE_MIPS);
        asm_out(SW_MIPS,T1_REG,shamt(0,T0_REG),SPACE_MIPS);
    }

    //asmcode<<endl;
}

/**
a * b					< *, a, b, >
**/
void mul_mips()
{
    //cout<<"mul_mips()"<<endl;
    //asmcode<<endl;

    if(isNumber(src1)) /// imm
    {
        asm_out(LI_MIPS,T1_REG,src1,SPACE_MIPS);
    }
    else if(isReg(src1)) /// reg
    {
        asm_out(MOVE_MIPS,T1_REG,src1,SPACE_MIPS);
    }
    else /// var
    {
        if(locVar.find(src1) != locVar.end()) /// local
        {
            asm_out(LW_MIPS,T2_REG,shamt(locVar[src1].offset,FP_REG),SPACE_MIPS);
        }
        else /// global
        {
            asm_out(LA_MIPS,T0_REG,src1,SPACE_MIPS);
            asm_out(LW_MIPS,T1_REG,shamt(0,T0_REG),SPACE_MIPS);
        }
    }

    if(isNumber(src2))
    {
        asm_out(LI_MIPS,T2_REG,src2,SPACE_MIPS);
    }
    else if(isReg(src2)) /// reg
    {
        asm_out(MOVE_MIPS,T2_REG,src2,SPACE_MIPS);
    }
    else
    {
        if(locVar.find(src2) != locVar.end())
        {
            asm_out(LW_MIPS,T1_REG,shamt(locVar[src2].offset,FP_REG),SPACE_MIPS);
        }
        else
        {
            asm_out(LA_MIPS,T0_REG,src2,SPACE_MIPS);
            asm_out(LW_MIPS,T2_REG,shamt(0,T0_REG),SPACE_MIPS);
        }
    }

    asm_out(MUL_MIPS,T1_REG,T1_REG,T2_REG);

    if(isReg(res)) /// reh
    {
        asm_out(MOVE_MIPS,res,T2_REG,SPACE_MIPS);
    }
    else if(locVar.find(res) != locVar.end())
    {
        asm_out(SW_MIPS,T1_REG,shamt(locVar[res].offset,FP_REG),SPACE_MIPS);
    }
    else
    {
        asm_out(LA_MIPS,T0_REG,res,SPACE_MIPS);
        asm_out(SW_MIPS,T1_REG,shamt(0,T0_REG),SPACE_MIPS);
    }

    //asmcode<<endl;
}

/**
a / b					< /, a, b, >
**/
void div_mips()
{
    //cout<<"div_mips()"<<endl;
    //asmcode<<endl;

    if(isNumber(src1)) /// imm
    {
        asm_out(LI_MIPS,T1_REG,src1,SPACE_MIPS);
    }
    else if(isReg(src1)) /// reg
    {
        asm_out(MOVE_MIPS,T1_REG,src1,SPACE_MIPS);
    }
    else /// var
    {
        if(locVar.find(src1) != locVar.end()) /// local
        {
            asm_out(LW_MIPS,T1_REG,shamt(locVar[src1].offset,FP_REG),SPACE_MIPS);
        }
        else /// global
        {
            asm_out(LA_MIPS,T0_REG,src1,SPACE_MIPS);
            asm_out(LW_MIPS,T1_REG,shamt(0,T0_REG),SPACE_MIPS);
        }
    }

    if(isNumber(src2))
    {
        asm_out(LI_MIPS,T2_REG,src2,SPACE_MIPS);
    }
    else if(isReg(src2)) /// reg
    {
        asm_out(MOVE_MIPS,T2_REG,src2,SPACE_MIPS);
    }
    else
    {
        if(locVar.find(src2) != locVar.end())
        {
            asm_out(LW_MIPS,T2_REG,shamt(locVar[src2].offset,FP_REG),SPACE_MIPS);
        }
        else
        {
            asm_out(LA_MIPS,T0_REG,src2,SPACE_MIPS);
            asm_out(LW_MIPS,T2_REG,shamt(0,T0_REG),SPACE_MIPS);
        }
    }

    asm_out(DIV_MIPS,T1_REG,T1_REG,T2_REG);

    if(isReg(res)) /// reh
    {
        asm_out(MOVE_MIPS,res,T2_REG,SPACE_MIPS);
    }
    else if(locVar.find(res) != locVar.end()) /// loc
    {
        asm_out(SW_MIPS,T1_REG,shamt(locVar[res].offset,FP_REG),SPACE_MIPS);
    }
    else
    {
        asm_out(LA_MIPS,T0_REG,res,SPACE_MIPS);
        asm_out(SW_MIPS,T1_REG,shamt(0,T0_REG),SPACE_MIPS);
    }

    //asmcode<<endl;
}

/**
a = b					< =, b, , a >
**/
void ass_mips()
{
    //asmcode<<endl;
//    if(src1[0]>='0' && src1[0]<='9')
//    {
//
//        asm_out(SW_MIPS,shamt(locVar[res],FP_REG),src1,SPACE_OP);
//    }
//    else
//    {
        if(isNumber(src1)) /// imm
        {
            asm_out(LI_MIPS,T1_REG,src1,SPACE_MIPS);
        }
        else if(isReg(src1)) /// reg
        {
            asm_out(MOVE_MIPS,T1_REG,src1,SPACE_MIPS);
        }
        else if(locVar.find(src1) != locVar.end()) /// loc
        {
            asm_out(LW_MIPS,T1_REG,shamt(locVar[src1].offset,FP_REG),SPACE_MIPS);
        }
        else /// glb
        {
            asm_out(LA_MIPS,T0_REG,src1,SPACE_MIPS);
            asm_out(LW_MIPS,T1_REG,shamt(0,T0_REG),SPACE_MIPS);
        }

        if(locVar.find(res) != locVar.end()) /// loc
        {
            asm_out(SW_MIPS,T1_REG,shamt(locVar[res].offset,FP_REG),SPACE_MIPS);
        }
        else /// glb
        {
            asm_out(LA_MIPS,T0_REG,res,SPACE_MIPS);
            asm_out(SW_MIPS,T1_REG,shamt(0,T0_REG),SPACE_MIPS);
        }

        //asm_out(MOVE_MIPS,T2_REG,T1_REG,SPACE_MIPS);

//    }
    //asmcode<<endl;
}

/**
a[i] = b;				< []=, b, i, a >
**/
void assArry_mips()
{
    /// src1
        if(isNumber(src1)) /// imm
        {
            asm_out(LI_MIPS,T1_REG,src1,SPACE_MIPS);
        }
        else if(isReg(src1)) /// reg
        {
            asm_out(MOVE_MIPS,T1_REG,src1,SPACE_MIPS);
        }
        else if(locVar.find(src1) != locVar.end()) /// loc
        {
            asm_out(LW_MIPS,T1_REG,shamt(locVar[src1].offset,FP_REG),SPACE_MIPS);
        }
        else /// glb
        {
            asm_out(LA_MIPS,T0_REG,src1,SPACE_MIPS);
            asm_out(LW_MIPS,T1_REG,shamt(0,T0_REG),SPACE_MIPS);
        }

    /// src2
        if(isNumber(src2)) /// imm
        {
            asm_out(LI_MIPS,T2_REG,src2,SPACE_MIPS);
        }
        else if(isReg(src2)) /// reg
        {
            asm_out(MOVE_MIPS,T2_REG,src2,SPACE_MIPS);
        }
        else if(locVar.find(src2) != locVar.end()) /// loc
        {
            asm_out(LW_MIPS,T2_REG,shamt(locVar[src2].offset,FP_REG),SPACE_MIPS);
        }
        else /// glb
        {
            asm_out(LA_MIPS,T0_REG,src2,SPACE_MIPS);
            asm_out(LW_MIPS,T2_REG,shamt(0,T0_REG),SPACE_MIPS);
        }

    /// res
        if(isReg(res)) /// reg
        {
            cout<<reg2var[res]<<endl;
            if(locVar[reg2var[res]].type == "int")
            {
                asm_out(MUL_MIPS,T2_REG,T2_REG,"4");
            }
            else
            {
                /// nop
            }

            asm_out(ADDI_MIPS,T0_REG,FP_REG,to_string(locVar[reg2var[res]].offset));

            asm_out(SUB_MIPS,T0_REG,T0_REG,T2_REG);

            asm_out(SW_MIPS,T1_REG,shamt(0,T0_REG),SPACE_MIPS);
        }
        else if(locVar.find(res) != locVar.end()) /// loc
        {

            if(locVar[res].type == "int")
            {
                asm_out(MUL_MIPS,T2_REG,T2_REG,"4");
            }
            else
            {
                /// nop
            }

            asm_out(ADDI_MIPS,T0_REG,FP_REG,to_string(locVar[res].offset));

            asm_out(SUB_MIPS,T0_REG,T0_REG,T2_REG);

            asm_out(SW_MIPS,T1_REG,shamt(0,T0_REG),SPACE_MIPS);
        }
        else /// glb
        {

            if(locVar[res].type == "int")
            {
                asm_out(MUL_MIPS,T2_REG,T2_REG,"4");
            }
            else
            {
                /// nop
            }

            asm_out(LA_MIPS,T0_REG,res,SPACE_MIPS);

            asm_out(SUB_MIPS,T0_REG,T0_REG,T2_REG);

            asm_out(SW_MIPS,T1_REG,shamt(0,T0_REG),SPACE_MIPS);
        }
}

/**
(int|char) fun();		< call, , , fun >
(void) fun();			< call, , , >
**/
void call_mips()
{
    //asmcode<<"in call_mips"<<endl;

    asm_out(SUBI_MIPS,SP_REG,SP_REG,"4"); /// $sp = $sp - 4;
    asm_out(SW_MIPS,RA_REG,shamt(0,SP_REG),SPACE_MIPS); /// ($sp) = $ra
    asm_out(SUBI_MIPS,SP_REG,SP_REG,"4"); /// $sp = $sp - 4;
    asm_out(SW_MIPS,FP_REG,shamt(0,SP_REG),SPACE_MIPS); /// ($sp) = $fp
    asm_out(MOVE_MIPS,FP_REG,SP_REG,SPACE_MIPS); /// $fp = $sp

//    asm_out(SW_MIPS,FP_REG,shamt(0,SP_REG),SPACE_MIPS); /// ($sp) = $fp
//    asm_out(MOVE_MIPS,FP_REG,SP_REG,SPACE_MIPS); /// $fp = $sp
//    asm_out(SUBI_MIPS,SP_REG,SP_REG,"4"); /// $sp = $sp - 4;
//    asm_out(SW_MIPS,RA_REG,shamt(0,SP_REG),SPACE_MIPS); /// ($sp) = $ra

    asm_out(JAL_MIPS,src1,SPACE_MIPS,SPACE_MIPS);
    asm_out(NOP_MIPS,SPACE_MIPS,SPACE_MIPS,SPACE_MIPS);


    asm_out(MOVE_MIPS,SP_REG,FP_REG,SPACE_MIPS);  /// recover sp
    asm_out(LW_MIPS,RA_REG,shamt(4,FP_REG),SPACE_MIPS); /// recover ra
    asm_out(LW_MIPS,FP_REG,shamt(0,FP_REG),SPACE_MIPS); /// recover fp

    /// store returnValue

    if(isReg(res)) /// reg
    {
        asm_out(MOVE_MIPS,res,V0_REG,SPACE_MIPS);
    }
    else if(locVar.find(res) != locVar.end()) /// loc
    {
        asm_out(SW_MIPS,V0_REG,shamt(locVar[res].offset,FP_REG),SPACE_MIPS);
    }
    else /// glb
    {
        asm_out(LA_MIPS,T0_REG,res,SPACE_MIPS);
        asm_out(SW_MIPS,V0_REG,shamt(0,T0_REG),SPACE_MIPS);
    }


    //asmcode<<"out call_mips"<<endl;
}

/**
fun(a);					< callpara, int|char, , a >
**/
void callpara_mips()
{
    //asmcode<<"in callpara_mips"<<endl;

    if(isNumber(res)) /// imm
    {
        asm_out(LI_MIPS,T1_REG,res,SPACE_MIPS);
    }
    else if(isReg(res)) /// reg
    {
        asm_out(LW_MIPS,T1_REG,res,SPACE_MIPS);
    }
    else if(locVar.find(res) != locVar.end()) /// loc
    {
        asm_out(LW_MIPS,T1_REG,shamt(locVar[res].offset,FP_REG),SPACE_MIPS);
    }
    else
    {
        asm_out(LA_MIPS,T0_REG,res,SPACE_MIPS);
        asm_out(LW_MIPS,T1_REG,shamt(0,T0_REG),SPACE_MIPS);
    }

    asm_out(SUBI_MIPS,SP_REG,SP_REG,"4");

    asm_out(SW_MIPS,T1_REG,shamt(0,SP_REG),SPACE_MIPS); ///
}

/**
set label				< lab, , , label >
**/
void lab_mips()
{
    //asmcode<<endl;
    //asmcode<<res<<":"<<endl;
    asm_out(res+":",SPACE_MIPS,SPACE_MIPS,SPACE_MIPS);
    //asmcode<<endl;
}

//void jne_mips()
//{
//    asm_out(BNE_MIPS,T8_REG,T9_REG,res);
//}

/**
jmp	label		 		< j, , , label>
**/
void jmp_mips()
{
    asm_out(JMP_MIPS,res,SPACE_MIPS,SPACE_MIPS);
}

/**
if(a < b)				< blt, a, b, label >
**/
void blt_mips()
{
    if(isNumber(src1)) /// imm
    {
        asm_out(LI_MIPS,T8_REG,src1,SPACE_MIPS);
    }
    else if(src1[0] == '$') /// reg
    {
        asm_out(MOVE_MIPS,T8_REG,src1,SPACE_MIPS);
    }
    else if(locVar.find(src1) != locVar.end()) /// loc
    {
        asm_out(LW_MIPS,T8_REG,shamt(locVar[src1].offset,FP_REG),SPACE_MIPS);
    }
    else /// glb
    {
        asm_out(LA_MIPS,T0_REG,src1,SPACE_MIPS);
        asm_out(LW_MIPS,T8_REG,shamt(0,T0_REG),SPACE_MIPS);
    }

    if(isNumber(src2)) /// imm
    {
        asm_out(LI_MIPS,T9_REG,src2,SPACE_MIPS);
    }
    else if(src2[0] == '$') /// reg
    {
        asm_out(MOVE_MIPS,T9_REG,src2,SPACE_MIPS);
    }
    else if(locVar.find(src2) != locVar.end()) /// loc
    {
        asm_out(LW_MIPS,T9_REG,shamt(locVar[src2].offset,FP_REG),SPACE_MIPS);
    }
    else /// glb
    {
        asm_out(LA_MIPS,T0_REG,src2,SPACE_MIPS);
        asm_out(LW_MIPS,T9_REG,shamt(0,T0_REG),SPACE_MIPS);
    }

    asm_out(BLT_MIPS,T8_REG,T9_REG,res);

}

/**
if(a <= b)				< ble, a, b, label >
**/
void ble_mips()
{
    if(isNumber(src1)) /// imm
    {
        asm_out(LI_MIPS,T8_REG,src1,SPACE_MIPS);
    }
    else if(src1[0] == '$') /// reg
    {
        asm_out(MOVE_MIPS,T8_REG,src1,SPACE_MIPS);
    }
    else if(locVar.find(src1) != locVar.end()) /// loc
    {
        asm_out(LW_MIPS,T8_REG,shamt(locVar[src1].offset,FP_REG),SPACE_MIPS);
    }
    else /// glb
    {
        asm_out(LA_MIPS,T0_REG,src1,SPACE_MIPS);
        asm_out(LW_MIPS,T8_REG,shamt(0,T0_REG),SPACE_MIPS);
    }

    if(isNumber(src2)) /// imm
    {
        asm_out(LI_MIPS,T9_REG,src2,SPACE_MIPS);
    }
    else if(src2[0] == '$') /// reg
    {
        asm_out(MOVE_MIPS,T9_REG,src2,SPACE_MIPS);
    }
    else if(locVar.find(src2) != locVar.end()) /// loc
    {
        asm_out(LW_MIPS,T9_REG,shamt(locVar[src2].offset,FP_REG),SPACE_MIPS);
    }
    else /// glb
    {
        asm_out(LA_MIPS,T0_REG,src2,SPACE_MIPS);
        asm_out(LW_MIPS,T9_REG,shamt(0,T0_REG),SPACE_MIPS);
    }

    asm_out(BLE_MIPS,T8_REG,T9_REG,res);

}

/**
if(a > b)				< bgt, a, b, label >
**/
void bgt_mips()
{
    if(isNumber(src1)) /// imm
    {
        asm_out(LI_MIPS,T8_REG,src1,SPACE_MIPS);
    }
    else if(src1[0] == '$') /// reg
    {
        asm_out(MOVE_MIPS,T8_REG,src1,SPACE_MIPS);
    }
    else if(locVar.find(src1) != locVar.end()) /// loc
    {
        asm_out(LW_MIPS,T8_REG,shamt(locVar[src1].offset,FP_REG),SPACE_MIPS);
    }
    else /// glb
    {
        asm_out(LA_MIPS,T0_REG,src1,SPACE_MIPS);
        asm_out(LW_MIPS,T8_REG,shamt(0,T0_REG),SPACE_MIPS);
    }

    if(isNumber(src2)) /// imm
    {
        asm_out(LI_MIPS,T9_REG,src2,SPACE_MIPS);
    }
    else if(src2[0] == '$') /// reg
    {
        asm_out(MOVE_MIPS,T9_REG,src2,SPACE_MIPS);
    }
    else if(locVar.find(src2) != locVar.end()) /// loc
    {
        asm_out(LW_MIPS,T9_REG,shamt(locVar[src2].offset,FP_REG),SPACE_MIPS);
    }
    else /// glb
    {
        asm_out(LA_MIPS,T0_REG,src2,SPACE_MIPS);
        asm_out(LW_MIPS,T9_REG,shamt(0,T0_REG),SPACE_MIPS);
    }

    asm_out(BGT_MIPS,T8_REG,T9_REG,res);

}

/**
if(a >= b)				< bge, a, b, label >
**/
void bge_mips()
{
    if(isNumber(src1)) /// imm
    {
        asm_out(LI_MIPS,T8_REG,src1,SPACE_MIPS);
    }
    else if(src1[0] == '$') /// reg
    {
        asm_out(MOVE_MIPS,T8_REG,src1,SPACE_MIPS);
    }
    else if(locVar.find(src1) != locVar.end()) /// loc
    {
        asm_out(LW_MIPS,T8_REG,shamt(locVar[src1].offset,FP_REG),SPACE_MIPS);
    }
    else /// glb
    {
        asm_out(LA_MIPS,T0_REG,src1,SPACE_MIPS);
        asm_out(LW_MIPS,T8_REG,shamt(0,T0_REG),SPACE_MIPS);
    }

    if(isNumber(src2)) /// imm
    {
        asm_out(LI_MIPS,T9_REG,src2,SPACE_MIPS);
    }
    else if(src2[0] == '$') /// reg
    {
        asm_out(MOVE_MIPS,T9_REG,src2,SPACE_MIPS);
    }
    else if(locVar.find(src2) != locVar.end()) /// loc
    {
        asm_out(LW_MIPS,T9_REG,shamt(locVar[src2].offset,FP_REG),SPACE_MIPS);
    }
    else /// glb
    {
        asm_out(LA_MIPS,T0_REG,src2,SPACE_MIPS);
        asm_out(LW_MIPS,T9_REG,shamt(0,T0_REG),SPACE_MIPS);
    }

    asm_out(BGE_MIPS,T8_REG,T9_REG,res);

}

/**
if(a == b)				< beq, a, b, label >
**/
void beq_mips()
{
    if(isNumber(src1)) /// imm
    {
        asm_out(LI_MIPS,T8_REG,src1,SPACE_MIPS);
    }
    else if(src1[0] == '$') /// reg
    {
        asm_out(MOVE_MIPS,T8_REG,src1,SPACE_MIPS);
    }
    else if(locVar.find(src1) != locVar.end()) /// loc
    {
        asm_out(LW_MIPS,T8_REG,shamt(locVar[src1].offset,FP_REG),SPACE_MIPS);
    }
    else /// glb
    {
        asm_out(LA_MIPS,T0_REG,src1,SPACE_MIPS);
        asm_out(LW_MIPS,T8_REG,shamt(0,T0_REG),SPACE_MIPS);
    }

    if(isNumber(src2)) /// imm
    {
        asm_out(LI_MIPS,T9_REG,src2,SPACE_MIPS);
    }
    else if(src2[0] == '$') /// reg
    {
        asm_out(MOVE_MIPS,T9_REG,src2,SPACE_MIPS);
    }
    else if(locVar.find(src2) != locVar.end()) /// loc
    {
        asm_out(LW_MIPS,T9_REG,shamt(locVar[src2].offset,FP_REG),SPACE_MIPS);
    }
    else /// glb
    {
        asm_out(LA_MIPS,T0_REG,src2,SPACE_MIPS);
        asm_out(LW_MIPS,T9_REG,shamt(0,T0_REG),SPACE_MIPS);
    }

    asm_out(BEQ_MIPS,T8_REG,T9_REG,res);

}

/**
if(a != b)				< bne, a, b, label >
**/
void bne_mips()
{
    if(isNumber(src1)) /// imm
    {
        asm_out(LI_MIPS,T8_REG,src1,SPACE_MIPS);
    }
    else if(src1[0] == '$') /// reg
    {
        asm_out(MOVE_MIPS,T8_REG,src1,SPACE_MIPS);
    }
    else if(locVar.find(src1) != locVar.end()) /// loc
    {
        asm_out(LW_MIPS,T8_REG,shamt(locVar[src1].offset,FP_REG),SPACE_MIPS);
    }
    else /// glb
    {
        asm_out(LA_MIPS,T0_REG,src1,SPACE_MIPS);
        asm_out(LW_MIPS,T8_REG,shamt(0,T0_REG),SPACE_MIPS);
    }

    if(isNumber(src2)) /// imm
    {
        asm_out(LI_MIPS,T9_REG,src2,SPACE_MIPS);
    }
    else if(src2[0] == '$') /// reg
    {
        asm_out(MOVE_MIPS,T9_REG,src2,SPACE_MIPS);
    }
    else if(locVar.find(src2) != locVar.end()) /// loc
    {
        asm_out(LW_MIPS,T9_REG,shamt(locVar[src2].offset,FP_REG),SPACE_MIPS);
    }
    else /// glb
    {
        asm_out(LA_MIPS,T0_REG,src2,SPACE_MIPS);
        asm_out(LW_MIPS,T9_REG,shamt(0,T0_REG),SPACE_MIPS);
    }

    asm_out(BNE_MIPS,T8_REG,T9_REG,res);

}

/**
scanf(a);				< scf, , , a >
**/
void scf_mips()
{
    //asmcode<<endl;
    if(locVar.find(res) != locVar.end()) /// loc
    {
        if(locVar[res].type == "int")
        {
            asm_out(LI_MIPS,V0_REG,SCFINT,SPACE_MIPS);
        }
        else
        {
            asm_out(LI_MIPS,V0_REG,SCFCHAR,SPACE_MIPS);
        }
    }
    else /// glb
    {
        if(glbVar[res].type == "int")
        {
            asm_out(LI_MIPS,V0_REG,SCFINT,SPACE_MIPS);
        }
        else
        {
            asm_out(LI_MIPS,V0_REG,SCFCHAR,SPACE_MIPS);
        }
    }

    asm_out(SYSCALL_MIPS,SPACE_MIPS,SPACE_MIPS,SPACE_MIPS); /// syscall


    if(isReg(res)) /// reg
    {
        asm_out(SW_MIPS,V0_REG,res,SPACE_MIPS);
    }
    else if(locVar.find(res) != locVar.end()) /// loc
    {
        asm_out(SW_MIPS,V0_REG,shamt(locVar[res].offset,FP_REG),SPACE_MIPS);
    }
    else /// glb
    {
        asm_out(LA_MIPS,V1_REG,res,SPACE_MIPS);
        asm_out(SW_MIPS,V0_REG,shamt(0,V1_REG),SPACE_MIPS);
    }


    /// 处理读入时的行末回车
    if(locVar.find(res) != locVar.end()) /// loc
    {
        if(locVar[res].type == "int")
        {
            /// nop
        }
        else
        {
            asm_out(LI_MIPS,V0_REG,SCFCHAR,SPACE_MIPS);
            asm_out(SYSCALL_MIPS,SPACE_MIPS,SPACE_MIPS,SPACE_MIPS);
        }
    }
    else /// glb
    {
        if(glbVar[res].type == "int")
        {
            /// nop
        }
        else
        {
            asm_out(LI_MIPS,V0_REG,SCFCHAR,SPACE_MIPS);
            asm_out(SYSCALL_MIPS,SPACE_MIPS,SPACE_MIPS,SPACE_MIPS);
        }
    }

    //asmcode<<endl;
}

/**
print("a",b);			< prf, a, b,  >
**/
void prf_mips()//< prf, a, b,  >
{
    if(src1 == SPACE_OP) /// src1==""
    {
        /// src1


    }
    else if(isString(src1)) /// src1==string
    {
        asm_out(LA_MIPS,A0_REG,stringTabel[src1],SPACE_MIPS);
        asm_out(LI_MIPS,V0_REG,PRFSTR,SPACE_MIPS);
        asm_out(SYSCALL_MIPS,SPACE_MIPS,SPACE_MIPS,SPACE_MIPS);
    }
    else /// src1==expr
    {
        if(isNumber(src1)) /// imm
        {
            asm_out(LI_MIPS,A0_REG,res,SPACE_MIPS);
            asm_out(LI_MIPS,V0_REG,PRFINT,SPACE_MIPS);
        }
        else if(isReg(src1)) /// reg
        {
            asm_out(MOVE_MIPS,A0_REG,src1,SPACE_MIPS);
        }
        else if(locVar.find(src1) != locVar.end()) /// loc
        {
            asm_out(LW_MIPS,A0_REG,shamt(locVar[src1].offset,FP_REG),SPACE_MIPS);
            if(locVar[src1].type == "int")
            {
                asm_out(LI_MIPS,V0_REG,PRFINT,SPACE_MIPS);
            }
            else
            {
                asm_out(LI_MIPS,V0_REG,PRFCHAR,SPACE_MIPS);
            }
        }
        else /// glb
        {
            asm_out(LA_MIPS,T0_REG,src1,SPACE_MIPS);
            asm_out(LW_MIPS,A0_REG,shamt(0,T0_REG),SPACE_MIPS);
            if(glbVar[src1].type == "int")
            {
                asm_out(LI_MIPS,V0_REG,PRFINT,SPACE_MIPS);
            }
            else
            {
                asm_out(LI_MIPS,V0_REG,PRFCHAR,SPACE_MIPS);
            }
        }
        asm_out(SYSCALL_MIPS,SPACE_MIPS,SPACE_MIPS,SPACE_MIPS);
    }


    /// src2
    if(src2 != SPACE_OP)
    {
        if(isNumber(src2)) /// imm
        {
            asm_out(LI_MIPS,A0_REG,res,SPACE_MIPS);
            asm_out(LI_MIPS,V0_REG,PRFINT,SPACE_MIPS);
        }
        else if(isReg(src2)) /// reg
        {
            asm_out(MOVE_MIPS,A0_REG,src2,SPACE_MIPS);
        }
        else if(locVar.find(src2) != locVar.end()) /// loc
        {
            asm_out(LW_MIPS,A0_REG,shamt(locVar[src2].offset,FP_REG),SPACE_MIPS);
            if(locVar[src2].type == "int")
            {
                asm_out(LI_MIPS,V0_REG,PRFINT,SPACE_MIPS);
            }
            else
            {
                asm_out(LI_MIPS,V0_REG,PRFCHAR,SPACE_MIPS);
            }
        }
        else /// glb
        {
            asm_out(LA_MIPS,T0_REG,src2,SPACE_MIPS);
            asm_out(LW_MIPS,A0_REG,shamt(0,T0_REG),SPACE_MIPS);
            if(glbVar[src2].type == "int")
            {
                asm_out(LI_MIPS,V0_REG,PRFINT,SPACE_MIPS);
            }
            else
            {
                asm_out(LI_MIPS,V0_REG,PRFCHAR,SPACE_MIPS);
            }
        }
        asm_out(SYSCALL_MIPS,SPACE_MIPS,SPACE_MIPS,SPACE_MIPS);
    }
}

/// 声明全局变量
void stringDecl()//声明全局变量
{
    map<string,string>::iterator iter;
    iter = stringTabel.begin();
    while(iter!=stringTabel.end())
    {
        string tmp = "";
        for(int i=0;i<(int)iter->first.length()-1;i++)
        {
            tmp += iter->first[i];
        }
        tmp += "\\0\"";
        asm_out(iter->second+":",".ascii",tmp,SPACE_MIPS);
        iter ++;
    }
}

void asm_run()//运行汇编语言生成
{
    state = asm_sta;

    asm_init();


    //asmcode<<".data"<<endl;
    asm_out(".data:",SPACE_MIPS,SPACE_MIPS,SPACE_MIPS);
//    asmSent[asmSentNum].line = ".data";
//    asmSent[asmSentNum].isText = isText;
//    asmSentNum ++;

    for(int i=1;i<=quaternaryNum;i++)
    {
        string op = quaternaryList[i].op;
        src1 = quaternaryList[i].src1;
        src2 = quaternaryList[i].src2;
        res = quaternaryList[i].res;
        if(op == FUNC_OP || op == PARA_OP) break;

        if(op == CONST_OP)
        {
            if(src1 == "int")
            {
                asm_out(res+":",".word","0",SPACE_MIPS);
            }
            else if(src1 == "char")
            {
                asm_out(res+":",".word","0",SPACE_MIPS);
            }
        }
        else if(op == INT_OP)
        {
            asm_out(res+":",".word","0",SPACE_MIPS);
        }
        else if(op == CHAR_OP)
        {
            asm_out(res+":",".word","0",SPACE_MIPS);
        }
        else if(op == INTA_OP)
        {
            asm_out(res+":",".space",to_string(atoi(src1.c_str())*intSize),SPACE_MIPS);
        }
        else if(op == CHARA_OP)
        {
            asm_out(res+":",".space",to_string(atoi(src1.c_str())*charSize),SPACE_MIPS);
        }
    }
    stringDecl();
    //asmcode<<".text"<<endl;

    asm_out(".text:",SPACE_MIPS,SPACE_MIPS,SPACE_MIPS);
//    asmSent[asmSentNum].line = ".text";
//    asmSent[asmSentNum].isText = isText;
//    asmSentNum ++;

    isText = true;
    //asm_out(JMP_MIPS,"main",SPACE_MIPS,SPACE_MIPS);
    bool flag = false;
    for(int i=1;i<=quaternaryNum;i++){
        string op = quaternaryList[i].op;
        src1 = quaternaryList[i].src1;
        src2 = quaternaryList[i].src2;
        res = quaternaryList[i].res;

        asmcode<<endl;

        if(!flag && (op == FUNC_OP || op == PARA_OP) )
        {
            //asmcode<<".text"<<endl;
            //asmcode<<".globl main"<<endl;
            asm_out(JMP_MIPS,"main",SPACE_MIPS,SPACE_MIPS);
            flag = true;
        }


        asmcode<<"#< ";
        asmcode<<setw(8)<<left<<op<<", ";
        asmcode<<setw(8)<<left<<src1<<", ";
        asmcode<<setw(8)<<left<<src2<<", ";
        asmcode<<setw(8)<<left<<res;
        asmcode<<" >"<<endl;


        if(op == INT_OP) int_mips();
        else if(op == CHAR_OP) char_mips();
        else if(op == ADD_OP) add_mips();
        else if(op == SUB_OP) sub_mips();
        else if(op == MUL_OP) mul_mips();
        else if(op == DIVS_OP) div_mips();
        else if(op == ASS_OP) ass_mips();
        else if(op == PARA_OP) para_mips();
        else if(op == FUNC_OP) func_mips();
        else if(op == CALL_OP) call_mips();
        else if(op == RET_OP) return_mips();
        else if(op == CALPARA_OP) callpara_mips();
        else if(op == LAB_OP) lab_mips();
        else if(op == JMP_OP) jmp_mips();
        else if(op == ST_OP) blt_mips();
        else if(op == SET_OP) ble_mips();
        else if(op == BT_OP) bgt_mips();
        else if(op == BET_OP) bge_mips();
        else if(op == EQL_OP) beq_mips();
        else if(op == NEQ_OP) bne_mips();
        else if(op == EXITS_OP) exits_mips();
        else if(op == SCF_OP) scf_mips();
        else if(op == PRTF_OP) prf_mips();
        else if(op == ASSARRY_OP) assArry_mips();
        else if(op == INTA_OP) intArry_mips();
        else if(op == CHARA_OP) charArry_mips();
        //outLocVar();
    }

}
