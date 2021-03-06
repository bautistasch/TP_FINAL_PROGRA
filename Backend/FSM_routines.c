/***************************************************************************//**
  @file     +FSM_routines.c+
  @brief    +Funciones que conforman las rutas de acción entre los diferentes
            estados de la FSM.+
  @author   +Grupo 3+
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "FSM_routines.h"
#include "event_queue/event_queue.h"
#include "../const.h"
#include <stdio.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define SPEED_LAPSE 5       // Intervalo de tiempo en el que se aumenta la velocidad.


#if PLATFORM == ALLEGRO                          // Se incluyen las funciones del front
#include "../Frontend/Allegro/headall.h"          

#elif PLATFORM == RASPI
#include "../Frontend/Raspi/headall.h" 
#endif

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

BOOL running = 1;

MENU_ITEM main_menu[] = {  
                            {.option = "Jugar", .essential = TRUE, .ID = PLAY_ID},
                            {.option = "Puntajes", .essential = TRUE, .ID = SCORE_ID},
                            {.option = "Instrucciones", .essential = FALSE, .ID = INSTUCTION_ID},
                            {.option = "Salir", .essential = TRUE, .ID = EXIT_ID},
                        };
 

MENU_ITEM pause_menu[] = {  
                            {.option = "Continuar", .essential = TRUE, .ID = RESUME_ID},
                            {.option = "Reiniciar", .essential = TRUE, .ID = PLAY_ID},
                            {.option = "Volver al menu", .essential = TRUE, .ID= BACK_ID} ,
                        };

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static int actual_option = 0;                            // Variable que marca la opcion del menú seleccionada.   
static char actual_name[NAME_SIZE+1];
static SCORE leadboard[LEADERBOARD_SIZE+1];                // Creo matriz con el leaderboard  
static int letter_counter=0;                             // Variable que da el indice de la letra a cargar.
static int letter;                                       // Variable que retiene la letra que se quiere cargar.   

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**********************************************************
 ***********************  SPLASH   *************************
 **********************************************************/

void show_splash(void){     
    splash_front(); 
    
    #ifdef DEBUG
        printf("Muestro el SPLASH. \n");
    #endif          
}

/**********************************************************
************************  MENU  ***************************
**********************************************************/

void my_menu(){
    actual_option=0;
    show_menu (main_menu, sizeof(main_menu)/sizeof(MENU_ITEM), actual_option);  //Actualizo el menu, resaltando la opcion actualizada.  
    #ifdef DEBUG
        printf("Muestro el menú principal. \n");
    #endif

}

void up_menu(MENU_ITEM* menu, int menu_size){ 
    
    #ifdef ONLY_ESSENTIAL                                                    
            #ifdef DEBUG
                printf("Estoy en modo ONLY_ESSENTIAL \n");
            #endif

            do{                                                                
                
                if(actual_option>0){                            // Si el front solo permite mostrar las opciones esenciales:
                    actual_option--;                                                        //subimos en el menú hasta la siguiente opcion esencial siempre
                }                                                                           //y cuando haya una arriba.
            } while ((menu[actual_option]).essential==FALSE && menu_size/sizeof(MENU_ITEM) > actual_option); 
    
    #else                                                                   // Si el front permite mostrar las opciones no esenciales:
        if(actual_option > 0){        
            actual_option--;                                                //subimos en el menú hasta la siguiente opcion
        }

    #endif

    #ifdef DEBUG
    printf("La nueva opción actual es: %d \n", main_menu[actual_option].ID);
    #endif 

    show_menu (menu, menu_size/sizeof(MENU_ITEM), actual_option);          // Actualizamos el front. 

    #ifdef DEBUG
    printf("Se actualizó el menú \n");
    #endif 
}

