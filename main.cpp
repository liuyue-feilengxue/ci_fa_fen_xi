#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;
//�ж������������ʱ���õ�
int line_num = 1;

//���ʵĽṹ��
typedef struct
{
    int typenum;//���ͱ���
    string token;
}WORD;

//��������
WORD* scaner(FILE *fp); // ���ļ�ɨ���ȡ
int keyOrIdentifier(string token);  // �ж��ǹؼ��ֻ��Ǳ�ʶ��
#define _KEY_WORD_END "waiting for your expanding"
string KEY_WORDS[]= {"main","return","if","else","for","include","printf","int","char","#include","stdio.h",_KEY_WORD_END};   /*�ؼ��ִ���*/
/**
�Զ�����������EOF���ж��Ƿ������
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
�������� 11
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

int keyOrIdentifier(string token)//�ж��ǲ��Ǳ�ʶ��
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

WORD* scaner(FILE *fp)//ɨ���ĵ�
{
    char ch;
    string token="";
    WORD* myword=new WORD;
    myword->typenum=0;
    myword->token="";

    ch=fgetc(fp);

    /* �жϻس� */
    if(int(ch)==10)
    {
        line_num++;
        return (myword);
    }
    /* �жϿո� */
    else if(isspace(ch))
    {
        return (myword);
    }
    /* ��ʶ�����ؼ��� */
    else if(isalpha(ch)||ch == '#')    //������ַ�����ĸ����ͷ�ļ�
    {
        while(isalpha(ch)||isdigit(ch)||ch == '#')//����ĸ�����־�һֱ����������д�꣬��ͷ�ļ��ľͰ�ͷ�ļ�д��
        {
            token=token+ch;
            ch=fgetc(fp);
        }
        fseek( fp, -1, SEEK_CUR );  //����ȡ���ַ��Ȳ�����ĸҲ�������֣���ָ����Ȼָ����ַ�
        myword->typenum=keyOrIdentifier(token);  // �жϻ�ȡ���ַ����ǹؼ��ֻ��Ǳ�ʶ������ȡ����Ӧ������
        myword->token=token;
        return(myword);
    }

    /* ���� */
    else if(isdigit(ch))
    {
        while(isdigit(ch))//������
        {
            token=token+ch;
            ch=fgetc(fp);
        }
        fseek( fp, 0, SEEK_CUR );
        //myword->typenum=20;���о��������ˣ�
        myword->typenum=11;
        myword->token=token;
        return(myword);
    }
    else
        switch(ch)
        {
        /* ����� */
        case '+':
            ch=fgetc(fp);//��ȡһ���ַ��������ǲ���++
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
            if (ch=='-')//ͬ���ж�--
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
        /* ���ź�ע�� */
        case '/':
            ch=fgetc(fp);
            if (ch=='/')
            {
                ch=fgetc(fp);
                while(int(ch)!=10&&ch!=EOF) // ����ע�ͷ��ţ���һֱ��ȡ��ֱ�������س�Ϊֹ
                {
                    ch=fgetc(fp);
                }
                if(int(ch)==10)
                {
                    line_num++;
                }
                return(myword);//ע��
                break;
            }
            fseek( fp, 0, SEEK_CUR );
            myword->typenum=15;//����ע�ͣ�ֻ����ͨ����
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


        /* ��� */
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


        /* ������*/
        case EOF:
            myword->typenum=1000;
            myword->token="OVER";
            return(myword);
            break;

        /* ����*/
        default:
            myword->typenum=-1;
            myword->token="ERROR";
            cout<<"��"<<line_num<<"�з����˴���"<<endl;
            return(myword);
        }
}


int main()
{
    int over = 1;//���������ִ���ͽ���
    FILE *fp;
    WORD* oneword=new WORD;
    if ((fp=fopen("input.txt","rt"))==NULL)//�ļ�Ϊ��
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
