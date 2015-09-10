
//Rafael Bellotti
#include "stdafx.h"
//Including main libs:
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>

//Including necessary classes:
#include "mazeProcessingClass.h"


using namespace std;

using namespace System;
using namespace System::IO::Ports;

int main(array<System::String ^> ^args)
{
	
	String^ answer;
	String^ portName;
	int baudRate=9600;
	unsigned counter = 0;

	//Apresentacao
	cout << "##########################"<< endl;
	cout << "#                        #" << endl;
	cout << "# Projeto elaborado por: #" << endl;
	cout << "# Rafael | Pedro | Joao  #" << endl;
	cout << "#                        #" << endl;
	cout << "##########################"<< endl;

	cout << "\n\n\n" << endl;

	//Inicializacao do objeto
	mazeProcessing *mazeObj =  new mazeProcessing();

	//Caso queira ver a resolucao grafica:
	//mazeObj->graphicalResult();


	Console::WriteLine("Type in a port name and hit ENTER");
	portName=Console::ReadLine();


	// arduino settings
	SerialPort^ arduino;
	arduino = gcnew SerialPort(portName, baudRate);



	// open port
	try
	{
		arduino->Open();

		do
		{
			String^ str = gcnew String(mazeObj->pathMaking.at(counter).c_str());
			arduino->WriteLine(str);
			//check that user typed one of the options
			Console::WriteLine("Press ENTER");
			// get answer
			answer=Console::ReadLine();
			// clear the screen
			Console::Clear();
			counter++;
		}while(counter < mazeObj->pathMaking.size());
		// close port to arduino
		arduino->Close();
	}
	catch (IO::IOException^ e  ) 
	{ 
		Console::WriteLine(e->GetType()->Name+": Port is not ready");
	}
	catch (ArgumentException^ e)
	{
		Console::WriteLine(e->GetType()->Name+": incorrect port name syntax, must start with COM/com");
	}
	// end program
	Console::Write("Press enter to close the program");
	Console::Read();
    return 0;
}
