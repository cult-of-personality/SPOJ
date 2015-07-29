/*  Author(s): Shreyansh Gandhi (ID: 2012A7PS070P), Himanshu Sharma(ID: 2012C6PS545P). Team Number = 36, Group No = 2.
  1. Top-down parser
  2. Symbol Table(Incomplete, commented)
  3. Abstract Syntax Tree (more or less complete)*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define IS_ARRAY 1
#define IS_FUNCTION 2
#define IS_STRUCTURE 4
#define IS_INSIDESTRUCT 8
#define IS_ASSIGNED 16

struct stackOp{
  char *s;
  struct stackOp *next;
};

struct stNode{  //linked list based stack
  char *s;
  struct stNode *next;
};

struct symbNode{
    int isFlags;
    char name[15];
    //int blockNum;
    char type;
    struct symbNode *next;
};

struct funcNode{
    char name[15];
    char type;
    char hasArgs;   //include argument list ??
    struct symbNode **table;
    struct funcNode *next;
};

struct hashNode{
  char s[12];
  int index;
  struct hashNode *next;
};

struct parseNode{//node of a parse tree: n-ary tree
  char *s;      //points to a string like NT_PROG, or TMAIN, TWHILE.
  struct parseNode* children[10]; //list of children, if any.
  short int childCount;
  struct parseNode* parent;
  char *idName;
};

typedef struct stackOp stOp;
stOp* PT[60][72];
struct stNode *stack = NULL;
struct hashNode *termLookup[100];
struct hashNode *nonTermLookup[100];
char buf[20];
int taken;
struct stackOp* empty;
int scope,decFlag,argsFlag,newStructFlag,structNumber,funcNumber,structIdx,isArray,blockNumber,structName;  //for symbol table.
char *type,*structs,*args,*decs,*funcs,*lBrace,*rBrace,*lPar,*rPar,*semi,*Char,*Bool,*Int,*Id,*comma,*Void,*returnst;
char funcname[15];
struct parseNode *idPtr = NULL;
struct funcNode *globe = NULL;

void pop()
{
    struct stNode *node;
    node = stack;
    stack = stack->next;
    free(node);
}

void push(char *str)
{
    struct stNode *newNode;
    newNode = (struct stNode*)malloc(sizeof(struct stNode));
    newNode->s = str;
    newNode->next = stack;
    stack = newNode;
}

void insert(struct hashNode* tbl[], char str[], int index)
{
	int i, len, val;
	len = strlen(str);
	val = 0;
	for(i=0; i<len; i++)
		{val += (int)str[i];}
	struct hashNode *newnode;
	newnode = malloc(sizeof(struct hashNode));
	strcpy(newnode->s, str);
	newnode->index = index;
	newnode->next = tbl[val%100];
	tbl[val%100] = newnode;
}

struct hashNode* search(struct hashNode* tbl[], char str[])
{
	int i, len, val;
	len = strlen(str);
	val = 0;
	for(i=0; i<len; i++)
		{val += (int)str[i];}
	struct hashNode *node = tbl[val%100];
	while(node){
		if (strcmp(node->s, str)==0)
			return node;
		node = node->next;
	}
	return NULL;
}

void deleteHashTables(struct hashNode* tbl[])
{
    //printf("Deleting Hash Table\n");
	int i;
	struct hashNode *temp, *cur;
	for (i=0; i<100; i++)
	{
		cur = tbl[i];
		while(cur){
			temp = cur->next;
			free(cur);
			cur = temp;
		}
	}
	printf("Hash Table Deleted\n");
}
void buildHashTables()
{
    //printf("Building Hash Tables\n");
	insert(termLookup, "TK_CREATE\0", 0);
	insert(termLookup, "TK_ID\0", 1);
	insert(termLookup, "TK_VOID\0", 2);
	insert(termLookup, "TK_FUNC\0", 3);
	insert(termLookup, "TK_MAIN\0", 4);
	insert(termLookup, "TK_TRY\0", 5);
	insert(termLookup, "TK_CATCH\0", 6);
	insert(termLookup, "TK_FINALLY\0", 7);
	insert(termLookup, "TK_CALL\0", 8);
	insert(termLookup, "TK_RETURN\0", 9);
	insert(termLookup, "TK_END\0", 10);
	insert(termLookup, "TK_NEXT\0", 11);
	insert(termLookup, "TK_IFSTAT\0", 12);
	insert(termLookup, "TK_ELSE\0", 13);
	insert(termLookup, "TK_LOOP\0", 14);
	insert(termLookup, "TK_INSTAT\0", 15);
	insert(termLookup, "TK_OUTSTAT\0", 16);
	insert(termLookup, "TK_ARRAY\0", 17);
	insert(termLookup, "TK_LVAL\0", 18);
	insert(termLookup, "TK_DOT\0", 19);
	insert(termLookup, "TK_INC\0", 20);
	insert(termLookup, "TK_DEC\0", 21);
	insert(termLookup, "TK_TRUE\0", 22);
	insert(termLookup, "TK_FALSE\0", 23);
	insert(termLookup, "TK_INT\0", 24);
	insert(termLookup, "TK_CHAR\0", 25);
	insert(termLookup, "TK_BOOL\0", 26);
	insert(termLookup, "TK_INTLIT\0", 27);
	insert(termLookup, "TK_CHARLIT\0", 28);
	insert(termLookup, "TK_LBRACE\0", 30);
	insert(termLookup, "TK_RBRACE\0", 31);
	insert(termLookup, "TK_LPAR\0", 32);
	insert(termLookup, "TK_RPAR\0", 33);
	insert(termLookup, "TK_LSQ\0", 34);
	insert(termLookup, "TK_RSQ\0", 35);
	insert(termLookup, "TK_COMMA\0", 36);
	insert(termLookup, "TK_COLON\0", 37);
	insert(termLookup, "TK_SEMI\0", 38);
	insert(termLookup, "TK_ASSIGNOP\0", 39);
	insert(termLookup, "TK_LOGOR\0", 40);
	insert(termLookup, "TK_LOGAND\0", 41);
	insert(termLookup, "TK_EQUALOP\0", 42);
	insert(termLookup, "TK_NOTEQ\0", 43);
	insert(termLookup, "TK_GREAT\0", 44);
	insert(termLookup, "TK_LESS\0", 45);
	insert(termLookup, "TK_LESSEQ\0", 46);
	insert(termLookup, "TK_GREATEQ\0", 47);
	insert(termLookup, "TK_MINUS\0", 48);
	insert(termLookup, "TK_PLUS\0", 49);
	insert(termLookup, "TK_MUL\0", 50);
	insert(termLookup, "TK_DIV\0", 51);
	insert(termLookup, "TK_MOD\0", 52);
	insert(termLookup, "TK_BITAND\0", 53);
	insert(termLookup, "TK_BITOR\0", 54);
	insert(termLookup, "TK_NOT\0", 55);
	/*insert(termLookup, "TK_STRUCT1", 56);
	insert(termLookup, "TK_STRUCT2", 57);
	insert(termLookup, "TK_STRUCT3", 58);
	insert(termLookup, "TK_STRUCT4", 59);
	insert(termLookup, "TK_STRUCT5", 60);
	insert(termLookup, "TK_STRUCT6", 61);
	insert(termLookup, "TK_STRUCT7", 62);
	insert(termLookup, "TK_STRUCT8", 63);
	insert(termLookup, "TK_STRUCT9", 64);
	insert(termLookup, "TK_STRUCT10", 65);
	insert(termLookup, "TK_STRUCT11", 66);
	insert(termLookup, "TK_STRUCT12", 67);
	insert(termLookup, "TK_STRUCT13", 68);
	insert(termLookup, "TK_STRUCT14", 69);*/
	insert(termLookup, "TK_OUTOP\0", 70);
	insert(termLookup, "TK_INOP\0", 71);
	insert(nonTermLookup, "NT_PROG\0", 0);
	insert(nonTermLookup, "NT_STRUCTS\0", 1);
	insert(nonTermLookup, "NT_STRUCT\0", 2);
	insert(nonTermLookup, "NT_DECS\0", 3);
	insert(nonTermLookup, "NT_IDLIST\0", 4);
	insert(nonTermLookup, "NT_T\0", 5);
	insert(nonTermLookup, "NT_FUNCS\0", 6);
	insert(nonTermLookup, "NT_Y\0", 7);
	insert(nonTermLookup, "NT_FUNCSIGN\0", 8);
	insert(nonTermLookup, "NT_RETTYPE\0", 9);
	insert(nonTermLookup, "NT_ARGS\0", 10);
	insert(nonTermLookup, "NT_T2\0", 11);
	insert(nonTermLookup, "NT_FUNCNAME\0", 12);
	insert(nonTermLookup, "NT_MAIN\0", 13);
	insert(nonTermLookup, "NT_BLOCK\0", 14);
	insert(nonTermLookup, "NT_TRY\0", 15);
	insert(nonTermLookup, "NT_STATS\0", 16);
	insert(nonTermLookup, "NT_STAT\0", 17);
	insert(nonTermLookup, "NT_ASSIGNST\0", 18);
	insert(nonTermLookup, "NT_FUNCCALL\0", 19);
	insert(nonTermLookup, "NT_TOSEND\0", 20);
	insert(nonTermLookup, "NT_S\0", 21);
	insert(nonTermLookup, "NT_RETURNST\0", 22);
	insert(nonTermLookup, "NT_TORET\0", 23);
	insert(nonTermLookup, "NT_IFSTAT\0", 24);
	insert(nonTermLookup, "NT_ELSEST\0", 25);
	insert(nonTermLookup, "NT_ITERST\0", 26);
	insert(nonTermLookup, "NT_INSTAT\0", 27);
	insert(nonTermLookup, "NT_OUTSTAT\0", 28);
	insert(nonTermLookup, "NT_ARRINIT\0", 29);
	insert(nonTermLookup, "NT_ARPART\0", 30);
	insert(nonTermLookup, "NT_Z\0", 31);
	insert(nonTermLookup, "NT_ARRAYST\0", 32);
	insert(nonTermLookup, "NT_X\0", 33);
	insert(nonTermLookup, "NT_EXP\0", 34);
	insert(nonTermLookup, "NT_CONDEXP\0", 35);
	insert(nonTermLookup, "NT_OREXP\0", 36);
	insert(nonTermLookup, "NT_F\0", 37);
	insert(nonTermLookup, "NT_ANDEXP\0", 38);
	insert(nonTermLookup, "NT_G\0", 39);
	insert(nonTermLookup, "NT_EQEXP\0", 40);
	insert(nonTermLookup, "NT_H\0", 41);
	insert(nonTermLookup, "NT_EQOP\0", 42);
	insert(nonTermLookup, "NT_RELEXP\0", 43);
	insert(nonTermLookup, "NT_J\0", 44);
	insert(nonTermLookup, "NT_RELOP\0", 45);
	insert(nonTermLookup, "NT_AREXP\0", 46);
	insert(nonTermLookup, "NT_ADDEXP\0", 47);
	insert(nonTermLookup, "NT_B\0", 48);
	insert(nonTermLookup, "NT_MULEXP\0", 49);
	insert(nonTermLookup, "NT_C\0", 50);
	insert(nonTermLookup, "NT_BITEXP\0", 51);
	insert(nonTermLookup, "NT_D\0", 52);
	insert(nonTermLookup, "NT_BITOP\0", 53);
	insert(nonTermLookup, "NT_UNEXP\0", 54);
	insert(nonTermLookup, "NT_K\0", 55);
	insert(nonTermLookup, "NT_NOTEXP\0", 56);
	insert(nonTermLookup, "NT_SIMPLE\0", 57);
	insert(nonTermLookup, "NT_LIT\0", 58);
	insert(nonTermLookup, "NT_TYPE\0", 59);
	insert(nonTermLookup, "NT_EMPTY\0",-1);
	printf("Hash Tables Built\n");
}

