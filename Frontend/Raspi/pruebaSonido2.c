#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "audio.h"            // ATENCAO LA RUTA // ../../some files/RPi_Files/audio/SDL2/src/audio.h
#include "timer/timer.h"

int main(void) 
{

    if ( initAudio() == NO_INIT)
    {
        fprintf(stderr, "Audio not initilized.\n");
	endAudio();
	return -1;
    }

    own_timer_t temp;
    setTimer(&temp, 8.0);
    startTimer(&temp);

    while(1)
    {
        while( !checkTimer(&temp) );

        printf("OSTRAS, SE DEBERIA DISPARAR YA\n");
        if( !(player_status() == PLAYING ) )
        {
            char mySong[] = "../Sounds/shoot.wav";
	        set_file_to_play(mySong);       				
	        play_sound();
        }
    }
}