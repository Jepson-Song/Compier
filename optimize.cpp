#include "global.h"
#include<cmath>
#include<algorithm>


vector<string> regTabel = {//T0_REG,T1_REG,T2_REG,
                           T3_REG,T4_REG,T5_REG,T6_REG,T7_REG,
                           S0_REG,S1_REG,S3_REG,S3_REG,S4_REG,S5_REG,S6_REG,S7_REG,
                           T8_REG,T9_REG,K0_REG,K1_REG};
int regNum;
int blockNum;
bool isText_optimize ;
bool regAllDone;
map<string, string> reg2var;
//int varWidth = 8;
void optimize_out(string op, string var1, string var2, string var3)
{
    optimize<<"\n#< ";
    optimize<<setw(varWidth)<<left<<op<<", ";
    optimize<<setw(varWidth)<<left<<var1<<", ";
    optimize<<setw(varWidth)<<left<<var2<<", ";
    optimize<<setw(varWidth)<<left<<var3;
    optimize<<" >"<<endl;
}


vector<pair<int,int> > blockPos;

vector<string> blockVar[maxn];

typedef struct node5
{
    int start;
    int endd;
    int next1,next2;
    string label;
    string condition;
}BLOCK;
vector<BLOCK> block;
map<string,int> label2Block;


void optimize_init()
{
    blockNum = 0;
    isText_optimize = false;
    blockPos.clear();
    regNum = regTabel.size();
    regAllDone = false;
}

bool isTarget(int index)
{
    string op = quaternaryList[index].op;

    /// label或function，转移语句的目标语句
    if(op == LAB_OP || op == FUNC_OP)
    {
        return true;
    }
    return false;
}

bool isTransfer(int index)
{
    string op = quaternaryList[index].op;

    /// 条件转移语句
//    cout<<op<<endl;
    if(op == EQL_OP || op == NEQ_OP || op == BT_OP || op == BET_OP ||
       op == ST_OP || op == SET_OP || op == JMP_OP ||
       op == JMP_OP || op == CALL_OP || op == RET_OP || op == EXITS_OP)
    {
        return true;
        //cout<<"************"<<op<<endl;
    }
    return false;
}

bool isBlockEntrance(int index)
{
    /// label或function，转移语句的目标语句
    if(isTarget(index))
    {
        return true;
        //cout<<"2"<<endl;
    }

    /// 上一条是条件转移语句
    else if(isTransfer(index-1))
    {
        return true;
        //cout<<"3"<<endl;
    }

    return false;

}

bool isBlockExit(int index)
{

    /// 下一条是入口
    if(isBlockEntrance(index+1))
    {
        return true;
    }

    return false;
}

/// 划分基本快
void optimize_blcDiv()
{
    string op,var1,var2,var3;
    //int nowBlock = -1;
//    bool nextIsNewBlock = false;

    int blockStartPos = 0;
    //int lastBlockEndPos = 0;
    bool isFirst = true;
    bool isData = true;
    BLOCK tmp;
    string label = "";

    for(int i=1;i<=quaternaryNum;i++)
    {
//        cout<<"i="<<i<<endl;
        op = quaternaryList[i].op;
        var1 = quaternaryList[i].src1;
        var2 = quaternaryList[i].src2;
        var3 = quaternaryList[i].res;

//        cout<<i<<"  < ";
//        cout<<setw(varWidth)<<left<<op<<", ";
//        cout<<setw(varWidth)<<left<<var1<<", ";
//        cout<<setw(varWidth)<<left<<var2<<", ";
//        cout<<setw(varWidth)<<left<<var3;
//        cout<<" >"<<endl;

        /// 跳过data部分
        if(isData)
        {
            if(op == FUNC_OP || op == PARA_OP)
            {
                isData = false;
                blockPos.push_back(make_pair(blockStartPos,i-1));

                tmp.start = blockStartPos;
                tmp.endd = i-1;
                tmp.next1 = tmp.next2 = -1;
                tmp.label = label;
                block.push_back(tmp);

                label2Block[label] = blockNum;

                blockStartPos = i;
            }
            else
            {
                quaternaryList[i].blockNum = blockNum;
                continue;
            }
        }

        if(isFirst) /// 程序第一条语句
        {
            blockNum ++;

            quaternaryList[i].isEntrance = true;

            isFirst = false;
            blockStartPos = i;

            label = "main";

//            cout<<"1"<<endl;
        }
        else if(isBlockEntrance(i)) /// 基本块入口
        {
            blockNum ++;

            quaternaryList[i].isEntrance = true;

            blockStartPos = i;

            label = var3;

//            cout<<"2"<<endl;
        }

//        if(i==48) cout<<"****************"<<op<<endl;
        if(isBlockExit(i))
        {
            quaternaryList[i].isExit = true;

//            if(i==48) cout<<"****************"<<op<<endl;
            blockPos.push_back(make_pair(blockStartPos,i));
            tmp.start = blockStartPos;
            tmp.endd = i;
            tmp.next1 = tmp.next2 = -1;
            tmp.label = label;
            block.push_back(tmp);

            label2Block[label] = blockNum;

            blockStartPos = i+1;

//            cout<<"3"<<endl;
        }
//        cout<<"00000000000"<<endl;

        quaternaryList[i].blockNum = blockNum;

//        cout<<"------- blockNum="<<blockNum<<endl;

    }

//    optimize_outAll();
//    cout<<"optimize_blcDiv done"<<endl;
}

