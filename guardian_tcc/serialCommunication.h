#include <stdio.h>
#include <iostream>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART

using namespace std;

class CommSerial
{
public:
    CommSerial();
    ~CommSerial();
    void recebe(char& mensagem);
    void envia(char& mensagem);
    void controlaPorta();

    char *arqSerial;
    int arqAberto;
    struct termios serial;

private:


};
