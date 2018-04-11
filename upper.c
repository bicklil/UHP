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

int arrondi_superieur(int nombre, int diviseur)
{
	if ( nombre%4 == 0)
		return nombre/4;
	else
		return nombre/4 +1;
}

main(int argc, char **argv)
{
	point *pts;
	pb_t* pb;
	int taille;
	int i;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <nb points>\n", *argv);
		exit(-1);
	}
	taille = atoi(argv[1]);
	pts = point_random(taille);
	point_print_gnuplot(pts, 0); /* affiche l'ensemble des points */

	liste_pb* pbs;
	pbs = liste_pb_alloc();

	ajout_pb(pbs,pb);

	for(i=1;i < arrondi_superieur(taille,4) ; i++)
	{

	}

	point_print_gnuplot(pts, 1); /* affiche l'ensemble des points restant, i.e
					l'enveloppe, en reliant les points */
	point_free(pts); 
}