ofstream dataStream;
void optimize_dataStream()
{
    string op,var1,var2,var3;

    dataStream.open("dataStream.txt", ios::out);
    dataStream<<setw(7)<<left<<"BLOCK"<<setw(12)<<left<<"CONDITION"<<setw(7)<<left<<"TRUE"<<setw(7)<<left<<"FALSE"<<endl;
    for(int i=1;i<=blockNum;i++)
    {
        op = quaternaryList[block[i].endd].op;
        var1 = quaternaryList[block[i].endd].src1;
        var2= quaternaryList[block[i].endd].src2;
        var3 = quaternaryList[block[i].endd].res;

        if(op == JMP_OP)
        {
            block[i].condition = op;
            block[i].next1 = label2Block[var3];
        }
        else if(op == CALL_OP)
        {
            block[i].condition = op;
            block[i].next1 = label2Block[var1];
        }
        else if(op == EXITS_OP)
        {
            block[i].condition = op;
            block[i].next1 = 0;
        }
        else if(op == EQL_OP || op == NEQ_OP || op == BT_OP || op == BET_OP ||
                op == ST_OP || op == SET_OP)
        {
            block[i].condition = var1 + op + var2;
            block[i].next1 = label2Block[var3];
            block[i].next2 = i+1;
        }
        else
        {
            block[i].condition = "TRUE";
            block[i].next1 = i+1;
        }
        dataStream<<setw(7)<<left<<i<<setw(12)<<left<<block[i].condition<<setw(7)<<left<<block[i].next1<<setw(12)<<left<<block[i].next2<<endl;
    }
    dataStream.close();
}


typedef struct node4
{
    string name;
    int cnt;
}VAR;

bool cmp(const VAR a, const VAR b)
{
    return a.cnt>b.cnt;
}

bool isVar(string s)
{
    /// null
    if(s == "")
    {
        return false;
    }
    ///tmp var
    if(s.length()>5 && s[0]=='_' && s[1]=='V' && s[2]=='a' && s[3]=='r' && s[4]=='_')
    {
        return false;
    }
    /// int char void
    if(s == "int" || s == "char" || s == "void")
    {
        return false;
    }
    /// number
    if(s[0] >= '0' && s[0] <= '9')
    {
        return  false;
    }
    /// label
    if(s.length()>7 && s[0]=='_' && s[1]=='L' && s[2]=='a' && s[3]=='b' && s[4]=='e' && s[5]=='l' && s[6]=='_')
    {
        return false;
    }
    return true;
}

