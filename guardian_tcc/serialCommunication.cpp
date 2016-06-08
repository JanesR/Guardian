#include "serialCommunication.h"

#include <cstdlib>

#include <stdio.h>
#include <iostream>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <pthread.h>


/*
    POR PADRÃO OS PINOS 8 E 10 DA RASPI
    SÃO OS PINOS DE RX/TX
*/

using namespace std;

CommSerial::CommSerial()
{
    arqSerial =  "/dev/ttyAMA0";
    cout << arqSerial << "\n";
    arqAberto = open(arqSerial, O_RDWR | O_NOCTTY | O_NDELAY);

    if(arqAberto == -1)
    {
        cerr << "Nao foi possivel acessar a porta serial!" << endl;
       // exit(1);
    }
    else
    {
        cout << "Serial Aberta. \n Configurando... \n";

        tcgetattr(arqAberto, &serial);
        serial.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
        serial.c_iflag = IGNPAR;
        serial.c_oflag = 0;
        serial.c_lflag = 0;
        tcflush(arqAberto, TCIFLUSH);
        tcsetattr(arqAberto, TCSANOW, &serial);

         cout << "Configuracoes aplicadas. \n";
    }
}

CommSerial::~CommSerial()
{
//----- CLOSE THE UART -----
    close(arqAberto);
}

void CommSerial::controlaPorta()
{

}


void CommSerial::envia(char& mensagem)
{
    while(true)
    {
        //----- TX BYTES -----
        unsigned char tx_buffer[256];
        unsigned char *p_tx_buffer;

        p_tx_buffer = &tx_buffer[0];
        *p_tx_buffer++ = mensagem;

        if (arqAberto != -1)
        {
            int count = write(arqAberto, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
            if (count < 0)
            {
                cout << "UART TX error\n";
            }
        }
    }

}

void CommSerial::recebe(char& mensagem)
{
    while(true)
    {
        //----- CHECK FOR ANY RX BYTES -----
            if (arqAberto != -1)
            {
                //Leitura de 255 caracteres
                unsigned char rx_buffer[256];

                int rx_length = read(arqAberto, (void*)rx_buffer, 255);		//Filestream, buffer to store in, number of bytes to read (max)
                if (rx_length < 0)
                {
                    //An error occured (will occur if there are no bytes)
                }
                else if (rx_length == 0)
                {
                    //No data waiting
                }
                else
                {
                    //Bytes received
                    rx_buffer[rx_length] = '\0';
                    printf("%i bytes read : %s\n", rx_length, rx_buffer);
                }
            }

    }

}
