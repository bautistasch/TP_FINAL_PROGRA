#include <stdio.h>
#include "../const.h"
#include "FSM_table.h"
#include "FSM_routines.h"
#include "event_queue/event_queue.h"

enum { PC, RASPI };
#define PLATFORM PC
// Cambiar por PC o Raspi para compilar para la plataforma adecuada

#if PLATFORM == PC
#include "../Frontend/Allegro/hallegro.h" 
#elif PLATFORM == RASPI
#include "../Frontend/Raspi/....."
#endif

BOOL running;   

int main()
{
    STATE* p_tabla_estado_actual = splash_state;

    if (init_front())             
    {
        return -1;          //init_front inicializa el front sea cual sea la plataforma,
    }                       //Debe estar definida en cada front y returnar !=0 si hay error.
    
    splash_front();

    while (running)
    {
        //NOTA: Agregar funcion redraw.
        
        update_front_event();               // Actualizar la lista de eventos que me indica el front.
        event_t evento = get_next_event();  // Tomo un nuevo evento de la cola de eventos.

        #if DEBUG

        #endif


        if (evento!= NULL_EVENT)
        {
            p_tabla_estado_actual = fsm_interprete(p_tabla_estado_actual, evento);  // Actualizo el estado
        }    
    }

    // TODO: destroy resources

    return 0;
}

STATE *fsm_interprete(STATE * p_tabla_estado_actual, event_t evento_actual)
{
    #ifdef DEBUG
    printf("Evento: %d. \n", evento_actual); // Para debuggear
    #endif  //DEBUG

    while (p_tabla_estado_actual -> evento != evento_actual && p_tabla_estado_actual -> evento !=FIN_TABLE)
    ++p_tabla_estado_actual;
    
    (*p_tabla_estado_actual -> p_rut_accion) (); // Ejecuta Rutina de accion correspondiente
    p_tabla_estado_actual = p_tabla_estado_actual -> proximo_estado; // Encuentro próximo estado
    return (p_tabla_estado_actual);    
}
