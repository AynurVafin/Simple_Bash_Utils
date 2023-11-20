#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<getopt.h>
#include<math.h>

typedef struct functions{
    int b;
    int e;
    int n;
    int s;
    int t;
    int v;
} fun;

const struct option long_analys[] = {{"-number-nonblank",0,0,'b'},
    {"-number",0,0,'n'}, {"-squeeze-blank",0,0,'s'}
};

void fun_read(int argc, char** argv, fun functions) {
    while( argc != optind) {
        int leng = 0, count_leng = 1, count_n = 2;
        int s, vl = 0;
        FILE *file_name = fopen(argv[optind], "r");
        optind ++;
        if (file_name != NULL) {
            while((s = fgetc(file_name)) != EOF) {
                if (s != '\n') count_n = 1;
                if (count_n >= 3 && functions.s) continue;
                if(!leng) {
                    if ((functions.b && s != '\n') || functions.n) {
                        printf("%6i\t", count_leng++);
                    } else {
                        if (functions.b && s == '\n'&& functions.e) {
                            printf("%6s\t", "");
                        }
                    }
                    leng=1;
                }
                if (s == '\n' && functions.e) printf("$");
                if (s == '\t' && functions.t) printf("^I");
                else {
                    if (s == '\n') { leng = 0; count_n++; }
                    if (functions.v) {
                        if (s > 127) {
                            printf("M-");
                            s-= 128;
                            if (s == '\n')  {printf("^J"); vl = 1;}
                            if (s == '\t') {printf("^I"); vl = 1;
                            }
                        }
                        if((s >= 0 && s < 32 && s != 10 && s != 9) ) {
                            printf("^%c", s+'A' - 1 );
                        }else if(s==127)
                            printf("^?");
                        else if (!vl)
                            printf("%c", s);
                        else vl = 0;
                    }
                    else
                        printf("%c", s);
                }
            }
        } else {
            fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[optind]);
        }
    }
}

int analysis(int argc, char** argv, fun * functions) {
    int symbol, flag = 1;
    int symbol_index, past_index = 1;
    int flags = 1;
    while((symbol = getopt_long(argc, argv, "+benstvET", long_analys, &symbol_index)) != -1) {
        if (abs(optind - past_index) > 1) flags = 0;
        else past_index = optind;
        if (flags)
            switch(symbol) {
                case 'b':functions->b = 1; break;
                case 'e':functions->e = 1; functions->v = 1; break;
                case 'n':functions->n = 1; break;
                case 's':functions->s = 1; break;
                case 't':functions->t = 1; functions->v = 1;break;
                case 'E':functions->e = 1; break;
                case 'T':functions->t = 1; break;
                case 'v':functions->v = 1; break;
                default: fprintf(stderr, "usage: cat [-benstuv] [file ...]\n"); flag = 0; break;
            }
    }
    optind = past_index;
    return flag;
}


int main(int argc, char ** argv){
    fun functions = {0};
    if (analysis(argc, argv, &functions)) {
        if (functions.b && functions.n) functions.n = 0;
        fun_read(argc, argv, functions);
    }
}

