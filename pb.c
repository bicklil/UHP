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

	pb = (pb_t *) malloc(sizeof(pb_t));
	pb->taille1 = pb->taille2 = pb->type = 0;
    pb->debut = pb->fin = 0;
	pb->data1 = pb->data2 = NULL;

	return pb;
}

void pb_free(pb_t *pb)
{	
	if(pb != NULL){

		if (pb->data1 != NULL){
			free(pb->data1);
			pb->data1 = NULL;
		}
		
		if (pb->data2 != NULL){
			free(pb->data2);
			pb->data2 = NULL;
		}

		free(pb);
		pb = NULL;
	}
}

/*
 * affichage d'un probleme
 */
void pb_print(pb_t *pb)
{	
	if(pb != NULL)
		fprintf(stdout, "PB:\n\t Debut -- > %i Fin -- > %i \n\t Taille1 -- > %i\n\t Taille2 -- > %i\n\t" , 
			pb->debut, pb->fin, pb->taille1, pb->taille2 );
}

/* 
 * envoi d'un probleme a un processus tid
 * le probleme est dealloue localement
 */
void send_pb(int tid, pb_t *pb)
{
	pvm_initsend(PvmDataDefault);
	pvm_pkint(&(pb->debut),1,1);
    pvm_pkint(&(pb->fin),1,1);
	pvm_pkint(&(pb->taille1), 1, 1);
	pvm_pkint(&(pb->taille2), 1, 1);
	pvm_pkint(&(pb->type), 1, 1);
	pvm_pkint(pb->data1, pb->taille1, 1);
	
	if(pb->taille2 != 0){
		pvm_pkint(pb->data2, pb->taille2, 1);
	}

	pvm_send(tid, MSG_PB);

	pb_free(pb);
}



/**
 * @brief      Convert a pb to a set of points
 *
 * @param      pb    Problem to convert
 * @param[in]  data  The data to convert
 *
 * @return     A set of points
 */
point * pb_to_points(pb_t * pb, int data){
	point * pts = NULL, * tmp;
	int i;

	if(data == 1 && pb->taille1 > 0)
	{	
		pts    = point_alloc();
		
		tmp = pts;

		for(i = 0; i < pb->taille1; i+= 2)
		{	
			tmp->x = pb->data1[i];
			tmp->y = pb->data1[i + 1];

			tmp->next = point_alloc();
			tmp       = tmp->next;
		}

	}
	else if(data == 2 && pb->taille2 > 0)
	{
		pts    = point_alloc();

		tmp = pts;
		for(i = 0; i < pb->taille2; i+= 2)
		{	
			tmp->x = pb->data2[i];
			tmp->y = pb->data2[i + 1];
			
			tmp->next = point_alloc();
			tmp       = tmp->next;
		}
	}

	return pts;
}


/**
 * @brief      Convert a set of points to a problem
 *
 * @param      pts   The points
 * @param      pb    The problem to modify
 *
 * @return     The problem setted with points values
 */
pb_t * points_to_pb(point * pts, pb_t * pb){
	point * tmp;
	int i = 0;

	if(pts != NULL)
		pb->taille1 = point_nb(pts) * 2;
	
	pb->data1 = realloc(pb->data1, sizeof(int) * pb->taille1);

	while(pts != NULL){

		pb->data1[i] = pts->x;
		pb->data1[i + 1] = pts->y;

		tmp = pts->next;
		point_free(pts);
		pts = tmp;

		i += 2;
	}

	return pb;
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
	int id_mes, tag_mes;
	
	id_mes = pvm_recv(tid, -1);
	pvm_bufinfo(id_mes, NULL, &tag_mes, sender);

	if (tag_mes != MSG_PB) return NULL;

	pb = pb_alloc();
    pvm_upkint(&(pb->debut), 1, 1);
    pvm_upkint(&(pb->fin), 1, 1);
	pvm_upkint(&(pb->taille1), 1, 1);
	pvm_upkint(&(pb->taille2), 1, 1);
	pvm_upkint(&(pb->type), 1, 1);
	pb->data1 = malloc(sizeof(int) * pb->taille1);
	pvm_upkint(pb->data1, pb->taille1, 1);
	
	if(pb->taille2 != 0){
		pb->data2 = malloc(sizeof(int) * pb->taille2);
		pvm_upkint(pb->data2, pb->taille2, 1);
	}

	return pb;	
}