//Libraries:
#include "stdafx.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>

//Class include:
#include "mazeProcessingClass.h"

using namespace std; 
using namespace cv; 

//Rafael Bellotti

//Processamento de imagem:

unsigned mazeProcessing::obtainMazeInfo()
{
	//Abre a imagem:
	unsigned counter; //contador para encontrar entrada/saida
	Mat src;

	try 
	{

		src = imread("8 by 8 orthogonal maze 1.png");


		//Caso nao encontre fecha o programa
		if (src.empty())
		{
			cout << "Image not found!" << endl;
			return IMAGE_NOT_FOUND;
		}	

	}

	catch(std::exception e) 
	{
		cout << "Exception:" << e.what() << endl;
	}
	//Convertendo a imagem para binario:
    Mat binary_image;
    cvtColor(src, binary_image, CV_BGR2GRAY);
    threshold(binary_image, binary_image, 10, 255, CV_THRESH_BINARY_INV);
	//imshow("binary", binary_image);

	//cout <<"Rows/cols:" << binary_image.rows << " " << binary_image.cols << endl;

	int rows = 0; //linhas
	int cols = 0; //colunas

	//Index para linha e coluna
	unsigned indexH = 0;
	unsigned indexV = 0;

	//Grava o numero de paredes horizontais/verticais nas variaveis private designadas:
	numHorizontal = (binary_image.rows - WALL_SIZE)/(WALL_SIZE + CELL_PIXELS) + 1;
	numVertical = (binary_image.cols - WALL_SIZE)/(WALL_SIZE + CELL_PIXELS);

	//Armazena dados para paredes horizontais
	for (rows = 0; rows < binary_image.rows; rows = rows + WALL_SIZE + CELL_PIXELS)
	{
		for (cols = WALL_SIZE; cols < binary_image.cols; cols = cols + WALL_SIZE + CELL_PIXELS)
		{
			if (binary_image.at<uchar>(rows,cols) == 0) //caso preto ou sem parede
				horizontalMem[indexH][indexV] = 0;//armazenar 0
			if (binary_image.at<uchar>(rows,cols) == 255) //caso branco ou com parede
				horizontalMem[indexH][indexV] = 1;//armazenar 1
			indexV++;
		}
		indexH++;
		indexV = 0;
	}
	indexH = 0;
	indexV = 0;
	
	//Armazena dados para paredes verticais:
	for (rows = WALL_SIZE; rows < binary_image.rows ; rows = rows + WALL_SIZE+ CELL_PIXELS)
	{
		for (cols = 0; cols < binary_image.cols; cols = cols + WALL_SIZE + CELL_PIXELS)
		{
			if (binary_image.at<uchar>(rows,cols) == 0) //caso preto
				verticalMem[indexH][indexV] = 0;//armazenar 0
			if (binary_image.at<uchar>(rows,cols) == 255) //caso branco
				verticalMem[indexH][indexV] = 1;//armazenar 1
			indexV++;
		}
		
		indexH++;
		indexV = 0;
	}

	//Armazenamento de coordenadas
	//Fecha a entrada e a saida no arquivo output para rodar corretamente no algoritmo necessario para resolver:

	//Procura pela entrada:
	for (counter = 0; horizontalMem[numHorizontal - 1][counter] == 1; counter++);
	pos_ini[0] = numHorizontal - 2;
	horizontalMem[numHorizontal - 1][counter] = 1;
	pos_ini[1] =  counter;

	//Procura pela saida:
	for (counter = 0; horizontalMem[0][counter] == 1; counter++);
	horizontalMem[0][counter] = 1;
	pos_obj[0] = 0 ;
	pos_obj[1] =  counter;

	//Imprimindo coordenadas para teste:
	//cout << pos_ini[0] << "  " << pos_ini[1] << endl;
	//cout << pos_obj[0] << "  " << pos_obj[1] << endl;
	
	//Juntando as duas em uma matriz so:
	altura = numHorizontal - 1;
	largura = numVertical;

	unsigned matrixRef = altura*2 + 1;
	unsigned counter2;
	unsigned counter3 = 0;

	paredes = new int*[matrixRef];
	bool turnChecker = true; //define se vai inserir a matriz horizontal ou a vertical primeiro

	for (counter = 0; counter < matrixRef; counter++)
	{
		if (turnChecker == true)
		{
			paredes[counter] = new int [numVertical];
			for (counter2 = 0; counter2 < numVertical; counter2++)
			{
				paredes[counter][counter2] = horizontalMem[counter3][counter2];
				//cout << paredes[counter][counter2] << "  "; 
			}
		}
		else
		{
			paredes[counter] = new int[numHorizontal];
			for (counter2 = 0; counter2 < numHorizontal; counter2++)
			{
				paredes[counter][counter2] = verticalMem[counter3][counter2];
				//cout << paredes[counter][counter2] << "  "; 
			}
			counter3++;
		}
		//cout << endl;
		turnChecker = !turnChecker;
	}

	return OK; //sem erros
}

