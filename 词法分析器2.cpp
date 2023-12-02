#include<stdio.h>
#include<ctype.h>
#include<string.h>
#define keywordSum 11
char *keyword[keywordSum]={"if","else","for","while","do","int","read","write","void","function","call"};
char singleword[50]="+-*(){}[];,:";//you can't use '='
char doubleword[10]="><=!";//you can't use '+=' and stuff like that but '>='
char Scanin[300],Scanout[300];
FILE *fin,*fout;
//δ��ɹ��ܣ� 
//λ���������
//���ε�keyword
//������keyword 
//���ҺͱȶԹ��̵��Ż� 
//�������͸����ͱ���d
//��������� 
//����ɹ��ܣ� 
//����ע��
//���ֺ��»��߿�ͷ�ı�ʶ������ 
//������ʶ�������»��� 
//��д��keyword�ܹ�ʶ����ʾ
//�Ƿ��ַ�
//�жϹ����й��EOF���ж� 
int line=1;

int TESTscan()
{
	char ch,token[40];
	int es=0,j;//token counter
	int n;//search keyword 
	printf("������Դ���������ļ���������·����:");
	scanf("%s",Scanin);
    printf("������Դ��������ļ���������·����:");
	scanf("%s",Scanout);
	if((fin=fopen(Scanin,"r"))==NULL)
	{
		printf("\n�򿪴ʷ����������ļ�����!\n");
		return(1);
	}
	if((fout=fopen(Scanout,"w"))==NULL)
	{
		printf("\n�����ʷ���������ļ�����!\n");
		return(2);
	}
	ch=getc(fin);//get a char and move pointer 
	while(ch!=EOF)//End Of File
	{
		while(ch==' '||ch=='\n'||ch=='\t'){
			if(ch=='\n') line++;
			ch=getc(fin);
		}
		if(isalpha(ch))//isalpha is a function , if it is a letter , return  non-zero number
		{	
			token[0]=ch;j=1;
			ch=getc(fin);
			while(isalnum(ch)||ch=='_')
			{
				token[j++]=ch;
				ch=getc(fin);
			}
			token[j]='\0';//finish inputting a string
			n=0;
			while((n<keywordSum)&&strcmp(token,keyword[n]))n++;
			if(n<keywordSum)
				fprintf(fout,"%s\t%s\n",token,token);//lower letter keyword
			else{
				char tokenassist[40];
				int i=0;
				do{
					tokenassist[i]=token[i];
					if(tokenassist[i]>=65&&tokenassist[i]<=90) tokenassist[i]+=32;
					i++;
				}while(token[i-1]!='\0');
				n=0;
				while((n<keywordSum)&&strcmp(tokenassist,keyword[n]))n++;
				if(n<keywordSum)
					fprintf(fout,"%s\t%s\n",tokenassist,tokenassist);//capital letter keyword
				else
					fprintf(fout,"%s\t%s\n","ID",token);//indentifier
			}
		}else if(isdigit(ch))
		{
			token[0]=ch;
			j=1;
			ch=getc(fin);
			if(isalpha(ch)||ch=='_'){//output illegal keyword
				do{
					token[j++]=ch;
					ch=getc(fin);
				}
				while(isalnum(ch)||ch=='_');
				token[j]='\0';
				printf("%s%d%s\t%s\t%s\n","��",line,"��","�Ƿ���ʶ��",token);
			}
			else{
				while(isdigit(ch))
				{
					token[j++]=ch;
					ch=getc(fin);
				}
				token[j]='\0';
				fprintf(fout,"%s\t%s\n","NUM",token);
			}
		}else if(ch=='_'){//when the first char is _ , it is illegal identifier
			token[0]=ch;
			j=1;
			ch=getc(fin);
			if(isalpha(ch)||ch=='_'){
				do{
					token[j++]=ch;
					ch=getc(fin);
				}
				while(isalnum(ch)||ch=='_');
				token[j]='\0';
				printf("%s%d%s\t%s\t%s\n","��",line,"��","�Ƿ���ʶ��",token);
			}
		}else if(strchr(singleword,ch)>0)
		{
			token[0]=ch;token[1]='\0';
			ch=getc(fin);
			fprintf(fout,"%s\t%s\n",token,token);
		}else if(strchr(doubleword,ch)>0)
		{
			token[0]=ch;
			ch=getc(fin);
			if(ch=='=')
			{
				token[1]=ch;token[2]='\0';
				ch=getc(fin);
			}else
				token[1]='\0';
			fprintf(fout,"%s\t%s\n",token,token);
		}else if(ch=='/')
		{
			ch=getc(fin);
			if(ch=='*')
			{
				char ch1;
				ch1=getc(fin);
				do
				{
					ch=ch1;ch1=getc(fin);}
				while((ch!='*'||ch1!='/')&&ch1!=EOF);
				ch=getc(fin);
			}else if(ch=='/')
			{
				do
				{
					ch=getc(fin);}
				while(ch!='\n'&&ch!=EOF);
				ch=getc(fin);
			}
			else
			{
				token[0]='/';token[1]='\0';
				fprintf(fout,"%s\t%s\n",token,token);
			}
		}else if(ch!=EOF)//output illegal char 
		{
			token[0]=ch;token[1]='\0';
		    ch=getc(fin);
			es=3;
			printf("%s%d%s\t%s\t%s\n","��",line,"��","�Ƿ��ַ�",token);
		}
	}
	fclose(fin);
	fclose(fout);
	return(es);
}


int main(){
	int es=0;
	
	es=TESTscan();
	if(es>0)printf("������ɣ��������г��ַǷ��ַ���\n");
	else printf("�ʷ������ɹ���\n");
	printf("39-��ѱ�-2220213112"); 
	return 1;
}