void deleteList(stOp* head)
{
    stOp* curr = head;
    stOp *next;
    while(curr){
        next = curr->next;
        free(curr);
        curr = next;
    }
}

void deleteParseTable()
{
    deleteList(PT[0][0]);
    deleteList(PT[1][0]);
    deleteList(PT[2][0]);
    deleteList(PT[3][17]);
    deleteList(PT[3][24]);
    deleteList(PT[1][3]);
    deleteList(PT[4][1]);
    deleteList(PT[5][36]);
    deleteList(PT[6][3]);
    deleteList(PT[7][5]);
    deleteList(PT[7][30]);
    deleteList(PT[8][32]);
    deleteList(PT[9][2]);
    deleteList(PT[9][24]);
    deleteList(PT[10][24]);
    deleteList(PT[10][2]);
    deleteList(PT[11][36]);
    deleteList(PT[12][1]);
    deleteList(PT[13][4]);
    deleteList(PT[14][30]);
    deleteList(PT[15][5]);
    deleteList(PT[16][1]);
    deleteList(PT[17][1]);
    deleteList(PT[17][8]);
    deleteList(PT[17][9]);
    deleteList(PT[17][10]);
    deleteList(PT[17][11]);
    deleteList(PT[17][12]);
    deleteList(PT[17][14]);
    deleteList(PT[17][15]);
    deleteList(PT[17][16]);
    deleteList(PT[17][17]);
    deleteList(PT[18][1]);
    deleteList(PT[19][8]);
    deleteList(PT[20][1]);
    deleteList(PT[20][22]);
    deleteList(PT[21][36]);
    deleteList(PT[22][9]);
    deleteList(PT[23][1]);
    deleteList(PT[23][22]);
    deleteList(PT[24][12]);
    deleteList(PT[25][13]);
    deleteList(PT[26][14]);
    deleteList(PT[27][15]);
    deleteList(PT[28][16]);
    deleteList(PT[29][17]);
    deleteList(PT[30][17]);
    deleteList(PT[31][34]);
    deleteList(PT[32][1]);
    deleteList(PT[33][19]);
    deleteList(PT[33][34]);
    deleteList(PT[34][22]);
    deleteList(PT[36][22]);
    deleteList(PT[37][40]);
    deleteList(PT[38][22]);
    deleteList(PT[39][41]);
    deleteList(PT[40][22]);
    deleteList(PT[41][42]);
    deleteList(PT[42][42]);
    deleteList(PT[42][43]);
    deleteList(PT[43][22]);
    deleteList(PT[44][44]);
    deleteList(PT[45][44]);
    deleteList(PT[45][45]);
    deleteList(PT[45][46]);
    deleteList(PT[45][47]);
    deleteList(PT[46][22]);
    deleteList(PT[47][22]);
    deleteList(PT[48][48]);
    deleteList(PT[48][49]);
    deleteList(PT[49][22]);
    deleteList(PT[50][50]);
    deleteList(PT[50][51]);
    deleteList(PT[50][52]);
    deleteList(PT[51][22]);
    deleteList(PT[52][53]);
    deleteList(PT[53][53]);
    deleteList(PT[53][54]);
    deleteList(PT[54][22]);
    deleteList(PT[55][20]);
    deleteList(PT[55][21]);
    deleteList(PT[56][1]);
    deleteList(PT[56][55]);
    deleteList(PT[57][1]);
    deleteList(PT[57][22]);
    deleteList(PT[57][32]);
    deleteList(PT[58][22]);
    deleteList(PT[58][23]);
    deleteList(PT[58][27]);
    deleteList(PT[58][28]);
    deleteList(PT[59][25]);
    deleteList(PT[59][26]);
    deleteList(PT[34][8]);
    printf("Parse Table Deleted\n");
}

