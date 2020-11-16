#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;
//判断行数，报错的时候用到
int line_num = 1;

//单词的结构体
typedef struct
{
    int typenum;//类型编码
    string token;
}WORD;

//函数声明
WORD* scaner(FILE *fp); // 对文件扫描读取
int keyOrIdentifier(string token);  // 判断是关键字还是标识符
#define _KEY_WORD_END "waiting for your expanding"
string KEY_WORDS[]= {"main","return","if","else","for","include","printf","int","char","#include","stdio.h",_KEY_WORD_END};   /*关键字词组*/
/**
自动结束（用了EOF来判断是否结束）
main 41
return 42
if 43
else 44
for 45
include 46
printf 47
int 48
char 49
#include 50
stdio.h 51
数字类型 11
+ 12
- 13
* 14
/ 15
++ 16
-- 17
== 18
!= 19
= 20
( 21
) 22
[ 23
] 24
{ 25
} 26
, 27
: 28
; 29
> 30
>= 31
< 32
<= 33
" 34
*/

int keyOrIdentifier(string token)//判断是不是标识符
{
    int i=0;
    int j=40;
    while(KEY_WORDS[i]!=_KEY_WORD_END)
    {
        if(KEY_WORDS[i]==token)
        {
            return j+1;
        }
        i=i+1;
        j=j+1;
    }
    return 52;
}

WORD* scaner(FILE *fp)//扫描文档
{
    char ch;
    string token="";
    WORD* myword=new WORD;
    myword->typenum=0;
    myword->token="";

    ch=fgetc(fp);

    /* 判断回车 */
    if(int(ch)==10)
    {
        line_num++;
        return (myword);
    }
    /* 判断空格 */
    else if(isspace(ch))
    {
        return (myword);
    }
    /* 标识符及关键字 */
    else if(isalpha(ch)||ch == '#')    //如果首字符是字母或者头文件
    {
        while(isalpha(ch)||isdigit(ch)||ch == '#')//是字母或数字就一直把整个单词写完，是头文件的就把头文件写完
        {
            token=token+ch;
            ch=fgetc(fp);
        }
        fseek( fp, -1, SEEK_CUR );  //当获取的字符既不是字母也不是数字，则指针依然指向该字符
        myword->typenum=keyOrIdentifier(token);  // 判断获取的字符串是关键字还是标识符，获取它对应的类型
        myword->token=token;
        return(myword);
    }

    /* 数字 */
    else if(isdigit(ch))
    {
        while(isdigit(ch))//是数字
        {
            token=token+ch;
            ch=fgetc(fp);
        }
        fseek( fp, 0, SEEK_CUR );
        //myword->typenum=20;（感觉这里打错了）
        myword->typenum=11;
        myword->token=token;
        return(myword);
    }
    else
        switch(ch)
        {
        /* 运算符 */
        case '+':
            ch=fgetc(fp);//再取一个字符，看看是不是++
            if (ch=='+')
            {
                myword->typenum=16;
                myword->token="++";
                return(myword);
            }
            fseek( fp, 0, SEEK_CUR );
            myword->typenum=12;
            myword->token="+";
            return(myword);
            break;
        case '-':
            ch=fgetc(fp);
            if (ch=='-')//同理，判断--
            {
                myword->typenum=17;
                myword->token="--";
                return(myword);
            }
            fseek( fp, 0, SEEK_CUR );
            myword->typenum=13;
            myword->token="-";
            return(myword);
            break;
        case '*':
            myword->typenum=14;
            myword->token="*";
            return(myword);
            break;
        /* 除号和注释 */
        case '/':
            ch=fgetc(fp);
            if (ch=='/')
            {
                ch=fgetc(fp);
                while(int(ch)!=10&&ch!=EOF) // 发现注释符号，则一直读取，直到遇到回车为止
                {
                    ch=fgetc(fp);
                }
                if(int(ch)==10)
                {
                    line_num++;
                }
                return(myword);//注释
                break;
            }
            fseek( fp, 0, SEEK_CUR );
            myword->typenum=15;//不是注释，只是普通除号
            myword->token="/";
            return(myword);
            break;
        case '(':
            myword->typenum=21;
            myword->token="(";
            return(myword);
            break;
        case '=':
            ch=fgetc(fp);
            if (ch=='=')
            {
                myword->typenum=18;
                myword->token="==";
                return(myword);
            }
            fseek( fp, 0, SEEK_CUR );
            myword->typenum=20;
            myword->token="=";
            return(myword);
            break;
        case '!':
            ch=fgetc(fp);
            if (ch=='=')
            {
                myword->typenum=19;
                myword->token="!=";
                return(myword);
            }
            fseek( fp, 0, SEEK_CUR );
            myword->typenum=-1;
            myword->token="ERROR";
            return(myword);
            break;
        case '>':
            ch=fgetc(fp);
            if (ch=='=')
            {
                myword->typenum=37;
                myword->token=">=";
                return(myword);
            }
            fseek( fp, 0, SEEK_CUR );
            myword->typenum=35;
            myword->token=">";
            return(myword);
            break;
        case '<':
            ch=fgetc(fp);
            if (ch=='=')
            {
                myword->typenum=38;
                myword->token="<=";
                return(myword);
            }
            fseek( fp, 0, SEEK_CUR );
            myword->typenum=36;
            myword->token="<";
            return(myword);
            break;


        /* 界符 */
        case ')':
            myword->typenum=22;
            myword->token=")";
            return(myword);
            break;
        case '[':
            myword->typenum=23;
            myword->token="[";
            return(myword);
            break;
        case ']':
            myword->typenum=24;
            myword->token="]";
            return(myword);
            break;
        case '{':
            myword->typenum=25;
            myword->token="{";
            return(myword);
            break;
        case '}':
            myword->typenum=26;
            myword->token="}";
            return(myword);
            break;
        case ',':
            myword->typenum=27;
            myword->token=",";
            return(myword);
            break;
        case ':':
            myword->typenum=28;
            myword->token=":";
            return(myword);
            break;
        case ';':
            myword->typenum=29;
            myword->token=";";
            return(myword);
            break;
        case '"':
            myword->typenum=34;
            myword->token="\"";
            return(myword);
            break;


        /* 结束符*/
        case EOF:
            myword->typenum=1000;
            myword->token="OVER";
            return(myword);
            break;

        /* 错误*/
        default:
            myword->typenum=-1;
            myword->token="ERROR";
            cout<<"第"<<line_num<<"行发生了错误！"<<endl;
            return(myword);
        }
}


int main()
{
    int over = 1;//如果程序出现错误就结束
    FILE *fp;
    WORD* oneword=new WORD;
    if ((fp=fopen("input.txt","rt"))==NULL)//文件为空
    {
        printf("Cannot open the file");
    }
    while (over<1000&&over!=-1)
    {
        oneword=scaner(fp);
        if(oneword->typenum<1000&&oneword->typenum>0)
        {
            cout<<"("<<oneword->typenum<<","<<oneword->token<<")"<<endl;
        }
        over=oneword->typenum;
    }
    fclose(fp);
    return 0;
}
