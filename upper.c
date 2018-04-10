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


/*
 * upper <nb points>
 * exemple :
 * % upper 200 
 * % evince upper_hull.pdf
 */

main(int argc, char **argv)
{
	point *pts;
	int taille,i;
	if (argc != 2) {
		fprintf(stderr, "usage: %s <nb points>\n", *argv);
		exit(-1);
	}
	taille = atoi(argv[1]);
	pts = point_random(taille);
	point_print_gnuplot(pts, 0); /* affiche l'ensemble des points */

	liste_pb* Pbs;
	Pbs = liste_pb_alloc();
	for(i=0;i<taille/4 + ( taille%4 : 0?1); i++){ // revoir le ternaire
		
	}
	

	point_print_gnuplot(pts, 1); /* affiche l'ensemble des points restant, i.e
					l'enveloppe, en reliant les points */
	point_free(pts); 
}