stOp* buildHeadNode(int r,int c,char *str)
{
  stOp* ret = (stOp*)malloc(sizeof(stOp));
  struct hashNode *h;
  if(str[0]=='N')
    h = search(nonTermLookup,str);
  else h = search(termLookup,str);
  ret->s = h->s;
  PT[r][c] = ret;
  return ret;
}

void addToList(stOp **tail,char *str)
{
    stOp* node;
    struct hashNode *h;
    node = (stOp*)malloc(sizeof(struct stackOp));
    if(str[0]=='N'){
        h = search(nonTermLookup,str);
        node->s = h->s;
    }
    else if(str[0]=='T'){
        h = search(termLookup,str);
        node->s = h->s;
    }
    (*tail)->next = node;
    *tail = node;
}

void buildParseTable()
{
  printf("Building Parse Table.\n");
  int i,j;
  for(i=0;i<60;i++){
    for(j=0;j<72;j++)
      PT[i][j] = NULL;
  }
  // a for pop, b for push
  stOp *node,*tail;
  struct hashNode *h;
  int size = sizeof(stOp);
  
  tail = buildHeadNode(0,0,"NT_MAIN\0");
  addToList(&tail,"NT_FUNCS\0");
  addToList(&tail,"NT_DECS\0");
  addToList(&tail,"NT_STRUCTS\0");
  tail->next = NULL;
  
  PT[0][3] = PT[0][0];
  PT[0][4] = PT[0][0];
  
  tail = buildHeadNode(1,0,"NT_STRUCTS\0");
  addToList(&tail,"NT_STRUCT\0");
  tail->next = NULL;
  
  node = (stOp*)malloc(size);
  h = search(nonTermLookup,"NT_EMPTY\0");
  node->s = h->s;
  node->next = NULL;
  PT[1][3] = node;
  
  PT[1][4] = node;
  PT[1][24] = node;
  PT[1][25] = node;
  PT[1][26] = node;
  
  tail = buildHeadNode(2,0,"TK_SEMI\0");
  addToList(&tail,"TK_RBRACE\0");
  addToList(&tail,"NT_DECS\0");
  addToList(&tail,"TK_LBRACE\0");
  addToList(&tail,"TK_ID\0");
  addToList(&tail,"TK_CREATE\0");
  tail->next = NULL;
  
  tail = buildHeadNode(3,24,"NT_DECS\0");
  addToList(&tail,"TK_SEMI\0");
  addToList(&tail,"NT_IDLIST\0");
  addToList(&tail,"NT_TYPE\0");
  tail->next = NULL;
  PT[3][1] = PT[1][3];
  PT[3][3] = PT[1][3];
  PT[3][4] = PT[1][3];
  PT[3][31] = PT[1][3];
  PT[3][25] = PT[3][24];
  PT[3][26] = PT[3][24];
  for(i=56;i<70;i++) //for user-defined structures.
    PT[3][i] = PT[3][24];
  
  tail = buildHeadNode(3,17,"NT_ARRINIT\0");
  tail->next = NULL;
  
  tail = buildHeadNode(4,1,"NT_T\0");
  addToList(&tail,"TK_ID\0");
  tail->next = NULL;
  
  PT[5][38] = PT[1][3];
  //<T> -> ,<id-list>
  tail = buildHeadNode(5,36,"NT_IDLIST\0");
  addToList(&tail,"TK_COMMA\0");
  tail->next = NULL;
  
  tail = buildHeadNode(6,3,"NT_FUNCS\0");
  addToList(&tail,"NT_Y\0");
  addToList(&tail,"NT_FUNCSIGN\0");
  addToList(&tail,"TK_COLON\0");
  addToList(&tail,"NT_FUNCNAME\0");
  addToList(&tail,"NT_RETTYPE\0");
  addToList(&tail,"TK_FUNC\0");
  tail->next = NULL;
  
  PT[6][4] = PT[1][3];   //gives empty;
  
  node = buildHeadNode(7,5,"NT_TRY\0");
  tail->next = NULL;
  
  tail = buildHeadNode(7,30,"NT_BLOCK\0");
  tail->next = NULL;
  
  tail = buildHeadNode(8,32,"TK_RPAR\0");
  addToList(&tail,"NT_ARGS\0");
  addToList(&tail,"TK_LPAR\0");
  tail->next = NULL;
  
  tail = buildHeadNode(9,2,"TK_VOID\0");
  tail->next = NULL;

  tail = buildHeadNode(9,24,"NT_TYPE\0");
  tail->next = NULL;

  PT[9][25] = PT[9][24];
  PT[9][26] = PT[9][24];
  for(i=56;i<70;i++)
    PT[9][i] = PT[9][24];
  
  tail = buildHeadNode(10,24,"NT_T2\0");
  addToList(&tail,"TK_ID\0");
  addToList(&tail,"NT_TYPE\0");
  tail->next = NULL;

  PT[10][25] = PT[10][24];
  PT[10][26] = PT[10][24];
  for(i=56;i<70;i++)
    PT[10][i] = PT[10][24];
  
  tail = buildHeadNode(10,2,"NT_T2\0");
  addToList(&tail,"NT_VOID\0");
  tail->next = NULL;
  
  PT[11][33] = PT[1][3];
  
  tail = buildHeadNode(11,36,"NT_ARGS\0");
  addToList(&tail,"TK_COMMA\0");
  tail->next = NULL;
  
  tail = buildHeadNode(12,1,"TK_ID\0");
  tail->next = NULL;
  
  tail = buildHeadNode(13,4,"NT_BLOCK\0");
  addToList(&tail,"TK_MAIN\0");
  tail->next = NULL;
  
  tail = buildHeadNode(14,30,"TK_RBRACE\0");
  addToList(&tail,"NT_STATS\0");
  addToList(&tail,"TK_LBRACE\0");
  tail->next = NULL;
  
  tail = buildHeadNode(15,5,"NT_BLOCK\0");
  addToList(&tail,"TK_FINALLY\0");
  addToList(&tail,"NT_BLOCK\0");
  addToList(&tail,"TK_CATCH\0");
  addToList(&tail,"NT_BLOCK\0");
  addToList(&tail,"TK_TRY\0");
  tail->next = NULL;
  
  tail = buildHeadNode(16,1,"NT_STATS\0");
  addToList(&tail,"NT_STAT\0");
  tail->next = NULL;
  
  PT[16][8]=PT[16][1];
  PT[16][9]=PT[16][1];
  PT[16][10]=PT[16][1];
  PT[16][11]=PT[16][1];
  PT[16][12]=PT[16][1];
  PT[16][14]=PT[16][1];
  PT[16][15]=PT[16][1];
  PT[16][16]=PT[16][1];
  PT[16][17]=PT[16][1];
  PT[16][18]=PT[16][1];
  PT[16][24]=PT[16][1];
  PT[16][25]=PT[16][1];
  PT[16][26]=PT[16][1];
  PT[16][31] = PT[1][3];
  for(i=56;i<70;i++)
    PT[16][i] = PT[16][1];
  
  tail=buildHeadNode(17,1,"NT_ASSIGNST\0");
  tail->next = NULL;
  
  tail=buildHeadNode(17,8,"TK_SEMI\0");
  addToList(&tail,"NT_FUNCCALL\0");
  tail->next = NULL;
  
  tail=buildHeadNode(17,9,"NT_RETURNST\0");
  tail->next = NULL;
  
  tail=buildHeadNode(17,10,"TK_SEMI\0");
  addToList(&tail,"TK_END\0");
  tail->next = NULL;
  
  tail=buildHeadNode(17,11,"TK_SEMI\0");
  addToList(&tail,"TK_NEXT\0");
  tail->next = NULL;
  
  tail=buildHeadNode(17,12,"NT_IFSTAT\0");
  tail->next = NULL;
  
  tail=buildHeadNode(17,14,"NT_ITERST\0");
  tail->next = NULL;
  
  tail=buildHeadNode(17,15,"NT_INSTAT\0");
  tail->next = NULL;
  
  tail=buildHeadNode(17,16,"NT_OUTSTAT\0");
  tail->next = NULL;
  
  tail=buildHeadNode(17,17,"NT_DECS\0");
  tail->next = NULL;
  
  PT[17][18]=PT[17][1];
  PT[17][24]=PT[17][17];
  PT[17][25]=PT[17][17];
  PT[17][26]=PT[17][17];
  for(i=56;i<70;i++)
    PT[17][i]=PT[17][17];
  
  tail=buildHeadNode(18,1,"TK_SEMI\0");
  addToList(&tail,"NT_EXP\0");
  addToList(&tail,"TK_ASSIGNOP\0");
  addToList(&tail,"NT_ARRAYST\0");
  tail->next = NULL;
  
  PT[18][18]=PT[18][1];
  
  tail=buildHeadNode(19,8,"TK_RPAR\0");
  addToList(&tail,"NT_TOSEND\0");
  addToList(&tail,"TK_LPAR\0");
  addToList(&tail,"TK_ID\0");
  addToList(&tail,"TK_CALL\0");
  tail->next = NULL;
  
  tail=buildHeadNode(20,1,"NT_S\0");
  addToList(&tail,"NT_ARRAYST\0");
  tail->next = NULL;
  PT[20][18]=PT[20][1];
  
  tail=buildHeadNode(20,22,"NT_S\0");
  addToList(&tail,"NT_LIT\0");
  tail->next = NULL;
  PT[20][23] = PT[20][22];
  PT[20][27] = PT[20][22];
  PT[20][28] = PT[20][22];
  
  PT[21][33] = PT[1][3];
  tail=buildHeadNode(21,36,"NT_TOSEND\0");
  addToList(&tail,"TK_COMMA\0");
  tail->next=NULL;
  
  tail=buildHeadNode(22,9,"TK_SEMI\0");
  addToList(&tail,"NT_TORET\0");
  addToList(&tail,"TK_RETURN\0");
  tail->next = NULL;
  
  tail=buildHeadNode(23,1,"NT_ARRAYST\0");
  tail->next = NULL;
  PT[23][18] = PT[23][1];
  
  tail=buildHeadNode(23,22,"NT_LIT\0");
  tail->next = NULL;
  PT[23][23] = PT[23][22];
  PT[23][27] = PT[23][22];
  PT[23][28] = PT[23][22];
  
  tail=buildHeadNode(24,12,"NT_ELSEST\0");
  addToList(&tail,"TK_RBRACE\0");
  addToList(&tail,"NT_STATS\0");
  addToList(&tail,"TK_LBRACE\0");
  addToList(&tail,"TK_RPAR\0");
  addToList(&tail,"NT_CONDEXP\0");
  addToList(&tail,"TK_LPAR\0");
  addToList(&tail,"TK_IFSTAT\0");
  tail->next = NULL;
  
  tail=buildHeadNode(25,13,"TK_RBRACE\0");
  addToList(&tail,"NT_STATS\0");
  addToList(&tail,"TK_LBRACE\0");
  addToList(&tail,"TK_ELSE\0");
  tail->next = NULL;
  
  PT[25][1]=PT[1][3];
  PT[25][8]=PT[1][3];
  PT[25][9]=PT[1][3];
  PT[25][10]=PT[1][3];
  PT[25][11]=PT[1][3];
  PT[25][12]=PT[1][3];
  PT[25][14]=PT[1][3];
  PT[25][15]=PT[1][3];
  PT[25][16]=PT[1][3];
  PT[25][17]=PT[1][3];
  PT[25][18]=PT[1][3];
  PT[25][24]=PT[1][3];
  PT[25][25]=PT[1][3];
  PT[25][26]=PT[1][3];
  
  tail=buildHeadNode(26,14,"TK_RBRACE\0");
  addToList(&tail,"NT_STATS\0");
  addToList(&tail,"TK_LBRACE\0");
  addToList(&tail,"TK_RPAR\0");
  addToList(&tail,"NT_CONDEXP\0");
  addToList(&tail,"TK_LPAR\0");
  addToList(&tail,"TK_LOOP\0");
  tail->next=NULL;
  
  tail=buildHeadNode(27,15,"TK_SEMI\0");
  addToList(&tail,"TK_ID\0");
  addToList(&tail,"TK_INOP\0");
  addToList(&tail,"TK_INSTAT\0");
  tail->next=NULL;
  
  tail=buildHeadNode(28,16,"TK_SEMI\0");
  addToList(&tail,"TK_ID\0");
  addToList(&tail,"TK_OUTOP\0");
  addToList(&tail,"TK_OUTSTAT\0");
  tail->next=NULL;
  
  tail=buildHeadNode(29,17,"TK_SEMI\0");
  addToList(&tail,"TK_ID\0");
  addToList(&tail,"NT_ARPART\0");
  tail->next=NULL;
  
  tail=buildHeadNode(30,17,"NT_Z\0");
  addToList(&tail,"TK_RSQ\0");
  addToList(&tail,"NT_AREXP\0");
  addToList(&tail,"TK_LSQ\0");
  addToList(&tail,"TK_RPAR\0");
  addToList(&tail,"NT_TYPE\0");
  addToList(&tail,"TK_LPAR\0");
  addToList(&tail,"TK_ARRAY\0");
  tail->next=NULL;
  
  tail=buildHeadNode(31,34,"NT_Z\0");
  addToList(&tail,"TK_RSQ\0");
  addToList(&tail,"NT_AREXP\0");
  addToList(&tail,"NT_LSQ\0");
  tail->next=NULL;
  
  PT[31][1] = PT[1][3];
  
  tail=buildHeadNode(32,1,"NT_X\0");
  addToList(&tail,"TK_ID\0");
  tail->next=NULL;
  
  tail=buildHeadNode(33,34,"NT_X\0");
  addToList(&tail,"TK_RSQ\0");
  addToList(&tail,"NT_AREXP\0");
  addToList(&tail,"TK_LSQ\0");
  tail->next = NULL;
  
  tail=buildHeadNode(33,19,"TK_ID\0");
  addToList(&tail,"TK_DOT\0");
  tail->next = NULL;
  
  PT[33][20] = PT[1][3];
  PT[33][21] = PT[1][3];
  for(i=38;i<=54;i++)
    PT[33][i] = PT[1][3];
    
  tail = buildHeadNode(34,22,"NT_OREXP\0");
  tail->next = NULL;
  
  PT[34][23] = PT[34][22];
  PT[34][27] = PT[34][22];
  PT[34][28] = PT[34][22];
  PT[34][55] = PT[34][22];
  PT[35][22] = PT[34][22];
  PT[35][23] = PT[34][22];
  PT[35][27] = PT[34][22];
  PT[35][28] = PT[34][22];
  PT[35][55] = PT[34][22];
  
  tail=buildHeadNode(36,22,"NT_F\0");
  addToList(&tail,"NT_ANDEXP\0");
  tail->next = NULL;
  PT[36][23] = PT[36][22];
  PT[36][27] = PT[36][22];
  PT[36][28] = PT[36][22];
  PT[36][55] = PT[36][22];
  //PT[34][27] = PT[36][27];
  PT[37][38] = PT[1][3];
  tail=buildHeadNode(37,40,"NT_F\0");
  addToList(&tail,"NT_ANDEXP\0");
  addToList(&tail,"TK_LOGOR\0");
  tail->next = NULL;
  
  tail = buildHeadNode(38,22,"NT_G\0");
  addToList(&tail,"NT_EQEXP\0");
  tail->next = NULL;
  PT[38][23] = PT[38][22];
  PT[38][27] = PT[38][22];
  PT[38][28] = PT[38][22];
  PT[38][55] = PT[38][22];
  
  PT[39][33] = PT[1][3];
  PT[39][38] = PT[1][3];
  PT[39][40] = PT[1][3];
  tail=buildHeadNode(39,41,"NT_G\0");
  addToList(&tail,"NT_EQEXP\0");
  addToList(&tail,"TK_LOGAND\0");
  tail->next = NULL;
  
  tail=buildHeadNode(40,22,"NT_H\0");
  addToList(&tail,"NT_RELEXP\0");
  tail->next = NULL;
  PT[40][23] = PT[40][22];
  PT[40][27] = PT[40][22];
  PT[40][28] = PT[40][22];
  PT[40][55] = PT[40][22];
  
  PT[41][33] = PT[1][3];
  PT[41][38] = PT[1][3];
  PT[41][40] = PT[1][3];
  PT[41][41] = PT[1][3];
  
  tail = buildHeadNode(41,42,"NT_RELEXP\0");
  addToList(&tail,"TK_EQUALOP\0");
  tail->next = NULL;
  PT[41][43] = PT[41][42];
  
  tail = buildHeadNode(42,42,"TK_EQUALOP\0");
  tail->next = NULL;
  tail = buildHeadNode(42,43,"TK_NOTEQ\0");
  tail->next = NULL;
  
  tail = buildHeadNode(43,22,"NT_J\0");
  addToList(&tail,"NT_AREXP\0");
  tail->next = NULL;
  PT[43][23] = PT[43][22];
  PT[43][27] = PT[43][22];
  PT[43][28] = PT[43][22];
  PT[43][55] = PT[43][22];
  
  PT[44][33] = PT[1][3];
  PT[44][38] = PT[1][3];
  PT[44][40] = PT[1][3];
  PT[44][41] = PT[1][3];
  PT[44][42] = PT[1][3];
  PT[44][43] = PT[1][3];
  tail = buildHeadNode(44,44,"NT_AREXP\0");
  addToList(&tail,"NT_RELOP\0");
  tail->next = NULL;
  PT[44][45] = PT[44][44];
  PT[44][46] = PT[44][44];
  PT[44][47] = PT[44][44];
  
  tail = buildHeadNode(45,44,"TK_GREAT\0");
  tail->next = NULL;
  tail = buildHeadNode(45,45,"TK_LESS\0");
  tail->next = NULL;
  tail = buildHeadNode(45,46,"TK_LESSEQ\0");
  tail->next = NULL;
  tail = buildHeadNode(45,47,"TK_GREATEQ\0");
  tail->next = NULL;
  
  tail = buildHeadNode(46,22,"NT_ADDEXP\0");
  tail->next = NULL;
  PT[46][23] = PT[46][22];
  PT[46][27] = PT[46][22];
  PT[46][28] = PT[46][22];
  PT[46][55] = PT[46][22];
  
  tail = buildHeadNode(47,22,"NT_B\0");
  addToList(&tail,"NT_MULEXP\0");
  tail->next = NULL;
  PT[47][23] = PT[47][22];
  PT[47][27] = PT[47][22];
  PT[47][28] = PT[47][22];
  PT[47][55] = PT[47][22];
  
  tail = buildHeadNode(48,48,"NT_B\0");
  addToList(&tail,"NT_MULEXP\0");
  addToList(&tail,"TK_MINUS\0");
  tail->next = NULL;
  
  tail = buildHeadNode(48,49,"NT_B\0");
  addToList(&tail,"NT_MULEXP\0");
  addToList(&tail,"TK_PLUS\0\0");
  tail->next = NULL;
  
  PT[48][33] = PT[1][3];
  PT[48][38] = PT[1][3];
  for(i=40;i<=47;i++)
    PT[48][i] = PT[1][3];
  
  tail = buildHeadNode(49,22,"NT_C\0");
  addToList(&tail,"NT_BITEXP\0");
  tail->next = NULL;
  PT[49][23] = PT[49][22];
  PT[49][27] = PT[49][22];
  PT[49][28] = PT[49][22];
  PT[49][55] = PT[49][22];
  
  PT[50][33] = PT[1][3];
  PT[50][38] = PT[1][3];
  for(i=40;i<=49;i++)
    PT[50][i] = PT[1][3];
  
  tail = buildHeadNode(50,50,"NT_C\0");
  addToList(&tail,"NT_MULEXP\0");
  addToList(&tail,"TK_MUL\0");
  tail->next = NULL;
  
  tail = buildHeadNode(50,51,"NT_C\0");
  addToList(&tail,"NT_MULEXP\0");
  addToList(&tail,"TK_DIV\0");
  tail->next = NULL;
  
  tail = buildHeadNode(50,52,"NT_C\0");
  addToList(&tail,"NT_MULEXP\0");
  addToList(&tail,"TK_MOD\0");
  tail->next = NULL;
  
  tail = buildHeadNode(51,22,"NT_D\0");
  addToList(&tail,"NT_UNEXP\0");
  tail->next = NULL;
  PT[51][23] = PT[51][22];
  PT[51][27] = PT[51][22];
  PT[51][28] = PT[51][22];
  PT[51][55] = PT[51][22];
  
  PT[52][33] = PT[1][3];
  PT[52][38] = PT[1][3];
  for(i=40;i<=52;i++)
    PT[52][i] = PT[1][3];
  
  tail = buildHeadNode(52,53,"NT_D\0");
  addToList(&tail,"NT_BITEXP\0");
  addToList(&tail,"NT_BITOP\0");
  tail->next = NULL;
  
  tail = buildHeadNode(53,53,"TK_BITAND\0");
  tail->next = NULL;
  
  tail = buildHeadNode(53,54,"TK_BITOR\0");
  tail->next = NULL;
  
  tail = buildHeadNode(54,22,"NT_K\0");
  addToList(&tail,"NT_NOTEXP\0");
  tail->next = NULL;
  PT[54][23] = PT[54][22];
  PT[54][27] = PT[54][22];
  PT[54][28] = PT[54][22];
  PT[54][55] = PT[54][22];
  
  tail = buildHeadNode(55,20,"TK_INC\0");
  tail->next = NULL;
  
  tail = buildHeadNode(55,21,"TK_DEC\0");
  tail->next = NULL;
  PT[55][33] = PT[1][3];
  PT[55][38] = PT[1][3];
  for(i=40;i<=54;i++)
    PT[55][i] = PT[1][3];
  
  tail = buildHeadNode(56,1,"NT_SIMPLE\0");
  tail->next = NULL;
  PT[56][18] = PT[56][1];
  PT[56][22] = PT[56][1];
  PT[56][23] = PT[56][1];
  PT[56][27] = PT[56][1];
  PT[56][28] = PT[56][1];
  PT[56][32] = PT[56][1];
  
  tail = buildHeadNode(56,55,"TK_NOT\0");
  addToList(&tail,"NT_SIMPLE\0");
  tail->next = NULL;
  
  tail = buildHeadNode(57,1,"NT_ARRAYST\0");
  tail->next = NULL;
  tail = buildHeadNode(57,22,"NT_LIT\0");
  tail->next = NULL;
  PT[57][23] = PT[57][22];
  PT[57][27] = PT[57][22];
  PT[57][28] = PT[57][22];
  
  tail = buildHeadNode(57,32,"TK_RPAR\0");
  addToList(&tail,"NT_EXP\0");
  addToList(&tail,"TK_LPAR\0");
  tail->next = NULL;
  
  tail = buildHeadNode(58,22,"TK_TRUE\0");
  tail->next = NULL;
  tail = buildHeadNode(58,23,"TK_FALSE\0");
  tail->next = NULL;
  
  tail = buildHeadNode(58,27,"TK_INTLIT\0");
  tail->next = NULL;
  tail = buildHeadNode(58,28,"TK_CHARLIT\0");
  tail->next = NULL;
  
  tail = buildHeadNode(59,24,"TK_INT\0");
  tail->next = NULL;
  tail = buildHeadNode(59,25,"TK_CHAR\0");
  tail->next = NULL;
  tail = buildHeadNode(59,26,"TK_BOOL\0");
  tail->next = NULL;
  tail = buildHeadNode(34,8,"NT_FUNCCALL\0");
  tail->next = NULL;
  
  PT[36][1] = PT[36][22];
  PT[38][1] = PT[38][22];
  PT[40][1] = PT[40][22];
  PT[43][1] = PT[43][22];
  PT[46][1] = PT[46][22];
  PT[47][1] = PT[47][22];
  PT[49][1] = PT[49][22];
  PT[51][1] = PT[51][22];
  PT[54][1] = PT[54][22];
  PT[48][35] = PT[1][3];
  PT[50][35] = PT[1][3];
  PT[52][35] = PT[1][3];
  PT[55][35] = PT[1][3];
  PT[33][33] = PT[1][3];
  PT[3][5] = PT[1][3];
  PT[3][8] = PT[1][3];
  PT[3][9] = PT[1][3];
  PT[3][10] = PT[1][3];
  PT[3][11] = PT[1][3];
  PT[3][12] = PT[1][3];
  PT[3][14] = PT[1][3];
  PT[3][15] = PT[1][3];
  PT[3][16] = PT[1][3];
  PT[33][35] = PT[1][3];
  PT[37][33] = PT[1][3];
  PT[34][1] = PT[34][22];
  PT[35][1] = PT[34][22];
  printf("Parse Table Built\n");}

