#ifndef LOCK_FREE_LIST_H
#define LOCK_FREE_LIST_H

using namespace std;

/*
 * =====================
 * DEFINIÇÃO DAS CLASSES
 * =====================
 */
template <class T>
class elementoAbs {
public:
	int flag;
	elementoAbs();
	virtual bool ehSentinela()=0; // método para verificar se é um elemento ou uma extremidade
	virtual elementoAbs * getNext()=0;
	virtual elementoAbs * getPrev()=0;
	virtual void setT(T *n)=0;
	virtual T *getT() = 0;
	virtual void setNext (elementoAbs<T> *p)=0;
	virtual void setPrev (elementoAbs<T> *p)=0;
};
template <class T>
elementoAbs<T>::elementoAbs () {
	flag = 0;
};

template <class T>
class elemento : public elementoAbs<T> {
private:
	T *number;
	elementoAbs<T> * next;
	elementoAbs<T> * prev;
public:
	elemento (T *n);
	bool ehSentinela() { return false; }
	elementoAbs<T> * getNext() { return next; }
	elementoAbs<T> * getPrev() { return prev; }
	void setT (T *n) { number = n; }
	T *getT() {
		return number;
		}
	void setNext (elementoAbs<T> *p) { next = p; }
	void setPrev (elementoAbs<T> *p) { prev = p; }
};
template <class T>
elemento<T>::elemento (T *n) : elementoAbs<T>() {
	number = n;
	next = NULL;
	prev = NULL;
}
template <class T>
class sentinelaFirst : public elementoAbs<T> { //Sentinela First é o primeiro elemento , o elemento da cola
private:
	elementoAbs<T> * next;
public:
	sentinelaFirst ();
	bool ehSentinela() { return true; }
	elementoAbs<T> * getNext() { return next; }
	elementoAbs<T> * getPrev() { return this; }
	void setT (T *n) { return; }
	T *getT() {
		return NULL;
		}
	void setNext (elementoAbs<T> *p) { next = p; }
	void setPrev (elementoAbs<T> *p) { return; }
}; 
template <class T>
sentinelaFirst<T>::sentinelaFirst () : elementoAbs<T>() {
	;
}
template <class T>
class sentinelaLast : public elementoAbs<T> {// Sentinela Last é o ultimo elemento , elemento da cabeça
private:
	elementoAbs<T> * prev;
public:
	sentinelaLast ();
	bool ehSentinela() { return true; }
	elementoAbs<T> * getNext() { return this; }
	elementoAbs<T> * getPrev() { return prev; }
	void setT (T *n) { return; }
	T *getT() {
		return NULL;//sentinela não possue numero
	}
	void setNext (elementoAbs<T> *p) { return; }
	void setPrev (elementoAbs<T> *p) { prev = p; }
}; 
template <class T>
sentinelaLast<T>::sentinelaLast () : elementoAbs<T>() {
	;
}
template <class T>
class lockFreeList{
private:
	elementoAbs<T> *first;
	elementoAbs<T> *last;
public:
	lockFreeList(T*);
	lockFreeList();
	
	elementoAbs<T> *getFirst(){
		return first;
	}
	elementoAbs<T> *getLast(){
		return last;
	}
	void *insert(T *n);
	void *insertHead(T *n);
	T &deletCola();
	T &deletHead();
	void print();
	
	
	
};

/*
 * ======================
 * MÉTODOS DA CLASSE lockFreelockFreeList
 * ======================
 */ 
template <class T>
lockFreeList<T>::lockFreeList(T *n){
//	cout << 1 << endl;
	first = new sentinelaFirst<T>();
	last = new sentinelaLast<T>();
	first->setNext(last);
	last->setPrev(first);
//	cout << 2 << endl;pp:123:1: err
}
template <class T>
lockFreeList<T>::lockFreeList(){
	first = new sentinelaFirst<T>();
	last = new sentinelaLast<T>();
	first->setNext(last);
	last->setPrev(first);

}


