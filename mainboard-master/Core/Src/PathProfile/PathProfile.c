/*
 * PathProfile.h
 *
 *  Created on: Sep 27, 2023
 *      Author: yvc
 *
 *  https://www.desmos.com/calculator/xirbeskaaz
 */

#include "PathProfile.h"

int number_of_profile = 0;

/*
 * Function Name		: Path_Profile_Init
 * Function Description : Called to init Path_Profile.
 * Function Remarks		: None
 * Function Arguments	: *prof			,	pointer to structure Path_Profile_t
 * Function Return		: None
 * Function Example		: Path_Profile_Init(&prof1);
 */
void Path_Profile_Init(Path_Profile_t *prof){
    if (number_of_profile == 0){
        P_to_Profile = (Path_Profile_t **)malloc(sizeof(Path_Profile_t *));
        *P_to_Profile = (Path_Profile_t *)malloc(sizeof(Path_Profile_t));
    }else{
        P_to_Profile = (Path_Profile_t **)realloc(P_to_Profile, sizeof(Path_Profile_t *) * (number_of_profile + 1));
        *(P_to_Profile) = (Path_Profile_t *)realloc(*(P_to_Profile), sizeof(Path_Profile_t) * (number_of_profile + 1));
    }
    P_to_Profile[number_of_profile] = prof;

    prof->state = 0;
    prof->error = 0;
    prof->lastd = 0;
    prof->counter = 0;

    number_of_profile++;
}

/*
 * Function Name		: Path_Profile_Inc
 * Function Description : Called to increment counter of all Path_Profile.
 * Function Remarks		: Call this function in 1ms timer interrupt.
 * Function Arguments	: None
 * Function Return		: None
 * Function Example		: Path_Profile_Inc();
 */
void Path_Profile_Inc(){
    for (uint8_t i = 0; i < number_of_profile; i++){
        if (P_to_Profile[i]->state != 0 && P_to_Profile[i]->error == 0){
			P_to_Profile[i]->counter++;
        }
    }
}

/*
 * Function Name		: Path_Profile_Set
 * Function Description : Called to set Path_Profile.
 * Function Remarks		: Sign of td indicate direction of path.
 *                        td must be greater than d1 + d3.
 * Function Arguments	: *prof			,	pointer to structure Path_Profile_t
 * 						  offset		,	offset of path
 * 						  d1			,	distance of accel
 * 						  td			,	total distance
 * 						  d3			,	distance of decel
 * 						  v				,	velocity
 * Function Return		: None
 * Function Example		: Path_Profile_Set(&prof1, 0, 1, 4, 2, 5);
 */
void Path_Profile_Set(Path_Profile_t *prof, float offset, float d1, float td, float d3, float v){
    prof->lastd = offset;
    prof->path = offset;
    prof->td = td;
    prof->dir = (prof->td >= 0) ? 1 : -1;
    prof->d1 = fabsf(d1) * prof->dir;
    prof->d3 = fabsf(d3) * prof->dir;
    prof->v = fabsf(v) * prof->dir;
    prof->d2 = prof->td - prof->d1 - prof->d3;
    if (fabsf(prof->d1) + fabs(prof->d3) > fabs(td)){
        prof->error = 1;
        return;
    }
    prof->state = 1;
    prof->counter = 0;
}

void Path_Profile_Absolute_Set(Path_Profile_t *prof, float d1, float abs_d, float d3, float v){
	Path_Profile_Set(prof, prof->lastd, d1, abs_d - prof->lastd, d3, v);
}

/*
 * Function Name		: Path_Profile_Reset
 * Function Description : Called to reset Path_Profile.
 * Function Remarks		: None
 * Function Arguments	: *prof			,	pointer to structure Path_Profile_t
 * Function Return		: None
 * Function Example		: Path_Profile_Reset(&prof1);
 */
void Path_Profile_Reset(Path_Profile_t *prof){
    prof->state = 0;
    prof->error = 0;
    prof->lastd = 0;
    prof->counter = 0;
}

/*
 * Function Name		: Path_Profile_Update
 * Function Description : Called to update Path_Profile.
 * Function Remarks		: Can be called in any time frame as it track time by independent counter.
 * Function Arguments	: *prof			,	pointer to structure Path_Profile_t
 * Function Return		: None
 * Function Example		: Path_Profile_Update(&prof1);
 */
void Path_Profile_Update(Path_Profile_t *prof){
    switch (prof->state)
    {
    case 0:
        break;
    case 1: // accel
        prof->d = prof->d1;
        prof->t = 2 * prof->d / prof->v;
        prof->x = prof->counter/1000.0 - prof->t * 1.2;
        prof->path = prof->lastd + 2 * prof->d / (1 + expf(-1.47157 / prof->t * prof->x * M_E));
        if (prof->x >= 0){
            prof->lastd += prof->d;
            prof->state = 2;
            prof->counter = 0;
        }
        break;
    case 2: // const
        prof->d = prof->d2;
        prof->t = prof->d / prof->v;
        prof->x = prof->counter/1000.0;
        prof->path = prof->lastd + prof->x * prof->v;
        if (prof->x >= prof->t){
            prof->lastd += prof->d;
            prof->state = 3;
            prof->counter = 0;
        }
        break;
    case 3: // decel
        prof->d = prof->d3;
        prof->t = 2 * prof->d / prof->v;
        prof->x = prof->counter/1000.0;
        prof->path = prof->lastd - prof->d + 2 * prof->d / (1 + expf(-1.47157 / prof->t * prof->x * M_E));
        if (prof->x >= prof->t * 1.5){
            prof->lastd += prof->d;
            prof->path = prof->lastd;
            prof->state = 4;
            prof->counter = 0;
        }
        break;
    case 4:
    	prof->state = 0;
    	break;
    }
}

/*
 * Function Name		: Path_Profile_Planner
 * Function Description : Called to plan path profile with array of path point.
 * Function Remarks		: All profile number of point should be the same
 * Function Arguments	: num_of_axis			,	number of axis
 * 						  args					,	pointer to each axis profile struct
 * Function Return		: None
 * Function Example		: Path_Profile_Planner(2, &prof1, &prof2){
 */
void Path_Profile_Planner(uint8_t num_of_axis, ...){

	Path_Profile_t *P_profiles[num_of_axis];

	uint8_t all_state;
	uint8_t max_num_of_points = 0;

	va_list profiles;
	va_start(profiles, num_of_axis);

	for (uint8_t a = 0; a < num_of_axis; a++){
		P_profiles[a] = va_arg(profiles, Path_Profile_t*);
		if (P_profiles[a]->plan.number_of_point > max_num_of_points)
			max_num_of_points = P_profiles[a]->plan.number_of_point;
	}

	va_end(profiles);

	for (uint8_t i = 0; i < max_num_of_points; i++){
		all_state = 1;
		for (uint8_t a = 0; a < num_of_axis; a++){
			Path_Profile_Set(P_profiles[a], P_profiles[a]->lastd, P_profiles[a]->plan.points[i][0], P_profiles[a]->plan.points[i][1], P_profiles[a]->plan.points[i][2], P_profiles[a]->plan.points[i][3]);
    	}

		while (all_state != 0){
			all_state = 0;
			for (uint8_t a = 0; a < num_of_axis; a++){
				all_state += P_profiles[a]->state;
			}
		}
    }
}


