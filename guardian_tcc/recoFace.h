#include <iostream>
#include <string.h>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/face.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace cv::face;
using namespace std;

class recoFace
{
	//criaçao dos compenentes publicos
	public:
		//inicializar classe com arquivos
		void infArq(string arquivo);
        //inicilaiza a leitura do arquivo e retorna informacoes de treino;
		void leArqivo(vector<Mat>& imagens, vector<int>& labels, char separador);
		//funcao para capturar imagem
		void capturaFoto(Mat& teste,  vector<Mat>& imagens, vector<int>& labels);
        //funcao para reconhecer pessoa
		bool reconhePessoa(Mat& teste, vector<Mat>& imagens, vector<int>& labels);
        //funcao para cadastrar pessoa
        void cadastraPessoa(int coduser, bool identifica);
        //funcao para identificar o rosto na foto
        void localizaRosto(Mat& frame, Mat& imageCompare);
        //funcao para rotacionar o rosto baseado nos olhos
        void rotaFace(Mat& rosto);
        // funcao para atualizar o csv
        void atualizaArquivo(vector<string>& fotos, int id);


    //criacao dos compenentes privados
	private:
	string localArquivo;
    Ptr<BasicFaceRecognizer> recModelo;
    Mat teste;
	vector<Mat> imagens;
	vector<int> labels;


};
