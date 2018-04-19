

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "pb.h"
#include "point.h"
#include "pvm3.h"

void calcul_env(pb_t* pb)
{
    point * upper = point_UH(pb_to_points(pb, 1));
    pb = points_to_pb(upper, pb);

    pb->type = PB_MERGE;
}

void merge_data(pb_t *pb)
{
    point * pt1, * pt2;

    pt1 = pb_to_points(pb, 1);
    pt2 = pb_to_points(pb, 2);

    pt1 = point_UH(pt1, pt2);

    pb = points_to_pb(pt1, pb);
}

/*
 * programme esclave
 */

int main()
{
	int parent = pvm_parent();
	int temp;
	pb_t* pb ;
	
    while((pb = receive_pb(parent,&temp)) != NULL){
		if (pb->type == PB_MERGE){
			merge_data(pb);
		}else {
			if (pb->type == PB_HULL)
				calcul_env(pb);
		}

		send_pb(parent,pb);
	}
    
    pb = receive_pb(parent, &temp);
    send_pb(parent, pb); 

	pvm_exit();
    return 0;
}

