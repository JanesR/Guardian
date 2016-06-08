#include "recoFace.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <cmath>
#include <dirent.h>
#include <opencv2/core.hpp>
#include <opencv2/face.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <QMessageBox>

using namespace cv;
using namespace cv::face;
using namespace std;


void recoFace::infArq(string arquivo)
{
	localArquivo = arquivo;
}

void recoFace::leArqivo(vector<Mat>& imagens, vector<int>& labels, char separador)
{

	ifstream leitor(localArquivo.c_str(), ifstream::in);

	if(!leitor)
	{
		cout << "Nao foi possivel abrir o Arquivo";
	}
	else
	{
		cout << "Lendo o arquivo de treino em: " << localArquivo << "\n\n";

		string aux,foto,label;
		Mat fotoReconhecer, rosto;

		while(getline(leitor,aux))
		{
		    //cria stream para trabalhar com a string do endereço da foto
		    stringstream linha(aux);
		    //set do endereço da foto
		    getline(linha, foto, separador);
		    //set do id da foto na variavel label
            getline(linha, label);

            if(!foto.empty() && !label.empty())
            {
                fotoReconhecer = imread(foto, CV_LOAD_IMAGE_GRAYSCALE);
                localizaRosto(fotoReconhecer,rosto);
                imagens.push_back(rosto);
                labels.push_back(atoi(label.c_str()));
            }

		}

		cout<<"Reconhecedor sendo Criado \n\n";

        //recModelo = createEigenFaceRecognizer(0,10000.0);
        recModelo = createEigenFaceRecognizer();

        cout<<"Reconhecedor criado \n\n";

        cout<<"Inicializando Treinamento\n\n";

        recModelo->train(imagens, labels);

        cout<<"Treinamento finalizado!\n\n";

        cout<<"Modulos Inicializados! \n\n";



	}
}

bool recoFace::reconhePessoa(Mat& teste, vector<Mat>& imagens, vector<int>& labels)
{
    int predicao = -1;
    double confianca = 0.0;
    bool retorno = false;

    try
    {
        recModelo->predict(teste, predicao, confianca);

        if(predicao>2 )
        {
            cout <<"Nao reconhecido!\n\n";
            retorno = false;
        }
        else
        {
             cout<<"Foi identidicado como sendo a pessoa: "<<predicao<<" \n Valor de condianca: "<<confianca<<" \n\n";
             sleep(5);
           retorno = true;
        }

    }
    catch(cv::Exception &e)
    {
     //   cout<< e.msg;
    }


    return retorno;
}

void recoFace::capturaFoto(Mat& teste,  vector<Mat>& imagens, vector<int>& labels)
{

    int cameraId = 0;
    bool reconhecimento = false;
    VideoCapture captura;
    Mat frame, faceMat;

    namedWindow("guardian",WINDOW_FULLSCREEN);

    captura.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    captura.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    captura.open(cameraId);

    while(!reconhecimento)
    {
        if(!captura.isOpened())
        {
            QMessageBox erro;
            erro.setText("ERRO: Nao foi possivel acessar a camera! \n\n");
            erro.exec();
            reconhecimento = true;
        }
        else
        {

            captura.read(frame);
            frame.copyTo(teste);

            localizaRosto(frame, faceMat);
            imshow("guardian",frame);
            waitKey(1);


            if(imagens.size() >= 2)
            {
                reconhecimento = reconhePessoa(faceMat,imagens,labels);
            }

        }
    }

    destroyAllWindows();

}

void recoFace::localizaRosto(Mat& foto, Mat& imageCompare)
{
    String classif = "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml";

    vector<Rect> faces;
    CascadeClassifier classificador;
    Mat rosto, aux;

    if(!classificador.load(classif))
    {
        cout<<"nao foi possivel acessar o classificador \n\n";
        sleep(2);
        exit(1);
    }
    else
    {
        faces.clear();
        classificador.detectMultiScale( foto, faces, 1.1, 2, CV_HAAR_FIND_BIGGEST_OBJECT, Size(30, 30) );

        if(faces.size()>0)
        {
            for(unsigned int i = 0;  i < faces.size() ; i++)
            {
                    Rect face_i = faces[i];

                    rectangle(foto , face_i, CV_RGB(0,255,0), 1);
                    imageCompare = foto(faces[i]);

                    Size tamanhoFrame;
                    tamanhoFrame.width = 90;
                    tamanhoFrame.height = 120;


                    Mat dst;
                    switch(imageCompare.channels())
                    {
                        case 1:
                            //normalize(imageCompare, dst, 0, 255, NORM_MINMAX, CV_8UC1);
                            //imageCompare.copyTo(dst);
                            equalizeHist(imageCompare,dst);
                        break;
                        case 2:
                            normalize(imageCompare,dst,0,255,NORM_MINMAX,CV_8UC2);
                            cvtColor(dst,dst,CV_BGR2GRAY,1);
                            equalizeHist(dst,dst);
                            break;
                        case 3:
                            normalize(imageCompare, dst, 0, 255, NORM_MINMAX, CV_8UC3);
                            cvtColor(dst,dst,CV_BGR2GRAY,1);
                            equalizeHist(dst,dst);
                        break;
                        case 4:
                            normalize(imageCompare, dst, 0, 255, NORM_MINMAX, CV_8UC4);
                            cvtColor(dst,dst,CV_BGR2GRAY,1);
                            equalizeHist(dst,dst);
                        break;
                        default:
                            imageCompare.copyTo(dst);
                            cvtColor(dst,dst,CV_BGR2GRAY,1);
                            equalizeHist(dst,dst);
                        break;
                    }
                    GaussianBlur(dst,dst,Size(1,1),0,0);
                    resize(dst,imageCompare,tamanhoFrame,0,0,INTER_LINEAR);


            }

        }
    }

}

