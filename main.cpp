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

//A su vez, implementamos un struct que represente una lista enlazada doble, que igualmente tendra datos genericos
template<typename T>
struct listaEnlazadaDoble {
	NodoDoble<T> *inicio, *fin;
	unsigned int tamaño;
};


template<typename T>
listaEnlazadaDoble<T>* iniciarListaEnlazada() {
	listaEnlazadaDoble<T> *lista = new listaEnlazadaDoble<T>();
	lista->fin = NULL;
	lista->inicio = NULL;
	lista->tamaño = 0;
};


//Prototipo de funciones
void mostrarInicio(listaEnlazadaDoble<Celda>*);
void buscar(listaEnlazadaDoble<Celda>*,int, NodoDoble<Celda>*&); 
void exportarCSV(listaEnlazadaDoble<Celda>*,string);

template<typename T>
void agregar(listaEnlazadaDoble<T>*, T); //Modificada para ser generica

void mostrarInicio();
void mostrarFinal();
void modificar(string, NodoDoble<Celda>*&);
void copiar(NodoDoble<Celda>*&, NodoDoble<Celda>*&);
void cortar(NodoDoble<Celda>*&, NodoDoble<Celda>*&);

//Definici�n de punteros globales
NodoDoble<Celda> *inicio, *fin;

int main(int argc, char** argv) {
	int op = 0, n=0, cp=0, nc=0;
	listaEnlazadaDoble<Celda> *lista = iniciarListaEnlazada<Celda>();
	string nuevo_valor = "";  //Contenido de un nodo
	NodoDoble<Celda> *referencia, *referenciaCopy; //Nodos de referencia para buscar, copiar y cortar
	
	Celda nueva_celda;
	nueva_celda.valor = nuevo_valor;
	//Llenado de la lista con los indices y valores vacios
	for(int i=1; i<11; i++){
		nueva_celda.id = i;
		agregar(lista, nueva_celda);//Se pasa por valor una celda para que la inserte en la lista
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
					mostrarInicio(lista);
					cout<<endl;
					break;
			case 2: 
					cout<<"Celda: "<<endl;
					cin>>n;
					buscar(lista, n, referencia);
					
					if(referencia != NULL){
						cout<<"Valor: "<<endl;
						cin>>nuevo_valor;
						modificar(nuevo_valor, referencia);
					}
					break;
			case 3:
					cout<<"Celda a copiar: "<<endl;
					cin>>n;
					buscar(lista, n, referencia);
					
					if(referencia != NULL){
						cout<<"Nueva celda: "<<endl;
						cin>>nc;
						buscar(lista, nc, referenciaCopy);
						
						if(referenciaCopy != NULL){
							copiar(referencia, referenciaCopy);
						}
					}
					break;
			case 4:
				cout<<"Celda a cortar: "<<endl;
					cin>>n;
					buscar(lista, n, referencia);
					
					if(referencia != NULL){
						cout<<"Nueva celda: "<<endl;
						cin>>nc;
						buscar(lista,nc, referenciaCopy);
						
						if(referenciaCopy != NULL){
							cortar(referencia, referenciaCopy);
						}
					}
				break;
			case 5:
				cout <<"Exportar csv"<<endl;
				exportarCSV(lista, "hoja.csv");
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
template<typename T>
void agregar(listaEnlazadaDoble<T> *lista, T dato){  //Se le pasa un puntero a la lista, y el dato que se almacenara en esta
	NodoDoble<T>* nuevo_nodo = new NodoDoble<T>();   //Creando un nuevo nodo
	nuevo_nodo->dato = dato;
	nuevo_nodo->sig = NULL;              //Los siguientes punteros apuntan a null
	nuevo_nodo->ant = NULL;
	
	if(lista->inicio == NULL){           //si el inicio de la lista est� vacia 
		lista->inicio = nuevo_nodo;
	}else{
		(lista->fin)->sig = nuevo_nodo;
		nuevo_nodo->ant = lista->fin;
	}
	
	lista->fin = nuevo_nodo;           //El final de la lista siempre ser� el nuevo elemento insertado
}


void mostrarInicio(listaEnlazadaDoble<Celda> *lista){          //Funci�n encargada de mostrar la lista de izquierda a derecha
	NodoDoble<Celda>* referencia2 = lista->inicio;   // se crea un nodo de referencia que apunte al incio para emepzar a recorrerla
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

void buscar(listaEnlazadaDoble<Celda> *lista, int x, NodoDoble<Celda>*& referencia){  //Funci�n encargada de buscar una celda pasada por parametro
	referencia = NULL;  //Se incializa la variable global referencia a NULL (vacio)
	bool encontrado = false;  //Bandera que permitir� salir del ciclo while
	
	if(x>((lista->fin)->dato.id/2)){  // La lista se parte a la mitad. Si x es mayor que la mitad, significar que se encuentra en la mitad derecha. 
		referencia = lista->fin; //Se correr� la lista de derecha a izqueirda
		while(referencia!=NULL && !encontrado){  //Si la referencia es NULL, significa que la lista termin� de recorrerse
			if(referencia->dato.id == x){
				encontrado = true;
				cout<<"Celda encontrada"<<endl;
			}else {
				referencia = referencia->ant; //Se pasa al nodo anterior
			}
		} 
	}else{
		referencia = lista->inicio;   //Se correr� la lista de izquierda a derecha
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

void exportarCSV(listaEnlazadaDoble<Celda> *lista, string nombredarchivo) {
    ofstream file(nombredarchivo);

    if (!file) {
        cerr << "Error abriendo archivo." << endl;
        return;
    }

    NodoDoble<Celda>* referencia = lista->inicio;
    while (referencia != nullptr) {
        file << referencia->dato.valor << ",";
        referencia = referencia->sig;
    }

    file.close();
    cout << "Datos exportados a " << nombredarchivo << " excitosamente." << endl;
}


