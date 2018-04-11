#include "point.h"

#define MSG_PB		0
#define MSG_END		1

#define PB_HULL      	0
#define PB_MERGE 		1

#define NB_ESCLAVE      10


struct st_pb {			/* probleme de tri */
    int debut, fin; // indice du premier et dernier element
    int taille1, taille2; //attention les tailles sont le double de la taille reelle a cause du x et y de chaque point
	int type;
	int *data1; // tab de x ,y pour chaque point
	int *data2;
};

typedef struct st_pb pb_t;

typedef struct st_liste_pb liste_pb;
typedef struct st_liste_pb{
    pb_t* pb;
    liste_pb* next;
};

liste_pb* liste_pb_alloc();
pb_t *pb_alloc();
void pb_free(pb_t *pb);
void pb_print(pb_t *pb);
void send_pb(int tid, pb_t *pb);
pb_t *receive_pb(int tid, int *sender);
liste_pb* ajout_pb(liste_pb* pbs,pb_t* pb);
pb_t* creation_pb(point* pts,int start, int end, int type);
point* cretion_point(pb_t* pb,int numero);
