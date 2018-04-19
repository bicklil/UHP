

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "pb.h"
#include "point.h"

/*
 * trie un tableau de taille N
 * compare_int pour qsort
 */

void calcul_env(pb_t* pb)
{
    point * pt1,*temp;
    pt1 = point_alloc();
    temp = pt1;
    int i;
    for (i=0;i<pb->taille1-2;i=i+2)
    {
        temp->x = pb->data1[i];
        temp->y = pb->data1[i+1];
        temp->next = point_alloc();
        temp = temp->next;
        fflush(stdout);
    }
    temp->x = pb->data1[pb->taille1-2];
    temp->y = pb->data1[pb->taille1-1];
    point_print(pt1);
    fflush(stdout);
    pt1 = point_UH(pt1);
    printf("taille pt %d\n",point_nb(pt1)); // point_nb marche pas
    pb->taille1 = (point_nb(pt1)*2);
    free(pb->data1);

	pb->data1 = malloc(sizeof(int)*pb->taille1);
    temp = pt1;
    for (i=0;i<pb->taille1;i = i+2)
    {
        pb->data1[i] = pt1->x;
        pb->data1[i-1] = pt1->y;
        pt1 = pt1->next;
    }

    pb->type = PB_MERGE;

}

void merge_data(pb_t *pb)
{
    int i;
    point * pt1, *pt2;
    point* temp;
    pt1 = point_alloc();
    pt2 = point_alloc();
    temp = pt1;
    for (i=0;i<pb->taille1-2;i++)
    {
        temp->x = pb->data1[2*i];
        temp->y = pb->data1[2*i+1];
        temp->next = point_alloc();
        temp = temp->next;
    }
    temp->x = pb->data1[pb->taille1-2];
    temp->y = pb->data1[pb->taille1-1];
    temp = pt2;
    for (i=0;i<pb->taille2-2;i++)
    {
        temp->x = pb->data2[2*i];
        temp->y = pb->data2[2*i+1];
        temp->next = point_alloc();
        temp = temp->next;
    }
    temp->x = pb->data2[pb->taille1-2];
    temp->y = pb->data2[pb->taille1-1];

	pt1 = point_merge_UH(pt1, pt2);

    pb->taille1 = (point_nb(pt1)*2);
    free(pb->data1);

	pb->data1 = malloc(sizeof(int)*pb->taille1);
    temp = pt1;
    for (i=0;i<pb->taille1;i = i+2)
    {
        pb->data1[i] = pt1->x;
        pb->data1[i-1] = pt1->y;
        pt1 = pt1->next;
    }
	free(pb->data2);
	pb->data2 = NULL;
	pb->taille2 = 0;
}

/*
 * programme esclave
 */

int main()
{
	int parent = pvm_parent();
	int temp;
	pb_t* pb = receive_pb(parent,&temp);
	while( pb != NULL){
		if (pb->type == PB_MERGE){
			merge_data(pb);
		}
		else {
			if (pb->type == PB_HULL)

				calcul_env(pb);
                fflush(stdout);
		}
		send_pb(parent,pb);
		pb = receive_pb(parent,&temp);
    return 0;
	}




	pvm_exit();
	exit(0);
}

