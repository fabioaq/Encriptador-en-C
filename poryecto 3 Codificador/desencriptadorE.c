#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

//para la clave
char *key;
size_t bufsizekey = 512;//tamanio maximo para la clave
int  characteres;//cantidad de caracteres sin "\n"
int codeProgress = 0;
//*
int cantThreads;
pthread_mutex_t sem;
pthread_mutex_t semP;
pthread_mutex_t semD;
FILE* codex;
FILE* fileO;
int triggerProducer = 1;
int fileSize;
int contCrypt = 0;
int contProd = 0;
int tambuff = 0;

//cola
typedef struct Queue{
    int bufferSize;
    int size;
    int front;
    int rear;
    unsigned char* buffer;

}Queue;

Queue* Q;

void createQueue(int maxBuffer){
    Q = (Queue *)malloc(sizeof(Queue));
    Q->buffer = (unsigned char *)calloc(maxBuffer, sizeof(unsigned char));
    Q->size = 0;
    Q->bufferSize = maxBuffer;
    Q->front = 0;
    Q->rear = -1;
}

unsigned char pop(){
    pthread_mutex_lock(&semD);
    unsigned char c = '\n';
    if(Q->size==0){
        printf("Buffer esta vacio!\n");
    }
    else{
        c = Q->buffer[Q->front];
        Q->size--;
        Q->front++;
        if(Q->front==Q->bufferSize){
            Q->front=0;
        }
    }
    pthread_mutex_unlock(&semD);
    return c;
}

void push(unsigned char byte){
    pthread_mutex_lock(&semD);
    if(Q->size == Q->bufferSize){
        printf("Buffer esta lleno!\n");
    }
    else{
        Q->size++;
        Q->rear = Q->rear + 1;
        if(Q->rear == Q->bufferSize){
            Q->rear = 0;
        }
        Q->buffer[Q->rear] = byte;
    }
    pthread_mutex_unlock(&semD);
    return;
}

int isEmpty(){
    int e = 0;
    if(Q->size == 0){
        e = 1;
    }
    return e;
}
/* */

/*misc*/
char* creatingCesar(const char* arr){
    int arrnameLenght;
    int cesLen;
    for(arrnameLenght=0; arr[arrnameLenght]!= '\0' ; arrnameLenght++);
    char* extension = ".cesar";
    for(cesLen=0; extension[cesLen]!= '\0' ; cesLen++);
    char* arrces = calloc((arrnameLenght - cesLen),sizeof(char));
    for(int i = 0; i < arrnameLenght - cesLen; i++){
      arrces[i] = arr[i];
    }
    return arrces;
}

void print(){
  pthread_mutex_lock(&semD);
  printf("\e[2J\e[H");
  printf("%s%i\n", "Memory Read =", contProd-1 );
  printf("%s%i\n", "Memory Write=", contCrypt-1 );
  printf("%s%i\n", "Memory Size =", fileSize );
  pthread_mutex_unlock(&semD);
}
/**/

/*Decrypter*/
void* decrypter(){
    unsigned char c;
    while (contCrypt <= fileSize) {
        pthread_mutex_lock(&sem);
        //print();
        while(isEmpty() && contProd <= fileSize);
        if(contCrypt <= fileSize){
            c = pop();
            c = ((unsigned char)c - (unsigned char)key[codeProgress]);
            fputc(c, codex);
            if(codeProgress < characteres && characteres != 1){
              codeProgress ++;
            }
            else{
              codeProgress = 0;
            }
            contCrypt ++;
        }
        pthread_mutex_unlock(&sem);
    }
    pthread_exit(0);
}
/**/

/*Filler*/
void* producer(){
    char c;
    while(contProd <= fileSize ){
        pthread_mutex_lock(&semP);
        //print();
        while (Q->size == Q->bufferSize);
        if(contProd <= fileSize){
            c = fgetc(fileO);
            push(c);
            contProd++;
        }
        pthread_mutex_unlock(&semP);
    }
    pthread_exit(0);
}
/**/

int main(int argc, char *argv[]){
	 if (argc != 4){
       	printf("Error!! cantidad erronea de parametros...\n");
       	return 1;
    }
    char* filename = argv[1];
    cantThreads = atoi(argv[2]);
    int tambuff = atoi(argv[3]);
    codex = fopen(creatingCesar(filename), "wb");
    key = (char *)malloc(bufsizekey * sizeof(char));
    if( key == NULL){
        perror("No se pudo alojar la key en memoria, intentelo de nuevo.");
        exit(1);
    }

    printf("Ingrese una key con la que codifico el archivo: ");
    characteres = (int)getline(&key,&bufsizekey,stdin)-1;
    printf("%i Cantidad de characteres en la clave.\n",characteres);
    printf("%s\n","open file" );
    /**/
    fileO = fopen(filename, "rb");
    fseek(fileO, 0L, SEEK_END);
    fileSize = ftell(fileO)-1;
    rewind(fileO);
    /**/
    createQueue(tambuff);
    pthread_t threads[cantThreads];
    pthread_t prod[cantThreads];
    printf("%s\n","send producer" );
    for(int i = 0; i < cantThreads; i++){
      pthread_create(&prod[i], NULL, producer, NULL);
    }
    printf("%s\n","send encrypter" );
    for(int i = 0; i < cantThreads; i++){
      pthread_create(&threads[i], NULL, decrypter, NULL);
    }
    for(int i = 0; i < cantThreads; i++){
      pthread_join(prod[i], NULL);
    }
    for(int i = 0; i < cantThreads; i++){
      pthread_join(threads[i], NULL);
    }
    fclose(fileO);
    fclose(codex);
    return(0);
}
