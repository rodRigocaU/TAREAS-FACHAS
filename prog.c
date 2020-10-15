#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#define MAX_LINE 80 

int ultiE = 0;
char history[40][40];



char **get_input(char *input) {
    
    char **argv2 = malloc(MAX_LINE/2 + 1)/**[MAX_LINE/2 + 1]*/;
    char **argv3 = malloc(MAX_LINE/2 + 1);

    char *separator = " ";
    char *separator2 = " ";

    char *input2; 
    strcpy(input2,input);

    char *parsed;
    char *parsed2;

    int index = 0;
    int index2 = 0;

      if(strcmp(input,"!!") == 0)
        {
          //printf("%s\n", history[ultiE - 1]);
          strcpy(input,history[ultiE - 1]);
        }
      else
      {
        strcpy(history[ultiE],input);
        ultiE++;
        if(ultiE > 30)
        {
          ultiE = 39;
        }
      }
      
    //PARA LA SALIDA DE ARCHIVOS
    int i = 0; 
    int j = 0;
    int k = 0;
    int x = 0;
    int lens = 0;
  
    //char* aux3[strlen(input)];
    char spa = ' ';
    char may = '>';
    char men = '<';


    //REDIRECCIONAR >

    parsed2 = strtok(input2, separator2);

    while (parsed2 != NULL) {
        argv3[index2] = parsed2;
        index2++;
        parsed2 = strtok(NULL, separator);
    }

    argv3[index2] = NULL;

    strcpy(input2,argv3[0]);
    if(index2 >= 2 && strcmp(argv3[1],">") == 0)
        {
         // printf("%s\n","ENTRE");
            int out = open(argv3[2],O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
            //printf("%s\n","sali"); 

            if(dup2(out,STDOUT_FILENO) == -1)
            {
                perror("fallo");
                exit(EXIT_FAILURE);
            }
            else
            {
              strcpy(input,input2);
            }
            
            dup2(out,STDOUT_FILENO);
            //printf("%s\n","sali"); 
        }
    else if(index2 >= 2 && strcmp(argv3[1],"<") == 0)
        {
            printf("%s\n","ENTRE");
            strcpy(input,argv3[0]);
            strcat(input," ");
            strcat(input,argv3[2]);           
        }  
    else if(strcmp(argv3[0],"stopp") == 0)
    {
        int pids = atoi(argv3[1]);
        kill(pids, SIGSTOP);
    }
    else if(strcmp(argv3[0],"cctn") == 0)
    {
        int pids = atoi(argv3[1]);
        kill(pids, SIGCONT);
    }
    printf("%s\n",input);  
    

    //signal


    parsed = strtok(input, separator);

    while (parsed != NULL) {
        argv2[index] = parsed;
        index++;
        parsed = strtok(NULL, separator);
    }

    argv2[index] = NULL;

    if(strcmp(argv2[0],"h") == 0)
        {
            int i;
            for(i = 0; i < ultiE;i++)
            {
              printf("%x\t", i + 1);
              printf("%s\n",history[i]);
            }
        }
    return argv2;
}


int main(void)
{
                
        int tam = 0;
        char **argv;

        char *inputString;
        int should_run = 1; 
        pid_t pid;
        int stat_loc;

        while (should_run)
        {
            printf("osh>");
            fflush(stdout);
            inputString = readline("");
            argv = get_input(inputString);
            
            if (!argv[0]) 
            {   
              free(inputString);
              free(argv);
              continue;
            }

            pid = fork();
            if(pid == 0)
            {
                execvp(argv[0],argv); 
            }
            else
            {
               waitpid(pid,&stat_loc,WUNTRACED);
            }
            free(inputString);
            free(argv);
        }
return 0;
}


