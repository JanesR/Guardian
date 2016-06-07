#ifndef SERIALCOMUNICATION_H_INCLUDED
#define SERIALCOMUNICATION_H_INCLUDED



#endif // SERIALCOMUNICATION_H_INCLUDED

#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART

class CommSerial
{
public:

    string arqSerial;
    int arqAberto = -1;
    struct termios serial;

    void CommSerial();
    void ~CommSerial();
    void recebe(char& mensagem);
    void envia(char& mensagem);
private:


};
