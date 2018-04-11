/* TP algorithmique parallele
 * LL
 * 03/03/18
 */

/* 
 * pb.c
 * - copie de tableau d'entiers
 * - (de)allocation de pb
 * - affichage
 * - envoi/reception de pb
 */

#include <stdio.h>
#include <malloc.h>
#include "pvm3.h"
#include "pb.h"

/*
 * (de)allocation d'un probleme
 */
pb_t *pb_alloc()
{
	pb_t *pb;

	pb = (pb_t *)malloc(sizeof(pb_t));
	pb->taille1 = pb->taille2 = pb->type = 0;
    pb->debut = pb->fin = 0;
	pb->data1 = pb->data2 = NULL;

	return pb;
}

void pb_free(pb_t *pb)
{
	if (pb->data1) free(pb->data1);
	if (pb->data2) free(pb->data2);
	free(pb);
}

/*
 * affichage d'un probleme
 */
void pb_print(pb_t *pb)
{

}

/* 
 * envoi d'un probleme a un processus tid
 * le probleme est dealloue localement
 */
void send_pb(int tid, pb_t *pb)
{
	pvm_initsend(PvmDataDefault);
	pvm_pkint(&(pb->taille1), 1, 1);
	pvm_pkint(&(pb->taille2), 1, 1);
	pvm_pkint(&(pb->type), 1, 1);
	pvm_pkint(pb->data1, pb->taille1, 1);
	pvm_pkint(pb->data2, pb->taille2, 1);
	pvm_send(tid, MSG_PB);

	pb_free(pb);
}

/*
 * reception d'un probleme venant d'un processus tid
 * allocation locale pour le probleme
 * met a jour le le tid de l'envoyeur dans sender 
 * (utile dans le cas ou la reception venait 
 * d'un processus indifferent (tid == -1))
 * retourne NULL si le message n'est pas de type MSG_PB
 */

pb_t *receive_pb(int tid, int *sender)
{
	pb_t* pb;
	int id_mes, tag_mes, tid_mes;
	
	id_mes = pvm_recv(tid,-1);
	pvm_bufinfo(id_mes,NULL,&tag_mes,&tid_mes);
	*sender = tid_mes;
	if (tag_mes != MSG_PB) return NULL;

	pb = pb_alloc();
    pvm_upkint(&(pb->debut),1,1);
    pvm_upkint(&(pb->fin),1,1);
	pvm_upkint(&(pb->taille1),1,1);
	pvm_upkint(&(pb->taille2),1,1);
	pvm_upkint(&(pb->type),1,1);
	pb->data1 = malloc(sizeof(int)*pb->taille1);
	pvm_upkint(pb->data1,pb->taille1,1);
	if(pb->taille2 != 0){
		pb->data2 = malloc(sizeof(int)*pb->taille2);
		pvm_upkint(pb->data2,pb->taille2,1);
	}

	return pb;
	
}

liste_pb* liste_pb_alloc(){
	liste_pb* liste;

	liste =  malloc(sizeof(liste_pb));
	liste->pb = pb_alloc();
	liste->next = NULL;
	
	return liste;
}

liste_pb* ajout_pb(liste_pb* pbs,pb_t* pb)
{
	//TODO
	liste_pb* temp = pbs;

}

pb_t* creation_pb(point* pts,int start, int end,int type)
{
	pb_t* pb;
	int i;
	point* temp = pts;
	int taille = point_nb(pts);
	pb = malloc(sizeof(pb_t));

	pb->taille1 = taille*2;
	pb->debut = start;
	pb->fin = end;
	pb->type = type;

	pb->data1 = malloc(sizeof(int)*pb->taille1);
	for(i=0;i<taille;i++)
	{
		pb->data1[i] = temp->x;
		pb->data1[i+1] = temp->y;
		temp = temp->next; 
	}

	return pb;
}


point* cretion_point(pb_t* pb,int numero)
{
	int i,taille;
	int* data;
	point* pt = point_alloc();

	if (numero == 1)
	{
		data = pb->data1;
		taille = pb->taille1;
	}
	else
	{
		data = pb->data2;
		taille = pb->taille2;
	}	

	if (taille == 0)
		return NULL;
	
	pt->x = data[0];
	pt->y = data[1];

	for(i=1;i<taille/2;i++)
	{
		pt->next = point_alloc();
		pt = pt->next;
		pt->x = data[i*2];
		pt->y = data[i*2 +1];

	}
}







