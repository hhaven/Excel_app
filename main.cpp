#include <iostream>
#include <string>
#include <fstream>
#include <functional>
//#include <optional> //Esta libreria requiere C++ 17

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
	return lista;
};

//Alias para mejorar la legibilidad del documento
using Fila = listaEnlazadaDoble<Celda>;
using Matriz = listaEnlazadaDoble<Fila>;


//Prototipo de funciones
template<typename T>
void agregar(listaEnlazadaDoble<T>*, T); //Modificada para ser generica

void mostrarInicio(Fila*,NodoDoble<Celda>*);
void buscar(Fila*,int, NodoDoble<Celda>*&); 
void exportarCSV(Fila*,string);

void mostrarFinal();
void modificar(string, NodoDoble<Celda>*&);
void copiar(NodoDoble<Celda>*&, NodoDoble<Celda>*&);
void cortar(NodoDoble<Celda>*&, NodoDoble<Celda>*&);

template<typename T>
T* buscarPorIndice(listaEnlazadaDoble<T> *lista, int); //Implementacion que busca formular de forma generica, lo que impide implementarse usando un valor por id. Por lo que usara la posicion en la lista en su lugar
template<typename T>
void forEach(listaEnlazadaDoble<T>*, function< void(T) >); //Recibe como parametros la lista a usar y una lambda que ejecuta para cada mienbro de la lista

//Definici�n de punteros globales
NodoDoble<Celda> *inicio, *fin;

int main(int argc, char** argv) {
	int op = 0, n=0, cp=0, nc=0;
	Fila *lista = iniciarListaEnlazada<Celda>();
	Matriz *matriz = iniciarListaEnlazada<Fila>();
	string nuevo_valor = "";  //Contenido de un nodo
	NodoDoble<Celda> *referencia, *referenciaCopy; //Nodos de referencia para buscar, copiar y cortar
	//optional<Fila> l = nullopt;//Variable que solo sirve para revibir datos de funciones que retornen optional

	Celda nueva_celda;
	nueva_celda.valor = nuevo_valor;
	//Llenado de la lista con los indices y valores vacios
	for(int j=1; j<=5; j++){
		//Se limpia la lista usada
		lista->inicio=NULL;
		lista->fin=NULL;
		lista->tamaño=0;
		//Poblamos dicha lista con nodos
		for (int i = 1; i <= 5; i++) {
					nueva_celda.id = i;
					agregar(lista, nueva_celda);//Se pasa por valor una celda para que la inserte en la lista
				}

		agregar(matriz, *lista);//Se pasa por valor la lista poblada
	}
	referencia = (matriz->inicio)->dato.inicio;


	lista = buscarPorIndice(matriz,0);
	
	do{
		cout<<endl;
		cout<<"|-------------------------|"<<endl;
		cout<<"|Menu                     |"<<endl;
		cout<<"|-------------------------|"<<endl;
		cout<<"|1)  Mostrar contenido    |"<<endl;
		cout<<"|2)  Ingresar contenido   |"<<endl;
		cout<<"|3)  Copiar contenido     |"<<endl;
		cout<<"|4)  Cortar contenido     |"<<endl;
		cout<<"|5)  Exportar CSV         |"<<endl;
		cout<<"|6)  Saltar a celda       |"<<endl;
		cout<<"|7)  Mover a la izquierda |"<<endl;
		cout<<"|8)  Mover a la derecha   |"<<endl;
		cout<<"|9)  Mover a arriba       |"<<endl;
		cout<<"|10) Mover a abajo        |"<<endl;
		cout<<"|11) Salir                |"<<endl;
		cout<<"|-------------------------|"<<endl;
		cin>>op;
		
		switch(op){
			case 1:
					cout<<endl;
					//Al parecer, al necesitar otro parametro ademas de la lamda, hace que el compilador tenga problemas determinando el tipo T
					forEach<Fila> (matriz, 
						[referencia](Fila e) { mostrarInicio(&e, referencia); });//Ahora tiene una referencia al nodo actual, para poder indicarlo al mostrar la matriz
					//Por eso hay que especificarlo explicitamente al invocar la funcion.
					cout<<endl;
					break;
			case 2: 
					//La parte de seleccionar celda ahora es su propia funcion en el menu
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
				cout<<"Fila: "<<endl;
				cin>>n;
				lista = buscarPorIndice(matriz, n-1); //l es una variable de tipo opcional de una lista de celdas, que es lo que retornara la funcion buscar por indice
				
				cout<<"Columna: "<<endl;
				cin>>n;
				referenciaCopy = referencia;; //Creamos un respaldo de la celda actual
				if(lista != NULL) {//Comprueba si el valor opcional recibio una lista valida	
					buscar(lista, n, referencia);}
				if(referencia == NULL) {
					referencia = referenciaCopy;} //SI referencia era nula, vuelve al valor que tenia antes
				referenciaCopy = NULL;//La dejamos como nulo para evitar interferencias con otras partes del codigo donde se use

				break;			
			case 7:
				if(referencia->ant != NULL)
					referencia = referencia->ant;
				break;
			case 8:
				if(referencia->sig != NULL)
					referencia = referencia->sig;
				break;
			case 9:
				break;
			case 10:
				break;
			case 11:
				break;
			default:
				cout<<"Opci�n invalida"<<endl;
				cout<<endl;
		}
		
	}while(op!=11);
	
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
	lista->tamaño++;
	cout << lista->tamaño;
}


