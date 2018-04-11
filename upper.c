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
	point *pts,*temp;
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

	for(i=1;i < taille/4 ; i++)
	{
		temp = point_cut(pts,4);
		pb = creation_pb(pts,i*4,i*4+3,PB_HULL);
		pbs = ajout_pb(pbs,pb);
		pts = temp;
	}
	if (pts != NULL)
	{
		pb = creation_pb(pts,(taille/4)*4,taille,PB_HULL);
		pbs = ajout_pb(pbs,pb);
	}
	// creation et lancement des enfants

	point_print_gnuplot(pts, 1); /* affiche l'ensemble des points restant, i.e
					l'enveloppe, en reliant les points */
	point_free(pts); 
}

