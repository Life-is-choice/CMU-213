/* 
 * CS:APP Data Lab 
 * 
 * <刘泽滨>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
    //对于每一个bit使用德.摩根律
    return ~((~x)|(~y));
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
    //注意多字节数据在内存中小端，在寄存器中大端。字面值此处编译成立即数
    return (x>>(n<<3))&0xff;
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
    //即使标准没有明确有符号数做算数右移，无符号数做逻辑右移，但约定的习惯让我们可以这样假设
    int boolN = !(!n);
    int mask = (0xff | 0xff<<8 | 0xff<<16 | 0xff <<24)^(boolN<<31);
    x = x>>boolN;
    x = x&mask;
    return x>>(n+(~boolN+1));
}

/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
    //第一反应是ans = x&1 + (x>>1)&1 ...+ x(>>31)&1;
    //本质上来说，我们只是要把x上所有的1做加法。注意到加法是一个可结合的二元操作，而且所有的操作元素都在同一个对象中，这允许我们用并行的思维来进行这项工作。这与用增广矩阵求逆有异曲同工之妙
    int mask1 = 0x55|(0x55<<8)|(0x55<<16)|(0x55<<24);//单个位作为加法的操作数
    int ans = (x&mask1) + ((x>>1)&mask1);
    int mask2 = 0x33|(0x33<<8)|(0x33<<16)|(0x33<<24);//两个位加法的和作为加法操作数
    ans = (ans&mask2)+((ans>>2)&mask2);
    int mask4 = 0x0f|(0x0f<<8)|(0x0f<<16)|(0x0f<<24);//同上
    ans = (ans&mask4)+((ans>>4)&mask4);
    int mask8 = 0xff|(0xff<<16);
    ans = (ans&mask8)+((ans>>8)&mask8);
    int mask16 = 0xff|(0xff<<8);
    ans = (ans&mask16)+((ans>>16)&mask16);
    return ans;
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
    //这题gg。利用0==-0的特性做出区分，很骚
    return (((x|(~x+1))>>31)+1)&1;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
    //将符号位置1,题目假设int为32位
  return 1<<31;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
    //正数n-1左边全为0；负数n左边全为1
    int isFitPositive = !(x>>(n+(~1+1)));
    int isFitNegitive = !(~(x>>(n+(~1+1))));
    return isFitNegitive|isFitPositive;
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2 10111>>2 = 101
 */
int divpwr2(int x, int n) {
    //向下舍入(x/y) == 向上舍入(x+(y-1)/y)
    int signMask = x>>31;
    int offset = ((1<<n)+(~1+1))&signMask;
    return (x+offset)>>n;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x+1;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
    //x>=0&&x!=0
  return (!(x>>31))&(!(!x));
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */

int isLessOrEqual(int x, int y) {
    //ans = bool(x-y<0)+bool(x-y==0)。x-y负负溢出时意味着x<0,y>0,直接返回1
    int signX = (x>>31)&1;
    int signY = (y>>31)&1;
    int isSameSign = !(signX^signY);
    //xy异号时
    int isLess = signX&(!signY);
    //xy同号时,做差
    int isNeg = ((x+(~y+1))>>31)&1;
    //xy相等时
    int isEqual = !(x^y);
    return (isNeg&isSameSign)|isEqual|isLess;
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
    //递归分治
    int startBit = 0;int halfLength = 32/2;
    int isFound16 = !!(x>>(startBit+halfLength));
    startBit += isFound16<<4;halfLength=halfLength/2;
    int isFound8 = !!(x>>(startBit+halfLength));
    startBit += isFound8<<3;halfLength=halfLength/2;
    int isFound4 = x>>(startBit+halfLength);
    startBit += isFound4<<2;halfLength=halfLength/2;
    int isFound2 = x>>(startBit+halfLength);
    startBit += isFound2<<1;halfLength=halfLength/2;
    int isFound1 = x>>(startBit+halfLength);
    startBit += isFound1<<0;halfLength=halfLength/2;
    return startBit;
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
    if((uf&0x7fffffff)>0x7f800000) return uf;
 return uf^0x80000000;
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
//1:8:23
unsigned float_i2f(int x) {
    
    int sign = (x>>31)&1;
    if(sign == 0){
        if(x==0){
            return 0;
        }
        int power = ilog2(x);int i = power;int mask=0;
        while(i--){
            mask = mask|(1<<i);
        }
        if(power+1>23){
            int res = ((x & mask)>>(power-23)) + (power<<23) + (sign<<31);
            return res;
        }
        else{
            int res = ((x & mask)<<(23-power)) + (power<<23) + (sign<<31);
            return res;
        }
    }
    else{
        if(x!=0x80000000){
            x = -x;
        }
        int power = ilog2(x);int i = power;int mask=0;
        while(i--){
            mask = mask|(1<<i);
        }
        if(power+1>23){
            int res = ((x & mask)>>(power-23)) + (power<<23) + (sign<<31);
            return res;
        }
        else{
            int res = ((x & mask)<<(23-power)) + (power<<23) + (sign<<31);
            return res;
        }
    }
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
    //this answer fail.
    unsigned mask = 0x7f800000;
    unsigned exponent = (uf&mask)>>23;
    return (uf^mask)+((exponent+1)<<23);
}