void emptyStack()
{
    struct stNode *node;
    while(stack){
        node = stack;
        stack = stack->next;
        free(node);
    }
}

void deleteTree(struct parseNode* root)
{
    if(!root)
        return;
    //post order traversal
    int i;
    for(i=0;i<10;i++){
      if(root->children[i])
        deleteTree(root->children[i]);
    }
    if(root->idName)
        free(root->idName);
    free(root);
}

void displayTree(struct parseNode *root,int spaces,struct parseNode *p) //needs to be amended.
{
    //preorder traversal.
    if(!root)
        return;
    int i;
    //root->parent = p;
    for(i=0;i<spaces;i++)
      printf(" ");
    printf("Node: %s, ",root->s);
    if(root->parent)
      printf("Parent = %s, ",(root->parent)->s);
    printf("Childcount=%d, ",root->childCount);
    if((root->s)[0]=='N'){
        printf("Children:");
        if(root->childCount == 0)
            printf(" (Empty : %s -> EPSILON)\n",root->s);
        else{
            for(i=0;i<=9;i++)
              if(root->children[i])
                printf("%s, ",root->children[i]->s);
            printf("\n");
            p = root;
            for(i=0;i<=9;i++)
              displayTree(root->children[i],spaces+1,root);
        }
    }
    else if(strcmp(root->s,"TK_ID\0")==0 && root->idName!=NULL)
        printf("Identifier %s\n",root->idName);
    else{
      printf("No Children \n");
    }
}

