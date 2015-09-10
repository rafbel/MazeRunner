//Rafael Bellotti

#ifndef MAZEPROCESSINGCLASS_H
#define MAZEPROCESSINGCLASS_H

#include <string>

//DEFINED:
#define OK 0
#define IMAGE_NOT_FOUND 1
#define ERROR_OPENING_FILE 2
#define CELL_PIXELS 14 //TAMANHO DO QUADRADO DA MATRIZ
#define WALL_SIZE 2 //GROSSURA DA PAREDE
#define MAX_STORAGE_SIZE 270 //capacidade maxima de armazenamento de paredes
#define PATH_NOT_FOUND -1


struct Coordinates{
	int x;
	int y;
};


class mazeProcessing
{
public:
	mazeProcessing();
	int graphicalResult();

	//Vetor de tomada de decisao
	std::vector <std::string> pathMaking; //c = cima, e = esquerda , d = direita

private:
	//Funcoes
	unsigned obtainMazeInfo();
	unsigned writeMazeFile();
	bool verifica_conexao(int*, int* , int**);
	int mapeia_distancias(int*, int*, int**, int**);
	void encontra_menor_caminho(int* , int** , int** , std::list<int> &, std::list<int> & );
	void pathChoice(std::list<int>, std::list<int>);

	//Variaveis
	//As matrizes a seguir informam sobre a existencia ou nao de paredes:
	unsigned horizontalMem[MAX_STORAGE_SIZE][MAX_STORAGE_SIZE];
	unsigned verticalMem[MAX_STORAGE_SIZE][MAX_STORAGE_SIZE];



	//Informa o numero de paredes:
	unsigned numHorizontal;
	unsigned numVertical;

	//Coordenadas da entrada:
	int pos_ini[2];
	
	//Coordenadas da saida:
	int pos_obj[2];

	//Matriz labirinto:
	int **paredes;

	//Dimensoes do labirinto
	int altura,largura;

	//Mapa distancias
	int **mapa_distancias;

	//Nota: as coordenadas X sao as unicas que importam no momento,
	//porque eh assumido que a entrada eh sempre em baixo e a saida sempre na parte de cima do labirinto
};

#endif