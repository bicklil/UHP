#define MSG_PB		0
#define MSG_END		1

#define PB_HULL      	0
#define PB_MERGE 		1


typedef struct st_pb {			/* probleme de tri */
    int debut, fin; // indice du premier et dernier element
    int taille1, taille2; //attention les tailles sont le double de la taille reelle a cause du x et y de chaque point
	int type;
	int *data1; // tab de x ,y pour chaque point
	int *data2;
}pb_t;

typedef struct liste_pb liste_pb;

struct liste_pb{
    pb_t * pb;
    liste_pb * next;
};

extern pb_t *pb_alloc();
extern void pb_free(pb_t * pb);