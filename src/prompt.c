#include <stdio.h>
#include <stdlib.h>

#include "mpc.h"
#ifdef _WIN32
#include <string.h>

#define INPUT_MAX_SIZE 2048

static char buffer[INPUT_MAX_SIZE];

char* readline(char* prompt)
{
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char* cpy = malloc(strlen(buffer)+1);
    memset(cpy, 0, strlen(cpy));
    strcpy(cpy, buffer);
    cpy[strlen(cpy)-1] = '\0';
    return cpy;
}

void add_histroy(char* unused)
{
    //nothing to do
}
#endif

#ifdef __linux__
#include <editline/readline.h>
#include <editline/history.h>

#endif
#ifdef __MACH__
#include <editline/readline.h>
#endif

int main(int args, char** argv)
{
    mpc_parser_t* number = mpc_new("number");
    mpc_parser_t* operator = mpc_new("operator");
    mpc_parser_t* expr = mpc_new("expr");
    mpc_parser_t* lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT, 
    "                                                       \
    number      : /-?[0-9]+/;                               \
    operator    : '+' | '-' | '*' | '/';                    \
    expr        : <number> | '(' <operator> <expr>+ ')';    \
    lispy       : /^/ <operator> <expr>+ /$/;               \
    ", number, operator, expr, lispy);

    puts("Lispy Version 0.0.0.0.1");
    puts("Press Ctrl+c to Exit\n");

    /*Read Input from stdin in a Loop*/
    while(1)
    {
        char* input = readline("lisp> ");
        add_history(input);

        mpc_result_t r;
        if (mpc_parse("<stdin>", input, lispy, &r))
        {
            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
        printf("%s\n", input);
        free(input);
    }

    mpc_cleanup(4, number, operator, expr, lispy);

    return 0;
}