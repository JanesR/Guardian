#include "mainwindow.h"
#include "ui_mainwindow.h"

//biblioteca para comtrole de io
#include <iostream>
//importação para usar vetores
#include <vector>
//bibliotecas do opencv
#include <time.h>
#include <unistd.h>
#include <cstdlib>
#include <pthread.h>
#include <opencv2/core.hpp>
#include <opencv2/face.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "recoFace.h"
#include "GPIOClass.h"
#include "serialCommunication.h"

using namespace cv;
using namespace cv::face;
using namespace std;


    GPIOClass gpio;
    CommSerial comunicaPIC;
    recoFace reconhecimento;
    Mat teste;
    vector<Mat> imagens;
    vector<int> labels;
    char separador = ';';
    string arquivoReco = "/home/pi/Documents/guardian.csv";


void reconhecimentoFacial()
{


    try
    {

      reconhecimento.capturaFoto(teste ,imagens , labels);

    }
    catch(cv::Exception& e)
    {
        cerr << "Erro ao tentar reconhecer pessoa. Erro:\n"  << e.msg << endl;
        exit(1);
    }


}

void cadastroDePessoa()
{
    int id = 1;
    bool identifica= false;
    //recoFace reconhecimento; //cria objeto para a classe reconhecimento facial
    try
    {
        reconhecimento.cadastraPessoa(id, identifica);
    }
    catch(cv::Exception& e)
    {
        cerr << "Erro tetnar cadastrar! Erro: \n"<< e.msg<< endl;
        exit(1);
    }

}

void *iniReco(void *valor)
{
    cout << valor;

      cout << "Inicializando os Modulos Faciais, favor aguardar!\n\n";

      try
      {
        //inicializa a classe informando o arquivo
        reconhecimento.infArq(arquivoReco);
            //solicita a leitura do arquivo
        reconhecimento.leArqivo(imagens , labels , separador);

      }
      catch(cv::Exception& e)
      {
        cerr << "Erro ao tentar reconhecer pessoa. Erro:\n"  << e.msg << endl;
        exit(1);
      }

      cout<<"Modulos Faciais Inicializados!\n\n";
      sleep(2);
    pthread_exit(0);

}

void *inicializaGPIO(void *valor)
{
    cout << valor;
      cout << "Inicializando GPIO da Placa, favor aguardar!\n\n";

      try
      {
        GPIOClass* gpio4 = new GPIOClass("4"); //create new GPIO object to be attached to  GPIO4
        GPIOClass* gpio17 = new GPIOClass("17"); //create new GPIO object to be attached to  GPIO17

        gpio4->export_gpio(); //export GPIO4
        gpio17->export_gpio(); //export GPIO17

        cout << " GPIO pins exported" << endl;

        gpio17->setdir_gpio("in"); //GPIO4 set to output
        gpio4->setdir_gpio("out"); // GPIO17 set to input

        cout << " Set GPIO pin directions" << endl;

      }
      catch(cv::Exception& e)
      {
          cerr << "Erro ao tentar Inicializar modulos!. Erro:\n"  << e.msg << endl;
          exit(1);
      }
        cout<<"Modulos de GPIO Inicializados!\n\n";
        sleep(2);

        pthread_exit(0);
}

void *iniUART(void *valor)
{
        cout << "Thread "<< valor << "\n";

        comunicaPIC.controlaPorta();

        pthread_exit(0);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);
    //pthread_t gp, rec;
        int x = 0;
        int y = 1;
        int z = 2;

        pthread_t thread[3];

    pthread_create(&thread[x],NULL,iniReco, (void *) x);
    pthread_create(&thread[y],NULL,inicializaGPIO, (void *) y);
    pthread_create(&thread[z],NULL,iniUART, (void *) z);

    pthread_join(thread[x], NULL);
    pthread_join(thread[y], NULL);
    pthread_join(thread[z], NULL);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_RecoBtn_3_clicked()
{
    reconhecimentoFacial();
}

void MainWindow::on_AlternativeBtn_3_clicked()
{

}
