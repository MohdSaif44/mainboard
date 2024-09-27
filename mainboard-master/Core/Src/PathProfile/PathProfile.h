/*
 * PathProfile.h
 *
 *  Created on: Sep 27, 2023
 *      Author: yvc
 */

#ifndef SRC_PATHPROFILE_PATHPROFILE_H_
#define SRC_PATHPROFILE_PATHPROFILE_H_

#include "../adapter.h"

typedef struct {
    uint8_t start;
    uint8_t point_number;
    uint8_t number_of_point;
    float points[][4];

} Path_Profile_Plan_t;

typedef struct {
    union {
        struct {
            uint8_t state : 4;
            int8_t dir : 2;
            uint8_t error : 1;
        };
        uint8_t status;
    };
    float d, d1, d2, d3, td, v, t, x, path, lastd;
    uint32_t counter;
    Path_Profile_Plan_t plan;

} Path_Profile_t;

extern int number_of_profile;
Path_Profile_t **P_to_Profile;

Path_Profile_t proftemp, prof1, prof2, prof3, prof4,l_belt_prof,r_belt_prof, profbottomshoot;
Path_Profile_t profx, profy, profz;

void Path_Profile_Init(Path_Profile_t *prof);
void Path_Profile_Inc();
void Path_Profile_Set(Path_Profile_t *prof, float offset, float d1, float td, float d3, float v);
void Path_Profile_Absolute_Set(Path_Profile_t *prof, float d1, float abs_d, float d3, float v);
void Path_Profile_Reset(Path_Profile_t *prof);
void Path_Profile_Update(Path_Profile_t *prof);
void Path_Profile_Planner(uint8_t num_of_axis, ...);

#endif /* SRC_PATHPROFILE_PATHPROFILE_H_ */
