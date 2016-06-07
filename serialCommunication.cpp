#include "serialComunication.h

#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART

#define SERIAL
/*
    POR PADRÃO OS PINOS 8 E 10 DA RASPI
    SÃO OS PINOS DE RX/TX
*/


void CommSerial()
{
    arqSerial = "/dev/ttyAMA0";
    cout << "Abrindo serial! \n";

    arqAberto = open(arqSerial, O_RDWR | O_NOCTTY | O_NDELAY);

    if (arqAberto = -1)
    {
        cerr << "Nao foi possivel acessar a porta serial!" << endl;
        exit(1);
    }
    else
    {
        cout << "Serial Aberta. \n Configurando... \n"

        tcgetattr(arqAberto, &options);
        options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        tcflush(arqAberto, TCIFLUSH);
        tcsetattr(arqAberto, TCSANOW, &options);

         cout << "Configuracoes aplicadas. \n"
    }
}

void ~CommSerial()
{
//----- CLOSE THE UART -----
	close(arqAberto);
}

void CommSerial::envia(char& mensagem)
{
	//----- TX BYTES -----
	unsigned char tx_buffer[256];
	unsigned char *p_tx_buffer;

	p_tx_buffer = &tx_buffer[0];
	*p_tx_buffer++ = 'H';
	*p_tx_buffer++ = 'e';
	*p_tx_buffer++ = 'l';
	*p_tx_buffer++ = 'l';
	*p_tx_buffer++ = 'o';

	if (arqAberto != -1)
	{
		int count = write(arqAberto, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
		if (count < 0)
		{
		    cout <<"UART TX error\n";
		}
	}
}

void CommSerial::recebe(char& mensagem)
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
