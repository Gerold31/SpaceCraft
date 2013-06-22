%{
#include <stdio.h>
#include <stdlib.h>

#include "hashmap.h"

extern int yylex();
extern int yylineno;
extern FILE* yyin;
void yyerror(const char*);
static void checksymtable(const char* id, int key);

#define YYERROR_VERBOSE yes
#define YYDEBUG 1

hashmap *symtable;

int ram[1<<16];
int pos;
int i;
int notdeflabel;
int error;

%}


%union {
    char* string;
    int intValue;
}

// used tokens
%token <intValue> INSTRUCTION2
%token <intValue> INSTRUCTION1
%token <intValue> REGISTER
%token <intValue> PUSH
%token <intValue> POP
%token <intValue> PEEK
%token <intValue> PICK
%token <intValue> SP
%token <intValue> PC
%token <intValue> EX
%token <intValue> CONST_INT
%token <string>   STRING
%token <string>   LABEL
%token DAT

%type  <intValue> instruction;
%type  <intValue> b;
%type  <intValue> a;
%type  <intValue> common;
%type  <intValue> literal;
%type  <intValue> reset;


%%

program:                /*empty*/
                    |   program instruction
                    |   program labeldef
                    |   program data;

labeldef:               addlabel ':';

addlabel:               LABEL                       {int j, lab = hashmapGet(symtable, $1);
                                                    if(lab == -1)
                                                        hashmapSet(symtable, pos, $1);
                                                    else if(lab >= 1<<16) {
                                                        hashmapSet(symtable, pos, $1);
                                                        for(j=0; j<pos; j++)
                                                            if(ram[j] == lab)
                                                                ram[j] = pos;
                                                    }else{
                                                        printf("label %s is already defined\n", $1);
                                                        exit(-1);
                                                    } };

data:                   DAT value

value:                  literal                     {ram[pos++] = $1;}
                    |   STRING                      {int j; for(j=1; j<strlen($1)-1; j++)ram[pos++]=$1[j];}
                    |   value ',' literal           {ram[pos++] = $3;}
                    |   value ',' STRING            {int j; for(j=1; j<strlen($3)-1; j++)ram[pos++]=$3[j];};



instruction:            reset INSTRUCTION2 b',' a   {ram[pos++] = ($2 | ($3 << 5) | ($5 << 10))&0xFFFF; pos += i;}
                    |   reset INSTRUCTION1 a        {ram[pos++] = (     ($2 << 5) | ($3 << 10))&0xFFFF; pos += i;/*should be sometimes b e.g. HWN*/};

b:                      common                      {$$ = $1;}
                    |   PUSH                        {$$ = $1;}
                    |   literal                     {$$ = $1;}; /*really? does this work in emulator?*/

a:                      common                      {$$ = $1;}
                    |   POP                         {$$ = $1;}
                    |   PUSH                        {$$ = $1;}
                    |   literal                     {if($1 >= -1 && $1 <= 0x1E) $$ = 0x21 + $1; else {ram[pos+(++i)] = $1; $$ = 0x1F;}};

common:                 REGISTER                    {$$ = $1;}
                    |   '['REGISTER']'              {$$ = $2 + 8;}
                    |   '['REGISTER '+' literal']'  {ram[pos+(++i)] = $4; $$ = $2 + 0x10;}
                    |   '['literal '+' REGISTER']'  {ram[pos+(++i)] = $2; $$ = $4 + 0x10;}
                    |   PEEK                        {$$ = $1;}
                    |   PICK literal                {ram[pos+(++i)] = $2; $$ = $1;}
                    |   SP                          {$$ = $1;}
                    |   PC                          {$$ = $1;}
                    |   EX                          {$$ = $1;}
                    |   '['literal']'               {ram[pos+(++i)] = $2; $$ = 0x1E;};


literal:                CONST_INT                   {$$ = $1 & 0xFFFF;}
                    |   LABEL                       {int lab = hashmapGet(symtable, $1); if(lab==-1) {hashmapSet(symtable, ++notdeflabel, $1); lab = notdeflabel;} $$ = lab;};

reset:                                              {i=0;};

%%

int main(int argc, char **argv)
{
    int j;
    if (argc != 2)
        yyin = stdin;
    else
    {
        yyin = fopen(argv[1], "r");
        if (!yyin)
        {
            fprintf(stderr, "Fehler: Konnte Datei %s nicht zum lesen oeffnen.\n", argv[1]);
            exit(-1);
        }
    }

    symtable = newHashmap(20);
    notdeflabel = 1<<16;
    error = 0;

    yydebug=0;
    yyparse();

    hashmapPrint(symtable, checksymtable);

    if(error)
        return -1;

    FILE *out = fopen("program.a", "w");
    if(!out)
    {
        fprintf(stderr, "Fehler: Konnte Datei %s nicht zum lesen oeffnen.\n", argv[1]);
        exit(-1);
    }

    for(j=0; j<pos; j++)
    {
        fputc((ram[j] >> 8) & 0xFF, out);
        fputc((ram[j] >> 0) & 0xFF, out);
    }
    fclose(out);

    return 0;
}

void yyerror(const char* msg) {
    fprintf(stderr, "line %d: %s\n", yylineno, msg);
    exit(-1);
}


static void checksymtable(const char* id, int key)
{
    if(key > 1<<16)
    {
        printf("label %s is not defined\n", id);
        error = 1;
    }
}
