#include "global.h"

ifstream source;
ofstream lexical;
ofstream syntax;
ofstream midcode;
ofstream debug;
ofstream asmcode;
ofstream optimize;

int state = 0;

int main()
{
    string filename;
    filename=".\\sourcecode.txt";
    //cin>>filename;
    cout<<"Start!"<<endl;
    debug.open("debug.txt", ios::out);


    source.open(filename.c_str(), ios::in);
    lexical.open("lexical.txt", ios::out);
    lex_run();
    source.close();
    lexical.close();

    cout<<"Lexical done!"<<endl;

    syntax.open("syntax.txt", ios::out);
    midcode.open("midcode.txt", ios::out);
    syn_run();
    syntax.close();
    midcode.close();

    cout<<"Syntax done!"<<endl;

    asmcode.open("asmcode.txt", ios::out);
    asm_run();

    cout<<"Asm done!"<<endl;

    optimize.open("optimize.txt", ios::out);
    optimize_run();
    optimize.close();

    asmcode.close();
    cout<<"Optimize done!"<<endl;


    debug.close();
    cout<<"Compier done!"<<endl;
    return 0;
}
