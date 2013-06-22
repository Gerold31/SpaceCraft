%option noyywrap
%option yylineno
%option nounput

WHITESPACE     [ \t\n\r]
INTEGER_DEC    "-"?[[:digit:]]+
INTEGER_HEX    "0x"[[:digit:]a-fA-F]+

%x COMMENT

%{
#include "minako-syntax.tab.h"
%}

%%

{WHITESPACE}+ { /* ignore whitespaces */ }
";".*\n       { /* Comment */ }

[Ss][Ee][Tt] {yylval.intValue = 0x01; return INSTRUCTION2;}
[Aa][Dd][Dd] {yylval.intValue = 0x02; return INSTRUCTION2;}
[Ss][Uu][Bb] {yylval.intValue = 0x03; return INSTRUCTION2;}
[Mm][Uu][Ll] {yylval.intValue = 0x04; return INSTRUCTION2;}
[Mm][Ll][Ii] {yylval.intValue = 0x05; return INSTRUCTION2;}
[Dd][Ii][Vv] {yylval.intValue = 0x06; return INSTRUCTION2;}
[Dd][Vv][Ii] {yylval.intValue = 0x07; return INSTRUCTION2;}
[Mm][Oo][Dd] {yylval.intValue = 0x08; return INSTRUCTION2;}
[Mm][Dd][Ii] {yylval.intValue = 0x09; return INSTRUCTION2;}
[Aa][Nn][Dd] {yylval.intValue = 0x0A; return INSTRUCTION2;}
[Bb][Oo][Rr] {yylval.intValue = 0x0B; return INSTRUCTION2;}
[Xx][Oo][Rr] {yylval.intValue = 0x0C; return INSTRUCTION2;}
[Ss][Hh][Rr] {yylval.intValue = 0x0D; return INSTRUCTION2;}
[Aa][Ss][Rr] {yylval.intValue = 0x0E; return INSTRUCTION2;}
[Ss][Hh][Ll] {yylval.intValue = 0x0F; return INSTRUCTION2;}
[Ii][Ff][Bb] {yylval.intValue = 0x10; return INSTRUCTION2;}
[Ii][Ff][Cc] {yylval.intValue = 0x11; return INSTRUCTION2;}
[Ii][Ff][Ee] {yylval.intValue = 0x12; return INSTRUCTION2;}
[Ii][Ff][Nn] {yylval.intValue = 0x13; return INSTRUCTION2;}
[Ii][Ff][Gg] {yylval.intValue = 0x14; return INSTRUCTION2;}
[Ii][Ff][Aa] {yylval.intValue = 0x15; return INSTRUCTION2;}
[Ii][Ff][Ll] {yylval.intValue = 0x16; return INSTRUCTION2;}
[Ii][Ff][Uu] {yylval.intValue = 0x17; return INSTRUCTION2;}
[Aa][Dd][Xx] {yylval.intValue = 0x1A; return INSTRUCTION2;}
[Ss][Bb][Xx] {yylval.intValue = 0x1B; return INSTRUCTION2;}
[Ss][Tt][Ii] {yylval.intValue = 0x1E; return INSTRUCTION2;}
[Ss][Tt][Dd] {yylval.intValue = 0x1F; return INSTRUCTION2;}

[Jj][Ss][Rr] {yylval.intValue = 0x01; return INSTRUCTION1;}
[Ii][Nn][Tt] {yylval.intValue = 0x08; return INSTRUCTION1;}
[Ii][Aa][Gg] {yylval.intValue = 0x09; return INSTRUCTION1;}
[Ii][Aa][Ss] {yylval.intValue = 0x0A; return INSTRUCTION1;}
[Rr][Ff][Ii] {yylval.intValue = 0x0B; return INSTRUCTION1;}
[Ii][Aa][Qq] {yylval.intValue = 0x0C; return INSTRUCTION1;}
[Hh][Ww][Nn] {yylval.intValue = 0x10; return INSTRUCTION1;}
[Hh][Ww][Qq] {yylval.intValue = 0x11; return INSTRUCTION1;}
[Hh][Ww][Ii] {yylval.intValue = 0x12; return INSTRUCTION1;}

[Dd][Aa][Tt] {return DAT;}

[Aa] {yylval.intValue = 0; return REGISTER;}
[Bb] {yylval.intValue = 1; return REGISTER;}
[Cc] {yylval.intValue = 2; return REGISTER;}
[Xx] {yylval.intValue = 3; return REGISTER;}
[Yy] {yylval.intValue = 4; return REGISTER;}
[Zz] {yylval.intValue = 5; return REGISTER;}
[Ii] {yylval.intValue = 6; return REGISTER;}
[Jj] {yylval.intValue = 7; return REGISTER;}

[Pp][Uu][Ss][Hh] {yylval.intValue = 0x18; return PUSH;}
[Pp][Oo][Pp]  {yylval.intValue = 0x18; return POP;}
[Pp][Ee][Ee][Kk] {yylval.intValue = 0x18; return PEEK;}
[Pp][Ii][Cc][Kk] {yylval.intValue = 0x18; return PICK;}
[Ss][Pp]   {yylval.intValue = 0x1B; return SP;}
[Pp][Cc]   {yylval.intValue = 0x1C; return PC;}
[Ee][Xx]   {yylval.intValue = 0x1D; return EX;}


{INTEGER_DEC}                      { yylval.intValue = strtol(yytext  , NULL, 10); return CONST_INT; }
{INTEGER_HEX}                      { yylval.intValue = strtol(yytext+2, NULL, 16); return CONST_INT; }
\"(\\.|[^\\"])*\"                  { yylval.string = yytext; return STRING;}
[a-zA-Z_]([[:digit:]a-zA-Z_])*     { yylval.string = yytext; return LABEL; }

.                                  { return yytext[0]; }

%%

