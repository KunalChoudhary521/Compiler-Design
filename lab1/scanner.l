%{
/**********************************************************************
 *
 * **YOUR GROUP INFO SHOULD GO HERE**
 *   KUNAL CHOUDHARY - 999981863 - kunal.choudhary@mail.utoronto.ca
 *
 *  Interface to the scanner module for CSC467 course project
 **********************************************************************/

/*
Questions for TA:
1.  what is parse.output for?
2.  explain tip#3 in lab handout.
3.  is student name and student_number enough for info.?
*/

#include "common.h"
#include "parser.tab.h"
#include "string.h"
#include "stdio.h"

#define YY_USER_INIT { yyin = inputFile; }
#define	yyinput      input
#define yTRACE(x)    { if (traceScanner) fprintf(traceFile, "TOKEN %3d : %s\n", x, yytext); }
#define yERROR(x)    { fprintf(errorFile, "\nLEXICAL ERROR, LINE %d: %s\n", yyline, x); errorOccurred = TRUE; yyterminate(); }

int CheckInt(void);
int yyline = 1;

%}
%option noyywrap

WS            [ \t\b\v]

LETTER        [A-Za-z]
DIGIT         [0-9]
ID            [A-Za-z][A-Za-z0-9]*

%%

{ID}         {yTRACE(ID); return ID;}
{DIGIT}       {yTRACE(DIGIT); return DIGIT;}
{LETTER}      {yTRACE(LETTER); return LETTER;}
{WS}          {yTRACE(WS); return WS;}

"lit"         {yTRACE(F_LIT); return F_LIT;}
"dp3"         {yTRACE(F_DP3); return F_DP3;}
"rsq"         {yTRACE(F_RSQ); return F_RSQ;}

bool          {yTRACE(BOOL); return BOOL;}
bvec2         {yTRACE(BVEC2); return BVEC2;}
bvec3         {yTRACE(BVEC3); return BVEC3;}
bvec4         {yTRACE(BVEC4); return BVEC4;}

int           {yTRACE(INT); return INT;}
ivec2         {yTRACE(IVEC2); return IVEC2;}
ivec3         {yTRACE(IVEC3); return IVEC3;}
ivec4         {yTRACE(IVEC4); return IVEC4;}

{DIGIT}+"."{DIGIT}*         {yTRACE(FLOAT); return FLOAT;}
vec2          {yTRACE(VEC2); return VEC2;}
vec3          {yTRACE(VEC3); return VEC3;}
vec4          {yTRACE(VEC4); return VEC4;}


"=="          {yTRACE(EQ_TO); return EQ_TO;}
"!="          {yTRACE(NOT_EQ); return NOT_EQ;}
">="          {yTRACE(GT_T_EQ); return GT_T_EQ;}
"<="          {yTRACE(LS_T_EQ); return LS_T_EQ;}
">"           {yTRACE(GT_T); return GT_T;}
"<"           {yTRACE(LS_T); return LS_T;}
"!"           {yTRACE(NOT); return NOT;}
"&&"          {yTRACE(AND); return AND;}
"||"          {yTRACE(OR); return OR;}


"="           {yTRACE(ASSIGN); return ASSIGN;}
"+"           {yTRACE(ADD); return ADD;}
"-"           {yTRACE(SUB); return SUB;}
"*"           {yTRACE(MULT); return MULT;}
"/"           {yTRACE(DIV); return DIV;}
"^"           {yTRACE(EXP); return EXP;}


"("           {yTRACE(LEFT_BR); return LEFT_BR;}
")"           {yTRACE(RIGHT_BR); return RIGHT_BR;}
"["           {yTRACE(LEFT_SQ_BR); return LEFT_SQ_BR;}
"]"           {yTRACE(RIGHT_SQ_BR); return RIGHT_SQ_BR;}
"{"           {yTRACE(LEFT_CURLY_BR); return LEFT_CURLY_BR;}
"}"           {yTRACE(RIGHT_CURLY_BR); return RIGHT_CURLY_BR;}

"\n"          {yTRACE(NEWLINE); return NEWLINE;}        
";"           {yTRACE(SEMI_COLON); return SEMI_COLON;}
","           {yTRACE(COMMA); return COMMA;}
     

const         {yTRACE(KEY_CONST); return KEY_CONST;}
if            {yTRACE(KEY_IF); return KEY_IF;}
else          {yTRACE(KEY_ELSE); return KEY_ELSE;}
while         {yTRACE(KEY_WHILE); return KEY_WHILE;}
void          {yTRACE(KEY_VOID); return KEY_VOID;}
true          {yTRACE(KEY_TRUE); return KEY_TRUE;}
false         {yTRACE(KEY_FALSE); return KEY_FALSE;} 


"/*"        {
              int c1      = 0;
              int c2      = yyinput();
              int curline = yyline;
              for(;;) {
                if (c2 == EOF) {
                  fprintf(errorFile, "\nLEXICAL ERROR, LINE %d: Unmatched /*\n", curline);
                  errorOccurred = TRUE;
                  yyterminate();
                }
                if (c1 == '*' && c2 == '/')
                  break;
                c1 = c2;
                c2 = yyinput();
                if (c1 == '\n' && c2 != EOF)
                  yyline++;
              }
            }

.           { yERROR("Unknown token"); }

%%