/// 寄存器分配
map<string,string> var2reg[maxn];
//vector<string> blkVar[maxn];
void optimize_regAllocation()
{
    string op,var1,var2,var3;
    map<string,int> varIndex;
    vector<VAR> varTabel;
    VAR tmp;
    int blkVarNum;


    asm_init();

    for(int i=1;i<=blockNum;i++)
    {
//        cout<<i<<":  "<<blockPos[i].first<<" "<<blockPos[i].second<<endl;

        varIndex.clear();
        varTabel.clear();
        /// 统计每个变量使用的次数
        blkVarNum = 0;
        for(int j=blockPos[i].first;j<=blockPos[i].second;j++)
        {
//            cout<<"j="<<j<<endl;
            op = quaternaryList[j].op;
            var1 = quaternaryList[j].src1;
            var2 = quaternaryList[j].src2;
            var3 = quaternaryList[j].res;

            if(quaternaryList[j].isLabelOrFunc) continue;

            if(isVar(var1))
            {
                if(varIndex.find(var1) != varIndex.end())
                {
                    varTabel[varIndex[var1]].cnt ++;
                }
                else
                {
                    varIndex[var1] = blkVarNum++;

                    tmp.name = var1;
                    tmp.cnt = 1;
                    varTabel.push_back(tmp);
                }
            }
            if(isVar(var2))
            {
                if(varIndex.find(var2) != varIndex.end())
                {
                    varTabel[varIndex[var2]].cnt ++;
                }
                else
                {
                    varIndex[var1] = blkVarNum++;

                    tmp.name = var2;
                    tmp.cnt = 1;
                    varTabel.push_back(tmp);
                }
            }
            if(isVar(var3))
            {
                if(varIndex.find(var3) != varIndex.end())
                {
                    varTabel[varIndex[var3]].cnt ++;
                }
                else
                {
                    varIndex[var1] = blkVarNum++;

                    tmp.name = var3;
                    tmp.cnt = 1;
                    varTabel.push_back(tmp);
                }
            }
        }
//        cout<<"count done"<<endl;

        /// 按使用次数将变量由大到小排序
        sort(varTabel.begin(),varTabel.end(),cmp);

        /// 给变量分配寄存器
        var2reg[i].clear();
        for(int j=0;j<blkVarNum;j++)
        {
//            cout<<"index:"<<j<<endl;
            if(j<regNum)
            {
                // 分配寄存器
                var2reg[i][varTabel[j].name] = regTabel[j];
            }
            else
            {
                // 未分配寄存器
                var2reg[i][varTabel[j].name] = varTabel[j].name;
            }

            blockVar[i].push_back(varTabel[j].name);
        }
//        cout<<"var2reg done"<<endl;

    }

    regAllDone = true;
    //optimize_outAll();
//    cout<<"optimize_var2reg done"<<endl;
}

void optimize_regGraint(int index)
{
    map<string,string>::iterator iter;
    iter = var2reg[index].begin();
    string var,reg;
    while(iter != var2reg[index].end())
    {
        var = iter->first;
        reg = iter->second;
        if(locVar.find(var) != locVar.end())
        {
            asm_out(LW_MIPS,reg,shamt(locVar[var].offset,FP_REG),SPACE_MIPS);

        }
        else
        {
            asm_out(LA_MIPS,reg,var," #--#");//SPACE_MIPS);
            asm_out(LW_MIPS,reg,shamt(0,reg)," #--#");//SPACE_MIPS);
        }
        iter ++;

        reg2var[reg] = var;
    }
}

void optimize_regRevoke(int index)
{
    map<string,string>::iterator iter;
    iter = var2reg[index].begin();
    string var,reg;
    while(iter != var2reg[index].end())
    {
        var = iter->first;
        reg = iter->second;
        if(locVar.find(var) != locVar.end())
        {
            asm_out(SW_MIPS,reg,shamt(locVar[var].offset,FP_REG),SPACE_MIPS);
        }
        else
        {
            asm_out(LA_MIPS,reg,var," #--#");//SPACE_MIPS);
            asm_out(SW_MIPS,reg,shamt(0,reg)," #--#");//SPACE_MIPS);

        }
        iter ++;
    }
}

void optimize_varReplace(int index)
{
    /// 用寄存器替换变量
    string op,var1,var2,var3;
//    for(int i=1;i<=blockNum;i++)
//    {
        for(int j=blockPos[index].first;j<=blockPos[index].second;j++)
        {
            op = quaternaryList[j].op;

            if(quaternaryList[j].isLabelOrFunc) continue;

            /// 不需要替换的四元式
            if(op == CONST_OP || op == INT_OP || op == CHAR_OP || op == PARA_OP)
            {
                continue;
            }

            var1 = quaternaryList[j].src1;
            var2 = quaternaryList[j].src2;
            var3 = quaternaryList[j].res;

            if(isVar(var1))
            {
                quaternaryList[j].src1 = var2reg[index][var1];
            }
            if(isVar(var2))
            {
                quaternaryList[j].src2 = var2reg[index][var2];
            }
            if(isVar(var3))
            {
                quaternaryList[j].res = var2reg[index][var3];
            }


//            if(op != EQL_OP && op != NEQ_OP && op != BT_OP && op != BET_OP && op != ST_OP && op != SET_OP && op != JMP_OP && op != CALL_OP && op != RET_OP &&
//               op != LAB_OP && op != FUNC_OP && op != PARA_OP && op != CALL_OP && op != CALPARA_OP && var2reg.find(var1) != var2reg.end() && isVar(var1))
//            {
//                quaternaryList[j].src1 = var2reg[var1];
//            }
//
//            if(op != EQL_OP && op != NEQ_OP && op != BT_OP && op != BET_OP && op != ST_OP && op != SET_OP && op != JMP_OP && op != CALL_OP && op != RET_OP &&
//               op != LAB_OP && op != FUNC_OP && op != PARA_OP && op != CALL_OP && op != CALPARA_OP && var2reg.find(var2) != var2reg.end() && isVar(var2))
//            {
//                quaternaryList[j].src2 = var2reg[var2];
//            }
//
//            if(op != EQL_OP && op != NEQ_OP && op != BT_OP && op != BET_OP && op != ST_OP && op != SET_OP && op != JMP_OP && op != CALL_OP && op != RET_OP &&
//               op != LAB_OP && op != FUNC_OP && op != PARA_OP && op != CALL_OP && op != CALPARA_OP && var2reg.find(var3) != var2reg.end() && isVar(var3))
//            {
//                quaternaryList[j].res = var2reg[var3];
//            }

        }
//    }

//    cout<<"optimize_varReplace done!"<<endl;
}

