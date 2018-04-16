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

#include "stdio.h"
#include "stdlib.h"
#include "point.h"
#include "pb.h"


liste_pb *  elt_alloc(int nbPts){
	liste_pb * elt;

	elt = malloc(sizeof(liste_pb));
	elt->pb = pb_alloc();
	elt->pb->taille1 = nbPts * 2;
	elt->pb->data1 = malloc(sizeof(int) * elt->pb->taille1);

	return elt;
}

void elt_set(liste_pb * elt, point * ptr_point, int nbPts){
	int i;

	for(i = 0; i < nbPts * 2 ; i+=2){
		elt->pb->data1[i] = ptr_point->x; 
		elt->pb->data1[i+1] = ptr_point->y;

		if((ptr_point)->next != NULL)
			* ptr_point = * ptr_point->next; 
 	}
}

void list_init(liste_pb * l, point * pts){
	point * ptr_point = pts;
	liste_pb * elt, * cur = l;
	int start = 1, end = 1, nbPts = point_nb(pts);
	
	while(nbPts >= 4 ){
		end = start + 3;

		elt = elt_alloc(4);
		elt_set(elt, ptr_point, 4);
		
		elt->pb->debut = start;
		elt->pb->fin = end;

		nbPts -= 4;
			
		cur->next = elt;
		* cur = * elt;
		
		start += 4;
	}

	if(nbPts > 0){
		end = start + nbPts - 1;

		elt = elt_alloc(nbPts);
		elt_set(elt, ptr_point, nbPts);

		elt->pb->debut = start;
		elt->pb->fin = end;

		* cur = * elt;
	}
}

void list_print(liste_pb * l){
	liste_pb * cur = l;
	int i;

	while(cur != NULL){

		for(i = 0; i < cur->pb->taille1; i += 2){
			printf("d: %i f: %i x: %i y: %i\n", cur->pb->debut, cur->pb->fin, cur->pb->data1[i], cur->pb->data1[i+1]);
		}

		for(i = 0; i < cur->pb->taille2; i += 2){
			printf("d: %i f: %i x: %i y: %i\n", cur->pb->debut, cur->pb->fin, cur->pb->data2[i], cur->pb->data2[i+1]);
		}

		cur = cur->next;
	}
}

void list_free(liste_pb * l){

}

/*
 * upper <nb points>
 * exemple :
 * % upper 200 
 * % evince upper_hull.pdf
 */
int main(int argc, char **argv)
{	
	point *pts;
	int nbPts; 

	if (argc != 2) {
		fprintf(stderr, "usage: %s <nb points>\n", *argv);
		exit(-1);
	}
	
	nbPts = atoi(argv[1]);

	pts = point_random(nbPts);
	point_print_gnuplot(pts, 0); /* affiche l'ensemble des points */

	// Allocation mémoire d'une liste chaînée de problèmes
	liste_pb * pbs;
	pbs = malloc(sizeof(liste_pb));

	list_init(pbs, pts);

	list_print(pbs);

	point_print_gnuplot(pts, 1); /* affiche l'ensemble des points restant, i.e
					l'enveloppe, en reliant les points */
	point_free(pts);
	list_free(pbs);
}