/*notas: a grossura das paredes eh sempre de 2 pixels em um labirinto perfeito
o tamanho de uma celula eh de 14x14 pixels*/

//Metodo interno da classe para operacoes com erro:

unsigned mazeProcessing::writeMazeFile()
{
	ofstream mazeFile; //variavel de criacao de arquivo
	unsigned col;
	unsigned row;

	mazeFile.open("mazeInfo.txt"); //cria o arquivo ou abre o arquivo para escrita
	if (!mazeFile.is_open()) //caso de erro
		return ERROR_OPENING_FILE;

	//Escreve no arquivo as coordenadas de entrada:
	mazeFile << pos_ini[0]<< " " << pos_ini[1] <<"\n"; //assume que a entrada sempre se localiza embaixo

	//nota: numHorizontal - 2, pois tem que descontar a parede superior e a inferior 

	//Escreve no arquivo as coordenadas de saida:
	mazeFile << pos_obj[0] << " " << pos_obj[1] <<"\n"; //assume que a saida sempre se localiza no topo

	for (row = 0; row < numHorizontal; row++) //escreve no arquivo as informacoes do labirinto
	{
		for (col = 0; col < numVertical; col++) //paredes horizontais
			mazeFile << horizontalMem[row][col] << " ";
		mazeFile <<"\n";
		if (row != numHorizontal - 1)
		{
			for (col = 0; col < numVertical + 1; col++) //paredes verticais
				mazeFile << verticalMem[row][col] << " ";
			mazeFile << "\n";
		}
	}
	mazeFile.close(); //fecha o arquivo

	return OK;

}

mazeProcessing::mazeProcessing()
{
	if (obtainMazeInfo() != IMAGE_NOT_FOUND)
	{

		//Usar esta funcao interna de testes caso queira operar erro obtido (armazena um arquivo de saida):
		//writeMazeFile();
	
		//Mapa com as distâncias de cada ponto até o ponto inicial
		mapa_distancias = new int*[altura];
		for(int i = 0; i < altura; i++)
		{
			mapa_distancias[i] = new int[largura];
			int maximo = altura * largura;
			for(int j = 0; j < largura; j++)
				mapa_distancias[i][j] = maximo;
		}

		std::list<int> caminho_x, caminho_y;
		cout << "Distância até o objetivo: " << mapeia_distancias(pos_ini, pos_obj, mapa_distancias, paredes) << std::endl;
		encontra_menor_caminho(pos_obj, mapa_distancias, paredes, caminho_x, caminho_y);
	
		pathChoice(caminho_x,caminho_y);

		/*std::list<int>::iterator it_x, it_y; //checagem de erro
		it_y = caminho_y.begin();
		for(it_x = caminho_x.begin(); it_x != caminho_x.end(); it_x++)
		{
			//Impressao na tela do caminho a ser feito:
			std::cout << "[" << *it_y << "," << *it_x << "], "; 

			//Tomada de decisao:
	

			it_y++;
		}*/
		cout << endl;
	
		for(int i = 0; i < altura; i++)
			delete[] mapa_distancias[i];
		delete[] mapa_distancias;
		int matrixRef = altura*2 + 1;
		for (int counter = 0; counter < matrixRef; counter++)
			delete [] paredes[counter];
		delete [] paredes;
	}
}
//Pedro de Carvalho Cayres Pinto

//Algoritmo de solucao:

bool mazeProcessing::verifica_conexao(int* pos_atual, int* pos_prox, int** paredes)
{
	if(pos_prox[0] - pos_atual[0] == 0)
	{
		//Direita
		if(pos_prox[1] - pos_atual[1] == 1)
			return paredes[2 * pos_atual[0] + 1][pos_atual[1] + 1] == 0;
		//Esquerda
		else
			return paredes[2 * pos_atual[0] + 1][pos_atual[1]] == 0;
	}
	else
	{
		//Baixo
		if(pos_prox[0] - pos_atual[0] == 1)
			return paredes[2 * pos_atual[0] + 2][pos_atual[1]] == 0;
		//Cima
		else
			return paredes[2 * pos_atual[0]][pos_atual[1]] == 0;
	}
}

