/* Honor of Thesis - A Bijection

This program was composed as an undergraduate honors research project by Wyatt S Carpenter at the University of Oklahoma in 2020 AD.

This program maps non-negative integers to grammatically correct English sentences, and vice versa.
Invoke the program with either a number or a sentence as argument to receive either a sentence or number back. Your result will be unique from the results of all other possible inputs. Run the program with the new output to get your input back.
0 maps to the null sentence (no words).

While there has been work on natural language parsing, natural language generation, hashing, and mnemonics, I did not find any work on this particular concept before. The closest I found was the Electronic Frontier Foundation's wordlist, and its precursors. I have used the EFF's wordlist in this program, as it seems well-suited. I have adapted the original text file into a C struct.

A few words about the implementation of this program:
While we read and write left-to-right in English, it seems like an arbitrary matter of convention whether we take our bits for generating sentences from the left (most significant) or right (least significant) side of the number. However, I decided to take them from the right side, as sometimes the program requires more bits to terminate than are in the numeral string. Since a number implicitly begins with an infinte number of zeros, we can safely use these beginning zeros as the required bits.
This program disregards punctuation, and certain other minor details of the English language. Therefore, the range of possible valid English sentences is quite large, since punctuation can be applied in many contortions to rescue the meaning of a sentence.
As it turns out, a large part of this project is precisely sheparding the entropy in the input, the machine, and the output, which all have incompatible radices.

Everything in this project is my original work, which I release into the public domain under CC0, except for the wordlist, which is either too trivial to fall under copyright or else probably falls under the purview EFF's CC-BY license (https://www.eff.org/copyright).
*/

#include <stdio.h>
#include <stdbool.h>

#include "wordlist.h"

#define DEBUG 1

#define dputs(s) if(DEBUG){puts(s);}
#define dprintf(...) if(DEBUG){fprintf (stderr, __VA_ARGS__); fflush(stdout);}


char *currentleftmostdigit = 0;
char *currentrightmostdigit = 0;

int chartoint(char c){return c - '0';}

char inttochar(int i){return i + '0';}

/*
bool wordeq(char *word){
  while(*word && word){
    if 
  }
}
*/

char getnextdigit(){
  if(currentrightmostdigit<currentleftmostdigit){
    return '0'; //all numbers implicitly have an infinite number of 0s before them
  } else {
    return *(currentrightmostdigit--);
  }
}

bool getnextbit(){
  dputs("getnextbit");
  //We have the problem that we want to get bits (here returned as bools)
  //but a decimal digit has log base 2 of 10 bits, ie about 3.3 bits
  //so we have to be a bit clever
  static int bitsremaining = 0; //I think we actually need to expose this some how
  static int bitbuffer = 0;
  if(bitsremaining){
    bool bit  = bitbuffer & 1;
    bitbuffer = bitbuffer >> 1;
    bitsremaining--;
    return bit;
  } else {
    bitbuffer += chartoint(getnextdigit()); //I'm fairly certain + is the right operation to use here.
    bitsremaining += 3; //waitaminute that can't be right TODO: fix this calculation?
    //actually i think the above line still works, so long as we don't overflow int before we get to the end.
    return getnextbit(); //lol
  }
}

unsigned int intlog2(unsigned int logarand){ //rounds down
  dputs("intlog2");
  //dprintf("%u\n",logarand);
  int result = -1;
  while(logarand){
    result++;
    logarand>>=1;
  }
  dprintf("end intlog2 %u\n", result);
  return (unsigned)result;
}

int intpow(int base, unsigned int exponent){
  dputs("intpow");
  //dprintf("%u\n",logarand);
  int result = 1;
  while(exponent>0){
    result*=base;
    exponent--;
  }
  return result;
}

char *getnextword(){
  dputs("getnextword");
  const int size = sizeof(wordlist)/sizeof(wordlist[0]);
  dprintf("size %d\n", size);
  const int bitcount = intlog2(size)+1;
  dprintf("bitcount %d\n", bitcount);

  //We have the problem that we want to get words (here stored as char pointers)
  //but we don't necessary have a power of two number of words, which is what bits let us navigate
  //so we have to be a bit clever
  static int bitsremaining = 0;
  static int bitbuffer = 0;
  if(bitsremaining >= bitcount){ // checks if greater than or equal to log2 of ucpots
    int remainder = bitbuffer - size; // take extra bits out
    if(remainder >0){
      bitbuffer -= remainder; // TODO: will this lead to biasing of output?
    }
    dprintf("bitbuffer %d\n", bitbuffer);
    char *word =  wordlist[bitbuffer];
    //dputs(word);
    bitbuffer = bitbuffer >> bitcount; //shift by log2
    bitbuffer += remainder; //add extra bits back in
    bitsremaining-bitcount;
    return word;
  } else {
    bitbuffer << 1;
    bitbuffer += getnextbit(); //I'm fairly certain + is the right operation to use here.
    bitsremaining++; //waitaminute that can't be right TODO: fix this calculation?
    //actually i think the above line still works, so long as we don't overflow int before we get to the end.
    return getnextword(); //lol
  }
}

char *strend(char *s){ //get the pointer to the last (non-NU//L) character of a string
  while(*s){
    s++; //note that this only effects our copy. Hmm maybe I should remember how const works again.
  }
  return s-1; //s was the NUL so we return the one before it
}

int putchars(char *chars){ //puts without newline
  while(*chars){putchar(*chars++);};
}

int main(int argc, char **argv){
  if(argc<=1 || !argv[1] || !argv[1][0]){ //no arguments
    //null sentence
  } else {
    //assume digit to word for now
    currentleftmostdigit = argv[1];
    currentrightmostdigit = strend(argv[1]);
    //I don't like how you have to set this up and stuff but idk a cleaner way to do it man
    while(currentrightmostdigit>=currentleftmostdigit){
      dputs("main while");
      putchars(getnextword());
      putchar(' ');
    }
  }
  putchar('\n');
}