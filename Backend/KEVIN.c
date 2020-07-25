#include <stdio.h>
#define NAME    6 // Implica que cada nombre tiene que tener 5 letras como máximo
#define OFFSET  '0'



typedef struct          // Estructura que contiene al nombre del usuario con su correspondiente puntaje 
{
    char name[NAME];
    unsigned long int pts;
} SCORE;

void put_score (char* name, unsigned long int score, int top, SCORE* my_pun); //coloca los nombres y scores en un txt odenados
//name: string con el nombres
//score: puntaje asignado al nombre
//top: cantidad de puntjes a mostrar
//mypun: puntero a una posicion de memoria donde se almacenaran los datos del score

int lect_score (SCORE* my_pun); // Coloco en memoria los datos del score
//mypun: puntero a una posicion de memoria donde se almacenaran los datos del score

void only_lect_score (SCORE* mypun); // coloco en memoria los datos del score
//mypun: puntero a una posicion de memoria donde se almacenaran los datos del score

void BubbleSort(SCORE arr[], int cont); // Ordena los eleementos de mayor a menor
//arr: arreglo  con los elementos a ordenar
//cont: cantidad de elementos a ordenar

int main (void){
    SCORE arr[50];                     //ni lo comento porque esto es un main de prueba
    SCORE* pun=arr;
   
    put_score ("KEVIN", 200, 5, pun);
    put_score ("PERON", 2104, 5, pun);
    put_score ("MARTI", 22, 5, pun);
    put_score ("CAR", 99923, 5, pun);
    put_score ("SERMA", 10034, 5, pun);
    put_score ("SCHEM", 5900, 5, pun);
    put_score ("BUSTE", 4100, 5, pun);
    
    put_score ("MESSI", 10395, 5, pun);
    put_score ("OSO", 44101, 5, pun);
    put_score ("BORRE", 10395, 5, pun);
    

    }
void put_score (char* name, unsigned long int score, int top, SCORE* my_pun)
{
    FILE* fp;
    fp= fopen ("score.txt","a+");                   // Abro el archivo con los puntajes
    fprintf (fp,"%s %lu\n", name, score);           // Le escribo el nombre y puntaje que fue provisto
    fclose(fp);                                     // Cierro el archivo
    int cant = lect_score (my_pun);                 // Cuento cuantos nombres hay y lo coloco en memoria
    if (cant<=top){                                 // Si la cantidad no supera el top   
        BubbleSort(my_pun, cant);                   // Lo agrego en el orden correspondiendte
        fp= fopen ("score.txt","w");         // Abro el archivo
        for (int i=0; i<cant; i++){
            fprintf(fp, "%s %lu\n", my_pun[i].name, my_pun[i].pts); // Le coloco los nombres y puntajes
        }
        fclose(fp);                                  // Cierro el archivo
    }
    else{
        BubbleSort(my_pun, top+1);                  // Agrego el nuevo punteje con el nombre correspondiente y lo ordeno 
        fp= fopen ("score.txt","w");         // Abro el archivo
        for (int i=0; i<top; i++){
            fprintf(fp, "%s %lu\n", my_pun[i].name, my_pun[i].pts); // Le coloco los nombres y puntajes
        }
                
        fclose(fp);                                 // Cierro el archivo
    }
	
}

int lect_score (SCORE* my_pun)
{
    FILE* fp;
    char str[NAME];             // Arreglo con el nombre
    SCORE score_data;           // Tipo de dato que almacena nombre y puntaje
    char c;                     // Caracter a leer
    unsigned long int num;      // Numero con el puntaje
    int cant =-1;               // Contador. Cuenta  la canitdad de puntajes que tiene el archivo
    fp=fopen ("score.txt","r"); // Abro el archivo
    int i;
    do
    {
        num=0;

        for (i=0; i<NAME-1 && (c=fgetc (fp))!=' ' ; i++){   //Leo hasta que se termine la palabra o llegue al maximo permitido
            if (c==EOF){
                return cant+1;                              // Si terminé de leer el archivo, salgo
            }
            str [i]=c;                                      // Cargo el nombre en un string
            
        }
        str [i]=0;                                          // Le agrego el terminador

        
        if (c!=' '){                                        // Si el ultimo caracter no era un espacio
            char temp=fgetc (fp);                             // Leo el espacio (basura)
        }


        while ((c=fgetc (fp))!=EOF && c!='\n'){ // Mientras que no Lea un enter, o el archivo se termine
            num=num*10+(c-OFFSET);              // vamos conformando el numero con los caracteres
        }
          for (int j=0; j<i+1; j++){
            score_data.name[j]=str[j];          // Cargamos el nombre a la memoria

        }
        score_data.pts=num;                     // Cargamos el puntaje a la memoria
        *my_pun=score_data;
        my_pun++;                               // Apuntamos al siguiente nombre y puntaje
        cant++;                               
    }while (c!=EOF);                            // Repetimos lo anterior hasta que hayamos leido todo el archivo
    fclose(fp);                                 // Cerramos el archivo 
    return cant;
}

void only_lect_score (SCORE* my_pun)
{
    FILE* fp;
    char str[NAME];             // Arreglo con el nombre
    SCORE score_data;           // Tipo de dato que almacena nombre y puntaje
    char c;                     // Caracter a leer
    int num;                    // Numero con el puntaje
    fp=fopen ("score.txt","r");

    do
    {
        num=0;
        fgets (str,NAME,fp);                    // Coloco el nombre en el Arreglo
        fgetc (fp);                             // Leo el espacio (basura)

        while ((c=fgetc (fp))!=EOF && c!='\n'){ // Mientras que no Lea un enter, o el archivo se termine
            num=num*10+(c-OFFSET);              // vamos conformando el numero con los caracteres
        }
        for (int i=0; i<NAME; i++){
            score_data.name[i]=str[i];          // Cargamos el nombre a la estructura
        }
        score_data.pts=num;                     // Cargamos el puntaje a la estructura
        *my_pun=score_data;
        my_pun++;                               // Apuntamos al siguiente nombre y puntaje
    }while (c!=EOF);                            // Repetimos lo anterior hasta que hayamos leido todo el archivo
    fclose(fp);                                 // Cerramos el archivo 
}

void BubbleSort (SCORE arr[], int cont){
	SCORE temp;
	for (int j=0; j<cont-1; j++){               // Mientras que no supere la cantidad        
		for (int i=0; i<cont-1; i++){	
			if (arr[i].pts<arr[i+1].pts){       // Voy ordenando el arreglo de mayor a menor
				temp= arr[i];
				arr [i]=arr[i+1];
				arr[i+1]=temp;
			}
		}
	}
}