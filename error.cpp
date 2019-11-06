#include "global.h"
#define cout debug
using namespace std;

int width = 5;

void errorContent(int errorID, int errorLine, int errorColumn)
{
    cout<<setw(width)<<left<<"Error: "<<setw(width)<<left<<errorID;
    cout<<setw(width)<<left<<"Line: "<<setw(width)<<left<<errorLine;
    cout<<setw(width)<<left<<"Position: "<<setw(width)<<left<<errorColumn;
    cout<<setw(width)<<left<<"Content: ";
    switch(errorID)
    {
        case UNCERTAIN_ERR:
            cout<<"a uncertaint error!"<<endl;
            break;
        case CONSTREDEF_ERR:
            cout<<"constant redefinition!"<<endl;
            break;
        case VARREDEF_ERR:
            cout<<"variable redefinition!"<<endl;
            break;
        case FUNREDEF_ERR:
            cout<<"funtion redefinition!"<<endl;
            break;
        case LACKSEMICOLON_ERR:
            cout<<"lack of \';\' !"<<endl;
            break;
        case VARUNDEF_ERR:
            cout<<"variable undefined!"<<endl;
            break;
        case FUNUNDEF_ERR:
            cout<<"funtion undefined!"<<endl;
            break;
        case CONSTREASSIGN_ERR:
            cout<<"constant reassignment!"<<endl;
            break;

        case ADDORSUB_OPE_ERR:
            cout<<"lack of \'*|/\' !"<<endl;
            break;
        case MULORDIV_OPE_ERR:
            cout<<"lack of \'+|-\' !"<<endl;
            break;
        case RELATION_OPE_ERR :
            cout<<"lack of \'relation\' !"<<endl;
            break;
        case CONST_SYMID_ERR :
            cout<<"lack of \'const\' !"<<endl;
            break;
        case INTORCHAR_SYMID_ERR :
            cout<<"lack of \'int | char\' !"<<endl;
            break;
        case ASS_SYMID_ERR :
            cout<<"lack of \'=\' !"<<endl;
            break;
        case NUMBER_ERR :
            cout<<"should be a number !"<<endl;
            break;
        case INT_ERR :
            cout<<"should be an int type !"<<endl;
            break;
        case IDENTIFIER_ERR :
            cout<<"should be an identifier !"<<endl;
            break;
        case LEFTSMALLBRACKET_ERR :
            cout<<"lack of \'(\' !"<<endl;
            break;
        case RIGHTSMALLBRACKET_ERR :
            cout<<"lack of \')\' !"<<endl;
            break;
        case LEFTMIDDLEBRACKET_ERR :
            cout<<"lack of \'[\' !"<<endl;
            break;
        case RIGHTMIDDLEBRACKET_ERR :
            cout<<"lack of \']\' !"<<endl;
            break;
        case LEFTBIGBRACKET_ERR :
            cout<<"lack of \'{\' !"<<endl;
            break;
        case RIGHTBIGBRACKET_ERR :
            cout<<"lack of \'}\' !"<<endl;
            break;
        case VOID_ERR :
            cout<<"lack of \'void\' !"<<endl;
            break;
        case MAIN_ERR :
            cout<<"lack of \'main\' !"<<endl;
            break;
        case PRINTF_ERR :
            cout<<"lack of \'printf\' !"<<endl;
            break;
        case SCANF_ERR :
            cout<<"lack of \'scanf\' !"<<endl;
            break;
        case COLON_ERR :
            cout<<"lack of \':\' !"<<endl;
            break;
        case CASE_ERR :
            cout<<"lack of \'case\' !"<<endl;
            break;
        case SWITCH_ERR :
            cout<<"lack of \'switch\' !"<<endl;
            break;
        case WHILE_ERR :
            cout<<"lack of \'while\' !"<<endl;
            break;
        case IF_ERR :
            cout<<"lack of \'if\' !"<<endl;
            break;
        default:
            cout<<"default error!"<<endl;
    }
}