void flushBuffer()
{
  int i;
  for(i=0;i<20;i++)
    buf[i] = '\0';
}

int parseRecursive(struct parseNode *root,FILE *fp)     //stack parsing with tree generation, recursive, generate in depth first mannner.
{
  if(stack == NULL){
    if(fscanf(fp,"%s",buf)==1){
      printf("Error: stack empty, file not empty\n");
      return 0;
    }
    printf("Successful Parsing\n");
    return 1;
  }
  if(taken==0){
    if(fscanf(fp,"%s",buf)!=1){
      printf("Error: Stack not empty, end of file\n");
      return 0;
    }
    taken = 1;
  }
  //printf("On Stack: %s, Input: %s\n",stack->s,buf);
  struct hashNode *h;
  struct stackOp *opNode;
  struct parseNode *cNode,*newNode;
  int idx = 9,i,ret,r,c;
  char ch = (stack->s)[0];
  //root->childCount = 0;
  switch(ch){
    case 'N': h = search(nonTermLookup,stack->s);
              r = h->index;
              h = search(termLookup,buf);
              c = h->index;
              opNode = PT[r][c];
              if(opNode==NULL){
                printf("Error: No Entry for %s & %s r=%d, c=%d\n",stack->s,buf,r,c);
                return 0;
              }
              pop();
              if(opNode!=empty){    //non empty non terminal.
                /*if(strcmp(stack->s,decs)==0)      //symbol table stuff
                  decFlag = 1;
                else if(strcmp(stack->s,args)==0)
                  argsFlag = 1;
                else if(strcmp(stack->s,structs)==0){
                  newStructFlag = 1;
                  structNumber++;   structName = 1;
                  structIdx++;
                }*/
                cNode = (struct parseNode*)malloc(sizeof(struct parseNode));
                cNode->s = opNode->s;
                cNode->parent = root;
                root->children[idx--] = cNode;
                push(opNode->s);
                //printf("%s pushed\n",opNode->s);
                opNode = opNode->next;
                while(opNode){
                  push(opNode->s);
                  //printf("%s pushed\n",opNode->s);
                  cNode = (struct parseNode*)malloc(sizeof(struct parseNode));
                  cNode->s = opNode->s;
                  if(strcmp(cNode->s,"TK_ID\0")==0)
                    idPtr = cNode;
                  cNode->idName = NULL;
                  root->children[idx--] = cNode;
                  cNode->parent = root;
                  opNode = opNode->next;
                }
                root->childCount = 9-(short int)idx;
                if(idx>=0)
                  root->children[idx] = NULL;
                for(i=idx+1;i<=9;i++){
                  if(root->children[i] != NULL)
                    ret = parseRecursive(root->children[i],fp);
                  if(ret==0)
                    return 0;
                }
                return 1;
              }
              else {
                //printf("Empty Node\n");
                root->childCount = 0;
                root->children[9] = NULL;
                return 1;
              }
              break;
    case 'T': if(strcmp(stack->s,buf)==0){
                //printf("%s identified, ",stack->s);
                pop();
                  root->children[9] = NULL;
                if(buf[3]=='I' && buf[4]=='D'){ //TK_ID
                  fscanf(fp,"%s",buf);
                  if(idPtr){
                    idPtr->idName = (char*)malloc((strlen(buf)+1));
                    strcpy(idPtr->idName,buf);
                    idPtr = NULL;
                  }
                  // need ID's name.
                  /*if(decFlag){        //symbol table stuff
                   //addToSymbolTable(scope,buf,);
                  }
                  else if(newStructFlag){
                    if(structName){//the Id is a structName,create an entry in symbol table,create entry in terminal lookup at index structIdx
                      structName = 0;
                    }
                  }
                  else if(argsFlag){ // the id is an argument
                    
                  }
                  else if(funcFlag){
                  }
                
                else if(strcmp(buf,semi)==0){
                  
                }
                else if(strcmp(buf,lBrace)==0){
                  blockNumber++;
                  scope++;
                }
                else if(strcmp(buf,rBrace)==0){
                  scope--;
                }
                else if(strcmp(buf,rPar)==0)
                  argsFlag = 0;
                
                */
                }
                taken = 0;
                //printf("%s matched\n",buf);
                root->childCount = 0;
                flushBuffer();
                return 1;
              }
              else{
                printf("Error: %s is missing\n",stack->s);
                return 0;
              } 
              break;
    }
}

