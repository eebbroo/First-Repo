#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


void read_cmd(void);
void tokenizer(char *buf, char *buf_cpy);
void exec_cmd(char **argv);


int main(int argc, char **argv)
{
        read_cmd();

        return (0);
}

/* READING FROM A COMMAND LINE */
void read_cmd(void)
{
        size_t buf_size;
        ssize_t nread;
        char *buf = NULL;
        char *buf_cpy = NULL;

        printf("input command: ");

        /*read a string from the standard input*/
        nread = getline(&buf, &buf_size, stdin);

        /*CHECK getline function for EOF-condition*/
        if (nread == -1)
        {
                printf("\n.....logging out\n");
                return;
        }
        buf_cpy = malloc(sizeof(char) * nread);
        strcpy(buf_cpy, buf);

        tokenizer(buf, buf_cpy);

        free(buf);
        free(buf_cpy);
}
/*PROCESS THE STRING/TEXT THAT WAS READ--- strtok*/
void tokenizer(char *buf, char *buf_cpy)
{
        char *token;
        char delim[] = " \n";
        char **argv = NULL;
        int num_tok = 0, i;

        token = strtok(buf, delim);
        while(token)
        {
                num_tok++;
                token = strtok(NULL, delim);
        }
        ++num_tok;
        argv = malloc(sizeof(char *) * num_tok);
        token = strtok(buf_cpy, delim);
        for (i = 0; token; i++)
        {
                argv[i] = malloc(sizeof(char) * strlen(token));
                strcpy(argv[i], token);
                token = strtok(NULL, delim);

                if(token == NULL)
                        argv[++i] = NULL;
        }
        exec_cmd(argv);
        free(argv);
        free(token);
}

/* EXECUTE COMMANDS*/
void exec_cmd(char **argv)
{
        pid_t pid;

        pid = fork();
        if(pid != 0)
        {
                 wait(NULL);
                 read_cmd();
        }
        else
        {
                if(execve(argv[0], argv, NULL) == -1)
                        perror("Input new command");
        }
}