//Calcula a distância da posição inicial às outras posições até encontrar o objetivo
int mazeProcessing::mapeia_distancias(int* pos_ini, int* pos_obj, int** mapa_distancias, int** paredes)
{
    //Distância / Número de passos até a posição inicial
    int n = 0;
    
    mapa_distancias[pos_ini[0]][pos_ini[1]] = n;
    if((pos_obj[0] == pos_ini[0]) && (pos_obj[1] == pos_ini[1]))
        return n;
    
    //Posições que estão à distância n da posição inicial
    std::list<int> pos_atuais_y, pos_atuais_x;
    pos_atuais_y.push_front(pos_ini[0]);
    pos_atuais_x.push_front(pos_ini[1]);
    while(!pos_atuais_x.empty())
    {
    	n++;
    	std::list<int> pos_futuras_y, pos_futuras_x;
    	std::list<int>::iterator it_x, it_y;
	    int pos_atual[2], pos_prox[2];
	    it_y = pos_atuais_y.begin();
    	for(it_x = pos_atuais_x.begin(); it_x != pos_atuais_x.end(); it_x++)
    	{
    		pos_atual[0] = *it_y;
    		pos_atual[1] = *it_x;
	    	int j = -1;
	    	for(int k = 0; k < 4; k++)
			{
				pos_prox[0] = pos_atual[0] + (k / 2) * j;
				pos_prox[1] = pos_atual[1] + ((3 - k) / 2) * j;
				if(verifica_conexao(pos_atual, pos_prox, paredes))
			    	if(mapa_distancias[pos_prox[0]][pos_prox[1]] > n)
			    	{
                        pos_futuras_x.push_front(pos_prox[1]);
                        pos_futuras_y.push_front(pos_prox[0]);
                        mapa_distancias[pos_prox[0]][pos_prox[1]] = n;
                        if ((pos_prox[0] == pos_obj[0]) && (pos_prox[1] == pos_obj[1]))
							return n;
			    	}
			    j *= -1;
			}
			it_y++;
    	}
    	pos_atuais_x = pos_futuras_x;
    	pos_atuais_y = pos_futuras_y;
    }
    //Não encontrou caminho até o objetivo
    return PATH_NOT_FOUND;
}

//Encontra o menor caminho até o objetivo
void mazeProcessing::encontra_menor_caminho(int* pos_obj, int** mapa_distancias, int** paredes, std::list<int> &caminho_x, std::list<int> &caminho_y)
{
	caminho_y.push_front(pos_obj[0]);
	caminho_x.push_front(pos_obj[1]);

    int pos_atual[2], pos_prox[2];
	pos_atual[0] = pos_obj[0];
	pos_atual[1] = pos_obj[1];
    while(mapa_distancias[pos_atual[0]][pos_atual[1]] != 0)
    {
    	int j = -1;
    	for(int i = 0; i < 4; i++)
		{
			pos_prox[0] = pos_atual[0] + (i / 2) * j;
			pos_prox[1] = pos_atual[1] + ((3 - i) / 2) * j;
			if(verifica_conexao(pos_atual, pos_prox, paredes))
		    	if(mapa_distancias[pos_prox[0]][pos_prox[1]] == mapa_distancias[pos_atual[0]][pos_atual[1]] - 1)
		    	{
		    		caminho_y.push_front(pos_prox[0]);
		    		caminho_x.push_front(pos_prox[1]);
		    		pos_atual[0] = pos_prox[0];
		    		pos_atual[1] = pos_prox[1];
		    		break;
		    	}
		    j *= -1;
		}
    }
}

//Rafael Cortez Bellotti de Oliveira

//Solucao Grafica