struct parseNode* parse(FILE *fp)
{
  /*scope = 0;      //symbol table indicators.
  decFlag = 0;
  newStructFlag = 0;
  structIdx = 56;   //index in parse table
  argsFlag = 0;
  funcNumber = 0;
  isArray = 0;*/
  taken = 0;
  idPtr = NULL;
  struct hashNode *h;
  h = search(nonTermLookup,"NT_TYPE\0");  //symbol table stuff;
  type = h->s;
  h = search(nonTermLookup,"NT_FUNCS\0");
  funcs = h->s;
  h = search(nonTermLookup,"NT_ARGS\0");
  args = h->s;
  h = search(nonTermLookup,"NT_DECS\0");
  decs = h->s;
  h = search(nonTermLookup,"NT_RETURNST\0");
  returnst = h->s;
  h = search(nonTermLookup,"NT_STRUCTS\0");
  structs = h->s;
  h = search(termLookup,"TK_LBRACE\0");
  lBrace = h->s;
  h = search(termLookup,"TK_RBRACE\0");
  rBrace = h->s;
  h = search(termLookup,"TK_LPAR\0");
  lPar = h->s;
  h = search(termLookup,"TK_RPAR\0");
  rPar = h->s;
  h = search(termLookup,"TK_BOOL\0");
  Bool = h->s;
  h = search(termLookup,"TK_CHAR\0");
  Char = h->s;
  h = search(termLookup,"TK_INT\0");
  Int = h->s;
  h = search(termLookup,"TK_SEMI\0");
  semi = h->s;
  h = search(termLookup,"TK_ID\0");
  Id = h->s;
  h = search(termLookup,"TK_COMMA\0");
  comma = h->s;
  h = search(termLookup,"TK_VOID\0");
  Void = h->s;
  empty = PT[1][3];
  h = search(nonTermLookup,"NT_PROG\0");
  push(h->s);
  if(feof(fp))
    return NULL;
  int status;
  flushBuffer();
  struct parseNode *root = (struct parseNode*)malloc(sizeof(struct parseNode));
  root->parent = NULL;
  root->s = h->s;
  status = parseRecursive(root,fp);
  if(status==1){
    printf("Successful Parsing\n");
    return root;
  }
  else{
    printf("Parsing Error(s).\n");
    emptyStack();
    deleteTree(root);
  }
}

