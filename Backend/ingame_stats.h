void reset_lives(void);
void reset_points(void);
void reset_shields(void);
void reset_level(void);

void increase_points(const int cant);
int decrease_lives(void);
void increase_level(void);

int get_lives(void);
int get_points(void);
int* get_shields(void);
int get_level();

void kill_alien(const int tipo_alien);
void shield_collision(int shield, int parte);
void increase_level();
void increase_speed(int cant);

#define INIT_LIFES 3
//DUDA: a const.h o a ingame.c?

//Puntaje que asingan los aliens.
#define CRAB_POINTS     10
#define OCTOPUS_POINTS  20  
#define SQUID_POINTS    30  
#define UFO_POINTS      50
#define MIN_RAND        2
#define MAX_RAND        4
//DUDA: Las constantes anteriores: a const.h o a ingame.c?


//NOTA: PASA PA'L FRONT.
#define SHIELDS         5
#define SHIELD_PARTS    2
#define SHIELD_LIFES    2

enum INVADERS{
    CRAB = 0,
    OCTOPUS,
    SQUID,
    UFO,
};

enum PARTES_SHIELD{
    IZQUIERDA=0,
    DERECHA,
};


