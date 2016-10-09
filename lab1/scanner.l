%{
/**********************************************************************
 *
 * **YOUR GROUP INFO SHOULD GO HERE**
 *   KUNAL CHOUDHARY - 999981863 - kunal.choudhary@mail.utoronto.ca
 *   BOYOWA DAVID OGBEIDE - 999644693 - boyowa.ogbeide@mail.utoronto.ca
 *
 *  Interface to the scanner module for CSC467 course project
 **********************************************************************/

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


ID                 [A-Za-z][A-Za-z0-9_]*
INTEGER            (([1-9][0-9]*)|0)
FLT                (([0-9]*"."[0-9]+)|([0-9]+"."[0-9]*))
%%

void          {yTRACE(KEY_VOID); return KEY_VOID;}
const         {yTRACE(KEY_CONST); return KEY_CONST;}
while         {yTRACE(KEY_WHILE); return KEY_WHILE;}
if            {yTRACE(KEY_IF); return KEY_IF;}
else          {yTRACE(KEY_ELSE); return KEY_ELSE;}
true          {yTRACE(KEY_TRUE); return KEY_TRUE;}
false         {yTRACE(KEY_FALSE); return KEY_FALSE;}

bool          {yTRACE(BOOL); return BOOL;}
bvec2         {yTRACE(BVEC2); return BVEC2;}
bvec3         {yTRACE(BVEC3); return BVEC3;}
bvec4         {yTRACE(BVEC4); return BVEC4;}

int           {yTRACE(INT); return INT;}
ivec2         {yTRACE(IVEC2); return IVEC2;}
ivec3         {yTRACE(IVEC3); return IVEC3;}
ivec4         {yTRACE(IVEC4); return IVEC4;}

float         {yTRACE(FLOAT); return FLOAT;}
vec2          {yTRACE(VEC2); return VEC2;}
vec3          {yTRACE(VEC3); return VEC3;}
vec4          {yTRACE(VEC4); return VEC4;}

lit         {yTRACE(F_LIT); return F_LIT;}
dp3         {yTRACE(F_DP3); return F_DP3;}
rsq         {yTRACE(F_RSQ); return F_RSQ;}

gl_FragColor                {yTRACE(V4_GL_FRAGCOLOR); return V4_GL_FRAGCOLOR; } 
gl_FragDepth                {yTRACE(BL_GL_FRAGDEPTH); return BL_GL_FRAGDEPTH; } 
gl_FragCoord                {yTRACE(V4_GL_FRAGCOORD); return V4_GL_FRAGCOORD; } 
gl_TexCoord                 {yTRACE(V4_GL_TEXCOORD); return V4_GL_TEXCOORD; } 
gl_Color                    {yTRACE(V4_GL_COLOR); return V4_GL_COLOR; } 
gl_Secondary                {yTRACE(V4_GL_SECONDARY); return V4_GL_SECONDARY; } 
gl_FogFragColor             {yTRACE(V4_GL_FOGFRAGCOLOR); return V4_GL_FOGFRAGCOLOR; } 
gl_Light_Half               {yTRACE(V4_GL_LIGHT_HALF); return V4_GL_LIGHT_HALF; } 
gl_Light_Ambient            {yTRACE(V4_GL_LIGHT_AMBIENT); return V4_GL_LIGHT_AMBIENT; } 
gl_Material_Shininess       {yTRACE(V4_GL_MATERIAL_SHININESS); return V4_GL_MATERIAL_SHININESS; } 
env1                        {yTRACE(V4_ENV1); return V4_ENV1; } 
env2                        {yTRACE(V4_ENV2); return V4_ENV2; } 
env3                        {yTRACE(V4_ENV3); return V4_ENV3; } 

null         {yTRACE(KEY_NULL); return KEY_NULL;/*Spec doesn't mention that null cannot be an ID-name*/}
{ID}         {
                //dynamically allocate memory for yylval.cval first
                //yylval.cval = yytext;
                yTRACE(ID); return ID;
             }

"("           {yTRACE(LEFT_BR); return LEFT_BR;}
")"           {yTRACE(RIGHT_BR); return RIGHT_BR;}
"["           {yTRACE(LEFT_SQ_BR); return LEFT_SQ_BR;}
"]"           {yTRACE(RIGHT_SQ_BR); return RIGHT_SQ_BR;}
"{"           {yTRACE(LEFT_CURLY_BR); return LEFT_CURLY_BR;}
"}"           {yTRACE(RIGHT_CURLY_BR); return RIGHT_CURLY_BR;}

";"           {yTRACE(SEMI_COLON); return SEMI_COLON;}
","           {yTRACE(COMMA); return COMMA;}

"="           {yTRACE(ASSIGN); return ASSIGN;}
"+"           {yTRACE(ADD); return ADD;}
"-"           {yTRACE(SUB); return SUB;}
"*"           {yTRACE(MULT); return MULT;}
"/"           {yTRACE(DIV); return DIV;}
"^"           {yTRACE(EXP); return EXP;}

"=="          {yTRACE(EQ_TO); return EQ_TO;}
"!="          {yTRACE(NOT_EQ); return NOT_EQ;}
">="          {yTRACE(GT_T_EQ); return GT_T_EQ;}
"<="          {yTRACE(LS_T_EQ); return LS_T_EQ;}
">"           {yTRACE(GT_T); return GT_T;}
"<"           {yTRACE(LS_T); return LS_T;}
"!"           {yTRACE(NOT); return NOT;}
"&&"          {yTRACE(AND); return AND;}
"||"          {yTRACE(OR); return OR;}
   
{INTEGER}     {                            
                int match = atoi(yytext);               
                char buffer[20];
                sprintf(buffer, "%d", match);//converting int to string
                            
                //if there is an integer overflow, buffer would be -1 because of using atoi on yytext
                if(strcmp(yytext,buffer) != 0)
                {
                    yERROR("Integer overflow");
                }
                else
                {
                    yylval.ival = match;
                    yTRACE(INT); return INT;
                }
              }

{FLT}         {
                float match = atof(yytext);
                
                yylval.fval = match;
                yTRACE(FLOAT); return FLOAT;
              }
              
{WS}+          {;}

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