/*// The following WORKS!!! (Normal Stack Parsing, no tree generation)
void parse(FILE *fp)
{
    taken = 0;
    char ch;
    char *empty;
    int r,c;
    struct hashNode *h;
    struct stackOp *opNode;
    h = search(nonTermLookup,"NT_EMPTY\0");
    empty = h->s;
    h = search(nonTermLookup,"NT_PROG\0");
    push(h->s);
    while(1){
        if(!stack){
          if(fscanf(fp," %s",buf)==1)
            printf("Error: Empty Stack, non-empty file\n");
          else printf("Successful\n");
          break;
        }
        if(taken==0){
          if(fscanf(fp," %s",buf)==1)
            taken = 1;
          else{
            if(stack!=NULL)
              printf("Error: End of file, non-empty stack\n");
            break;
          }
        }
        printf("On stack %s, input = %s\n",stack->s,buf);
        ch = (stack->s)[0];
        switch(ch)
        {
          case 'N': h = search(nonTermLookup,stack->s);
                    r = h->index;
                    h = search(termLookup,buf);
                    c = h->index;
                    opNode = PT[r][c];
                    if(opNode==NULL){
                      printf("Error: No entry for r = %d, c = %d\n",r,c);
                      emptyStack();
                      return;
                    }
                    else{
                      pop();
                      if(strcmp(opNode->s,empty)!=0){
                        while(opNode){
                          push(opNode->s);
                          //printf("%s pushed\n",opNode->s);
                          opNode = opNode->next;
                        }
                      }
                      else printf("Empty Node\n");
                    }
                    break;
          case 'T': if(strcmp(stack->s,buf)==0){
                      pop();
                      if(strcmp("TK_ID\0",buf)==0)
                        fscanf(fp,"%s",buf);
                      taken = 0;
                    }
                    else{
                      printf("Error: %s missing\n",stack->s);
                      emptyStack();
                      return;
                    }
                    break;
        }
    }
}
*/

void correctArray(struct parseNode *root){
  if(!root)
    return;
  struct parseNode *copy[10];
  int i,j;
  short int count=0;
  for(i=0;i<10;i++){
    copy[i] = root->children[i];
    if(copy[i]!=NULL)
      count++;
    root->children[i] = NULL;   //empty root's array.
  }
  root->childCount = count;
  i = (10-count);
  for(j=0;j<=9,i<=9;j++){    // re-arranging root's children array.
    if(copy[j])
      root->children[i++] = copy[j];
  }
}

struct parseNode* pruneParantheses(struct parseNode *root)  //working.
{
  if(!root)     //root doesn't exist;
    return NULL;
  int i=(10-(root->childCount));
  short int original = root->childCount;
  struct parseNode *pruned,*cNode;
  while(i<=9){
    cNode = root->children[i];
    if((strcmp(cNode->s,lPar)==0)||(strcmp(cNode->s,rPar)==0)){
      pruned = root->children[i];
      root->children[i] = NULL;
      printf("Removing %s\n",pruned->s);
      free(pruned);
      root->childCount--;
    }
    i++;
  }
  if(original!=root->childCount)
    correctArray(root);
  for(i=(10-root->childCount);i<=9;i++)
    root->children[i] = pruneParantheses(root->children[i]);
  return root;
}

struct parseNode* pruneSemiColon(struct parseNode *root)    //working.
{
  if(!root)
    return NULL;
  int i = (10 - root->childCount);
  struct parseNode *cNode,*removed;
  short int orig = root->childCount;
  while(i<=9){
    cNode = root->children[i];
    if(strcmp(cNode->s,"TK_SEMI\0")==0){    // a child is a semicolon.
      removed = root->children[i];
      root->children[i] = NULL;
      printf("Removing %s\n",removed->s);
      free(removed);
      root->childCount--;
    }
    i++;
  }
  if(orig!=(root->childCount))
    correctArray(root);
  for(i=(10-root->childCount);i<=9;i++)
    root->children[i] = pruneSemiColon(root->children[i]);
  return root;
}


void pruneExtraNT(struct parseNode *root)   //going wrong somewhere, need to work on it.
{
  if(!root)
    return;
  if(root->s[0]=='T' && root->childCount==0)   //the node is a terminal.
    return;
  int i;
  struct parseNode *child;
  while(root->childCount==1){
    //promote that child.
    printf("Extra: Removing %s\n",root->s);
    child = root->children[9];
    root->s = child->s;
    root->childCount = child->childCount;
    for(i=0;i<10;i++){
        root->children[i] = child->children[i];
        if(child->children[i])
          (child->children[i])->parent = root;
    }
    free(child);
  }
  //correctArray(root); is this necessary??
  short int count = root->childCount;
  for(i=(10-count);i<=9;i++)
    pruneExtraNT(root->children[i]);
}

void removeEpsilons(struct parseNode *root) //working..., not correcting childcounts...
{
  if(!root)
    return;
  if(root->s[0]=='T' && root->childCount==0)   //the node is a token.
    return;
  int i,j;
  short int count = root->childCount;
  for(i=(10-count);i<=9;i++)
    removeEpsilons(root->children[i]);
  correctArray(root);
  struct parseNode *parent = root->parent;
  if(!parent)   //the root of the tree
    return;
  /*count = 0;
  for(i=0;i<10;i++){    //reaffirming childCount after removing epsilon children.
    if(root->children[i])
      count++;
  }*/
  //root->childCount = count;
  if(root->childCount == 0){    //non terminal deriving epsilon
    for(j=0;j<10;j++)
      if(parent->children[j]==root)
        break;
    parent->children[j] = NULL;
    printf("Epsilon: Removing %s\n",root->s);
    free(root);
  }
}