//Ainda gera certos erros: 
int mazeProcessing::graphicalResult()
{
	//Abre a imagem:
    Mat src = imread("solveMaze.png");

	//Caso nao encontre fecha o programa
    if (src.empty())
	{
		cout << "Image not found!" << endl;
        return IMAGE_NOT_FOUND;
	}
	//Convertendo a imagem para binario:
    Mat binary_image;
    cvtColor(src, binary_image, CV_BGR2GRAY);
    threshold(binary_image, binary_image, 10, 255, CV_THRESH_BINARY_INV);
	//imshow("binary", binary_image);

	//Cria o vector de contornos:
    vector<vector<Point> > contours;
    findContours(binary_image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	//Escolhe o caminho:
    Mat path = Mat::zeros(src.size(), CV_8UC1);
    drawContours(path, contours, 0, CV_RGB(255,255,255), CV_FILLED);
	//imshow("path",path);

	//Dilata tal caminho:
    Mat kernel = Mat::ones(19, 19, CV_8UC1); 
    dilate(path, path, kernel);
	//imshow("dilate",path);

	//Erode o caminho:
    Mat path_erode;
    erode(path, path_erode, kernel);
	//imshow("erode",path_erode);

	//Calcula a diferenca do dilate e do erode:
    absdiff(path, path_erode, path);
	imshow("difference",path);

	//Divide o caminho em 3 channels, sendo o predominante o vermelho:
    vector<Mat> channels;
    split(src, channels);
    channels[0] &= ~path;
    channels[1] &= ~path;
    channels[2] |= path;

	//Da merge na imagem do caminho com o labirinto para mostrar a resposta graficamente:
    Mat dst;
    merge(channels, dst);
    imshow("solution", dst);

	return OK;
}


//Escolha do caminho a ser tomado

void mazeProcessing::pathChoice(std::list<int> caminho_x, std::list<int> caminho_y)
{
	list<int>::iterator it_x, it_y;
	int currentX,currentY; //coordenadas atuais
	char currentFacing = 'f'; //para que direcao esta olhando (f = forward, b = backwards, r = right, l = left); default = f
	int armX, armY;

	cout << "Caminho: ";
	it_y = caminho_y.begin();
	it_x = caminho_x.begin();
	currentY = *it_y;
	currentX = *it_x;
	cout << "[" << *it_y << "," << *it_x << "], ";
	it_x++;
	it_y++;
	for(; it_x != caminho_x.end(); it_x++)
	{

		//Tomada de decisao:

		//Checa mudanca de posicao
		if (*it_x == currentX) //se move para cima ou para baixo
				{
					armX = 0;
					armY = *it_y - currentY;
				}
				else //se move para os lados
				{
					armX = *it_x - currentX;
					armY = 0;
				}

		//Checando o lado para qual o carro esta virado
		switch (currentFacing)
		{
			case ('f'): //forward
				if (armX == 0)
				{
					pathMaking.push_back("c"); //anda para cima
				}
				else
				{
					if (armX < 0)
					{
						pathMaking.push_back("c");
						pathMaking.push_back("e"); //anda para esquerda
						currentFacing = 'l';
					}
					else
					{
						pathMaking.push_back("c");
						pathMaking.push_back("d"); //anda para a direita
						currentFacing = 'r';
					}
				}

			break;

			case ('r'): //right
				if (armX == 0)
				{
					if (armY < 0) 
					{
						pathMaking.push_back("c");
						pathMaking.push_back("e"); //vira para esquerda
						currentFacing = 'f';
					}
					else
					{
						pathMaking.push_back("c");
						pathMaking.push_back("d"); //vira pra direita
						currentFacing = 'b';
					}
				}
				else
				{
					pathMaking.push_back("c"); //continua indo reto
				}
			break;

			case ('l'): //left
				if (armX == 0)
				{
					if (armY < 0) 
					{
						pathMaking.push_back("c");
						pathMaking.push_back("d"); //vira para esquerda
						currentFacing = 'f';
					}
					else
					{
						pathMaking.push_back("c");
						pathMaking.push_back("e"); //vira pra direita
						currentFacing = 'b';
					}
				}
				else
				{
					pathMaking.push_back("c"); //continua indo reto
				}
			break;

			case ('b'): //forward
				if (armX == 0)
				{
					pathMaking.push_back("c"); //anda reto
				}
				else
				{
					if (armX < 0)
					{
						pathMaking.push_back("c");
						pathMaking.push_back("d"); //anda para direita
						currentFacing = 'l';
					}
					else
					{
						pathMaking.push_back("c");
						pathMaking.push_back("e"); //anda para a esquerda
						currentFacing = 'r';
					}
				}

			break;

			default:
				break;

		}
		//Impressao na tela do caminho a ser feito
		cout << "[" << *it_y << "," << *it_x << "], ";
		currentY = *it_y;
		currentX = *it_x;
		it_y++;

	}
	pathMaking.erase(pathMaking.begin()); // remove a parte inicial do caminho que eh gerada por acidente
	pathMaking.push_back("c"); //adiciona a parte final do trajeto


	//Teste:
	/*for (unsigned counter = 0; counter < pathMaking.size() ; counter++)
		cout << pathMaking.at(counter);*/
}