void mostrarInicio(Fila *lista, NodoDoble<Celda>* actual){          //Funci�n encargada de mostrar la lista de izquierda a derecha
	NodoDoble<Celda>* referencia2 = lista->inicio;   // se crea un nodo de referencia que apunte al incio para emepzar a recorrerla
	char indicadorActual=' ';
	string dato;

	for (int i = 0; i < lista->tamaño; i++) //Linea delimitadora
		cout<<"|----------";
	cout<<"|\n|";	
	
	while(referencia2 != NULL){
		dato = referencia2->dato.valor;
		if(referencia2->dato.valor.size() < 8)
			dato.insert(dato.size(), 8 - dato.size(), ' ');
		else
			dato = dato.substr(0,8);
		indicadorActual = (actual==referencia2) ? '*': ' ';
		cout<<indicadorActual<<dato<<indicadorActual<<"|";   //Se imprime el valor
		referencia2 = referencia2->sig;          //Se pasa el siguiente nodo
	}
	delete referencia2;                         //Se elimina el nodo, para liberar espacio en memoria (heap)
	cout << endl;
}

//void mostrarFinal(){			//Funci�n encargada de mostrar la lista de derecha a izquierda. 
//	NodoDoble* referencia2 = fin; // Ahora el nodo de referencia se inicializa al final
//	while(referencia2 != NULL){
//		cout<<referencia2->dato.valor<<" | ";
//		referencia2 = referencia2->ant;  //Se pasa al nodo anterior
//	}
//	delete referencia2;
//}

void buscar(Fila *fila, int x, NodoDoble<Celda>*& referencia){  //Funci�n encargada de buscar una celda pasada por parametro
	referencia = NULL;  //Se incializa la variable global referencia a NULL (vacio)
	bool encontrado = false;  //Bandera que permitir� salir del ciclo while
	
	if(x>((fila->fin)->dato.id/2)){  // La lista se parte a la mitad. Si x es mayor que la mitad, significar que se encuentra en la mitad derecha. 
		referencia = fila->fin; //Se correr� la lista de derecha a izqueirda
		while(referencia!=NULL && !encontrado){  //Si la referencia es NULL, significa que la lista termin� de recorrerse
			if(referencia->dato.id == x){
				encontrado = true;
				cout<<"Celda encontrada"<<endl;
			}else {
				referencia = referencia->ant; //Se pasa al nodo anterior
			}
		} 
	}else{
		referencia = fila->inicio;   //Se correr� la lista de izquierda a derecha
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

template<typename T> //T es un template, representa el tipo de variable. En este caso podria ser una dato o una lista doble de celdas
T* buscarPorIndice(listaEnlazadaDoble<T> *lista, int indice) { //Ahora el retorno es un puntero. Si es nulo, dabremos que no encontro nada, caso contrario podremos acceder al valor
       int tamañoDesde0 = (lista->tamaño)-1;
       //Comprueba si el valor del indice no sea mayor que el tamaño de la lista
       if(indice > tamañoDesde0 || indice < 0)
           return NULL;//Si es el caso retorna nullopt
       
       int i = 0;
       NodoDoble<T> *n = NULL;
       if(indice > (tamañoDesde0/2)){  // La lista se parte a la mitad. Si index es mayor que la mitad, significar que se encuentra en la mitad derecha. 
    	n = lista->fin; //Se correr� la lista de derecha a izqueirda
        i = tamañoDesde0;
    	while(i > indice){
    		i--;
            n = n->ant;
    	} 
    }else{
    	n = lista->inicio;   //Se correr� la lista de izquierda a derecha
    	while(i < indice){
    		i++;
            n = n->sig;
    	} 
    }
	cout<<"Fila: "<<i<<endl;
    return &(n->dato);	    
}

template<typename T>
void forEach(listaEnlazadaDoble<T> *lista, function< void(T) > lambda) {
    NodoDoble<T> *n = lista->inicio;
    while(n != NULL) {
        lambda(n->dato);
        n = n->sig;
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

void exportarCSV(Fila *lista, string nombredarchivo) {
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


