// Allowed functions: malloc, free, write, close, fork, waitpid, signal, kill, exit, chdir, execve, dup, dup2, pipe, strcmp, strncmp
// --------------------------------------------------------------------------------------

// Write a program that will behave like executing a shell command
// - The command line to execute will be the arguments of this program
// - Executable's path will be absolute or relative but your program must not build a path (from the PATH variable for example)
// - You must implement "|" and ";" like in bash
// 	- we will never try a "|" immediately followed or preceded by nothing or "|" or ";"
// - Your program must implement the built-in command cd only with a path as argument (no '-' or without parameters)
// 	- if cd has the wrong number of argument your program should print in STDERR "error: cd: bad arguments" followed by a '\n'
// 	- if cd failed your program should print in STDERR "error: cd: cannot change directory to path_to_change" followed by a '\n' with path_to_change replaced by the argument to cd
// 	- a cd command will never be immediately followed or preceded by a "|"
// - You don't need to manage any type of wildcards (*, ~ etc...)
// - You don't need to manage environment variables ($BLA ...)
// - If a system call, except execve and chdir, returns an error your program should immediatly print "error: fatal" in STDERR followed by a '\n' and the program should exit
// - If execve failed you should print "error: cannot execute executable_that_failed" in STDERR followed by a '\n' with executable_that_failed replaced with the path of the failed executable (It should be the first argument of execve)
// - Your program should be able to manage more than hundreds of "|" even if we limit the number of "open files" to less than 30.

// for example this should work:
// $>./microshell /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell
// microshell
// i love my microshell
// $>

// Hints:
// Don't forget to pass the environment variable to execve

// Hints:
// Do not leak file descriptors!

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGS 100

typedef struct s_cmd
{
    //char *path;
    char **args;
    int argi;
    int fds[2];
    pid_t pid;
    struct s_cmd *next;
} t_cmd;


t_cmd *lstnew();
void lstclear(t_cmd *cmd);
void print_lst(t_cmd *pipeline, int level);
void xclose(int *fd);
void putstr(char *s);


t_cmd *parse(int *i, char **argv, char **env)
{
    (void)env;
    int pi = *i;
    t_cmd *pipeline = lstnew();
    t_cmd *cur = pipeline; 
    t_cmd *nxt;

    while(argv[++pi])
    {
        if (strncmp(argv[pi],";",2) == 0)
            break;
        if (strncmp(argv[pi],"|",2) == 0)
        {
            nxt = lstnew();
            cur->next = nxt;
            cur = nxt;
        }
        //|か;になるまでargvを進める
        else
        {
            cur->args[cur->argi++] = argv[pi];
        }
    }
    *i = pi;
    #ifdef DEBUG_SEMIC
    printf("%s iter: %d\n",__func__,*i);
    #endif
    return pipeline;
}
// 	- if cd has the wrong number of argument your program should print in STDERR "error: cd: bad arguments" followed by a '\n'
// 	- if cd failed your program should print in STDERR "error: cd: cannot change directory to path_to_change" followed by a '\n' with path_to_change replaced by the argument to cd
// 	- a cd command will never be immediately followed or preceded by a "|"
//移動しないけど、、、多分大丈夫？
void builtin_cd(t_cmd *pipeline)
{
    #ifdef DEBUG
        putstr("builtin cd\n");
    #endif
    if (pipeline->argi != 2)
        return (putstr("error: cd: bad arguments\n"));
    //printf("%s",pipeline->args[1]);
    if (chdir(pipeline->args[1]))
        return (
            putstr("error: cd: cannot change directory to "),
            putstr(pipeline->args[1]),
            putstr("\n")
            );
     #ifdef DEBUG
        putstr("builtin cd done\n");
    #endif
    return ;
}


// - If execve failed you should print "error: cannot execute executable_that_failed" in STDERR followed by a '\n' with executable_that_failed replaced with the path of the failed executable (It should be the first argument of execve)
void exec_cmd(t_cmd *cmd, int fd_in, int fd_out, char **env)
{
    #ifdef DEBUG
        putstr((char *)__func__);
    #endif
    if (!cmd->args[0])
        return ;
    cmd->pid = fork();
    if (cmd->pid == -1)
        return(putstr("error: fatal\n"), xclose(&fd_in), xclose(&fd_out));
    if (cmd->pid == 0)
    {
        if (fd_in != -1)
            dup2(fd_in, STDIN_FILENO);xclose(&fd_in);
        if (fd_out != -1)
            dup2(fd_out, STDOUT_FILENO);xclose(&fd_out);
        execve(cmd->args[0], cmd->args, env);
        putstr("error: cannot execute ");
        putstr(cmd->args[0]);
        putstr("\n");
        #ifdef TEST
            putstr("error in execve\n");
        #endif
        exit(1);
    }

}


void  exec(t_cmd *pipeline, char **env)
{
    if (!pipeline || !pipeline->args[0])
        return ;
    if (strncmp(pipeline->args[0],"cd",3) == 0)
        return (builtin_cd(pipeline));
    int fd_in = -1;
    // int fd_out;
    int pipefd[2];
    while(pipeline->next)
    {
        if (pipe(pipefd))
            return (putstr("error: fatal\n"));
        //                　読み込み　　書き込み
        exec_cmd(pipeline, fd_in, pipefd[1], env);
        xclose(&pipefd[1]);
        xclose(&fd_in);
        //次のコマンドの準備
        fd_in = pipefd[0];
        pipeline = pipeline->next;
    }
    //最後のコマンドだけパイプしないで実行
    exec_cmd(pipeline, fd_in, -1, env);

    return;
}


/**** 
 MAIN 
 ****/

int main(int argc, char **argv, char **env)
{
    // (void)argc;
    //./microshell /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell
    int i = 0;
    t_cmd *pipeline;
    //parse
    while(i<argc)
    {
        pipeline = parse(&i, argv, env);
        #ifdef DEBUG_SEMIC
            print_lst(pipeline,0); 
        #endif
        exec(pipeline, env);
        lstclear(pipeline);
    }

    return 0;
}



t_cmd *lstnew()
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    cmd->argi = 0;
    cmd->args = malloc(sizeof(char *) * 100);
    cmd->args[0] = NULL;
    cmd->fds[0] = -1;
    cmd->fds[1] = -1;
    cmd->next = NULL;

    return cmd;
}

void xclose(int *fd)
{
    if (*fd == -1)
        return ;
    if(close(*fd))
    {
        putstr("error: fatal\n");
        #ifdef TEST
            putstr("error in close\n");
        #endif
    }
    *fd = -1;
}

void lstclear(t_cmd *cmd)
{
    #ifdef DEBUG
        putstr((char *)__func__);
    #endif

    if(!cmd)
        return ;
    lstclear(cmd->next);
    free(cmd->args);
    waitpid(cmd->pid, NULL, 0);
    xclose(&cmd->fds[0]);
    xclose(&cmd->fds[1]);
}

#ifdef DEBUG_SEMIC
void print_lst(t_cmd *pipeline,int level)
{
    if (!pipeline)
        return ;
    for (int i = 0; i < level; i++)
    {
        printf("   ");   
    }
    printf("path: %s argi %d\n",pipeline->args[0],pipeline->argi);
    print_lst(pipeline->next, ++level);
}
#endif


void putstr(char *s)
{
    while(*s)
        write(2, s++, 1);
}