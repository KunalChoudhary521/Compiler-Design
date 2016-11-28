{int a = 6.3;}

/*tests that pass:
test#1: {<type> <id> =  <value>}
{const bool a = false;}
{float a = 3.3;}
{int b = 78;}
{bool c = true;}

test#2: {<type> <id>; <id> =  <value>;}
{float d; d = 1.2;}
{int e; e = 3;}
{bool f; f = false;}

test#3:{<type1> <id1>; <id1> =  <value1>;<type2> <id2>;}
{int a;float b = 5.3;a = 45;}
{bool a;float b = 5.3;a = true;}

test#4: {<type> <id> = <unary_op> <value>;}
{float a = -2.3;}
{bool a = !false;}

test#5: {<type> <id> = <value> <binary_op> <value>;}
{int a = 5 + 4;}
{float a; a = 5.1 * 2.4;}
{bool a = true && false;}

test#6: {<array_type> <id> = <ctor>(<value1>, ...);}
{ivec2 a = ivec2(2,4);}
{vec3 b = vec3(0.2,4.5,112.1);}
{bvec4 c = vec4(true,false,false,true);}

test#7: {<type> <id> = <funct>(<args>, ...);}
{float a = dp3(b,c);}
{float a = rsq(4.2);}
{vec4 a = lit(vec4(1.2,5.4,7.8,9.9);}

test#8: if and if_else statements
{if(a) a = 1.2;}
{int a; if(a) a = 2; else a = 1;}

test#10: {accessing array index}
{vec2 a = vec2(2.3,4.2); float b = a[1];}
{bvec3 a = bvec3(true,false,true); bool b = a[2];}

*/



