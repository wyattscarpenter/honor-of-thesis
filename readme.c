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
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "wordlist.h"

#define DEBUG 0

#define dputs(s) if(DEBUG){puts(s);}
#define dprintf(...) if(DEBUG){fprintf (stderr, __VA_ARGS__); fflush(stdout);}

long long unsigned int bignumber = 0;

const int size = sizeof(wordlist)/sizeof(wordlist[0]);

bool streq(const char *str1, const char *str2){
  return !strcmp(str1, str2);
}

void putchars(char *chars){ //puts without newline
  while(*chars){putchar(*chars++);};
}

void embiggenificate(char * word){
  static long long unsigned int embiggenificationcoef = 1;
  for(int i = 0; i < size; i++){
    if(streq(wordlist[i], word)){
      bignumber+=i*embiggenificationcoef;
      embiggenificationcoef*=size;
      break;
    }
  }
}

int main(int argc, char **argv){  
  if(argc<=1 || !argv[1] || !argv[1][0]){ //no arguments
    putchar('0');
  } else if (isdigit(argv[1][0])) {
    bignumber = /*(ong long unsigned int)*/atoll(argv[1]);
    dprintf("bignumber %llu\n", bignumber);
    dprintf("size %d\n", size);
    while(bignumber){
      putchars(wordlist[bignumber%size]);
      putchar(' ');
      bignumber /= size;
    }
  } else {
    //sentence to digit
    //check all arguments so the user can use bare words
    int i = 1;
    while(argv[i]){
      char *word = strtok(argv[i]," ");
      while(word){
        dputs(word);
        embiggenificate(word);
	word = strtok(NULL," ");
      }
      i++;
    }
    printf("%llu", bignumber);
  }
  putchar('\n');
}