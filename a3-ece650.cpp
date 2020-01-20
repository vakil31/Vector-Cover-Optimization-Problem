#include <unistd.h>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <cstdlib>
#include <time.h>       /* time */
#include <stdlib.h>

#include<vector>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
//#include "using_rand.cpp"

/// Entry point of process A
/*int procA(void) {
    // Process A writing to C
    while (!std::cin.eof()) {
        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);
        if (line.size () > 0)
            std::cout << line << std::endl;
    }
   std::cout << "[B] saw EOF" << std::endl;
    return 0;
}
int main(int argc,char **argv)
{
    std::vector<pid_t> kids;
    // create a pipe
    int ABtoC[2];
    pipe(ABtoC);


    pid_t child_pid;
    child_pid = fork ();
    /*if (child_pid == 0)
    {
        // redirect stdout to the pipe
        dup2(ABtoC[1], STDOUT_FILENO);
        close(ABtoC[0]);
        close(ABtoC[1]);     // Close this too!
        int rgen=execv("./using_rand",argv);
        // start process A
        return rgen;
    }
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }
     
    kids.push_back(child_pid);

    child_pid = fork();*/
  /*  if (child_pid == 0)
    {
        // redirect stdin from the pipe
        dup2(ABtoC[0], STDIN_FILENO);
        close(ABtoC[1]);
        close(ABtoC[0]);
        char *argv1[3];
        argv1[0]=(char *)"python";
        argv1[1]=(char *)"./a1-ece650.py";
        argv1[2]=nullptr;
        //Copied
        dup2(ABtoC[1], STDOUT_FILENO);
        close(ABtoC[0]);
        close(ABtoC[1]);     // Close this too!
        // start process C
        return execvp("python",argv1);
    }
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }
    if (child_pid == 0)
    {
        // redirect stdin from the pipe
        dup2(ABtoC[0], STDIN_FILENO);
        close(ABtoC[1]);
        close(ABtoC[0]);
        
        //Copied
        //dup2(ABtoC[1], STDOUT_FILENO);
        //close(ABtoC[0]);
        //close(ABtoC[1]);     // Close this too!
        // start process C
        return execv("a2-ece650",argv);
    }
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }
    kids.push_back(child_pid);
    child_pid = 0;

    // redirect stdout to the pipe
    dup2(ABtoC[1], STDOUT_FILENO);
    close(ABtoC[0]);
    close(ABtoC[1]);

    // start process B
    int res =  procA();

    // send kill signal to all children
    for (pid_t k : kids) {
        int status;
        kill (k, SIGTERM);
        waitpid(k, &status, 0);
    }

    // exit with return code of process B
    return res;

}*/
int procA(void) {
    // Process A writing to C
    while (!std::cin.eof()) 
    {
        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);
        if (line.size () > 0)
        {
            std::cout << line << std::endl;
            //std::cout.flush();
        }
    }
   //std::cout << "[A] saw EOF" << std::endl;
   return 0;
}
int main(int argc,char **argv)
{
    // create a pipe
    int pipe1[2];
    pipe(pipe1);
    int pipe2[2];
    pipe(pipe2);
    pid_t child_pid1;
    pid_t child_pid2;
    pid_t child_pid3;       
    child_pid1 = fork ();
    if (child_pid1 == 0)
    {
        // redirect stdout to the pipe
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[0]);
        close(pipe1[1]);     // Close this too!

        // redirect stdout to the pipe
        return execv("rgen",argv);
    }
    else if (child_pid1 < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }


    child_pid2 = fork();
    if (child_pid2 == 0)
    {
        // redirect stdin from the pipe
        dup2(pipe1[0], STDIN_FILENO);
        close(pipe1[1]);
        close(pipe1[0]);
        
        char *argv1[3];
        argv1[0]=(char *)"python";
        argv1[1]=(char *)"a1-ece650.py";
        argv1[2]=nullptr;
        // redirect stdin from the pipe
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe2[0]);
        close(pipe2[1]);     // Close this too!
        return execvp("python",argv1);
    }
    else if (child_pid2 < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }

    child_pid3 = fork();
    if (child_pid3 == 0)
    {
        // redirect stdin from the pipe
        dup2(pipe2[0], STDIN_FILENO);
        close(pipe2[1]);
        close(pipe2[0]);
        return execv("a2-ece650",argv);
    }
    else if (child_pid3 < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }

    // redirect stdout to the pipe
    dup2(pipe2[1], STDOUT_FILENO);
    close(pipe2[0]);
    close(pipe2[1]);

    // start process A
    int res =  procA();

    // send kill signal to all children
    int status;
    kill (child_pid1, SIGTERM);
    waitpid(child_pid1, &status, 0);
    kill (child_pid2, SIGTERM);
    waitpid(child_pid2, &status, 0);
    kill (child_pid3, SIGTERM);
    waitpid(child_pid3, &status, 0);
    // exit with return code of process B
    return res;

}