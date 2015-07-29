/*  Author(s): Shreyansh Gandhi (ID: 2012A7PS070P), Himanshu Sharma(ID: 2012C6PS545P).
    Lexical Analyzer. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define TOINT(c) ((int)c)

struct lnode{
    char str[10];
    char tok[10];
    struct lnode *next;
};

struct lnode **keyword_lookup;

void createHash()
{
    keyword_lookup = calloc(sizeof(struct lnode*),100);
}

void deleteHash()
{
    int i;
    struct lnode *n,*next;
    for(i=0;i<100;i++){
        //delete the list starting here
        n = keyword_lookup[i];
        while(n){
            next = n->next;
            free(n);
            n = next;
        }
    }
    free(keyword_lookup);
}

void insertHash(char str[],char tok[])
{
    int i,len,val=0;
    len = strlen(str);
    for(i=0;i<len;i++)
        val+=TOINT(str[i]);
    struct lnode *newNode;
    newNode = (struct lnode*)malloc(sizeof(struct lnode));
    strcpy(newNode->str,str);
    strcpy(newNode->tok,tok);
    newNode->next = keyword_lookup[val%100];
    keyword_lookup[val%100] = newNode;
}

struct lnode* searchHash(char str[])
{
    int i,len,val=0;
    len = strlen(str);
    for(i=0;i<len;i++)
        val+=TOINT(str[i]);
    struct lnode *node = keyword_lookup[val%100];
    while(node){
        if(strcmp(str,node->str)==0)
            return node;
        node = node->next;
    }
    return NULL;
}

void buildHashTable()
{
    createHash();
    insertHash("function","TK_FUNC");
    insertHash("return","TK_RETURN");
    insertHash("while","TK_LOOP");
    insertHash("true","TK_TRUE");
    insertHash("false","TK_FALSE");
    insertHash("create","TK_CREATE");
    insertHash("main","TK_MAIN");
    insertHash("next","TK_NEXT");
    insertHash("end","TK_END");
    insertHash("call","TK_CALL");
    insertHash("inc","TK_INC");
    insertHash("dec","TK_DEC");
    insertHash("int","TK_INT");
    insertHash("char","TK_CHAR");
    insertHash("boolean","TK_BOOL");
    insertHash("void","TK_VOID");
    insertHash("array","TK_ARRAY");
    insertHash("if","TK_IFSTAT");
    insertHash("else","TK_ELSE");
    insertHash("input","TK_INSTAT");
    insertHash("output","TK_OUTSTAT");
    insertHash("constant","TK_CONSTANT");
    insertHash("try","TK_TRY");
    insertHash("catch","TK_CATCH");
    insertHash("finally","TK_FINALLY");
}

void flushBuffer(char buf[])
{
    int i;
    for(i=0;i<25;i++)
        buf[i] = '\0';
}

void lexical_analysis(char filename[])
{
    FILE *fp = fopen(filename,"r");
    FILE *out = fopen("lexer_output.txt","w");
    buildHashTable();
    if(fp==NULL || out==NULL){
        printf("File Error: Could not open file...\n");
        exit(1);
    }
    int state=0,line=1,len,taken=0,error=0;
    // error is the number of errors. 
    char ch;    int i;
    char buf[25];
    struct lnode *res;
    buildHashTable();
    while(1){
      if(feof(fp))
        break;
      if(taken == 0)      // if taken is 1, then use the value stored in c.
        ch = fgetc(fp);
      switch(state)
      {
        case 0: //start state.
                len = 0;
                taken = 0;
                if(ch == '\n')   // delimiters.
                  line++;
                else if(ch==' ' || ch=='\t')
                  break; 
                if(isalpha(ch)){
                  state = 1;  //keyword or identifier.
                  buf[len++] = ch; //buf will be sent for lookup.
                 }
                else if(isdigit(ch))
                  state = 2;  //integer literal.
                  else{   //handling symbols
                    state = 0;
                    switch(ch){
                      case ',': fprintf(out,"TK_COMMA ");
                                break;
                      case 39: state = 3;
                                break;
                      case ';': fprintf(out,"TK_SEMI\n");
                                break;
                      case ':': state = 4;
                                break;
                      case '(': fprintf(out,"TK_LPAR ");
                                break;
                      case ')': fprintf(out,"TK_RPAR ");
                                break;
                      case '[': fprintf(out,"TK_LSQ ");                                     
                                break;
                      case ']': fprintf(out,"TK_RSQ ");
                                break;
                      case '+': fprintf(out,"TK_PLUS ");
                                break;
                      case '-': fprintf(out,"TK_MINUS ");
                                break;
                      case '/': fprintf(out,"TK_DIV ");
                                break;
                      case '*': fprintf(out,"TK_MUL ");
                                break;
                      case '&': state = 5;
                                break;
                      case '>': state = 6;
                                break;
                      case '<': state = 7;
                                break;
                      case '=': state = 8;
                                break;
                      case '|': state = 9;
                                break;
                      case '!': //fprintf(out,"TK_NOT ");
                                state = 10;
                                break;
                      case '.': fprintf(out,"TK_DOT ");
                                break;
                      case '#': fprintf(out,"TK_LVAL ");
                                break;
                      case '%': fprintf(out,"TK_MOD ");
                                break;
                      case '{': fprintf(out,"TK_LBRACE ");
                                break;
                      case '}': fprintf(out,"TK_RBRACE ");
                                break;
                      default:  break;
                    }
                  }
                  if(state==0)
                    flushBuffer(buf);
                  break;
        case 1: // keyword|identifier
                if(isalnum(ch)){      //letter or digit.
                  buf[len++] = ch;
                }
                else{   //pattern has broken.
                  taken = 1;
                  state = 0;
                  res = searchHash(buf);
                  if(res != NULL)
                    fprintf(out,"%s ",res->tok);  //write to lexer output.
                  else fprintf(out,"TK_ID %s ",buf);
                  flushBuffer(buf);
                }
                break;
        case 2: //integer literal, broken only by a symbol or a delimiter.
                if(!isdigit(ch)){
                // 9fc should be illegal.. what can come after an integer literal: ; , ( ) [ ] or a relational/arithmetic op?
                  if(!isalpha(ch)){
                    taken = 1;
                    state = 0;
                    fprintf(out,"TK_INTLIT ");
                  }
                  else{
                    error++;
                    fprintf(out,"ERR_Int_Lit ");
                  }
                  flushBuffer(buf);
                }
                break;
        case 3: if((ch = fgetc(fp))==EOF){  //the character literal...
                  fprintf(out,"ERR_Ch_Lit ");
                  error++;
                  break;
                }
                ch = fgetc(fp);
                if(TOINT(ch)!=39)
                  fprintf(out,"TK_CHARLIT ");
                else{ error++;
                      fprintf(out,"ERR_Ch_Lit ");
                }
                flushBuffer(buf);
                state = 0;
                break;
        case 4: //assignment operator := colon and equal to  already read
                if(ch == '=')
                  fprintf(out,"TK_ASSIGNOP ");
                else{ //error++;
                      fprintf(out,"TK_COLON ");
                }
                flushBuffer(buf);
                state = 0;
                break;
        case 5: if(ch=='&'){
                  fprintf(out,"TK_LOGAND ");
                  flushBuffer(buf);
                }
                else{ fprintf(out,"TK_BITAND ");
                  taken = 1;
                }
                state = 0;
                break;
        case 6: if(ch == '='){
                  fprintf(out,"TK_GREATEQ ");
                  flushBuffer(buf);
               }
                else if(ch=='>')
                  fprintf(out,"TK_INOP ");
                else{ fprintf(out,"TK_GREAT ");
                  taken = 1;}
                state = 0;
                break;
        case 7: if(ch == '='){
                  fprintf(out,"TK_LESSEQ ");
                  flushBuffer(buf);
                }
                else if(ch=='<'){
                  fprintf(out,"TK_OUTOP ");
                  flushBuffer(buf);
                }
                else {fprintf(out,"TK_LESS ");
                  taken = 1;}
                state = 0;
                break;
        case 8: if(ch == '=')
                  fprintf(out,"TK_EQUALOP ");
                else{ fprintf(out,"ERR_EqualOp ");
                    error++;}
                state = 0;
                flushBuffer(buf);
                break;
        case 9: if(ch == '|'){
                  fprintf(out,"TK_LOGOR ");
                  flushBuffer(buf);
                }
                else{   fprintf(out,"TK_BITOR ");
                  taken = 1;}
                state = 0;
                break;
        case 10: if(ch=='='){
                   fprintf(out,"TK_NOTEQ ");
                   flushBuffer(buf);
                 }
                 else{
                   fprintf(out,"TK_NOT ");
                   taken = 1;
                 }
                 state = 0;
                 break;
        default: break;
      }
    }
    printf("Lexical Analysis completed.\n");
    printf("%d lexical errors found.\n",error);
    fclose(fp);
    fclose(out);
    deleteHash();
}

int main(int argc, char *argv[])
{
    //file name as command line argument.
    if(argc < 2){
        printf("Error: No file name provided.\n");
        exit(0);
    }
    lexical_analysis(argv[1]);
    return 0;
}
