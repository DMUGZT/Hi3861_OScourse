#include<stdio.h>
#include<ctype.h>
#include<string.h>
#define keywordSum 12
int line=1;
char *keyword[keywordSum]={"if","else","for","while","do","int","float","read","write","void","function","call"};
char singleword[50]="+-*(){}[];,:";//you can't use '='
char doubleword[10]="><=!";//you can't use '+=' and stuff like that but '>='
char Scanin[300],Scanout[300];
FILE *fin,*fout;

int TESTscan()
{
	char ch,token[40];
	int es=0,j;//token counter
	int n;//search keyword 
//	printf("������Դ���������ļ���������·����:");
//	scanf("%s",Scanin);
	strcpy(Scanin,"source.txt");
//    printf("������Դ��������ļ���������·����:");
//	scanf("%s",Scanout);
	strcpy(Scanout,"tuples.txt");
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

//ABSTRACT SYNTAX TREE 
#define ELEMENTtype char
#define CHILDRENMAX 10
int level=0;//tree level used for drawing 
char ASTtxt[30];//put AST name in this array
FILE *astfin; 

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <string.h>
#define MAX_SYMBOL_NUM 5//the maximum of symbol table

typedef struct TreeNode {
    ELEMENTtype data[30];
    struct TreeNode *children[CHILDRENMAX];
} TreeNode;

TreeNode* createNode(ELEMENTtype *data) {//definition in heap area
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(node->data,data);
    for (int i = 0; i < CHILDRENMAX; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void addChild(TreeNode *parent, TreeNode *child) {//connect parents node and child node
    for (int i = 0; i < CHILDRENMAX; i++) {
        if (parent->children[i] == NULL) {
            parent->children[i] = child;
            return;
        }
    }
}

void traverseTree(TreeNode *root,int level) {
    if (root == NULL) {
        return;
    }
    if(level>0) for(int i=level;i>0;i--) fprintf(astfin,"  ");

    fprintf(astfin,"%s\n",root->data);
    for (int i = 0; i < CHILDRENMAX; i++) {
        traverseTree(root->children[i],level+1);
    }
}

void drawAST(TreeNode *root,int level){
//	printf("������AST�ļ���������·������");
//	scanf("%s", ASTtxt);
	strcpy(ASTtxt,"AST.txt");
	if((astfin=fopen(ASTtxt,"w"))==NULL)
	{
		printf("\n����AST����ļ�����!\n");
		return;
	}
	traverseTree(root,level);
}
TreeNode *root;
TreeNode *node1;
TreeNode *node2;
//use these nodes to implement the first part of the AST
TreeNode *statNode;
//use this node to implement the second part of the AST

enum Category_symbol {empty,variable,function};//identifier has two kind of type
int TESTparse();
int program();
int fun_declaration();
int main_declaration();
int function_body(TreeNode* subroot);
int compound_stat(TreeNode* subroot);
int statement(TreeNode* subroot);
int expression_stat(TreeNode* subroot);
int expression(TreeNode* subroot);
int bool_expr(TreeNode* subroot);
int additive_expr(TreeNode* subroot);
int term(TreeNode* subroot);
int factor(TreeNode* subroot);
int if_stat(TreeNode* subroot);
int while_stat(TreeNode* subroot);
int for_stat(TreeNode* subroot);
int do_while_stat(TreeNode* subroot);
int write_stat(TreeNode* subroot);
int call_stat(TreeNode* subroot);
int read_stat(TreeNode* subroot);
int declaration_stat();
int declaration_list();
int statement_list(TreeNode* subroot);
int insert_Symbol(enum Category_symbol category, char *name);
int lookup(char *name, int *pPosition,enum Category_symbol category);

char token[20], token1[40]; //two-tuples , first attribute got with token , second one got with token1
char tokenfile[30];//storage of tuples file
char Codeout[30]; //����ļ���

FILE *fpTokenin;  //check tuples file with this pointer
//FILE *fpCodeBinary;   //�м����������ļ�ָ��
FILE *fpCodeText; //�м�����ı��ļ�ָ��
int back;//sometime we need to recall the last tuple with this variable

struct //symbol type
{
    char name[20];
    enum Category_symbol kind;//identifier type
    int address;
} symbol[MAX_SYMBOL_NUM];

typedef struct Code
{
    char opt[10];    //������
    int operand;     //������
} Code;
Code codes[200];    //����м����Ľṹ������

int codesIndex;  //codes�����е�һ����Ԫ�ص��±꣬0����һ��Ҫ���ɵĴ�����codes�е�λ�ã�
int symbolIndex = 0; //symbol�����е�һ����Ԫ�ص��±꣬0����һ��Ҫ����ı�ʶ���ڷ��ű��е�λ�ã�
int offset;//�ֲ������������庯���ڲ�����Ե�ַ

//���������Ҫ�����ķ�����
//���ͼ�飺�����Ϸ��� call ID() read ID ���ʽ�� ��������δ���� ���������ظ����� ��������mainʱ ��Ϊmain���������ַ����﷨���� ���������������໥�ظ� 
//����չ�������������ͼ�����������ʶ�� 

//�﷨������������������ɳ���
int TESTparse()
{
    codesIndex = 0;
    int i;
    int es = 0;
//    printf("�����뵥�����ļ���������·������");
//    scanf("%s", tokenfile);
	strcpy(tokenfile,"tuples.txt");
    if((fpTokenin = fopen(tokenfile, "r")) == NULL){
        printf("\n��%s����!\n",tokenfile );
        es = 10;
        return(es);
       }
    es = program();
    fclose(fpTokenin);

    printf("==�﷨������������������ɳ�����==\n");
    switch(es){
        case 0: printf("�﷨����������ɹ��������������ɴ���!\n"); break;
        case 10: printf("���ļ� %sʧ��!\n", tokenfile); break;
        case 1: printf("�˴�Ӧ����������!\n"); break;
        case 2: printf("ȱ��}!\n"); break;
        case 3: printf("ȱ�ٱ�ʶ��!\n"); break;
        case 4: printf("�ٷֺ�!\n"); break;
        case 5: printf("ȱ��(!\n"); break;
        case 6: printf("ȱ��)!\n"); break;
        case 7: printf("ȱ�ٲ�����!\n"); break;
    	case 8: printf("ȱ�ٵȺ�!\n"); break;/////////////ȱ��=����8 
    	case 9: printf("ȱ��*��/!\n"); break;/////////////< term >��<factor>{(*| /)< factor >}��ȱ��*��/ 
        case 11: printf("������ͷȱ��{!\n"); break;
        case 12: printf("��������ȱ��}!\n"); break;
        case 13:printf("���һ������������Ӧ����main!\n"); break;
        case 14: printf("ȱ����䣡"); break; 
        case 15: printf("do_while���û��while!"); break;
        case 16: printf("�������ȱ��}!"); break;
        case 21: printf("���ű����!\n"); break;
        case 22: printf("����%s�ظ�����!\n",token1); break;
        case 23: printf("����δ����!\n"); break;
        case 24:printf("������main���������󣬻������������ַ�\n");break;
        case 25: printf("����δ����!\n"); break;
        case 26: printf("�����ͱ������ظ�����!\n"); break;
        case 32: printf("�������ظ�����!\n"); break;
        case 34: printf("call������ı�ʶ��%s���Ǳ�����!\n",token1 ); break;
        case 35: printf("read������ı�ʶ�����Ǳ�����!\n"); break;
        case 36: printf("��ֵ������ֵ%s���Ǳ�����!\n",token1); break;
        case 37: printf("���Ӷ�Ӧ�ı�ʶ�����Ǳ�����!\n"); break;
		case 40: printf("�������ʽ���ݴ���!\n"); break;/////////////////////////bool_expr���׸�Ԫ��û��IDû��NUMҲû��( 
		case 41: printf("ȱ�ٱ��ʽ!\n"); break;
        }

//    printf("������Ҫ���ɵ��ı���ʽ���м�����ļ������֣�����·������");
//    scanf("%s", Codeout);
	strcpy(Codeout,"intermediate_code.txt");
    if((fpCodeText = fopen(Codeout, "w")) == NULL) {
        printf("\n����%s����!\n", Codeout);
        es = 10;
        return(es);
        }

    for(i=0;i<codesIndex;i++){
		if(strcmp(codes[i].opt,"LOAD")==0||strcmp(codes[i].opt,"LOADI")==0||strcmp(codes[i].opt,"STO")==0||
		strcmp(codes[i].opt,"BR")==0||strcmp(codes[i].opt,"BRF")==0||strcmp(codes[i].opt,"CAL")==0||strcmp(codes[i].opt,"ENTER")==0)
			fprintf(fpCodeText," %3d %-6s %d\n",i,codes[i].opt,codes[i].operand);
		else
			fprintf(fpCodeText," %3d %-5s\n",i,codes[i].opt);
        }
    fclose(fpCodeText);
//
//    printf("������Ҫ���ɵĶ�������ʽ���м�����ļ������֣��ṹ��洢��:");
//    scanf("%s",Codeout);
//    if((fpCodeBinary = fopen(Codeout,"wb")) == NULL) {
//        printf("\n����%s����!\n", Codeout);
//        es = 10;
//        return(es);
//        }
//    fwrite(codes,sizeof(Code),codesIndex,fpCodeBinary);
//    fclose(fpCodeBinary);
    return(es);
}

//<program> ��{ fun_declaration }<main_declaration>
//<fun_declaration> �� function ID��(�� �� )��< function_body>�����'function'ָ�����д'function'��ʶ���'function'
//<main_declaration>��  main��(�� �� )�� < function_body>
int program()
{
    int es = 0, i;
    root = createNode("program");
//    strcpy(codes[codesIndex++].opt,"BR");//codes����ĵ�һ��ָ����������ת�Ƶ�main��������ڣ���ڵ�ַ��Ҫ����
    fscanf(fpTokenin, "%s %s\n", token, token1);
    while(strcmp(token, "function")==0) {//   it is a normal function , do
		node1 = createNode(token1);
		addChild(root, node1);	   
		fscanf(fpTokenin, "%s %s\n", token, token1);
    	es=fun_declaration();
       	if(es>0) return es;
    }
    if(strcmp(token, "ID")){//identify this two-tuple as ID main 
        es=1;
        return es;
        }
    if(strcmp(token1, "main"))  {
        es=13;
        return es;
        }
    node1 = createNode(token1);
	addChild(root, node1);	   
    fscanf(fpTokenin, "%s %s\n", token, token1);
    es=main_declaration();
    if(es > 0) return(es);
	getc(fpTokenin);		//�̵�EOF���ټ�� 
    if(!feof(fpTokenin))    //judge whether there're still some chars after expected end
	   return (es=24);

    //������ű������
//    printf("     ���ű�\n");
//    printf(" ����\t \t���� \t��ַ\n");
//    for(i = 0; i < symbolIndex; i++)
//        printf("  %-8s \t%d \t%d\n", symbol[i].name, symbol[i].kind,symbol[i].address);
    return(es);
}

//<fun_declaration> �� function ID��(�� �� )��< function_body>
int fun_declaration()//it needs to come in with new two-tuple , and leave without tuple
{
    int es=0;
    if (strcmp(token,"ID")){//token����ID������if 
            es=3;
            return es;
        }
    node2 = createNode(token1);
	addChild(node1, node2);
    es=insert_Symbol(function,token1);//��ID������Ҫ��������������ű�
	if(es>0) return(es);
    fscanf(fpTokenin, "%s %s\n", token, token1);
    if (strcmp(token,"(")) {
        es=5;
        return es;
        }
    fscanf(fpTokenin, "%s %s\n", token, token1);
    if (strcmp(token,")")){
       es=6;
       return es;
       }
    fscanf(fpTokenin, "%s %s\n", token, token1);
    node2 = createNode("function_body");
	addChild(node1, node2);
    es=function_body(node2);
    return es;
}

//<main_declaration>�� main��(�� �� )�� < function_body>
int main_declaration()//it needs to come in with new two-tuple , and leave without tuple
{
    int es=0;
    es=insert_Symbol(function,"main");
    if (strcmp(token,"(")){
        es=5;
        return es;
        }
    fscanf(fpTokenin, "%s %s\n", token, token1);
    if (strcmp(token,")")) {
        es=6;
        return es;
        }

//    codes[0].operand=codesIndex;//����codes����ĵ�0��ָ���ת��Ŀ��Ϊmain�����ĺ��������

    fscanf(fpTokenin, "%s %s\n", token, token1);
	node2 = createNode("function_body");
	addChild(node1, node2);
    es= function_body(node2);
    return es;
}

//<function_body>��'{'<declaration_list><statement_list>'}'
int function_body(TreeNode* subroot)//it needs to come in with new two-tuple , and leave without tuple
{
    int es=0;
    if(strcmp(token,"{")){//�ж��Ƿ�'{'
        es=11;
        return(es);
        }
//    symbol[symbolIndex-1].address=codesIndex;//�Ѻ��������ڵ�ַ���뺯�����ڷ��ű��еĵ�ַ�ֶ�
    offset=2;//����һ���µĺ�������������Ե�ַ��2��ʼ
    if(fscanf(fpTokenin,"%s %s\n",&token,&token1)<2){
    	es=12;
    	return(es);
	}

    es=declaration_list();
    if (es>0) return(es);
//    strcpy(codes[codesIndex].opt, "ENTER");//������Ŀ�ʼ
//    codes[codesIndex].operand=offset;
//    codesIndex++;
    es=statement_list(subroot);
    if (es>0) return(es);
    if(strcmp(token,"}")){//�ж��Ƿ�'}'
		es=12;
        return(es);
        }
    fscanf(fpTokenin, "%s %s\n", &token, &token1);
//    strcpy(codes[codesIndex++].opt, "RETURN");//������Ľ���
    return es;
}

//<declaration_list>��{<declaration_stat>}
int declaration_list()//����ĺ����ڵ���䣬ֻ���ж����������� 
{
    int es = 0;

    while(strcmp(token, "int") == 0||strcmp(token, "float") == 0) {
    	node1 = createNode(token1);
		addChild(node2, node1);
        es = declaration_stat();
        if(es > 0) return(es);
    }
    return(es);
}

//<declaration_stat>��int ID||float ID;
int declaration_stat()
{
    int es = 0;
	TreeNode *tempNode=node2;
    fscanf(fpTokenin, "%s %s\n", &token, &token1);
    if(strcmp(token, "ID"))	return(es = 3); //���Ǳ�ʶ����ûȡ��ID����ֱ��return�˳� 
	node2 = createNode(token1);
	addChild(node1, node2);
	node2=tempNode;
    es = insert_Symbol(variable,token1); //������ű�
    if(es > 0) return(es);
    fscanf(fpTokenin, "%s %s\n", &token, &token1);
    if(strcmp(token, ";"))	return(es = 4);
    if(fscanf(fpTokenin,"%s %s\n",&token,&token1)<2){
    	es=12;
    	return(es);
	}
    return(es);
}

//<statement_list>��{<statement>}
int statement_list(TreeNode* subroot)///it needs to come in with new two-tuple , and leave without tuple
{					//����function��main������ 
    int es = 0;
//	!(strcmp(token, "if") == 0||strcmp(token, "while") == 0||strcmp(token, "for") == 0||strcmp(token, "read") == 0||
//			strcmp(token, "write") == 0||strcmp(token, "{") == 0||strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0 || 
//					strcmp(token, "(") == 0||strcmp(token,";")==0)
    while(strcmp(token, "if") == 0||strcmp(token, "while") == 0||strcmp(token, "for") == 0||strcmp(token, "do") == 0||
			strcmp(token, "read") == 0||strcmp(token, "call") == 0||strcmp(token, "write") == 0||strcmp(token, "{") == 0||strcmp(token, "ID") == 0 ||
					strcmp(token, "NUM") == 0 || strcmp(token, "(") == 0||strcmp(token,";")==0)  {//����statement��first�ͽ���ѭ���� 
        es = statement(subroot);
        if(es > 0) return(es);
        if(strcmp(token, "function")==0||strcmp(token1, "main")==0) return(es);
    }
//    fscanf(fpTokenin, "%s %s\n", &token, &token1);
    return(es);
}

//<statement>�� <if_stat>|<while_stat>|<for_stat>
//               |<compound_stat> |<expression_stat>| <call _stat>
int statement(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{
    int es = 0;
    if(es == 0 && strcmp(token, "if") == 0) es = if_stat(subroot); //<if���>
    else if(es == 0 && strcmp(token, "while") == 0) es = while_stat(subroot); //<while���>
    else if(es == 0 && strcmp(token, "for") == 0) es = for_stat(subroot); //<for���>
    else if(es == 0 && strcmp(token, "do") == 0) es = do_while_stat(subroot); //<do_while���>
    else if(es == 0 && strcmp(token, "read") == 0) es = read_stat(subroot); //<read���>
    else if(es == 0 && strcmp(token, "write") == 0) es = write_stat(subroot); //<write���>
    else if(es == 0 && strcmp(token, "{") == 0) es = compound_stat(subroot); //<�������>
    else if(es == 0 && strcmp(token, "call") == 0) es = call_stat(subroot);//<�����������> 
    else if(es == 0 && (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0 || strcmp(token, "(") == 0||strcmp(token,";")==0)) es = expression_stat(subroot); //<���ʽ���>
	else if(strcmp(token, "}") == 0) return(es);
	else if(es==0&&(strcmp(token, "function") == 0||strcmp(token1, "main") == 0)){
		es=14;
		return(es);
	}//�ܹ���������ڲ�Ƕ�׶��庯������ �ͺ���û��}ʱ ��������һ��function��main��ʱ��һ������ȱ�ٲ�����䣨��һ������û������ 
	return(es);
}

//<if_stat>�� if '('<expr>')' <statement > [else < statement >]
int if_stat(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{
	statNode = createNode(token1);
	addChild(subroot, statNode);
	TreeNode *tempNode=statNode;
	fscanf(fpTokenin, "%s %s\n", token, token1);
    int es = 0,cx1, cx2; //if
    if (strcmp(token,"(")){
        es=5;
        return es;
    }
    fscanf(fpTokenin, "%s %s\n", token, token1);
    if(strcmp(token,"(")==0||strcmp(token,"ID")==0||strcmp(token,"NUM")==0){
    	es=expression(statNode); 
    	if(es>0) return(es);
	}
	else{
		es=41;
		return(es); 
	}
    if (strcmp(token,")")) {
        es=6;
        return es;
    }
//�����ж�if(expression)�󲻸�һ�����ʱ���� 
	back=ftell(fpTokenin);
	char token2[20], token3[40];
	strcpy(token2,token);
	strcpy(token3,token1);
    fscanf(fpTokenin, "%s %s\n", &token, &token1);
	if(!(strcmp(token, "if") == 0||strcmp(token, "while") == 0||strcmp(token, "for") == 0||strcmp(token, "do") == 0||
			strcmp(token, "read") == 0||strcmp(token, "write") == 0||strcmp(token, "call") == 0||strcmp(token, "{") == 0||strcmp(token, "ID") == 0 ||
					strcmp(token, "NUM") == 0 || strcmp(token, "(") == 0||strcmp(token,";")==0)){
		es = 14;
		return(es);
   	}
   	else{
 		fseek(fpTokenin,back,0);
   	 	strcpy(token,token2);
		strcpy(token1,token3);
		fscanf(fpTokenin, "%s %s\n", &token, &token1);
		statNode = tempNode;
    	es = statement(statNode);
   		if(es>0) return(es);
	}
    if(es>0) return(es); 
////������ 
    if (strcmp(token,"else")==0) {
    	statNode = createNode(token1);
		addChild(subroot, statNode);
//�����ж�else�󲻸�һ�����ʱ���� 
		back=ftell(fpTokenin);
		char token2[20], token3[40];
		strcpy(token2,token);
		strcpy(token3,token1);
    	fscanf(fpTokenin, "%s %s\n", &token, &token1);
		if(!(strcmp(token, "if") == 0||strcmp(token, "while") == 0||strcmp(token, "for") == 0||strcmp(token, "do") == 0||
				strcmp(token, "read") == 0||strcmp(token, "write") == 0||strcmp(token, "call") == 0||strcmp(token, "{") == 0||strcmp(token, "ID") == 0 ||
						strcmp(token, "NUM") == 0 || strcmp(token, "(") == 0||strcmp(token,";")==0)){
			es = 14;
			return(es);
   		}
   		else{
 			fseek(fpTokenin,back,0);
   	 		strcpy(token,token2);
			strcpy(token1,token3);
			fscanf(fpTokenin, "%s %s\n", &token, &token1);
    		es = statement(statNode);
   			if(es>0) return(es);
		}
    	if(es>0) return(es); 
	}
   	else return(es);
}

//<while_stat>�� while '('<expr >')' < statement >
int while_stat(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{
	statNode = createNode(token1);
	addChild(subroot, statNode);
	TreeNode *tempNode=statNode;
    int es = 0; 
	fscanf(fpTokenin, "%s %s\n", token, token1);
    if (strcmp(token,"(")){
        es=5;
        return es;
    }
    fscanf(fpTokenin, "%s %s\n", token, token1);
    if(strcmp(token,"(")==0||strcmp(token,"ID")==0||strcmp(token,"NUM")==0){
    	es=expression(statNode); 
    	if(es>0) return(es);
	}
	else{
		es=41;
		return(es); 
	}
    if (strcmp(token,")")) {
        es=6;
        return es;
    }
    /////////////////////////////////
    //�����ж�while(expression)�󲻸�һ�����ʱ���� 
	back=ftell(fpTokenin);
	char token2[20], token3[40];
	strcpy(token2,token);
	strcpy(token3,token1);
    fscanf(fpTokenin, "%s %s\n", &token, &token1);
	if(!(strcmp(token, "if") == 0||strcmp(token, "while") == 0||strcmp(token, "for") == 0||strcmp(token, "do") == 0||
			strcmp(token, "read") == 0||strcmp(token, "write") == 0||strcmp(token, "call") == 0||strcmp(token, "{") == 0||strcmp(token, "ID") == 0 ||
					strcmp(token, "NUM") == 0 || strcmp(token, "(") == 0||strcmp(token,";")==0)){
		es = 14;
		return(es);
   	}
   	else{
 		fseek(fpTokenin,back,0);
   	 	strcpy(token,token2);
		strcpy(token1,token3);
		fscanf(fpTokenin, "%s %s\n", &token, &token1);
		statNode=tempNode;
    	es = statement(statNode);
   		if(es>0) return(es);
	}
    if(es>0) return(es); 
    return(es);
}

//<for_stat>�� for'('<expr>;<expr>;<expr>')'<statement>
int for_stat(TreeNode* subroot){//it needs to come in with new two-tuple , and leave with tuple
	statNode = createNode(token1);
	addChild(subroot, statNode);
	TreeNode *tempNode=statNode;
	int es = 0; 
	fscanf(fpTokenin, "%s %s\n", token, token1);
    if (strcmp(token,"(")){
        es=5;
        return es;
    }
    fscanf(fpTokenin, "%s %s\n", token, token1);
    statNode=tempNode;
    es = expression(statNode);
    if (strcmp(token,";")){
        es=4;
        return es;
    }
    fscanf(fpTokenin, "%s %s\n", token, token1);
	statNode=tempNode;
    es = expression(statNode);
    if (strcmp(token,";")){
        es=4;
        return es;
    }
    fscanf(fpTokenin, "%s %s\n", token, token1);
    statNode=tempNode;
    es = expression(statNode);
    if (strcmp(token,")")){
        es=6;
        return es;
    }
    //�����ж�for(;;)�󲻸�һ�����ʱ���� 
	back=ftell(fpTokenin);
	char token2[20], token3[40];
	strcpy(token2,token);
	strcpy(token3,token1);
    fscanf(fpTokenin, "%s %s\n", &token, &token1);
	if(!(strcmp(token, "if") == 0||strcmp(token, "while") == 0||strcmp(token, "for") == 0||strcmp(token, "do") == 0||
			strcmp(token, "read") == 0||strcmp(token, "write") == 0||strcmp(token, "call") == 0||strcmp(token, "{") == 0||strcmp(token, "ID") == 0 ||
					strcmp(token, "NUM") == 0 || strcmp(token, "(") == 0||strcmp(token,";")==0)){
		es = 14;
		return(es);
   	}
   	else{
 		fseek(fpTokenin,back,0);
   	 	strcpy(token,token2);
		strcpy(token1,token3);
		fscanf(fpTokenin, "%s %s\n", &token, &token1);
		statNode=tempNode;
    	es = statement(statNode);
   		if(es>0) return(es);
	}
    if(es>0) return(es); 
////������ 
//    fscanf(fpTokenin, "%s %s\n", token, token1);
    es=statement(subroot);
    return(es);
}

//<do_while_stat>��do<statement>while'('<expression>')';
int do_while_stat(TreeNode* subroot)
{
	statNode = createNode(token1);
	addChild(subroot, statNode);
	TreeNode *tempNode=statNode;
	int es=0;
	back=ftell(fpTokenin);
	char token2[20], token3[40];
	strcpy(token2,token);
	strcpy(token3,token1);
    fscanf(fpTokenin, "%s %s\n", &token, &token1);
	if(!(strcmp(token, "if") == 0||strcmp(token, "while") == 0||strcmp(token, "for") == 0||strcmp(token, "do") == 0||
			strcmp(token, "read") == 0||strcmp(token, "write") == 0||strcmp(token, "call") == 0||strcmp(token, "{") == 0||strcmp(token, "ID") == 0 ||
					strcmp(token, "NUM") == 0 || strcmp(token, "(") == 0||strcmp(token,";")==0)){
		es = 14;
		return(es);
   	}
   	else{
 		fseek(fpTokenin,back,0);
   	 	strcpy(token,token2);
		strcpy(token1,token3);
		fscanf(fpTokenin, "%s %s\n", &token, &token1);
    	es = statement(statNode);
    	if(es==12) es=15;
   		if(es>0) return(es);
	}
    if(strcmp(token, "while")){
    	es=15;
    	return(es);
	}
	statNode = createNode(token1);
	addChild(subroot, statNode);
	fscanf(fpTokenin, "%s %s\n", &token, &token1);
    if (strcmp(token,"(")){
        es=5;
        return es;
    }
    fscanf(fpTokenin, "%s %s\n", token, token1);
    if(strcmp(token,"(")==0||strcmp(token,"ID")==0||strcmp(token,"NUM")==0){
    	es=expression(statNode); 
    	if(es>0) return(es);
	}
	else{
		es=41;
		return(es); 
	}
    if (strcmp(token,")")) {
        es=6;
        return es;
    }
    fscanf(fpTokenin, "%s %s\n", token, token1);
    if (strcmp(token,";")) {
        es=4;
        return es;
    }
    fscanf(fpTokenin, "%s %s\n", token, token1);
	return(es);
}


//<write_stat>��write <expression>;
int write_stat(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{
	statNode = createNode(token1);
	addChild(subroot, statNode);
    int es = 0;

    fscanf(fpTokenin, "%s %s\n", &token, &token1);
    es = expression(statNode);
    if(es > 0)return(es);
    if(strcmp(token, ";"))  return(es = 4); //�ٷֺ�
//    strcpy(codes[codesIndex++].opt, "OUT");
    fscanf(fpTokenin, "%s %s\n", &token, &token1);
    return(es);
}

//<read_stat>��read ID;
int read_stat(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{
	statNode = createNode(token1);
	addChild(subroot, statNode);
	int es = 0; 
	fscanf(fpTokenin, "%s %s\n", token, token1);
    if (strcmp(token,"ID")){
        es=3;
        return es;
    }
	TreeNode *IDNode = createNode(token1);
	addChild(statNode, IDNode);
	//�ж�ID�Ƿ�Ϊ���� 
	int IDposition; 
	es=lookup(token1, &IDposition,variable);
	if(es>0) return(es);
	//////////////////
//    es=lookup(token1,&symbolPos);
//	if(es>0)return(es);
//	if(!symbol[symbolPos].kind==variable) return(es=35);
//	strcpy(codes[codesIndex++].opt,"IN");
//	strcpy(codes[codesIndex].opt,"STO");
//	codes[codesIndex++].operand=symbol[symbolPos].adress;
    fscanf(fpTokenin, "%s %s\n", token, token1);
    if (strcmp(token,";")){
        es=4;
        return es;
    }
    fscanf(fpTokenin, "%s %s\n", token, token1);
    return(es);
}

//<compound_stat>��'{'<statement_list>'}'
int compound_stat(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{
	statNode = createNode("compound");
	addChild(subroot, statNode);
    int es = 0;
	if(fscanf(fpTokenin, "%s %s\n", &token, &token1)<2||strcmp(token1,"main")==0||strcmp(token,"function")==0){
		es=14;
		return(es);
	}
    es = statement_list(statNode);
    if(strcmp(token1,"}")){
    	es=16;
		return(es);
	}
//    fscanf(fpTokenin,"%s %s\n",&token,&token1);if while for do read write { call ID NUM ( ;  strcmp(token1,"main")==0||strcmp(token,"function")==0
		if(fscanf(fpTokenin, "%s %s\n", &token, &token1)<2||!(strcmp(token1,"if")==0&&strcmp(token1,"while")==0&&
				strcmp(token1,"for")==0&&strcmp(token1,"do")==0&&strcmp(token1,"read")==0&&strcmp(token1,"write")==0&&
						strcmp(token1,"{")==0&&strcmp(token1,"call")==0&&strcmp(token1,"ID")==0&&strcmp(token1,"NUM")==0&&
								strcmp(token1,"(")==0)&&strcmp(token1,";")==0){
        	es=12;
        	return(es);
		}
		
    return(es);
}

//< call _stat>��call ID'(' ')';
int call_stat(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{
	statNode = createNode(token1);
	addChild(subroot, statNode);
    int es = 0, symbolPos;
    fscanf(fpTokenin,"%s %s\n",&token,&token1);
    if (strcmp(token,"ID")){
        es=3;
        return es;
    }
    TreeNode *IDNode = createNode(token1);
	addChild(statNode, IDNode);
	//�ж�ID�Ƿ�Ϊ���� 
	int IDposition; 
	es=lookup(token1, &IDposition,function); 
	if(es>0) return(es);
	//////////////////
    fscanf(fpTokenin,"%s %s\n",&token,&token1);
    if (strcmp(token,"(")){
        es=5;
        return es;
    }
    fscanf(fpTokenin,"%s %s\n",&token,&token1);
    if (strcmp(token,")")){
        es=6;
        return es;
    }
    fscanf(fpTokenin,"%s %s\n",&token,&token1);
    if (strcmp(token,";")){
        es=4;
        return es;
    }
    fscanf(fpTokenin,"%s %s\n",&token,&token1);
	return(es);
}

//<expression_stat>��<expression>;|;
int expression_stat(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{

    int es = 0;
    if(strcmp(token, ";") == 0){

        if(fscanf(fpTokenin, "%s %s\n", &token, &token1)<2||strcmp(token1,"main")==0||strcmp(token,"function")==0){
        	es=12;
        	return(es);
		}
		statNode = createNode("empty_statement");
		addChild(subroot, statNode);
        return(es);
        }

    es = expression(subroot);
    if(es > 0) return(es);
    if(strcmp(token, ";") == 0){
    	if(fscanf(fpTokenin, "%s %s\n", &token, &token1)<2||strcmp(token1,"main")==0||strcmp(token,"function")==0){
    		es=12;
    		return(es);
		}
        return(es);
        }
    else{
        es = 4;
        return(es);//�ٷֺ�
        }
}

//<expression>�� ID=<bool_expr>|<bool_expr> 
int expression(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{
	int es=0;
//�����ж�ID�����ĸ���ѡʽ 
    if(strcmp(token, "ID")==0){//��ID���ж�= 
    	//�ж�ID�Ƿ�Ϊ���� 
		int IDposition; 
		es=lookup(token1, &IDposition,variable);
		if(es>0) return(es);
		//////////////////
    	TreeNode* tempNode=createNode(token1);
		back=ftell(fpTokenin);
		char token2[20], token3[40];
		strcpy(token2,token);
		strcpy(token3,token1);
    	fscanf(fpTokenin, "%s %s\n", &token, &token1);
		if(strcmp(token, "=")==0){//��=�� 
			statNode = createNode("=");
			addChild(subroot, statNode);
			addChild(statNode, tempNode);
			fscanf(fpTokenin, "%s %s\n", &token, &token1);
			es = bool_expr(statNode);  
			if(es>0) return(es);
    	}
   	 	else{
   	 		fseek(fpTokenin,back,0);
   	 		strcpy(token,token2);
			strcpy(token1,token3);
    		es = bool_expr(subroot);
    		if(es>0) return(es);
		}
    }
    else{
    	es=bool_expr(subroot);
    	if(es>0) return(es);
	}
//������ 
    return(es);
}

 //<bool_expr>-><additive_expr> |< additive_expr >(>|<|>=|<=|==|!=)< additive_expr >
int bool_expr(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{
    int es = 0;
	
    es = additive_expr(subroot);
    if(es > 0) return(es);
    if(strcmp(token, ">") == 0 || strcmp(token, ">=") == 0
            || strcmp(token, "<") == 0 || strcmp(token, "<=") == 0
            || strcmp(token, "==") == 0 || strcmp(token, "!=") == 0)  {

        char token2[20];
        strcpy(token2, token); //���������
       	statNode = createNode(token1);
		addChild(subroot, statNode);
        fscanf(fpTokenin, "%s %s\n", &token, &token1);
        es = additive_expr(statNode);
        if(es > 0) return(es);
//        if(strcmp(token2, ">") == 0)
//                 strcpy(codes[codesIndex++].opt, "GT");//great 
//        if(strcmp(token2, ">=") == 0)
//                 strcpy(codes[codesIndex++].opt, "GE");//great equal
//        if(strcmp(token2, "<") == 0)
//                 strcpy(codes[codesIndex++].opt, "LES");//less
//        if(strcmp(token2, "<=") == 0)
//                 strcpy(codes[codesIndex++].opt, "LE");//less equal
//        if(strcmp(token2, "==") == 0)
//                 strcpy(codes[codesIndex++].opt, "EQ");//equal
//        if(strcmp(token2, "!=") == 0)
//               strcpy(codes[codesIndex++].opt, "NOTEQ");//not equal
        }
    return(es);
}

//<additive_expr>��<term>{(+|-)< term >}
int additive_expr(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{
    int es = 0;
    es = term(subroot);
    if(es > 0) return(es);
    while(strcmp(token, "+")==0||strcmp(token,"-")==0){//�����+������-����ʽΪ1��ִ��ѭ���� 
        statNode = createNode(token1);
		addChild(subroot, statNode);
		fscanf(fpTokenin, "%s %s\n", &token, &token1);
		es = term(statNode);
		if(es>0) return(es);
	}
	return(es);
}

//< term >��<factor>{(*| /)< factor >}
int term(TreeNode* subroot)//��Ҫ���¶�Ԫ����룬����¶�Ԫ�鵯�� 
{
    int es = 0;
	es = factor(subroot);
	if(es>0) return(es);
	while(strcmp(token, "*")==0||strcmp(token,"/")==0){//�����*������/����ʽΪ1��ִ��ѭ���� 
	    statNode = createNode(token1);
		addChild(subroot, statNode);
		fscanf(fpTokenin, "%s %s\n", &token, &token1);
		es = factor(statNode);
		if(es>0) return(es);
	}
	return(es);
}

//< factor >��'('<additive_expr>')'| ID|NUM
int factor(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{
    int es = 0;
    if(strcmp(token, "ID")==0){//�������ID���˳� 
    	statNode = createNode(token1);
		addChild(subroot, statNode);
		//�ж�ID�Ƿ�Ϊ�����������ظ� 
		int IDposition; 
		es=lookup(token1, &IDposition,variable);
		if(es>0) return(es);
		//////////////////
		fscanf(fpTokenin, "%s %s\n", &token, &token1);
		return(es); 
    }
    if(strcmp(token, "NUM")==0){//�������NUM���˳� 
        statNode = createNode(token1);
		addChild(subroot, statNode);
		fscanf(fpTokenin, "%s %s\n", &token, &token1);
		return(es); 
	}
	if(strcmp(token, "(")==0){//�����(��ִ��then,�������(�����
		fscanf(fpTokenin, "%s %s\n", &token, &token1);
		es = additive_expr(subroot);
		if(es>0) return(es);
	}
	else{
		es=40;
		return(es);
	}
	if(strcmp(token, ")")){//����)��es=6 
    	es=6;
        return es;
    }
    fscanf(fpTokenin, "%s %s\n", &token, &token1);
    return(es);
}

//ɨ�赽��ʶ���Ķ����Գ���ʱ�����������ű�
int insert_Symbol(enum Category_symbol category, char *name)
{
    int i, es = 0;

    if(symbolIndex >= MAX_SYMBOL_NUM) return(21);//���ű��� 
    switch (category) {
            case function:
                for(i = symbolIndex - 1; i >= 0; i--) {//����ű�
                    if((strcmp(symbol[i].name, name) == 0)&&(symbol[i].kind==function)){//��ID��������ID���Ѷ���ĺ�����Ӧ������ 
                        es = 32; //32��ʾ�������ظ�
                        break;
                        }
                    else if(strcmp(symbol[i].name,name)==0&&symbol[i].kind==variable){
						es=26;
						return(es);
						}
                    }
                symbol[symbolIndex].kind=function;
                break;
            case variable:
                for(i = symbolIndex - 1; i >= 0; i--) {//����ű�
                    if((strcmp(symbol[i].name, name) == 0)&&(symbol[i].kind==variable)){
                        es = 22; //22��ʾ�����ظ�����
                        break;
                        }
                    else if(strcmp(symbol[i].name,name)==0&&symbol[i].kind==function){
						es=26;
						return(es);
						}
                    }
                symbol[symbolIndex].kind=variable;
                symbol[symbolIndex].address = offset;
                offset++;//����һ����Ԫ��������ָ��� 
                break;
            }
    if(es > 0)   return(es);
    strcpy(symbol[symbolIndex].name, name);
    symbolIndex++;
    return es;
    }

//ȡ�ñ�ʶ���ڷ��ű��е�λ��
int lookup(char *name, int *pPosition,enum Category_symbol category)
{
	int es=0;
	for(int i=0;i<MAX_SYMBOL_NUM;i++){
		if(symbol[i].kind==category&&strcmp(symbol[i].name,name)==0){
			*pPosition = i;
			return(es);
		}
	}
	if(category==function){
		es=25;
		return(es);
	}
	if(category==variable){
		es=23;
		return(es);
	}
    return(es);
}

int main(){
	TESTscan(); 
	TESTparse();
	drawAST(root,level);
	return 1;
} 
