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
//	printf("请输入源程序输入文件名（包括路径）:");
//	scanf("%s",Scanin);
	strcpy(Scanin,"source.txt");
//    printf("请输入源程序输出文件名（包括路径）:");
//	scanf("%s",Scanout);
	strcpy(Scanout,"tuples.txt");
	if((fin=fopen(Scanin,"r"))==NULL)
	{
		printf("\n打开词法分析输入文件出错!\n");
		return(1);
	}
	if((fout=fopen(Scanout,"w"))==NULL)
	{
		printf("\n创建词法分析输出文件错误!\n");
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
				printf("%s%d%s\t%s\t%s\n","第",line,"行","非法标识符",token);
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
				printf("%s%d%s\t%s\t%s\n","第",line,"行","非法标识符",token);
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
			printf("%s%d%s\t%s\t%s\n","第",line,"行","非法字符",token);
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
//	printf("请输入AST文件名（包括路径）：");
//	scanf("%s", ASTtxt);
	strcpy(ASTtxt,"AST.txt");
	if((astfin=fopen(ASTtxt,"w"))==NULL)
	{
		printf("\n创建AST输出文件错误!\n");
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
char Codeout[30]; //输出文件名

FILE *fpTokenin;  //check tuples file with this pointer
//FILE *fpCodeBinary;   //中间代码二进制文件指针
FILE *fpCodeText; //中间代码文本文件指针
int back;//sometime we need to recall the last tuple with this variable

struct //symbol type
{
    char name[20];
    enum Category_symbol kind;//identifier type
    int address;
} symbol[MAX_SYMBOL_NUM];

typedef struct Code
{
    char opt[10];    //操作码
    int operand;     //操作数
} Code;
Code codes[200];    //存放中间代码的结构体数组

int codesIndex;  //codes数组中第一个空元素的下标，0序（下一条要生成的代码在codes中的位置）
int symbolIndex = 0; //symbol数组中第一个空元素的下标，0序（下一个要填入的标识符在符号表中的位置）
int offset;//局部变量在所定义函数内部的相对地址

//语义分析需要包括的分析项
//类型检查：操作合法性 call ID() read ID 表达式中 函数变量未声明 函数变量重复声明 定义两个main时 认为main函数后还有字符即语法错误 函数变量的名称相互重复 
//可拓展其他的数据类型及相关语义分析识别 

//语法、语义分析及代码生成程序
int TESTparse()
{
    codesIndex = 0;
    int i;
    int es = 0;
//    printf("请输入单词流文件名（包括路径）：");
//    scanf("%s", tokenfile);
	strcpy(tokenfile,"tuples.txt");
    if((fpTokenin = fopen(tokenfile, "r")) == NULL){
        printf("\n打开%s错误!\n",tokenfile );
        es = 10;
        return(es);
       }
    es = program();
    fclose(fpTokenin);

    printf("==语法、语义分析及代码生成程序结果==\n");
    switch(es){
        case 0: printf("语法、语义分析成功并抽象机汇编生成代码!\n"); break;
        case 10: printf("打开文件 %s失败!\n", tokenfile); break;
        case 1: printf("此处应该是主函数!\n"); break;
        case 2: printf("缺少}!\n"); break;
        case 3: printf("缺少标识符!\n"); break;
        case 4: printf("少分号!\n"); break;
        case 5: printf("缺少(!\n"); break;
        case 6: printf("缺少)!\n"); break;
        case 7: printf("缺少操作数!\n"); break;
    	case 8: printf("缺少等号!\n"); break;/////////////缺少=返回8 
    	case 9: printf("缺少*或/!\n"); break;/////////////< term >→<factor>{(*| /)< factor >}中缺少*或/ 
        case 11: printf("函数开头缺少{!\n"); break;
        case 12: printf("函数结束缺少}!\n"); break;
        case 13:printf("最后一个函数的名字应该是main!\n"); break;
        case 14: printf("缺少语句！"); break; 
        case 15: printf("do_while语句没有while!"); break;
        case 16: printf("复合语句缺少}!"); break;
        case 21: printf("符号表溢出!\n"); break;
        case 22: printf("变量%s重复定义!\n",token1); break;
        case 23: printf("变量未声明!\n"); break;
        case 24:printf("程序中main函数结束后，还有其它多余字符\n");break;
        case 25: printf("函数未声明!\n"); break;
        case 26: printf("函数和变量名重复定义!\n"); break;
        case 32: printf("函数名重复定义!\n"); break;
        case 34: printf("call语句后面的标识符%s不是变量名!\n",token1 ); break;
        case 35: printf("read语句后面的标识符不是变量名!\n"); break;
        case 36: printf("赋值语句的左值%s不是变量名!\n",token1); break;
        case 37: printf("因子对应的标识符不是变量名!\n"); break;
		case 40: printf("布尔表达式内容错误!\n"); break;/////////////////////////bool_expr的首个元素没填ID没填NUM也没填( 
		case 41: printf("缺少表达式!\n"); break;
        }

//    printf("请输入要生成的文本形式的中间代码文件的名字（包括路径）：");
//    scanf("%s", Codeout);
	strcpy(Codeout,"intermediate_code.txt");
    if((fpCodeText = fopen(Codeout, "w")) == NULL) {
        printf("\n创建%s错误!\n", Codeout);
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
//    printf("请输入要生成的二进制形式的中间代码文件的名字（结构体存储）:");
//    scanf("%s",Codeout);
//    if((fpCodeBinary = fopen(Codeout,"wb")) == NULL) {
//        printf("\n创建%s错误!\n", Codeout);
//        es = 10;
//        return(es);
//        }
//    fwrite(codes,sizeof(Code),codesIndex,fpCodeBinary);
//    fclose(fpCodeBinary);
    return(es);
}

//<program> →{ fun_declaration }<main_declaration>
//<fun_declaration> → function ID’(‘ ‘ )’< function_body>这里的'function'指代码就写'function'并识别成'function'
//<main_declaration>→  main’(‘ ‘ )’ < function_body>
int program()
{
    int es = 0, i;
    root = createNode("program");
//    strcpy(codes[codesIndex++].opt,"BR");//codes数组的第一条指令是无条件转移到main函数的入口，入口地址需要返填
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
	getc(fpTokenin);		//吞掉EOF后再检测 
    if(!feof(fpTokenin))    //judge whether there're still some chars after expected end
	   return (es=24);

    //输出符号表的内容
//    printf("     符号表\n");
//    printf(" 名字\t \t类型 \t地址\n");
//    for(i = 0; i < symbolIndex; i++)
//        printf("  %-8s \t%d \t%d\n", symbol[i].name, symbol[i].kind,symbol[i].address);
    return(es);
}

//<fun_declaration> → function ID’(‘ ‘ )’< function_body>
int fun_declaration()//it needs to come in with new two-tuple , and leave without tuple
{
    int es=0;
    if (strcmp(token,"ID")){//token不是ID，运行if 
            es=3;
            return es;
        }
    node2 = createNode(token1);
	addChild(node1, node2);
    es=insert_Symbol(function,token1);//是ID，所以要将函数名插入符号表
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

//<main_declaration>→ main’(‘ ‘ )’ < function_body>
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

//    codes[0].operand=codesIndex;//返填codes数组的第0条指令的转移目标为main函数的函数体入口

    fscanf(fpTokenin, "%s %s\n", token, token1);
	node2 = createNode("function_body");
	addChild(node1, node2);
    es= function_body(node2);
    return es;
}

//<function_body>→'{'<declaration_list><statement_list>'}'
int function_body(TreeNode* subroot)//it needs to come in with new two-tuple , and leave without tuple
{
    int es=0;
    if(strcmp(token,"{")){//判断是否'{'
        es=11;
        return(es);
        }
//    symbol[symbolIndex-1].address=codesIndex;//把函数体的入口地址填入函数名在符号表中的地址字段
    offset=2;//进入一个新的函数，变量的相对地址从2开始
    if(fscanf(fpTokenin,"%s %s\n",&token,&token1)<2){
    	es=12;
    	return(es);
	}

    es=declaration_list();
    if (es>0) return(es);
//    strcpy(codes[codesIndex].opt, "ENTER");//函数体的开始
//    codes[codesIndex].operand=offset;
//    codesIndex++;
    es=statement_list(subroot);
    if (es>0) return(es);
    if(strcmp(token,"}")){//判断是否'}'
		es=12;
        return(es);
        }
    fscanf(fpTokenin, "%s %s\n", &token, &token1);
//    strcpy(codes[codesIndex++].opt, "RETURN");//函数体的结束
    return es;
}

//<declaration_list>→{<declaration_stat>}
int declaration_list()//定义的函数内的语句，只能有定义变量的语句 
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

//<declaration_stat>→int ID||float ID;
int declaration_stat()
{
    int es = 0;
	TreeNode *tempNode=node2;
    fscanf(fpTokenin, "%s %s\n", &token, &token1);
    if(strcmp(token, "ID"))	return(es = 3); //不是标识符即没取到ID，则直接return退出 
	node2 = createNode(token1);
	addChild(node1, node2);
	node2=tempNode;
    es = insert_Symbol(variable,token1); //插入符号表
    if(es > 0) return(es);
    fscanf(fpTokenin, "%s %s\n", &token, &token1);
    if(strcmp(token, ";"))	return(es = 4);
    if(fscanf(fpTokenin,"%s %s\n",&token,&token1)<2){
    	es=12;
    	return(es);
	}
    return(es);
}

//<statement_list>→{<statement>}
int statement_list(TreeNode* subroot)///it needs to come in with new two-tuple , and leave without tuple
{					//带着function或main弹出的 
    int es = 0;
//	!(strcmp(token, "if") == 0||strcmp(token, "while") == 0||strcmp(token, "for") == 0||strcmp(token, "read") == 0||
//			strcmp(token, "write") == 0||strcmp(token, "{") == 0||strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0 || 
//					strcmp(token, "(") == 0||strcmp(token,";")==0)
    while(strcmp(token, "if") == 0||strcmp(token, "while") == 0||strcmp(token, "for") == 0||strcmp(token, "do") == 0||
			strcmp(token, "read") == 0||strcmp(token, "call") == 0||strcmp(token, "write") == 0||strcmp(token, "{") == 0||strcmp(token, "ID") == 0 ||
					strcmp(token, "NUM") == 0 || strcmp(token, "(") == 0||strcmp(token,";")==0)  {//读到statement的first就进入循环体 
        es = statement(subroot);
        if(es > 0) return(es);
        if(strcmp(token, "function")==0||strcmp(token1, "main")==0) return(es);
    }
//    fscanf(fpTokenin, "%s %s\n", &token, &token1);
    return(es);
}

//<statement>→ <if_stat>|<while_stat>|<for_stat>
//               |<compound_stat> |<expression_stat>| <call _stat>
int statement(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{
    int es = 0;
    if(es == 0 && strcmp(token, "if") == 0) es = if_stat(subroot); //<if语句>
    else if(es == 0 && strcmp(token, "while") == 0) es = while_stat(subroot); //<while语句>
    else if(es == 0 && strcmp(token, "for") == 0) es = for_stat(subroot); //<for语句>
    else if(es == 0 && strcmp(token, "do") == 0) es = do_while_stat(subroot); //<do_while语句>
    else if(es == 0 && strcmp(token, "read") == 0) es = read_stat(subroot); //<read语句>
    else if(es == 0 && strcmp(token, "write") == 0) es = write_stat(subroot); //<write语句>
    else if(es == 0 && strcmp(token, "{") == 0) es = compound_stat(subroot); //<复合语句>
    else if(es == 0 && strcmp(token, "call") == 0) es = call_stat(subroot);//<函数调用语句> 
    else if(es == 0 && (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0 || strcmp(token, "(") == 0||strcmp(token,";")==0)) es = expression_stat(subroot); //<表达式语句>
	else if(strcmp(token, "}") == 0) return(es);
	else if(es==0&&(strcmp(token, "function") == 0||strcmp(token1, "main") == 0)){
		es=14;
		return(es);
	}//能够解决函数内部嵌套定义函数问题 和函数没有}时 紧接着下一个function或main此时上一个函数缺少部分语句（上一个函数没结束） 
	return(es);
}

//<if_stat>→ if '('<expr>')' <statement > [else < statement >]
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
//回溯判断if(expression)后不跟一条语句时报错 
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
////回溯完 
    if (strcmp(token,"else")==0) {
    	statNode = createNode(token1);
		addChild(subroot, statNode);
//回溯判断else后不跟一条语句时报错 
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

//<while_stat>→ while '('<expr >')' < statement >
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
    //回溯判断while(expression)后不跟一条语句时报错 
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

//<for_stat>→ for'('<expr>;<expr>;<expr>')'<statement>
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
    //回溯判断for(;;)后不跟一条语句时报错 
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
////回溯完 
//    fscanf(fpTokenin, "%s %s\n", token, token1);
    es=statement(subroot);
    return(es);
}

//<do_while_stat>→do<statement>while'('<expression>')';
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


//<write_stat>→write <expression>;
int write_stat(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{
	statNode = createNode(token1);
	addChild(subroot, statNode);
    int es = 0;

    fscanf(fpTokenin, "%s %s\n", &token, &token1);
    es = expression(statNode);
    if(es > 0)return(es);
    if(strcmp(token, ";"))  return(es = 4); //少分号
//    strcpy(codes[codesIndex++].opt, "OUT");
    fscanf(fpTokenin, "%s %s\n", &token, &token1);
    return(es);
}

//<read_stat>→read ID;
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
	//判断ID是否为变量 
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

//<compound_stat>→'{'<statement_list>'}'
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

//< call _stat>→call ID'(' ')';
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
	//判断ID是否为函数 
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

//<expression_stat>→<expression>;|;
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
        return(es);//少分号
        }
}

//<expression>→ ID=<bool_expr>|<bool_expr> 
int expression(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{
	int es=0;
//回溯判断ID进入哪个候选式 
    if(strcmp(token, "ID")==0){//是ID则判断= 
    	//判断ID是否为变量 
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
		if(strcmp(token, "=")==0){//是=则 
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
//回溯完 
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
        strcpy(token2, token); //保存运算符
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

//<additive_expr>→<term>{(+|-)< term >}
int additive_expr(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{
    int es = 0;
    es = term(subroot);
    if(es > 0) return(es);
    while(strcmp(token, "+")==0||strcmp(token,"-")==0){//如果是+或者是-则表达式为1，执行循环体 
        statNode = createNode(token1);
		addChild(subroot, statNode);
		fscanf(fpTokenin, "%s %s\n", &token, &token1);
		es = term(statNode);
		if(es>0) return(es);
	}
	return(es);
}

//< term >→<factor>{(*| /)< factor >}
int term(TreeNode* subroot)//需要带新二元组进入，会带新二元组弹出 
{
    int es = 0;
	es = factor(subroot);
	if(es>0) return(es);
	while(strcmp(token, "*")==0||strcmp(token,"/")==0){//如果是*或者是/则表达式为1，执行循环体 
	    statNode = createNode(token1);
		addChild(subroot, statNode);
		fscanf(fpTokenin, "%s %s\n", &token, &token1);
		es = factor(statNode);
		if(es>0) return(es);
	}
	return(es);
}

//< factor >→'('<additive_expr>')'| ID|NUM
int factor(TreeNode* subroot)//it needs to come in with new two-tuple , and leave with tuple
{
    int es = 0;
    if(strcmp(token, "ID")==0){//如果读到ID则退出 
    	statNode = createNode(token1);
		addChild(subroot, statNode);
		//判断ID是否为变量或名称重复 
		int IDposition; 
		es=lookup(token1, &IDposition,variable);
		if(es>0) return(es);
		//////////////////
		fscanf(fpTokenin, "%s %s\n", &token, &token1);
		return(es); 
    }
    if(strcmp(token, "NUM")==0){//如果读到NUM则退出 
        statNode = createNode(token1);
		addChild(subroot, statNode);
		fscanf(fpTokenin, "%s %s\n", &token, &token1);
		return(es); 
	}
	if(strcmp(token, "(")==0){//如果是(则执行then,如果不是(则出错
		fscanf(fpTokenin, "%s %s\n", &token, &token1);
		es = additive_expr(subroot);
		if(es>0) return(es);
	}
	else{
		es=40;
		return(es);
	}
	if(strcmp(token, ")")){//不是)则es=6 
    	es=6;
        return es;
    }
    fscanf(fpTokenin, "%s %s\n", &token, &token1);
    return(es);
}

//扫描到标识符的定义性出现时，将其插入符号表
int insert_Symbol(enum Category_symbol category, char *name)
{
    int i, es = 0;

    if(symbolIndex >= MAX_SYMBOL_NUM) return(21);//符号表满 
    switch (category) {
            case function:
                for(i = symbolIndex - 1; i >= 0; i--) {//查符号表
                    if((strcmp(symbol[i].name, name) == 0)&&(symbol[i].kind==function)){//是ID，但可能ID是已定义的函数，应当报错 
                        es = 32; //32表示函数名重复
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
                for(i = symbolIndex - 1; i >= 0; i--) {//查符号表
                    if((strcmp(symbol[i].name, name) == 0)&&(symbol[i].kind==variable)){
                        es = 22; //22表示变量重复定义
                        break;
                        }
                    else if(strcmp(symbol[i].name,name)==0&&symbol[i].kind==function){
						es=26;
						return(es);
						}
                    }
                symbol[symbolIndex].kind=variable;
                symbol[symbolIndex].address = offset;
                offset++;//分配一个单元，数据区指针加 
                break;
            }
    if(es > 0)   return(es);
    strcpy(symbol[symbolIndex].name, name);
    symbolIndex++;
    return es;
    }

//取得标识符在符号表中的位置
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