void down_menu(MENU_ITEM* menu, int menu_size){
    
    #ifdef ONLY_ESSENTIAL   
        #ifdef DEBUG
                printf("Estoy en modo ONLY_ESSENTIAL \n");
        #endif         
        do{
           if(menu_size/sizeof(MENU_ITEM)-1 > actual_option) {                          // Si el front solo permite mostrar las opciones esenciales:
                actual_option++;                                                        //bajamos en el menú hasta la siguiente opción esencial siempre
           }                                                                            //y cuando haya una abajo.
        } while ((menu[actual_option]).essential==FALSE && actual_option>0);
    
    #else                                                                               // Si el front permite mostrar las opciones no esenciales:
        
        if(menu_size/sizeof(MENU_ITEM)-1 > actual_option) {
            actual_option++;                                                            //bajamos en el menú hasta la siguiente opcion
        }
        
    #endif
    

    #ifdef DEBUG
    printf("La nueva opción actual es: %d \n", main_menu[actual_option].ID);
    #endif 
    
    show_menu (menu, menu_size/sizeof(MENU_ITEM), actual_option);                       // Actualizamos el front. 

    #ifdef DEBUG
    printf("Se actualizó el menú \n");
    #endif 
}

void up_menu_main()
{
    up_menu(main_menu, sizeof(main_menu));              // Subo en el menu principal
}

void down_menu_main()
{
    down_menu(main_menu, sizeof(main_menu));            // Bajo en el menu principal
}

void up_menu_pause()
{
    up_menu(pause_menu, sizeof(pause_menu));             // Subo en el menu de pausa
}

void down_menu_pause()
{
    down_menu(pause_menu, sizeof(pause_menu));           // Bajo en el menu de pausa
}


void click_menu()
{     
    switch ((main_menu[actual_option]).ID)
    {
        case PLAY_ID:
            add_event(PLAY_EVENT);        // Añadimos a  la cola de eventos
        break;

        case SCORE_ID:
            add_event(SCORE_EVENT);       // Añadimos a  la cola de eventos
        break;

        case INSTUCTION_ID:
            add_event(INSTRUCTION_EVENT);     // Añadimos a  la cola de eventos
        break;

        case EXIT_ID:
            add_event(EXIT_EVENT);        // Añadimos a  la cola de eventos
        break;        
    }

    #ifdef DEBUG
        printf("Se agregó a la cola de eventos el ID: %d \n", main_menu[actual_option].ID);
    #endif      
    actual_option=0;
}

void click_menu_pause()
{      

    switch (pause_menu[actual_option].ID)
    {
        case PLAY_ID:
            add_event(PLAY_EVENT);            // Añadimos a  la cola de eventos
                                                    //el evento para redireccionar la FSM.
        break;

        case RESUME_ID:
            add_event(RESUME_EVENT);          // Añadimos a  la cola de eventos
                                                    //el evento para redireccionar la FSM. 
        break;
        
        case BACK_ID:
            add_event(BACK_EVENT);            // Añadimos a  la cola de eventos
                                                    //el evento para redireccionar la FSM.
        break;
    }

    actual_option=0;

    #ifdef DEBUG
        printf("Se agregó a la cola de eventos: %d \n", main_menu[actual_option].ID);
        
    #endif 
}


/**********************************************************
*************  PAUSE/RESUME/START/QUIT   ******************
**********************************************************/

void pause_game(void){
    actual_option=0;
    show_menu(pause_menu, sizeof(pause_menu)/sizeof(MENU_ITEM), actual_option);
    pause_game_front();
    #ifdef DEBUG
        printf("Mostrando menú de pausa. \n");
    #endif
}

void resume_game(void){
    resume_game_front();
    
    #ifdef DEBUG
        printf("Reanudo partida. \n");
    #endif
}

void start_game(void){
    reset_lives();              // Renuevo las vidas.
    reset_points();             // Reinicio el contador de puntos.
    reset_level();              // Reinicio el contador de niveles.
    reset_killed_aliens();      // Reinicio el contador de aliens asesinados.
    #ifdef DEBUG
        printf("Se revivieron todos los invaders. \n");
    #endif    
    
    clean_shoots();             // Limpio los disparos en pantalla. 
    
    #ifdef DEBUG
        printf("Se borraron los tiros de la pantalla. \n");
    #endif
    init_game();
    
    #ifdef DEBUG
        printf("Iniciando partida. \n");
    #endif
}