void genAsm(string op)
{
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
}


/// 输出分配后的结果
void optimize_outAll()
{
    string op,var1,var2,var3;
//    bool flag = false;

    asm_init();

    asm_out(".data:",SPACE_MIPS,SPACE_MIPS,SPACE_MIPS);
//    asmSen[asmSentNum].line = ".data";
//    asmSent[asmSentNum].isText = isText;
//    asmSentNum ++;

    for(int i=1;i<=quaternaryNum;i++)
    {
        string op = quaternaryList[i].op;
        string src1 = quaternaryList[i].src1;
        string src2 = quaternaryList[i].src2;
        string res = quaternaryList[i].res;
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


    bool flag = false;
    for(int i=1;i<=quaternaryNum;i++)
    {
        optimize<<endl;

        if(!flag && (op == FUNC_OP || op == PARA_OP) )
        {
            asm_out(JMP_MIPS,"main",SPACE_MIPS,SPACE_MIPS);
            flag = true;
        }

        optimize_out(quaternaryList[i].op, quaternaryList[i].src1,quaternaryList[i].src2,quaternaryList[i].res);

        /// 一个新的基本块
        if(quaternaryList[i].isEntrance)
        {

            map<string,string>::iterator iter;
            iter = var2reg[quaternaryList[i].blockNum].begin();
            while(iter != var2reg[quaternaryList[i].blockNum].end()){
                optimize<<"# "<<iter->first<<" ---> "<<iter->second<<endl;
                iter ++;
            }

            /// 输出这个基本块的上界
            optimize<<endl;
            optimize<<"#Block_"<<setw(3)<<left<<quaternaryList[i].blockNum<<"*******************************#"<<endl;
            /// 每一基本块开始时先把变量的值从内存读到分配好的寄存器中

            if(!isTarget(i) && regAllDone)
            {
                optimize_regGraint(quaternaryList[i].blockNum);
                optimize_varReplace(quaternaryList[i].blockNum);
            }
        }


        op = quaternaryList[i].op;
        src1 = quaternaryList[i].src1;
        src2 = quaternaryList[i].src2;
        res = quaternaryList[i].res;
        //optimize<<src1<<src2<<res<<endl;
        genAsm(op);

        /// 一个新的基本块
        if(quaternaryList[i].isEntrance )
        {
            /// 每一基本块开始时先把变量的值从内存读到分配好的寄存器中
            if(regAllDone && isTarget(i))
            {
                optimize_regGraint(quaternaryList[i].blockNum);
                optimize_varReplace(quaternaryList[i].blockNum);
            }
        }


        if(quaternaryList[i].isExit)
        {
            /// 在输出上一个基本块的下界之前，先把上个基本块的变量从分配好的寄存器存回内存
            if(regAllDone)
            {
                optimize_regRevoke(quaternaryList[i].blockNum);
            }
            /// 输出上一个基本块的下界
            optimize<<"#****************************************#"<<endl;
            //optimize<<endl;
        }

    }
}

void optimize_run()
{

    state = opt_sta;

    optimize_init();

    optimize_blcDiv();

    optimize_dataStream();

    optimize_regAllocation();

    optimize_outAll();

    //for(int i=0;i<regTabel.size();i++) cout<<regTabel[i]<<endl;
}
