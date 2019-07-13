#define _GNU_SOURCE
#include <emmintrin.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "concise_atomic.h"
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdalign.h>


unsigned long vec_id = 0; 
_Atomic(long) a_t = 42;

enum {
   nodesize = 32 * __SIZEOF_POINTER__,
};
unsigned u;
#define vec_t(T) __typeof__(T)*

struct pk {
   char c;
   int i;
   short s;
}pk;

int main(void){
   unsigned char b[32];
   unsigned char c[32];
   
   alignas(64) char *node = aligned_alloc(512,nodesize*2);
   alignas(64) unsigned *pp = aligned_alloc(64,nodesize);

   memcpy(b,(node-32),32);
   memcpy(c,(pp-8),32);   

   for (int i=0; i < 32; i++)
      printf("%02hhx",b[i]);
   printf("\n\n");
   for (int i=0; i < 32; i++)
      printf("%02hhx",c[i]);


/*   
   printf("%zu, %lu pdiff\n",sizeof(pk),(uintptr_t)&pk.s - (uintptr_t)&pk.c);


//   printf("%c\n",__builtin_types_compatible_p(__typeof__(p),__typeof__(node))?'t':'f');
   printf("%u\n",nodesize);
   printf("\n%p\n%p\n\n",node, pp);
   printf("\n%p\n%p\n\n",&node, &pp);
   printf("%lu\n",sizeof(*node));
   printf("%lu\n",sizeof(pp[4]));
   
*/   
   free(pp);
   free(node);
   return 0;
}















enum popcount_consts{
   m1   =  0b0101010101010101010101010101010101010101010101010101010101010101UL, 
   m2   =  0b0011001100110011001100110011001100110011001100110011001100110011UL, 
   m4   =  0b0000111100001111000011110000111100001111000011110000111100001111UL, 
   m8   =  0b0000000011111111000000001111111100000000111111110000000011111111UL, 
   m16  =  0b0000000000000000111111111111111100000000000000001111111111111111UL, 
   m32  =  0b0000000000000000000000000000000011111111111111111111111111111111UL, 
   h01  =  0b0000000100000001000000010000000100000001000000010000000100000001UL,   //the sum of 256 to the power of 0000,0001,2,0000000011...
};


                                           //This uses fewer arithmetic operations than any other known  
                                           //implementation on machines with slow multiplication.
                                           //This algorithm uses 17 arithmetic operations.
int eg_popcount64b(uint64_t x)
{
    x -= (x >> 1) & m1;                  //count each 2 bits [eg 10.10:one,one; 11.00:two,zero]: 1010->0101=0101 / 1100->0100=1000 / 0110->0001=0101 / 1110->0101=1001  
    x =  (x & m2) + ((x >> 2) & m2);     //count each 4 bits: 01011001->00010001+00010010=0010011
    x =  (x + (x >> 4)) & m4;                      //put count of each 8 bits into those 8 bits 
    x += x >>  8;                                  //put count of each 16 bits into their lowest 8 bits
    x += x >> 16;                                  //put count of each 32 bits into their lowest 8 bits
    x += x >> 32;                                  //put count of each 64 bits into their lowest 8 bits
    return x & 0x7f;
}

                                           //This uses fewer arithmetic operations than any other known  
                                           //implementation on machines with fast multiplication.
                                           //This algorithm uses 12 arithmetic operations, one of which is a multiply.
int eg_popcount64c(uint64_t x)
{
    x -= (x >> 1) & m1;                  //count each 2 bits: into those 2 bits
    x =  (x & m2) + ((x >> 2) & m2);                //put count of each 4 bits into those 4 bits 
    x =  (x + (x >> 4)) & m4;                       //put count of each 8 bits into those 8 bits 
    return (x * h01) >> 56;                        //returns left 8 bits of x + (x<<8) + (x<<16) + (x<<24) + ... 
}


//paddb       _mm_add_epi8(__m128i, __m128i)  add sixteen pairs of 8-bit integer
bool somefn(void)
{   
   static const unsigned char refc[32] = "%%%%%%%%%%%%%%%^!!!!!!!!!!!!!!!";
   __m128i mmc = _mm_setr_epi8(refc[0],   refc[1],   refc[2],   refc[3],
                               refc[4],   refc[5],   refc[6],   refc[7],
                               refc[8],   refc[9],   refc[10],  refc[11],
                               refc[12],  refc[13],  refc[14],  refc[15]);
   __m128i mmd = _mm_setr_epi8(refc[16],  refc[17],  refc[18],  refc[19],
                               refc[20],  refc[21],  refc[22],  refc[23],
                               refc[24],  refc[25],  refc[26],  refc[27],
                               refc[28],  refc[29],  refc[30],  refc[31]);

   __m128i ive = _mm_add_epi8(mmc,mmd);
   
   return true;
}

#define LOG2N(N) (__lzcnt32(N^N) - __lzcnt32(N))