// Método para inserir um elemento na LOCK_FREE_lockFreelockFreeList.
template <class T>
void *lockFreeList<T>::insert(T *n) {

	int cont;
	elemento<T> *new_element= new elemento<T>(n);
	new_element->setPrev(first);
	while (!__sync_bool_compare_and_swap(&first->flag, 0, 1)) {

	}
	while (!__sync_bool_compare_and_swap(&first->getNext()->flag, 0, 1)) {

	}
	
	if(first->getNext()->ehSentinela()){		
		new_element->setNext(last);
		first->setNext(new_element);
		last->setPrev(new_element);
		__sync_lock_release(&first->flag);
		__sync_lock_release(&last->flag);
		return NULL;
	}

	new_element->setNext(first->getNext());
	first->getNext()->setPrev(new_element);
	first->setNext(new_element);
	__sync_lock_release(&first->getNext()->getNext()->flag);
	__sync_lock_release(&first->flag);
	return NULL;
}

template <class T>
T &lockFreeList<T>::deletCola(){
 	elementoAbs<T>  *aux;	
//	T *k;  // k sera usado para cast
	T *i;	
	int cont;
        
		while( !__sync_bool_compare_and_swap(&first->flag, 0 ,1)){
		}
		REINICIO:
		if( ! __sync_bool_compare_and_swap(&first->getNext()->flag,0,1)){	
				__sync_lock_release(&first->flag);
				goto REINICIO; // goto libera as flags e ja ocupada e volta para o começo
			}
		if(first->getNext()->ehSentinela()){
			__sync_lock_release(&first->flag);
			__sync_lock_release(&last->flag);
			i=NULL;
			return *i;
		}else {
			while(!__sync_bool_compare_and_swap(&first->getNext()->getNext()->flag, 0 ,1) ){
				if(cont == 1000 ){
					__sync_lock_release(&first->getNext()->flag);
					__sync_lock_release(&first->flag);
					goto REINICIO;  // goto libera as flags e ja ocupada e volta para o começo
				}
				cont ++;
				
			}
			aux= first->getNext();
			i=first->getNext()->getT();
			first->getNext()->getNext()->setPrev(first);
			first->setNext(first->getNext()->getNext());
			//free (aux);
			
			__sync_lock_release(&first->flag);
			__sync_lock_release(&first->getNext()->flag);
		}	
 		//k=(T*)i;

 		return *aux->getT();		
 }	
 		
 
 
// delete head
 template <class T>
T &lockFreeList<T>::deletHead(){
 	elementoAbs<T>  *aux;	
	//T *k;  // k sera usado para cast
	T *i;	
	int cont;
	
		
		while( !__sync_bool_compare_and_swap(&last->flag, 0 ,1)){
        		
		}
		REINICIO:	
		if(  !__sync_bool_compare_and_swap(&last->getPrev()->flag,0,1)){	
				__sync_lock_release(&last->flag);
				goto REINICIO; // goto libera as flags e ja ocupada e volta para o começo
			}
		if(last->getPrev()->ehSentinela()){
			__sync_lock_release(&first->flag);
			__sync_lock_release(&last->flag);
			i=NULL;
			return *i;
		}else {
			cont =0;
			while(!__sync_bool_compare_and_swap(&last->getPrev()->getPrev()->flag, 0 ,1) ){
				if(cont == 1000 ){
					__sync_lock_release(&last->flag);
					__sync_lock_release(&last->getPrev()->flag);
					goto REINICIO; // goto libera as flags e ja ocupada e volta para o começo
				}
				cont ++;	
			}
			aux= last->getPrev();
			i = last->getPrev()->getT();
			last->getPrev()->getPrev()->setNext(last);
			last->setPrev(last->getPrev()->getPrev());
			//free(aux);
			__sync_lock_release(&last->flag);
			__sync_lock_release(&last->getPrev()->flag);
		}
 	
	//	k=(T*)i;
 		return *aux->getT();
 }	
 		

#endif