void recoFace::rotaFace(Mat& rosto)
{
    String arqClassOlhos = "/usr/local/share/OpenCV/haarcascades/haarcascade_eye.xml";
    CascadeClassifier classOlhos;

    vector<Rect> olhos;

    int xFace[2];
    int yFace[2];

    float angGiro = 0.0;

    if(!classOlhos.load(arqClassOlhos))
    {
        cout<<"Nao foi possivel encontrar o classificador.";
        exit(1);
    }
    else
    {
        olhos.clear();
        classOlhos.detectMultiScale( rosto, olhos, 1.1, 2, CV_HAAR_SCALE_IMAGE, Size(1, 1),Size(30,30) );

        if(olhos.size()>1)
        {
            for(unsigned int i = 0; i < olhos.size(); i++)
            {
                xFace[i] = olhos[i].x;
                yFace[i] = olhos[i].x;
            }

            angGiro = atan( (float)yFace[1]-(float)yFace[0]/(float)xFace[1]-(float)xFace[0] );

            Mat faceRotate;
            Mat imgRotate;

            faceRotate = getRotationMatrix2D(Point2f(rosto.rows/2,rosto.cols/2),angGiro,1);
            warpAffine(rosto,imgRotate,faceRotate,rosto.size());
            rosto = imgRotate;

        }

    }
}

void recoFace::cadastraPessoa(int codUser, bool atualiza)
{

    if(atualiza)
    {
        string folder = "mkdir /home/guardian/Documents/face_data/r";
        string aux;
        stringstream s1;
        s1 << codUser;
        aux = s1.str();
        folder+= aux;

        char comando[1024];

        strncpy(comando, folder.c_str(), sizeof(comando));
        system(comando);
        sleep(2);
    }


    cout << "Inicializando cadastro! \n Por favor siga as instruções!\n\n";


    int cameraId = 0;
    bool lendo = true;
    VideoCapture captura;
    Mat frame, faceMat,teste;
    vector<string> fotos;

    captura.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    captura.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    captura.open(cameraId);

    while(lendo)
    {
        if(!captura.isOpened())
        {
            cerr<<"ERRO: Nao foi possivel acessar a camera! \n\n"<< endl;
            sleep(2);
            break;
        }
        else
        {
            for(int i=1; i<= 10; i++)
            {
                switch(i)
                {
                    case 1:
                    cout<<"Sorria! \n";
                    sleep(3);
                    break;

                    case 2:
                    cout<<"Faça cara seria! \n";
                    sleep(3);
                    break;

                    case 3:
                    cout<<"Faça cara de triste! \n";
                    sleep(3);
                    break;

                    case 4:
                    cout<<"Faça cara de bravo! \n";
                    sleep(3);
                    break;

                    case 5:
                    cout<<"Vire para direita! \n";
                    sleep(3);
                    break;

                    case 6:
                    cout<<"Vire para esquerda! \n";
                    sleep(3);
                    break;

                    case 7:
                    cout<<"Olhe para cima! \n";
                    sleep(3);
                    break;

                    case 8:
                    cout<<"Olhe para baixo! \n";
                    sleep(3);
                    break;

                    case 9:
                    cout<<"Faça uma careta! \n";
                    sleep(3);
                    break;

                    case 10:
                    cout<<"Sorria novamente! \n";
                    sleep(3);
                    break;
                }
                captura.read(frame);
                frame.copyTo(teste);
                localizaRosto(frame, faceMat);
                stringstream ss, ss2;
                ss << i;
                ss2 << codUser;

                string cap = "/home/pi/Documents/face_data/r";
                cap+= ss2.str();
                cap+= "/captura ";
                cap+= ss.str() ;
                cap+= ".png";

                cout << cap << "\n";

                imwrite(cap,faceMat);

                cap+=";";

                fotos.push_back(cap);
                imshow("guardian", frame);

                waitKey(1);
                if(i==10)
                {
                    lendo = false;
                    if(atualiza == false)
                    {
                        atualizaArquivo(fotos,codUser);
                    }
                }
            }
        }
    }
    destroyAllWindows();
}

void recoFace::atualizaArquivo(vector<string>& fotos, int id)
{
    ofstream arqSaida;
    arqSaida.open(localArquivo.c_str(), ios_base::app);

    for(unsigned int i = 0; i< fotos.size() ; i++)
    {
        arqSaida << fotos[i] << id << "\n";
    }

    arqSaida.close();
}