void quit_game(void) {
    destroy_front();
    running=0;                          // Bajo el flag para correr el programa.                                    
    #ifdef DEBUG
        printf("Salgo del juego. \n");
    #endif
}

/**********************************************************
***********************  SCORE   **************************
**********************************************************/    

void show_game_score(){

    pause_game_front();                             // Pauso los timers para que el front no escuche eventos no deseados.

    unsigned long int score= get_points();          // Guardo la cantidad de puntos obtenidos en la partida.
    int level= get_level();                         // Guardo el nivel alcanzado en la partida.
    int killed_crabs= get_killed_aliens(CRAB);      // Guardo los crabs asesinados en la partida.   
    int killed_octo= get_killed_aliens(OCTO);       // Guardo los octopus asesinados en la partida.
    int killed_squid= get_killed_aliens(SQUID);     // Guardo los squids asesinados en la partida.
    int killed_ufo= get_killed_aliens(UFO);         // Guardo los UFOs asesinados en la partida.
    
    game_score_front(score, level, killed_crabs, killed_octo, killed_squid, killed_ufo);

    #ifdef DEBUG
        printf("Mostrando las estadisticas de la partida. \n");
    #endif
}


void show_global_score(void) {
    SCORE* p_leadboard=leadboard;                     // Coloco un puntero a su preimer elemento
    int cant= lect_score(p_leadboard);                // Coloco en memoria los datos del score y guardo la cantidad en cant
    if (cant==LEADERBOARD_SIZE+1){
        cant=LEADERBOARD_SIZE;
    }
    put_score ("Pueba", 0, LEADERBOARD_SIZE, p_leadboard);  // Obligo a que se reordene la lista sí o sí.
    show_score (p_leadboard, cant);                   // Llamo a la funcion que se encragra de mostrarlo en pantalla
}

void next_letter()
{
    if (letter_counter < NAME_SIZE-1)                         // Si me quedan letras por guardar:
    {
    letter_counter++;                              // Paso a la siguiente letra.                                           
    letter=actual_name[letter_counter];            // Cargo la siguiente letra de la nueva ubicación.
    
    if(letter=='_')    
    {
        actual_name[letter_counter]='A';
    } 

    #ifdef DEBUG
        printf("Se confirmo la letra: %c. El arreglo quedó %s.\n", actual_name[letter_counter], actual_name);
    #endif

    score_name_front(actual_name, NAME_SIZE, letter_counter, get_points()); // Muestra la pantalla de carga de puntaje.    
    
    }
}      

void previous_letter()
{
    if(letter_counter>0)                                // Si no estoy en la primer letra:
    {
        letter_counter--;                               //Retrocedo una letra.
        #ifdef DEBUG
            printf("Se retrocedió una letra \n");
        #endif
        
        letter=actual_name[letter_counter];             // Cargo la anterior letra de la nueva ubicación.

    
    score_name_front(actual_name, NAME_SIZE, letter_counter, get_points()); // Muestra la pantalla de carga de puntaje.

    }   
}

void upper_letter()
{
    if (letter=='Z'){                           // Si tengo cargada una 'Z'                        
        letter= '_';                            //cargo el caracter ' '.
    }  
    else if (letter=='_'){                      // Si tengo cargada un ' '                        
        letter= 'A';                            //cargo el caracter 'A'.
    }    

    else{
        letter++;                               // Si es una letra cualquiera paso a la siguiente del
                                                //abcedario.
    }
    #ifdef DEBUG
        printf("Se pasó a la letra %c \n", letter);
    #endif
    actual_name[letter_counter]=letter;

    score_name_front(actual_name, NAME_SIZE, letter_counter, get_points()); // Muestra la pantalla de carga de puntaje.
}

