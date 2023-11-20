#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<regex.h>

struct functions{
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;
    int h;
    int s;
    int f;
    int o;
};

#define functions struct functions

int function_parser (char ** argv, functions *fun, int argc) {
    int flag_correct_work = 1, argv_count = 1;
    while (argv_count < argc&& flag_correct_work) {
        if (argv[argv_count][0] == '-') {
            for (unsigned long i = 1; i < strlen(argv[argv_count]) && flag_correct_work; i++) {
                switch(argv[argv_count][i]) {
                    case 'e': (fun->e) = 1;  i = strlen(argv[argv_count]); break;
                    case 'i': (fun->i) = 1; break;
                    case 'v': (fun->v) = 1; break;
                    case 'c': (fun->c) = 1; break;
                    case 'l': (fun->l) = 1; break;
                    case 'n': (fun->n) = 1; break;
                    case 'h': (fun->h) = 1; break;
                    case 's': (fun->s) = 1; break;
                    case 'f': (fun->f) = 1; i = strlen(argv[argv_count]);break;
                    case 'o': (fun->o) = 1; break;
                    default: flag_correct_work = 0; break;
                }
            }
        }
        argv_count++;
    }
    return flag_correct_work;
}

int read_from_file(char *file_name, char * ans_search, functions fun) {
    int flag = 1;
    FILE *file_template = fopen(file_name, "r");
    if (file_template != NULL) {
        char *copy_file;
        size_t Len;
        int result_input = getline(&copy_file, &Len, file_template);
        while (result_input != EOF) {
            if (copy_file && result_input != EOF) {
                if (ans_search[0] != 0)
                    strcat(strcat(ans_search, "|"), copy_file);
                else strcpy(ans_search,  copy_file);
            }
            result_input = getline(&copy_file, &Len, file_template);
        }
    } else {
        if (!fun.s)
            fprintf(stderr, "grep: %s: No such file or directory", file_name);
        flag = 0;
    }
    fclose(file_template);
    return flag;
}

char * search_template_function(functions fun, char ** argv, char file_name[256][256], char * ans_search) {
    int template = 0, q = 0, flag = 1;
    char  copy_search[10240] = {0};
    if (fun.e + fun.f == 0) {
        for (int  i = 1; argv[i]; i++) {
            if (argv[i][0] != '-' ) {
                if (!template) {
                    strcpy(ans_search, argv[i]);
                    template = 1;
                } else {
                    strcpy(file_name[q++], argv[i]);
                }
            }
        }
    } else {
        for (int  i = 1; argv[i]; i++) {
            if (argv[i][0] == '-') {
                unsigned long j = 1;
                while (j< strlen(argv[i]) && argv[i][j] != 'e' && argv[i][j] != 'f' ) {
                    j++;
                }
                if (j != strlen(argv[i])) {
                    if (j+1 != strlen(argv[i])) {
                        for (;j < strlen(argv[i]); j++) {
                            strcat(copy_search, &argv[i][j]);
                        }
                        if(argv[i][j] == 'e') if(ans_search[0] != 0) strcat(strcat(ans_search, "|"), copy_search); else
                            strcpy(ans_search,  copy_search);
                        else {
                            flag = read_from_file(copy_search, ans_search, fun);
                        }
                    } else {
                        if (argv[i][j] == 'e' && argv[i+1])
                            if(ans_search[0] != 0) strcat(strcat(ans_search, "|"), argv[i+1]); else {
                                strcpy(ans_search, argv[i+1]);
                            }
                            else {
                                flag = read_from_file(argv[i+1], ans_search, fun);
                            }
                        i++;
                    }
                }
            } else {
                strcpy(file_name[q++], argv[i]);
            }
        }
    }
    if (!flag)for(int i = 0; i< 10240; i++) ans_search[i] = 0;
    return ans_search;
}
                 
void work_template_output(functions fun, char *Line, regex_t Regex, char file_name[256][256], int ki, int count_su, int count_line, int eflag) {
    if (file_name[1][0] != 0 && ((!fun.h && !fun.c) || (count_su == 1 && fun.c && !fun.h))) {
        printf("%s:", file_name[ki]);
    }
    if (!fun.c) {
        if (fun.n && !fun.o) printf("%i:", count_line);
        if (fun.o) {
            int compare = 0, status_reg;
            regmatch_t Regmatch[10];
            if (fun.v) compare = REG_NOMATCH;
            if((status_reg = regexec(&Regex, Line, 1, Regmatch, eflag)) == compare) {
                if (fun.n ) printf("%i:", count_line);
                if (compare == REG_NOMATCH) {
                    printf("%s", Line);
                } else {
                    
                    for (int i = Regmatch[0].rm_so; i< Regmatch[0].rm_eo;i ++)
                        printf("%c", Line[i]);
                    printf("\n");
                    work_template_output(fun, Line+Regmatch[0].rm_eo, Regex, file_name, ki, count_su, count_line, REG_NOTBOL);
                }
            }
            
        } else {
            printf("%s", Line);
        }
    }
}
                 
void grep_work(functions fun, char ** argv, char file_name[256][256]) {
    char search_template[10240] = {0};
    search_template_function(fun, argv, file_name, search_template);
    regex_t Regex;
    int status_reg, cflags = 0, success, compare = 0, lk = 0;
    if (fun.v) compare = REG_NOMATCH;
    if (fun.i) cflags = REG_ICASE;
    if ((status_reg = regcomp(&Regex, search_template, REG_EXTENDED | cflags)) == 0) {
        for (int i = 0; file_name[i][0]; i++) {
            FILE *file = fopen(file_name[i], "r");
            if (file != NULL) {
                size_t Len;
                int count_line = 0, count_su = 0, result_input = 35;
                char * Line = NULL;
                result_input = getline(&Line, &Len, file);
                while (result_input != EOF) {
                    if (Line && result_input != EOF) {
                        count_line++;
                        success = regexec (&Regex, Line, 0, NULL, 0);
                        if (success == compare) {
                            count_su++;
                            if (!fun.l || fun.c)
                                work_template_output(fun, Line, Regex, file_name, i, count_su, count_line, 0);
                        }
                    }
                    if(Line[strlen(Line)-1] != '\n') lk = 1;
                    else lk = 0;
                    result_input = getline(&Line, &Len, file);
                    if (result_input == EOF && lk && fun.v && !fun.c && !fun.l && count_su!=0) printf("\n");
                }
                if (fun.c) {
                    if (fun.l ) {
                        if (file_name[1][0] && !fun.h)  printf("%s:", file_name[i]);
                        if (count_su >= 1) printf("1\n");
                        else printf("0\n");
                    } else {
                        if (count_su == 0 && file_name[1][0] && !fun.h)  printf("%s:", file_name[i]);
                        printf("%i\n", count_su);
                    }
                }
                if (fun.l && count_su) {
                    printf("%s\n", file_name[i]);
                }
                
                free(Line);
            } else {
                if (!fun.s)
                    fprintf(stderr, "grep: %s: No such file or directory", file_name[i]);
            }
            fclose(file);
            
        }
    } else {
        char Buf [1024];
        regerror(status_reg, &Regex, Buf, sizeof(Buf));
        fprintf(stderr, "%s", Buf);
    }
    regfree(&Regex);
}
                 
int main(int argc, char ** argv) {
    functions fun = {0};
    
    char file_name[256][256] = {{0}};
    if (function_parser(argv, &fun, argc)) {
        grep_work(fun, argv, file_name);
    } else {
        fprintf(stderr, "grep: invalid option");
    }
}
