#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//Id puede ser una letra
struct Celda {
	int id;
	string valor;
};

//Implementando los nodos para que alberguen un dato generico
template<typename T>
struct NodoDoble {
	T dato; 
	NodoDoble* sig;
	NodoDoble* ant;
};

//Prototipo de funciones
void agregar(int);
void mostrarInicio();
void mostrarFinal();
void buscar(int, NodoDoble<Celda>*&);
void modificar(string, NodoDoble<Celda>*&);
void copiar(NodoDoble<Celda>*&, NodoDoble<Celda>*&);
void cortar(NodoDoble<Celda>*&, NodoDoble<Celda>*&);
void exportarCSV(string);

//Definici�n de punteros globales
NodoDoble<Celda> *inicio, *fin;

int main(int argc, char** argv) {
	int op = 0, n=0, cp=0, nc=0;
	string nuevo_valor = "";  //Contenido de un nodo
	NodoDoble<Celda> *referencia, *referenciaCopy; //Nodos de referencia para buscar, copiar y cortar
	
	//Llenado de la lista con los indices y valores vacios
	for(int i=1; i<11; i++){
		agregar(i);
	}
	
	do{
		cout<<endl;
		cout<<"|---------------------|"<<endl;
		cout<<"|Menu                 |"<<endl;
		cout<<"|---------------------|"<<endl;
		cout<<"|1) Mostrar contenido |"<<endl;
		cout<<"|2) Ingresar contenido|"<<endl;
		cout<<"|3) Copiar contenido  |"<<endl;
		cout<<"|4) Cortar contenido  |"<<endl;
		cout<<"|5) Exportar CSV      |"<<endl;
		cout<<"|6) Salir             |"<<endl;
		cout<<"|---------------------|"<<endl;
		cin>>op;
		
		switch(op){
			case 1:
					cout<<endl;
					mostrarInicio();
					cout<<endl;
					break;
			case 2: 
					cout<<"Celda: "<<endl;
					cin>>n;
					buscar(n, referencia);
					
					if(referencia != NULL){
						cout<<"Valor: "<<endl;
						cin>>nuevo_valor;
						modificar(nuevo_valor, referencia);
					}
					break;
			case 3:
					cout<<"Celda a copiar: "<<endl;
					cin>>n;
					buscar(n, referencia);
					
					if(referencia != NULL){
						cout<<"Nueva celda: "<<endl;
						cin>>nc;
						buscar(nc, referenciaCopy);
						
						if(referenciaCopy != NULL){
							copiar(referencia, referenciaCopy);
						}
					}
					break;
			case 4:
				cout<<"Celda a cortar: "<<endl;
					cin>>n;
					buscar(n, referencia);
					
					if(referencia != NULL){
						cout<<"Nueva celda: "<<endl;
						cin>>nc;
						buscar(nc, referenciaCopy);
						
						if(referenciaCopy != NULL){
							cortar(referencia, referenciaCopy);
						}
					}
				break;
			case 5:
				cout <<"Exportar csv"<<endl;
				exportarCSV("hoja.csv");
				break;
			case 6:
				break;
			default:
				cout<<"Opci�n invalida"<<endl;
				cout<<endl;
		}
		
	}while(op!=6);
	
	return 0;
}

//Funci�n que s�lo ser� usada una vez, para cargar la cuadricula en pantalla
void agregar(int dato){  //Se le pasa es indice del for
	NodoDoble<Celda>* nuevo_nodo = new NodoDoble<Celda>();   //Creando un nuevo nodo
	nuevo_nodo->dato.id = dato;          //insertando el identificador de celda
	nuevo_nodo->dato.valor = " ";        //insertando una cadena vacia
	nuevo_nodo->sig = NULL;              //Los siguientes punteros apuntan a null
	nuevo_nodo->ant = NULL;
	
	if(inicio == NULL){           //si el inicio de la lista est� vacia 
		inicio = nuevo_nodo;
	}else{
		fin->sig = nuevo_nodo;
		nuevo_nodo->ant = fin;
	}
	
	fin = nuevo_nodo;           //El final de la lista siempre ser� el nuevo elemento insertado
}

void mostrarInicio(){          //Funci�n encargada de mostrar la lista de izquierda a derecha
	NodoDoble<Celda>* referencia2 = inicio;   // se crea un nodo de referencia que apunte al incio para emepezar a recorrerla
	while(referencia2 != NULL){
		cout<<referencia2->dato.valor<<" | ";   //Se imprime el valor
		referencia2 = referencia2->sig;          //Se pasa el siguiente nodo
	}
	delete referencia2;                         //Se elimina el nodo, para liberar espacio en memoria (heap)
}

//void mostrarFinal(){			//Funci�n encargada de mostrar la lista de derecha a izquierda. 
//	NodoDoble* referencia2 = fin; // Ahora el nodo de referencia se inicializa al final
//	while(referencia2 != NULL){
//		cout<<referencia2->dato.valor<<" | ";
//		referencia2 = referencia2->ant;  //Se pasa al nodo anterior
//	}
//	delete referencia2;
//}

void buscar(int x, NodoDoble<Celda>*& referencia){  //Funci�n encargada de buscar una celda pasada por parametro
	referencia = NULL;  //Se incializa la variable global referencia a NULL (vacio)
	bool encontrado = false;  //Bandera que permitir� salir del ciclo while
	
	if(x>(fin->dato.id/2)){  // La lista se parte a la mitad. Si x es mayor que la mitad, significar que se encuentra en la mitad derecha. 
		referencia = fin; //Se correr� la lista de derecha a izqueirda
		while(referencia!=NULL && !encontrado){  //Si la referencia es NULL, significa que la lista termin� de recorrerse
			if(referencia->dato.id == x){
				encontrado = true;
				cout<<"Celda encontrada"<<endl;
			}else {
				referencia = referencia->ant; //Se pasa al nodo anterior
			}
		} 
	}else{
		referencia = inicio;   //Se correr� la lista de izquierda a derecha
		while(referencia!=NULL && !encontrado){
			if(referencia->dato.id == x){
				encontrado = true;
				cout<<"Celda encontrado"<<endl;
			}else {
				referencia = referencia->sig;  //Se pasa al siguiente
			}
		}
	}
	
	if(!encontrado){  //Si no se enuentra en la lista
		cout<<"Celda no encontrada"<<endl;
	}
	
}



void modificar(string nuevo_valor, NodoDoble<Celda>*& referencia){  //Funci�n encargada de modificar la celda
	referencia->dato.valor = nuevo_valor; //La variable global referencia, estar� justo en el nodo que se desea modificar
}

void copiar(NodoDoble<Celda>*& referencia, NodoDoble<Celda>*& referenciaCopy) {
    referenciaCopy->dato.valor = referencia->dato.valor;
}

void cortar(NodoDoble<Celda>*& referencia, NodoDoble<Celda>*& referenciaCopy) {
    referenciaCopy->dato.valor = referencia->dato.valor;
    referencia->dato.valor = "";
}

void exportarCSV(string nombredarchivo) {
    ofstream file(nombredarchivo);

    if (!file) {
        cerr << "Error abriendo archivo." << endl;
        return;
    }

    NodoDoble<Celda>* referencia = inicio;
    while (referencia != nullptr) {
        file << referencia->dato.valor << ",";
        referencia = referencia->sig;
    }

    file.close();
    cout << "Datos exportados a " << nombredarchivo << " excitosamente." << endl;
}


