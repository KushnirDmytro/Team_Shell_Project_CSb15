#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <sstream>
#include <map>
#include <sys/mman.h>
#include <fcntl.h>

//========================CLASSES IMPORT==================
#include "Env/ConsoleView.h"
#include "Shell_core/EmbeddedFunc.h"
#include "Shell_core/Utils/LineSplitter.h"
#include "Shell_core/LaneInterpreter.h"
//====================CLASSES IMPORT END=====================


//==============================DEFINITIONS====================


using callable_function =  int (*)(size_t, char **);

//============================DEFINITIONS END======================




//=============FUNCTIONS AND STRUCTURES DECLARATIONS END =============


namespace sh_core {
    env::Env *environment;
    sh_core::LaneInterpreter *interpreter;
}


void error(const std::string &m) {
    std::string message("[server] ERROR " + m);
    perror(message.c_str());
    shm_unlink("/test_shmfd");
    exit(1);
}

void log(const std::string &m) {
    printf("[server] %s \n", m.c_str());
}

std::vector<std::string> split(const std::string &line) {
    // std::operator>> separates by spaces
    std::vector<std::string> words;
    std::string word;
    std::istringstream split(line);

    while (split >> word) {
        words.push_back(word);
    }

    return words;
}

int start_loop(int newsockfd) {
    int status = 1;
    char buffer[256];
    std::vector<std::string> received;
    std::string response;
    std::string str_buffer;

    /*initialization of Interpreter*/
    sh_core::environment = new env::Env();
    sh_core::interpreter = new sh_core::LaneInterpreter();

    do {
        /* If connection is established then start communicating */
        bzero(buffer, 256);

        if (read(newsockfd, buffer, 255) < 0) {
            error("reading from socket");
        }
//        std::cout << "at start_loop: buffer = "<<buffer << std::endl;

        str_buffer = buffer;
        received = split(buffer);

        if (strlen(str_buffer.c_str()) == 0) { //handling situation of empty line been red
            continue;

            //TODO handler for returning "0"
        }

        std::cout << "at start_loop: str_buffer = "<<str_buffer << std::endl;
        status = sh_core::interpreter->processSting(&str_buffer);
        std::cout << "at start_loop: status = "<<status << std::endl;

        response = "response";
        /* Write a response to the client
           * equal to send() without any flags as last arg
        */

        if (write(newsockfd, response.c_str(), response.size()) < 0) {
            error("writing to socket");
        }

    } while (!status);

    //=====================MEMORY CLEAN / SHUTDOWN==========================

    delete sh_core::interpreter;
    delete sh_core::environment;
    //=====================MEMORY CLEAN SHUTDOWN END==========================

    return status;
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, pid, execution_result_code;
    socklen_t clilen;

    struct sockaddr_in serv_addr, cli_addr;

    int shm_fd = shm_open("/test_shmfd", O_RDWR | O_CREAT | O_EXCL, 0666);
    if (shm_fd < 0)
        error("opening shm_open");

    /* Create socket file descriptor */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        error("opening socket");
    else
        log("Has opened socket");


    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));

    /* Getting portno from args*/
//    if(argc > 1)
//        portno = atoi(argv[1]); // converts string to int
//    else
    portno = 5002; // default port


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Bind the host address with file descriptor.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("on binding");
    else
        log("Has bind host address");


    /* Now start listening for the clients, here process will
       * go in sleep mode and will wait for the incoming connection
       * place all incoming connection into a backlog queue
       * until accept() call accepts the connection.
       * set the maximum size for the backlog queue to 5
    */
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (true) {
        /* Accept actual connection from the client
            * returns a new socket file descriptor for the accepted connection.
            * the original socket file descriptor can continue to be used
         */
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if (newsockfd < 0) {
            error("on accept");
        }

        char str[INET_ADDRSTRLEN];
        printf("[server] got connection from %s port %d\n",
               inet_ntop(AF_INET, &(cli_addr.sin_addr), str, INET_ADDRSTRLEN), portno);

        pid = fork();

        switch (pid) {
            case -1: //error
                error("on fork");
                break;
            case 0: //client process
                close(sockfd);
                execution_result_code = start_loop(newsockfd);
                shm_unlink("/test_shmfd");
                return execution_result_code;
            default:  //parent process
                close(newsockfd);
                break;
        }

    }
}