/* TP algorithmique parallele
 * maitrise
 * LL
 * 13/03/2018
 * utilise gnuplot au lieu de jgraph
 */

/*
 * upper.c
 *
 * programme principal en sequentiel
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "point.h"
#include "pb.h"
#include "pvm3.h"

#define MAX_PB_SIZE 4


/**
 * @brief      Allocate memory for a new problem and init it.
 *
 * @param[in]  nbPts  The number of points
 *
 * @return     A new problem initialized.
 */
liste_pb *  elt_alloc(int nbPts){
	liste_pb * elt;

	elt              = malloc(sizeof(liste_pb));
	elt->pb          = pb_alloc();
	elt->pb->taille1 = nbPts * 2;
	elt->pb->data1   = malloc(sizeof(int) * elt->pb->taille1);
	elt->next        = NULL;

	return elt;
}


/**
 * @brief      Free memory allocated for a problem
 *
 * @param      elt   The element
 */
void elt_free(liste_pb * elt){
	if(elt != NULL){
		pb_free(elt->pb);
		free(elt);
	}
}


/**
 * @brief      Set points in current problem
 *
 * @param      elt        The element
 * @param      ptr_point  The pointer point
 */
void elt_set_pb(liste_pb * elt, point * ptr_point){
	int i;
	point * current = ptr_point;

	if(!current) return;

	for(i = 0; i < elt->pb->taille1 ; i+=2){
		elt->pb->data1[i] = current->x; 
		elt->pb->data1[i+1] = current->y;

		if(current->next != NULL)
			current = current->next; 
 	}
}


/**
 * @brief      Set indexes of points in current problem
 *
 * @param      elt    The element
 * @param[in]  start  The start
 * @param[in]  end    The end
 */
void elt_set_indexes(liste_pb * elt, int start, int end){
	elt->pb->debut = start; 
	elt->pb->fin = end; 
}

/**
 * @brief      Made only for debug purpose.
 *
 * @param      head  The head of problems list
 */
void list_print(liste_pb * head){
	liste_pb * cur = head;
	int i;

	while(cur != NULL){

		printf("Datas 1 (%i-%i):\n", cur->pb->debut, cur->pb->fin);
		for(i = 0; i < cur->pb->taille1; i += 2){
			printf("d: %i f: %i x: %i y: %i\n", cur->pb->debut, cur->pb->fin, cur->pb->data1[i], cur->pb->data1[i+1]);
		}

		printf("\nDatas 2 (%i-%i):\n", cur->pb->debut, cur->pb->fin);
		for(i = 0; i < cur->pb->taille2; i += 2){
			printf("d: %i f: %i x: %i y: %i\n", cur->pb->debut, cur->pb->fin, cur->pb->data2[i], cur->pb->data2[i+1]);
		}
		printf("\n\n");

		cur = cur->next;
	}
}


/**
 * @brief      Free memory allocated for problems
 *
 * @param      head  The head
 */
void list_free(liste_pb * head){
	liste_pb * current = head, * tmp;

	while(current != NULL){
		tmp = current;
		current = current->next;
		elt_free(tmp);
	}
}

/**
 * @brief      Initialization of problems list
 *
 * @param      head  The head
 * @param      pts   The points
 */
void list_init(liste_pb ** head, point * pts){
	liste_pb * newElt, * current;
	point * current_point = pts;
	int start = 1, end = 1, nbPts = point_nb(pts), i;
	
	if(nbPts <= 0) return;

	if(* head == NULL){
		
		if(nbPts < MAX_PB_SIZE){
			newElt = elt_alloc(nbPts);
			end = start + nbPts - 1;
			nbPts = 0;
		}else{
			newElt = elt_alloc(MAX_PB_SIZE);
			end = start + MAX_PB_SIZE - 1;
			nbPts -= MAX_PB_SIZE;
		}

		elt_set_pb(newElt, current_point);
		elt_set_indexes(newElt, start, end);

		* head = newElt;
	}
	
	for(i = 0; i < end - start + 1; i++){
		current_point = current_point->next;
	}

	start         = end + 1;
	current       = * head;

	while(nbPts > 0){

		if(nbPts < MAX_PB_SIZE){
			newElt = elt_alloc(nbPts);
			end = start + nbPts - 1;
			nbPts = 0;
		}else{
			newElt = elt_alloc(MAX_PB_SIZE);
			end = start + MAX_PB_SIZE - 1;
			nbPts -= MAX_PB_SIZE;
		}

		elt_set_pb(newElt, current_point);
		elt_set_indexes(newElt, start, end);

		current->next = newElt;
		current = current->next;

		for(i = 0; i < end - start + 1; i++){
			current_point = current_point->next;
		}

		start = end + 1;
	}

}