struct parseNode* buildAST(struct parseNode* root)
{
  root = pruneParantheses(root);
  printf("Parantheses Removed\n");
  root = pruneSemiColon(root);
  printf("Semicolons Removed\n");
  removeEpsilons(root);
  printf("Epsilons Removed\n");
  pruneExtraNT(root);
  return root;
}

void insertFunc(struct funcNode *hashTbl[],struct funcNode *node)
{
    int i,len,val=0;
    len = strlen(node->name);
    for(i=0;i<len;i++)
        val += (int)((node->name)[i]-'a');
    node->next = hashTbl[val%50];
    hashTbl[val%50] = node;
}

struct funcNode* searchFunc(struct funcNode *hashTbl[],char name[])
{
    int i,len,val=0;
    len = strlen(name);
    for(i=0;i<len;i++)
        val += (int)(name[i]-'a');
    struct funcNode *s = hashTbl[val%50];
    while(s){
        if(strcmp(s->name,name)==0)
            return s;
        s = s->next;
    }
    return NULL;
}

void deleteTable2(struct symbNode *table[]){
    int i;
    struct symbNode *n,*m;
    for(i=0;i<50;i++){
        n = table[i];
        while(n){
          m = n->next;
          free(n);
          n = m;
        }
    }
}

void deleteSymbolTable(struct funcNode* table[]){
    int i;
    struct funcNode *n,*m;
    for(i=0;i<50;i++){
        n = table[i];
        while(n){
          n = table[i];
          m = n->next;
          deleteTable2(n->table); //delete the function's adjoining table.
          free(n);
          n = m;
        }
    }
}

struct symbNode* search2(struct symbNode *table[],char *name)
{
    int i,val=0,len;
    len = strlen(name);
    for(i=0;i<len;i++)
        val += (int)(name[i]-'a');
    struct symbNode *s = table[val%50];
    while(s){
        if(strcmp(s->name,name)==0)
            return s;
        s = s->next;
    }
    return NULL;
}

void insertSymb(struct symbNode *table[],char *type,char name[] )
{
    int i,len,val=0;
    struct symbNode *newNode = malloc(sizeof(struct symbNode));
    len = strlen(name);
    for(i=0;i<len;i++)
        val += (int)((name)[i]-'a');
    strcpy(newNode->name,name);
    if(type == Int)
        newNode->type = 'i';
    else if(type == Bool)
        newNode->type = 'b';
    else if(type == Char)
        newNode->type = 'c';
    else newNode->type = 'o';   //other.
    newNode->isFlags = 0;
    newNode->next = table[val%50];
    table[val%50] = newNode;
}
int populateFuncTable(struct symbNode *table[],struct parseNode *root)
{
    //search for NT_DECS. search all children
    if(root==NULL)
        return 1;
    int i;
    if(root->s == decs){    //NT_DECS
        decsFlag = 1;
        type = (root->children[10-root->childCount])->s;
        //now take care of the identifiers.
        for(i=11-root->childCount;i<10;i++){
            if((root->children[i])->s == Id){   //new variable.
                if((root->children[i])->idName){    // name is not null
                    if(search2(globe->table,(root->children[i])->idName)!=NULL){
                        printf("Global Redeclaration Error: %s in function\n",(root->children[i])->idName);
                        return 0;
                    }
                    else if(search2(table,(root->children[i])->idName)!=NULL){
                        printf("Redeclaration Error: %s in function\n",(root->children[i])->idName);
                        return 0;
                    }
                    else{//no issues.
                        insertSymb(table,type,idName);
                        
                    }
                }
                else{
                    printf("Idname is NULL\n");
                    return 0;
                }
            }
            else if((root->children[i])->s == comma)
                continue;
            else if(populateFuncTable(table,root->children[i])==0){
                return 0;
            }
        }
    }
    else{
        for(i=(10-root->childCount);i<10;i++){
            if(populateFuncTable(table,root->children[i])==0)
                return 0;
        }
    }
    return 1;
}

int buildFuncTable(struct parseNode *root,struct funcNode *functions[])
{
    //populate functions. root is NT_PROG search for NT_FUNCS in its children.
    int i,startIdx,ret;
    do{
        startIdx = 10 - root->childCount;
        for(i=startIdx;i<10;i++){
         if((root->children[i])->s == funcs) //present => not empty.
            break;
        }
        if(i==10)  //no functions, just main, else wouldn't have parsed.
         return;
        if((root->children[i])->s == funcs){ //NT_FUNCS char ptr,at least 1
            root = root->children[i];
            startIdx = 10 - root->childCount;
            struct funcNode *newNode = (struct funcNode*)malloc(sizeof(struct funcNode));
            if((root->children[startIdx+1])->s == Bool)
                newNode->type = 'b';
            else if((root->children[startIdx+1])->s == Char)
                newNode->type = 'c';        
            else if((root->children[startIdx+1])->s == Int)
                newNode->type = 'i';  //need function name too.
            
            if((root->children[startIdx+2])->idName)
                strcpy(newNode->name,(root->children[startIdx+2])->idName);
            //return type & name done,check for arguments.
            if(searchFunc(functions,newNode->name)!=NULL){  //func exists.
                printf("Function %s already exists\n",newNode->name);
                deleteSymbolTable(functions);   //check syntax.
                free(newNode);
                return -1;
            }
            if(checkArg(root,startIdx)==1)
                newNode->hasArgs = 'y';
            else newNode->hasArgs = 'n';
            newNode->table = (struct symbNode**)calloc(sizeof(struct symbNode*),50);
            ret = populateFuncTable(newNode->table,root);
            if(ret==-1){
                deleteSymbolTable(functions);   //check syntax.
                free(newNode);
                return -1;
            }
            insertFunc(functions,newNode);
        }
    }while((root->children[9])->s == funcs);
    return 1;
}

int typeChecking11(struct parseNode *root,char rettype)
{
    struct parseNode *firstChild = (root->children[10-(root->childCount)]);
    if(firstChild->s == returnst){
        //this is a return statement;
        
    }
}

int typeChecking1(struct parseNode *root,struct funcNode *functions[])
{
    //checking return types of functions.
    //search for NT_FUNCS.
    if(root == NULL || root->s[0] == 'T')
        return 1;
    int i,startIdx,ret;
    startIdx = 10-root->childCount;
    for(i=startIdx;i<10;i++){
        if((root->children[i])->s == funcs)
            break;
    }
    if(i==10)   //no functions.
      return 1;
    root = root->children[i];
    startIdx = 10-root->childCount;
    char *Funcname = (root->children[startIdx+2])->idName;
    struct funcNode *node = searchFunc(functions,Funcname);
    char rettype = node->type;
    //now search & check for NT_RETURNST 
    //typeChecking11(root,);
}

int typeChecking(struct parseNode *root)
{
    // types of actual & formal parameters must match.
    // operations on datatypes should be valid.
    // return type should match the returned variable or literal
    // search for a function first, check all return statements within.
    if(typeChecking1(root)!=1)
        return 0;
}

int main(int argc,char *argv[])
{
  buildHashTables();
  buildParseTable();
  FILE *fp = fopen("lexer_output.txt","r");
  struct parseNode* root = parse(fp);
  //parse(fp);
  if(root){
    //printf("Parse Tree:-\n");
    //displayTree(root,0);
    //printf("\n\nAfter Pruning:-\n");
    //displayTree(root,0);
    root = buildAST(root);
    printf("After removing epsilons, extra non-terminals, semicolons, parantheses:-\n");
    //printf("After removing epsilons:-\n");
    displayTree(root,0,NULL);
    
    deleteTree(root);
    printf("Parse Tree Deleted\n");
  }
  deleteParseTable();
  deleteHashTables(nonTermLookup);
  deleteHashTables(termLookup);
  return 0;
}