void lower_letter()
{
    if (letter=='A'){                           // Si tengo cargada una 'A'                        
        letter= '_';                            //cargo el caracter ' '.
    }  
    else if (letter=='_'){                      // Si tengo cargada un ' '                        
        letter= 'Z';                            //cargo el caracter 'Z'.
    }    
    else 
    {                                           // Si es una letra cualquiera paso a la anterior del
        letter--;                               //abcedario.
    }
    #ifdef DEBUG
        printf("Se pasó a la letra %c\n", letter);
    #endif
    actual_name[letter_counter]=letter;

    score_name_front(actual_name, NAME_SIZE, letter_counter, get_points()); // Muestra la pantalla de carga de puntaje.
}

void save_score(){

    #ifdef DEBUG
    printf("El nombre a guardar es: %s\n", actual_name);
    #endif

    SCORE* p_leadboard=leadboard;                       // Coloco un puntero a su primer elemento
    int not_null_char=0;                                // Creo una variable que cuente los ' '.
    for(int i=0; i<NAME_SIZE; i++)                      // Para cada caracter no terminador de
    {                                                   //actual_name:
        if (actual_name[i] != '_'){                     // Reviso si NO es ' ' y si es así              
            not_null_char++;}                           // incremento la cantidad de posiciones
                                                        // que tinen valor.
        else{
            actual_name [i] = ' ';       
        }
    }                                                   

    if (not_null_char>0)                                // Si afirmativamente hay algun caracter guardado:
    {

        put_score (actual_name, get_points(), LEADERBOARD_SIZE, p_leadboard);   // Ejecuto la función que guarda el 
                                                                                //puntaje ordenado en el archivo.

        #ifdef DEBUG
        printf("Se guadro el nuevo score\n");
        #endif
    }

    my_menu();                                          // Precargo el menú antes de salir.
}

void saving_init()
{
    actual_name[0]='A';

    for(int i=1; i<NAME_SIZE; i++)
    {
        actual_name[i]= '_';        // Inicio el arreglo con espacios.
    }
    actual_name[NAME_SIZE]=0;       // Cargo el terminador.
    letter_counter=0;               // Apunto al primer elemento del arreglo nombre
    letter= actual_name[letter_counter];

    #ifdef DEBUG
        printf("Se inicializó la carga \n");
    #endif

    score_name_front(actual_name, NAME_SIZE, letter_counter, get_points());    
}


/**********************************************************
*********************  COLLISION   ************************
**********************************************************/
void crab_coll()
{
    kill_alien(CRAB);       //Ejecutamos la función que guarda en
                            // la info de la partida que se asesinó 
                            // un crab.
}
void octo_coll()
{
    kill_alien(OCTO);    //Ejecutamos la función que guarda en
                            // la info de la partida que se asesinó 
                            // un octopus. 
}

void squid_coll()
{
    kill_alien(SQUID);      //Ejecutamos la función que guarda en
                            // la info de la partida que se asesinó 
                            // un squid. 
}

void ufo_coll()
{
    kill_alien(UFO);        //Ejecutamos la función que guarda en
                            // la info de la partida que se asesinó 
                            // un UFO.    
}

void cannon_coll()
{
    if(decrease_lives()<1)          //Si las vidas llegan a 0,
    {                               // se considera que el jugador ya perdió
        add_event(END_GAME_EVENT);  // y se agrega el evento de que se acabó la partida.


        #ifdef DEBUG
            printf("El jugador se quedó sin vidas\n");
        #endif
    }
   
    reviveCanon();
}

/**********************************************************
**********************  MOVEMENT   ************************
**********************************************************/
void move_cannon_left()
{
   move_cannon(LEFT);
}

void move_cannon_right()
{
   move_cannon(RIGHT);
}

void stop_cannon_right()
{
   move_cannon(STOP_LEFT);
}

void stop_cannon_left()
{
   move_cannon(STOP_RIGHT);
}

/**********************************************************
*********************  VARIOUS   **************************
**********************************************************/

void refresh(void){
    speed_update(SPEED_LAPSE);                                  // Actualizo la velocidad con la que se mueven los invaders.
    redraw(get_points(),get_lives(),get_level());               // Redibujo la pantalla.
    if(checkWin()){                                             // Verifico si se pasó de nivel o no.
        add_event(NEXT_LEVEL_EV);
    }
}

void doNothing() {
    return;
}