void range_pb(liste_pb* pbs,pb_t* pb)
{	
	liste_pb* pt_list, * tmp, * tmp2;
	
	if (pbs->pb == NULL){
		pbs->pb = pb;
		return;
	}

	pt_list = pbs;
	tmp = pt_list;

	/* Cherche où positionner le problème dans la liste */
	while(pt_list->pb->type == PB_HULL && pt_list->next != NULL && pb->debut > pt_list->pb->debut){
		pt_list = pt_list->next;
		if(pb->debut > pt_list->pb->debut)
			tmp = pt_list;
	}

	if (pb->debut > pt_list->pb->debut && tmp->next == NULL) /* Fin de liste et problème à placer à la suite */
	{
		tmp->next = malloc(sizeof(liste_pb));
		tmp->next->pb = pb;
		tmp->next->next = NULL;
	}else{/* Met à jour les chaînons en respectant l'ordre de tri */
		tmp2            = tmp->next;
		tmp->next->pb   = pb;
		tmp->next->next = tmp2;
	}

}

pb_t* trouve_prox(liste_pb* pbs, pb_t* pb)
{
	liste_pb* pt_list,*tmp = NULL;
	pb_t * res;

	if (pbs->pb == NULL)
		return NULL;

	pt_list = pbs;
	tmp     = pt_list;

	// On cherche le problème qui suit celui passé en paramètre
	while(pt_list->next != NULL){	
		
		// Le prochain élément est celui recherché
		if(pt_list->next != NULL && (pb->fin + 1) == pt_list->next->pb->debut){
			tmp = pt_list;
		}else if(pt_list->next != NULL && (pt_list->next->pb->fin + 1) == pb->debut){
			tmp = pt_list;
		}

		pt_list = pt_list->next;
	}

	// Le prochain élément qui est sensé être celui que l'on souhaite, ne satisfait pas les conditions
	// de proximité des points (cas où on aurait parcouru toute la liste sans rien trouver).
	if((pb->fin + 1) != tmp->next->pb->debut || (tmp->next->pb->fin + 1) != pb->debut)
 		return NULL;

 	res = tmp->next->pb;

 	if(tmp->next != NULL) /* Mise à jour des problèmes dans la liste */
 		tmp->next = tmp->next->next;

 	return res;
}

void fusion(pb_t* pb1, pb_t* pb2)
{
	if (pb1->debut > pb2->debut)
	{
		pb_t* temp = pb1;
		pb1 = pb2;
		pb2 = temp;
	}

	pb1->fin     = pb2->fin;	
	pb1->taille2 = pb2->taille1;
	pb1->data2   = malloc(sizeof(int) * pb1->taille2);
	
	memcpy(pb1->data2 , pb2->data1, sizeof(int) * pb1->taille2);
}

/*
 * upper <nb points>
 * exemple :
 * % upper 200 
 * % evince upper_hull.pdf
 */
int main(int argc, char **argv){	
	liste_pb * pbs = NULL;
	int fils[NB_CHILD];
	point * pts;
	pb_t* pb, *pb2;
	int nbPts, i, sender = 0;
	 

	if (argc != 2) {
		fprintf(stderr, "usage: %s <nb points>\n", *argv);
		exit(-1);
	}
	pvm_catchout(stdout);
	nbPts = atoi(argv[1]);

	pts = point_random(nbPts);
	point_print_gnuplot(pts, 0); /* affiche l'ensemble des points */

	list_init(&pbs, pts);

	list_print(pbs);

	pvm_spawn(EPATH "/slave", (char**)0, 0, "", NB_CHILD, fils);
	
	for(i=0;i < NB_CHILD && pbs->pb != NULL; i++){
		send_pb(fils[i], pbs->pb);

		if (pbs->next != NULL)
			pbs = pbs->next;
		else{
			pbs->pb = NULL;
		}
	}

	while (1) {

		pb = receive_pb(-1, &sender);

		if (pb->debut == 1 && pb->fin == nbPts)  break;

		if (pbs->pb == NULL){
			pbs->pb = pb;
		}
		else{

			if(pbs->pb->type == PB_HULL){
				pb2 = pbs->pb;

				if(pbs->next != NULL)
					pbs = pbs->next;
				else
					pbs->pb =  NULL;

				send_pb(sender, pb2);
				range_pb(pbs, pb); /* Range le problème dans la liste */
			}

			if(pbs->pb->type == PB_MERGE){

				pb2 = trouve_prox(pbs, pb); /* Cherche l'enveloppe avec laquelle fusionner */

				if(pb2 == NULL){
					range_pb(pbs, pb); /* Range le problème dans la liste */
				}else{
					fusion(pb, pb2);
					send_pb(sender, pb);
					pb_free(pb2);
				}
			}
		}

	}

	pts = pb_to_points(pb, 1);

	pvm_initsend(PvmDataDefault);
	pvm_mcast(fils, NB_CHILD, MSG_END); /* fin esclaves */
	point_print_gnuplot(pts, 1); /* affiche l'ensemble des points restant, i.e
					l'enveloppe, en reliant les points */
	point_free(pts);
	list_free(pbs);
}