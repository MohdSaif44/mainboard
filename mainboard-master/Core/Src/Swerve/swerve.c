/*
 * swerve.c
 *
 *  Created on:
 *      Author:
 */

#include "swerve.h"
#include <math.h>

void SwerveInit(int swerve_type, int turn_mode, float maximum_turn, float width, float length, float swerve_gear_ratio)
{
	swerve.turnmode = turn_mode;
	swerve.maxturn = maximum_turn;

	if (swerve_type == FWD_SWERVE)
	{
		swerve.type = FWD_SWERVE;

		MODNRobotBaseVelInit(MODN_FWD_SWERVE, width, length, &modn);
		MODNRobotConInit(&x_vel, &y_vel, &w_vel, &modn);
		MODNWheelVelInit(&v1, &v2, &v3, &v4, &modn);
		MODNWheelDirInit(&d1, &d2, &d3, &d4, &modn);

		VESCInit(VESC1, VESC2, VESC3, VESC4, &vesc); //FL,FR,BL,BR

		RBMS_Init(&rbms1, &hcan2, RBMS_1234);
		RBMS_Config(&rbms1, RBMS1, C610, swerve_gear_ratio);//FL
		RBMS_Config(&rbms1, RBMS2, C610, swerve_gear_ratio);//FR
		RBMS_Config(&rbms1, RBMS3, C610, swerve_gear_ratio);//BL
		RBMS_Config(&rbms1, RBMS4, C610, swerve_gear_ratio);//BR
		RBMS_PID_Init(&rbms1);
		RBMS_Set_Control_Mode(&rbms1, RBMS1, POSITION);
		RBMS_Set_Control_Mode(&rbms1, RBMS2, POSITION);
		RBMS_Set_Control_Mode(&rbms1, RBMS3, POSITION);
		RBMS_Set_Control_Mode(&rbms1, RBMS4, POSITION);
		rbms1.motor[RBMS1].config.vel_limit = 300;
		rbms1.motor[RBMS2].config.vel_limit = 300;
		rbms1.motor[RBMS3].config.vel_limit = 300;
		rbms1.motor[RBMS4].config.vel_limit = 300;
		rbms1.motor[RBMS1].config.POS_P = 200;
		rbms1.motor[RBMS2].config.POS_P = 200;
		rbms1.motor[RBMS3].config.POS_P = 200;
		rbms1.motor[RBMS4].config.POS_P = 200;
	}
	else if (swerve_type == TRI_SWERVE)
	{
		swerve.type = TRI_SWERVE;

		MODNRobotBaseVelInit(MODN_TRI_SWERVE, width, length, &modn);
		MODNRobotConInit(&x_vel, &y_vel, &w_vel, &modn);
		MODNWheelVelInit(&v1, &v2, &v3, &v4, &modn);
		MODNWheelDirInit(&d1, &d2, &d3, &d4, &modn);

		VESCInit(VESC1, VESC2, VESC3, 0, &vesc); //FL,FR,B

		RBMS_Init(&rbms1, &hcan2, RBMS_1234);
		RBMS_Config(&rbms1, RBMS1, C610, swerve_gear_ratio);//FL
		RBMS_Config(&rbms1, RBMS2, C610, swerve_gear_ratio);//FR
		RBMS_Config(&rbms1, RBMS3, C610, swerve_gear_ratio);//B
		RBMS_PID_Init(&rbms1);
		RBMS_Set_Control_Mode(&rbms1, RBMS1, POSITION);
		RBMS_Set_Control_Mode(&rbms1, RBMS2, POSITION);
		RBMS_Set_Control_Mode(&rbms1, RBMS3, POSITION);
		rbms1.motor[RBMS1].config.vel_limit = 300;
		rbms1.motor[RBMS2].config.vel_limit = 300;
		rbms1.motor[RBMS3].config.vel_limit = 300;
		rbms1.motor[RBMS1].config.POS_P = 200;
		rbms1.motor[RBMS2].config.POS_P = 200;
		rbms1.motor[RBMS3].config.POS_P = 200;
	}
//	swerve.imu = 1;
//	if (swerve.imu)
//	{
	PIDSourceInit(&pid_angle_error, &pid_angle_output, &imu_rotate);
	PIDGainInit(0.005, 1.0, 1.0/180.0, 2.0, 15.0, 0.0, 0.5, 200.0, &imu_rotate);
	PIDDelayInit(&imu_rotate);
//	swerve.imu = 0;
//	}
}

void SwerveRun(float max_speed, int mode, float brake_current, int brake_time, float target_angle)
{
	RBMS_5ms(&rbms1);

	if (swerve.aligned)
	{
		if (mode == manualnolock)
		{
			PIDDelayInit(&imu_rotate);
			x_vel = ps4.joyL_x;
			y_vel = ps4.joyL_y;
			w_vel = -(ps4.joyR_2 - ps4.joyL_2)*1.8;
		}
		else if (mode == manuallock)
		{
			pid_angle_error = target_angle - IMU.real_z;
			PID(&imu_rotate);
			x_vel = ps4.joyL_x;
			y_vel = ps4.joyL_y;
			w_vel = (-1)*pid_angle_output;
		}
		else if (mode == perspective)
		{
			PIDDelayInit(&imu_rotate);
			x_vel = (ps4.joyL_x*cos(-IMU.real_zrad)-ps4.joyL_y*sin(-IMU.real_zrad));
			y_vel = (ps4.joyL_x*sin(-IMU.real_zrad)+ps4.joyL_y*cos(-IMU.real_zrad));
			w_vel = -(ps4.joyR_2 - ps4.joyL_2)*1.8;
		}
		else if (mode == pathplan)
		{

		}

		MODNUpdate(&modn);

		if (swerve.turnmode == halfturn)
		{
			if (swerve.run)
			{
				swerve.run = 0;

				if (swerve.type == FWD_SWERVE)
				{
					swerve.sign[0] = (d1 > 90.0 || d1 < -90.0) ? -1.0 : 1.0 ;
					swerve.sign[1] = (d2 > 90.0 || d2 < -90.0) ? -1.0 : 1.0 ;
					swerve.sign[2] = (d3 > 90.0 || d3 < -90.0) ? -1.0 : 1.0 ;
					swerve.sign[3] = (d4 > 90.0 || d4 < -90.0) ? -1.0 : 1.0 ;
					d1 = (d1 > 90.0) ? d1 - 180 : (d1 < -90.0) ? d1 + 180 : d1 ;
					d2 = (d2 > 90.0) ? d2 - 180 : (d2 < -90.0) ? d2 + 180 : d2 ;
					d3 = (d3 > 90.0) ? d3 - 180 : (d3 < -90.0) ? d3 + 180 : d3 ;
					d4 = (d4 > 90.0) ? d4 - 180 : (d4 < -90.0) ? d4 + 180 : d4 ;

					swerve.fvel[0] = v1 * swerve.sign[0] * max_speed * fabs(cos((swerve.finalang[0]-d1) * (M_PI / 180.0))) ;
					swerve.fvel[1] = v2 * swerve.sign[1] * max_speed * fabs(cos((swerve.finalang[1]-d2) * (M_PI / 180.0))) ;
					swerve.fvel[2] = v3 * swerve.sign[2] * max_speed * fabs(cos((swerve.finalang[2]-d3) * (M_PI / 180.0))) ;
					swerve.fvel[3] = v4 * swerve.sign[3] * max_speed * fabs(cos((swerve.finalang[3]-d4) * (M_PI / 180.0))) ;

					swerve.finalang[0] = d1;
					swerve.finalang[1] = d2;
					swerve.finalang[2] = d3;
					swerve.finalang[3] = d4;

					VESCPDC(swerve.fvel[0], swerve.fvel[1], swerve.fvel[2], swerve.fvel[3], &vesc);

					RBMS_Set_Target_Position(&rbms1, RBMS1, swerve.finalang[0] / 360.0);
					RBMS_Set_Target_Position(&rbms1, RBMS2, swerve.finalang[1] / 360.0);
					RBMS_Set_Target_Position(&rbms1, RBMS3, swerve.finalang[2] / 360.0);
					RBMS_Set_Target_Position(&rbms1, RBMS4, swerve.finalang[3] / 360.0);
				}
				else if (swerve.type == TRI_SWERVE)
				{
					swerve.sign[0] = (d1 > 90.0 || d1 < -90.0) ? -1.0 : 1.0 ;
					swerve.sign[1] = (d2 > 90.0 || d2 < -90.0) ? -1.0 : 1.0 ;
					swerve.sign[2] = (d3 > 90.0 || d3 < -90.0) ? -1.0 : 1.0 ;
					d1 = (d1 > 90.0) ? d1 - 180 : (d1 < -90.0) ? d1 + 180 : d1 ;
					d2 = (d2 > 90.0) ? d2 - 180 : (d2 < -90.0) ? d2 + 180 : d2 ;
					d3 = (d3 > 90.0) ? d3 - 180 : (d3 < -90.0) ? d3 + 180 : d3 ;

					swerve.fvel[0] = v1 * swerve.sign[0] * max_speed * fabs(cos((swerve.finalang[0]-d1) * (M_PI / 180.0))) ;
					swerve.fvel[1] = v2 * swerve.sign[1] * max_speed * fabs(cos((swerve.finalang[1]-d2) * (M_PI / 180.0))) ;
					swerve.fvel[2] = v3 * swerve.sign[2] * max_speed * fabs(cos((swerve.finalang[2]-d3) * (M_PI / 180.0))) ;

					swerve.finalang[0] = d1;
					swerve.finalang[1] = d2;
					swerve.finalang[2] = d3;

					VESCPDC(swerve.fvel[0], swerve.fvel[1], swerve.fvel[2], 0, &vesc);

					RBMS_Set_Target_Position(&rbms1, RBMS1, swerve.finalang[0] / 360.0);
					RBMS_Set_Target_Position(&rbms1, RBMS2, swerve.finalang[1] / 360.0);
					RBMS_Set_Target_Position(&rbms1, RBMS3, swerve.finalang[2] / 360.0);
				}
				swerve.braketimer = 0;
			}
			else
			{
				if (swerve.type == FWD_SWERVE)
				{
					swerve.fvel[0] = 0.0;
					swerve.fvel[1] = 0.0;
					swerve.fvel[2] = 0.0;
					swerve.fvel[3] = 0.0;

					if (swerve.braketimer < brake_time)
					{
						swerve.braketimer++;

						VESCCurrBrake(brake_current, &vesc);

						RBMS_Set_Target_Position(&rbms1, RBMS1, swerve.finalang[0] / 360.0);
						RBMS_Set_Target_Position(&rbms1, RBMS2, swerve.finalang[1] / 360.0);
						RBMS_Set_Target_Position(&rbms1, RBMS3, swerve.finalang[2] / 360.0);
						RBMS_Set_Target_Position(&rbms1, RBMS4, swerve.finalang[3] / 360.0);
					}
					else
					{
						RBMS_Set_Target_Position(&rbms1, RBMS1, 0);
						RBMS_Set_Target_Position(&rbms1, RBMS2, 0);
						RBMS_Set_Target_Position(&rbms1, RBMS3, 0);
						RBMS_Set_Target_Position(&rbms1, RBMS4, 0);

						for(int i = 0; i < 4; i++)
						{
							swerve.sign[i] = 0.0;
							swerve.finalang[i] = 0.0;
						}
					}
				}
				else if (swerve.type == TRI_SWERVE)
				{
					swerve.fvel[0] = 0.0;
					swerve.fvel[1] = 0.0;
					swerve.fvel[2] = 0.0;

					if (swerve.braketimer < brake_time)
					{
						swerve.braketimer++;

						VESCCurrBrake(brake_current, &vesc);

						RBMS_Set_Target_Position(&rbms1, RBMS1, swerve.finalang[0] / 360.0);
						RBMS_Set_Target_Position(&rbms1, RBMS2, swerve.finalang[1] / 360.0);
						RBMS_Set_Target_Position(&rbms1, RBMS3, swerve.finalang[2] / 360.0);
					}
					else
					{
						RBMS_Set_Target_Position(&rbms1, RBMS1, 0);
						RBMS_Set_Target_Position(&rbms1, RBMS2, 0);
						RBMS_Set_Target_Position(&rbms1, RBMS3, 0);

						for(int i = 0; i < 4; i++)
						{
							swerve.sign[i] = 0.0;
							swerve.finalang[i] = 0.0;
						}
					}
				}
			}
		}
		else if (swerve.turnmode == unlimitedturn)
		{
			if (swerve.run)
			{
				swerve.run = 0;

				if (swerve.type == FWD_SWERVE)
				{
					swerve.ang[0] = d1;
					swerve.ang[1] = d2;
					swerve.ang[2] = d3;
					swerve.ang[3] = d4;

					for(int i = 0; i < 4; i++)
					{
						if(swerve.ang[i] - swerve.angprev[i] < -180.0)
						{
							swerve.addvalue[i] += 360.0;
						}
						else if(swerve.ang[i] - swerve.angprev[i] > 180.0)
						{
							swerve.addvalue[i] -= 360.0;
						}

						swerve.fang[i] = swerve.ang[i] + swerve.addvalue[i];

						swerve.ang1[i] = swerve.fang[i] - swerve.fangprev[i];
						swerve.ang2[i] = (swerve.ang1[i] >= 0)? swerve.ang1[i] - 180.0 : swerve.ang1[i] + 180.0;
						swerve.angprev[i] = swerve.ang[i];
						swerve.fangprev[i] = swerve.fang[i];

						if(swerve.state[i] == 0)
						{
							swerve.finalang[i] = swerve.finalangprev[i] + swerve.ang1[i];
							swerve.sign[i] = 1;
							if(fabs(swerve.ang1[i]) > fabs(swerve.ang2[i]))
							{
								swerve.state[i] = 1;
								swerve.finalang[i] = swerve.finalangprev[i] + swerve.ang2[i];
								swerve.sign[i] = -1;
							}
						}
						else if(swerve.state[i] == 1)
						{
							swerve.finalang[i] = swerve.finalangprev[i] + swerve.ang1[i];
							swerve.sign[i] = -1;
							if(fabs(swerve.ang1[i]) > fabs(swerve.ang2[i]))
							{
								swerve.state[i] = 0;
								swerve.finalang[i] = swerve.finalangprev[i] + swerve.ang2[i];
								swerve.sign[i] = 1;
							}
						}

						swerve.finalangdif[i] = swerve.finalang[i]-swerve.finalangprev[i];
						swerve.finalangprev[i] = swerve.finalang[i];
					}

					swerve.fvel[0] = v1 * swerve.sign[0] * max_speed * fabs(cos((swerve.finalangdif[0]) * (M_PI / 180.0))) ;
					swerve.fvel[1] = v2 * swerve.sign[1] * max_speed * fabs(cos((swerve.finalangdif[1]) * (M_PI / 180.0))) ;
					swerve.fvel[2] = v3 * swerve.sign[2] * max_speed * fabs(cos((swerve.finalangdif[2]) * (M_PI / 180.0))) ;
					swerve.fvel[3] = v4 * swerve.sign[3] * max_speed * fabs(cos((swerve.finalangdif[3]) * (M_PI / 180.0))) ;

					VESCPDC(swerve.fvel[0], swerve.fvel[1], swerve.fvel[2], swerve.fvel[3], &vesc);

					RBMS_Set_Target_Position(&rbms1, RBMS1, swerve.finalang[0] / 360.0 + swerve.closestzero[0]);
					RBMS_Set_Target_Position(&rbms1, RBMS2, swerve.finalang[1] / 360.0 + swerve.closestzero[1]);
					RBMS_Set_Target_Position(&rbms1, RBMS3, swerve.finalang[2] / 360.0 + swerve.closestzero[2]);
					RBMS_Set_Target_Position(&rbms1, RBMS4, swerve.finalang[3] / 360.0 + swerve.closestzero[3]);

				}
				else if (swerve.type == TRI_SWERVE)
				{
					swerve.ang[0] = d1;
					swerve.ang[1] = d2;
					swerve.ang[2] = d3;

					for(int i = 0; i < 3; i++)
					{
						if(swerve.ang[i] - swerve.angprev[i] < -180.0)
						{
							swerve.addvalue[i] += 360.0;
						}
						else if(swerve.ang[i] - swerve.angprev[i] > 180.0)
						{
							swerve.addvalue[i] -= 360.0;
						}

						swerve.fang[i] = swerve.ang[i] + swerve.addvalue[i];

						swerve.ang1[i] = swerve.fang[i] - swerve.fangprev[i];
						swerve.ang2[i] = (swerve.ang1[i] >= 0)? swerve.ang1[i] - 180.0 : swerve.ang1[i] + 180.0;
						swerve.angprev[i] = swerve.ang[i];
						swerve.fangprev[i] = swerve.fang[i];

						if(swerve.state[i] == 0)
						{
							swerve.finalang[i] = swerve.finalangprev[i] + swerve.ang1[i];
							swerve.sign[i] = 1;
							if(fabs(swerve.ang1[i]) > fabs(swerve.ang2[i]))
							{
								swerve.state[i] = 1;
								swerve.finalang[i] = swerve.finalangprev[i] + swerve.ang2[i];
								swerve.sign[i] = -1;
							}
						}
						else if(swerve.state[i] == 1)
						{
							swerve.finalang[i] = swerve.finalangprev[i] + swerve.ang1[i];
							swerve.sign[i] = -1;
							if(fabs(swerve.ang1[i]) > fabs(swerve.ang2[i]))
							{
								swerve.state[i] = 0;
								swerve.finalang[i] = swerve.finalangprev[i] + swerve.ang2[i];
								swerve.sign[i] = 1;
							}
						}

						swerve.finalangdif[i] = swerve.finalang[i]-swerve.finalangprev[i];
						swerve.finalangprev[i] = swerve.finalang[i];
					}

					swerve.fvel[0] = v1 * swerve.sign[0] * max_speed * fabs(cos((swerve.finalangdif[0]) * (M_PI / 180.0))) ;
					swerve.fvel[1] = v2 * swerve.sign[1] * max_speed * fabs(cos((swerve.finalangdif[1]) * (M_PI / 180.0))) ;
					swerve.fvel[2] = v3 * swerve.sign[2] * max_speed * fabs(cos((swerve.finalangdif[2]) * (M_PI / 180.0))) ;

					VESCPDC(swerve.fvel[0], swerve.fvel[1], swerve.fvel[2], 0, &vesc);

					RBMS_Set_Target_Position(&rbms1, RBMS1, swerve.finalang[0] / 360.0 + swerve.closestzero[0]);
					RBMS_Set_Target_Position(&rbms1, RBMS2, swerve.finalang[1] / 360.0 + swerve.closestzero[1]);
					RBMS_Set_Target_Position(&rbms1, RBMS3, swerve.finalang[2] / 360.0 + swerve.closestzero[2]);

					//				swerve.valuecheck[0] = swerve.finalang[0] / 360.0 + swerve.closestzero[0];
					//				swerve.valuecheck[1] = swerve.finalang[1] / 360.0 + swerve.closestzero[1];
					//				swerve.valuecheck[2] = swerve.finalang[2] / 360.0 + swerve.closestzero[2];
					//				swerve.valuecheck[3] = swerve.finalang[3] / 360.0 + swerve.closestzero[3];
				}
				swerve.getclosestzero = 1;
				swerve.braketimer = 0;
			}
			else
			{
				if (swerve.type == FWD_SWERVE)
				{
					swerve.fvel[0] = 0.0;
					swerve.fvel[1] = 0.0;
					swerve.fvel[2] = 0.0;
					swerve.fvel[3] = 0.0;

					if (swerve.braketimer < brake_time)
					{
						swerve.braketimer++;

						VESCCurrBrake(brake_current, &vesc);

						RBMS_Set_Target_Position(&rbms1, RBMS1, swerve.finalang[0] / 360.0 + swerve.closestzero[0]);
						RBMS_Set_Target_Position(&rbms1, RBMS2, swerve.finalang[1] / 360.0 + swerve.closestzero[1]);
						RBMS_Set_Target_Position(&rbms1, RBMS3, swerve.finalang[2] / 360.0 + swerve.closestzero[2]);
						RBMS_Set_Target_Position(&rbms1, RBMS4, swerve.finalang[3] / 360.0 + swerve.closestzero[3]);
					}
					else
					{
						if (swerve.returnzero == 1)
						{
							RBMS_Set_Target_Position(&rbms1, RBMS1, 0);
							RBMS_Set_Target_Position(&rbms1, RBMS2, 0);
							RBMS_Set_Target_Position(&rbms1, RBMS3, 0);
							RBMS_Set_Target_Position(&rbms1, RBMS4, 0);

							for(int i = 0; i < 4; i++)
							{
								swerve.ang[i] = 0.0;
								swerve.angprev[i] = 0.0;
								swerve.addvalue[i] = 0.0;
								swerve.fang[i] = 0.0;
								swerve.fangprev[i] = 0.0;
								swerve.finalang[i] = 0.0;
								swerve.finalangprev[i] = 0.0;
								swerve.closestzero[i] = 0.0;
								swerve.state[i] = 0;
							}
						}
						else
						{
							if (swerve.getclosestzero == 1)
							{
								swerve.closestzero[0] = roundf(swerve.finalang[0] / 360.0) + swerve.closestzero[0];
								swerve.closestzero[1] = roundf(swerve.finalang[1] / 360.0) + swerve.closestzero[1];
								swerve.closestzero[2] = roundf(swerve.finalang[2] / 360.0) + swerve.closestzero[2];
								swerve.closestzero[3] = roundf(swerve.finalang[2] / 360.0) + swerve.closestzero[3];
								swerve.getclosestzero = 0;
							}
							RBMS_Set_Target_Position(&rbms1, RBMS1, swerve.closestzero[0]);
							RBMS_Set_Target_Position(&rbms1, RBMS2, swerve.closestzero[1]);
							RBMS_Set_Target_Position(&rbms1, RBMS3, swerve.closestzero[2]);
							RBMS_Set_Target_Position(&rbms1, RBMS4, swerve.closestzero[3]);

							for(int i = 0; i < 4; i++)
							{
								swerve.ang[i] = 0.0;
								swerve.angprev[i] = 0.0;
								swerve.addvalue[i] = 0.0;
								swerve.fang[i] = 0.0;
								swerve.fangprev[i] = 0.0;
								swerve.finalang[i] = 0.0;
								swerve.finalangprev[i] = 0.0;
								swerve.state[i] = 0;
							}
						}
					}
					VESCReleaseMotor(&vesc);
					VESCStop(&vesc);
				}
				else if (swerve.type == TRI_SWERVE)
				{
					swerve.fvel[0] = 0.0;
					swerve.fvel[1] = 0.0;
					swerve.fvel[2] = 0.0;

					if (swerve.braketimer < brake_time)
					{
						swerve.braketimer++;

						VESCCurrBrake(brake_current, &vesc);

						RBMS_Set_Target_Position(&rbms1, RBMS1, swerve.finalang[0] / 360.0 + swerve.closestzero[0]);
						RBMS_Set_Target_Position(&rbms1, RBMS2, swerve.finalang[1] / 360.0 + swerve.closestzero[1]);
						RBMS_Set_Target_Position(&rbms1, RBMS3, swerve.finalang[2] / 360.0 + swerve.closestzero[2]);
					}
					else
					{
						if (swerve.returnzero == 1)
						{
							RBMS_Set_Target_Position(&rbms1, RBMS1, 0);
							RBMS_Set_Target_Position(&rbms1, RBMS2, 0);
							RBMS_Set_Target_Position(&rbms1, RBMS3, 0);

							for(int i = 0; i < 4; i++)
							{
								swerve.ang[i] = 0.0;
								swerve.angprev[i] = 0.0;
								swerve.addvalue[i] = 0.0;
								swerve.fang[i] = 0.0;
								swerve.fangprev[i] = 0.0;
								swerve.finalang[i] = 0.0;
								swerve.finalangprev[i] = 0.0;
								swerve.closestzero[i] = 0.0;
								swerve.state[i] = 0;
							}
						}
						else
						{
							if (swerve.getclosestzero == 1)
							{
								swerve.closestzero[0] = roundf(swerve.finalang[0] / 360.0) + swerve.closestzero[0];
								swerve.closestzero[1] = roundf(swerve.finalang[1] / 360.0) + swerve.closestzero[1];
								swerve.closestzero[2] = roundf(swerve.finalang[2] / 360.0) + swerve.closestzero[2];
								swerve.getclosestzero = 0;
							}
							RBMS_Set_Target_Position(&rbms1, RBMS1, swerve.closestzero[0]);
							RBMS_Set_Target_Position(&rbms1, RBMS2, swerve.closestzero[1]);
							RBMS_Set_Target_Position(&rbms1, RBMS3, swerve.closestzero[2]);

							for(int i = 0; i < 4; i++)
							{
								swerve.ang[i] = 0.0;
								swerve.angprev[i] = 0.0;
								swerve.addvalue[i] = 0.0;
								swerve.fang[i] = 0.0;
								swerve.fangprev[i] = 0.0;
								swerve.finalang[i] = 0.0;
								swerve.finalangprev[i] = 0.0;
								swerve.state[i] = 0;
							}
						}
					}
					VESCReleaseMotor(&vesc);
					VESCStop(&vesc);
				}
			}
		}
	}
}


void SwerveEnq(UART_HandleTypeDef* huartx)
{
	char s[400];
	//	if (swerve.type == FWD_SWERVE)
	//	{
	//		sprintf(s, "angle= %.2f %.2f %.2f %.2f\n", d1,d2,d3,d4);
	//		HAL_UART_Transmit(huartx, (uint8_t *)s, strlen(s), HAL_MAX_DELAY);
	//		sprintf(s, "speed= %.2f %.2f %.2f %.2f\n", swerve.fvel1,swerve.fvel2,swerve.fvel3,swerve.fvel4);
	//		HAL_UART_Transmit(huartx, (uint8_t *)s, strlen(s), HAL_MAX_DELAY);
	//		sprintf(s, "added= %.2f\n", swerve.addvalue2);
	//		HAL_UART_Transmit(huartx, (uint8_t *)s, strlen(s), HAL_MAX_DELAY);
	//		sprintf(s, "real_angle= %.2f %.2f %.2f %.2f\n", swerve.finalang1,swerve.finalang2,swerve.finalang3,swerve.finalang4);
	//		HAL_UART_Transmit(huartx, (uint8_t *)s, strlen(s), HAL_MAX_DELAY);
	//	}
	//	else if (swerve.type == TRI_SWERVE)
	//	{
	//		sprintf(s, "angle= %.2f %.2f %.2f\n", d1,d2,d3);
	//		HAL_UART_Transmit(huartx, (uint8_t *)s, strlen(s), HAL_MAX_DELAY);
	//		sprintf(s, "speed= %.2f %.2f %.2f\n", swerve.fvel1,swerve.fvel2,swerve.fvel3);
	//		HAL_UART_Transmit(huartx, (uint8_t *)s, strlen(s), HAL_MAX_DELAY);
	//	}
}

void SwerveAligninit(uint16_t A_GPIO_Pin, uint16_t B_GPIO_Pin, uint16_t C_GPIO_Pin, uint16_t D_GPIO_Pin)
{
	if (swerve.type == FWD_SWERVE)
	{
		GPIOPinsInit(GPIOC, A_GPIO_Pin, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_HIGH,GPIO_NOPULL);//IP16_Analog1_PIN
		GPIOPinsInit(GPIOC, B_GPIO_Pin, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_HIGH,GPIO_NOPULL);//IP17_Analog2_PIN
		GPIOPinsInit(GPIOC, C_GPIO_Pin, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_HIGH,GPIO_NOPULL);//IP18_Analog3_PIN
		GPIOPinsInit(GPIOC, D_GPIO_Pin, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_HIGH,GPIO_NOPULL);//IP19_Analog4_PIN
		ExtixInit(A_GPIO_Pin, 9, 0, &ExtiPin); //GPIO_PIN_0
		ExtixInit(B_GPIO_Pin, 9, 1, &ExtiPin); //GPIO_PIN_1
		ExtixInit(C_GPIO_Pin, 9, 2, &ExtiPin); //GPIO_PIN_2
		ExtixInit(D_GPIO_Pin, 9, 3, &ExtiPin); //GPIO_PIN_3
	}
	else if (swerve.type == TRI_SWERVE)
	{
		GPIOPinsInit(GPIOC, A_GPIO_Pin, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_HIGH,GPIO_NOPULL);//IP16_Analog1_PIN
		GPIOPinsInit(GPIOC, B_GPIO_Pin, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_HIGH,GPIO_NOPULL);//IP17_Analog2_PIN
		GPIOPinsInit(GPIOC, C_GPIO_Pin, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_HIGH,GPIO_NOPULL);//IP18_Analog3_PIN
		ExtixInit(A_GPIO_Pin, 9, 0, &ExtiPin); //GPIO_PIN_0
		ExtixInit(B_GPIO_Pin, 9, 1, &ExtiPin); //GPIO_PIN_1
		ExtixInit(C_GPIO_Pin, 9, 2, &ExtiPin); //GPIO_PIN_2
	}
}

void SwerveAlign(float A_angle, float B_angle, float C_angle, float D_angle, GPIO_TypeDef * AhallsGPIOx, uint16_t AhallsGPIO_Pin, GPIO_TypeDef * BhallsGPIOx, uint16_t BhallsGPIO_Pin, GPIO_TypeDef * ChallsGPIOx, uint16_t ChallsGPIO_Pin, GPIO_TypeDef * DhallsGPIOx, uint16_t DhallsGPIO_Pin)
{
	swerve.aligntolerance = 5.0;//angle aligntolerance if u put 0 it will never align, why is the value jumping so much is that normal
	swerve.alignspeed = 0.006;//0.005 is hella fking super fast really dun play & higher value less accuracy
	swerve.alignangle = 0.5;//ur swerve will turn this +- rotation to find the align angle, dun more than 1

	if (swerve.type == FWD_SWERVE)
	{
		PWMEncoder_Angle_Update(&enc1);
		PWMEncoder_Angle_Update(&enc2);
		PWMEncoder_Angle_Update(&enc3);
		PWMEncoder_Angle_Update(&enc4);
		if (swerve.aligned == 0)
		{
			if (swerve.done[0] != 2 || swerve.done[1] != 2 || swerve.done[2] != 2 || swerve.done[3] != 2)
			{
				if((HAL_GPIO_ReadPin(AhallsGPIOx, AhallsGPIO_Pin) != 0) && swerve.done[0] < 2)
				{
					if(rbms1.motor[RBMS1].t_pos > 0.5)
					{ swerve.done[0] = 1; }
					else if(rbms1.motor[RBMS1].t_pos < -0.5)
					{ swerve.done[0] = 2; }
					if(swerve.done[0] == 0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS1, rbms1.motor[RBMS1].t_pos + 0.006); }
					else
					{ RBMS_Set_Target_Position(&rbms1, RBMS1, rbms1.motor[RBMS1].t_pos - 0.006); }
				}
				else
				{ swerve.done[0] = 2; }

				if((HAL_GPIO_ReadPin(BhallsGPIOx, BhallsGPIO_Pin) != 0) && swerve.done[1] < 2)
				{
					if(rbms1.motor[RBMS2].t_pos > 0.5)
					{ swerve.done[1] = 1; }
					else if(rbms1.motor[RBMS2].t_pos < -0.5)
					{ swerve.done[1] = 2; }
					if(swerve.done[1] == 0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS2, rbms1.motor[RBMS2].t_pos + 0.006); }
					else
					{ RBMS_Set_Target_Position(&rbms1, RBMS2, rbms1.motor[RBMS2].t_pos - 0.006); }
				}
				else
				{ swerve.done[1] = 2; }

				if((HAL_GPIO_ReadPin(ChallsGPIOx, ChallsGPIO_Pin) != 0) && swerve.done[2] < 2)
				{
					if(rbms1.motor[RBMS3].t_pos > 0.5)
					{ swerve.done[2] = 1; }
					else if(rbms1.motor[RBMS3].t_pos < -0.5)
					{ swerve.done[2] = 2; }
					if(swerve.done[2] == 0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS3, rbms1.motor[RBMS3].t_pos + 0.006); }
					else
					{ RBMS_Set_Target_Position(&rbms1, RBMS3, rbms1.motor[RBMS3].t_pos - 0.006); }
				}
				else
				{ swerve.done[2] = 2; }

				if((HAL_GPIO_ReadPin(DhallsGPIOx, DhallsGPIO_Pin) != 0) && swerve.done[3] < 2)
				{
					if(rbms1.motor[RBMS4].t_pos > 0.5)
					{ swerve.done[3] = 1; }
					else if(rbms1.motor[RBMS4].t_pos < -0.5)
					{ swerve.done[3] = 2; }
					if(swerve.done[3] == 0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS4, rbms1.motor[RBMS4].t_pos + 0.006); }
					else
					{ RBMS_Set_Target_Position(&rbms1, RBMS4, rbms1.motor[RBMS4].t_pos - 0.006); }
				}
				else
				{ swerve.done[3] = 2; }
			}
			else
			{
				swerve.alignerror[0] = A_angle - enc1.Angle;
				swerve.alignerror[1] = B_angle - enc2.Angle;
				swerve.alignerror[2] = C_angle - enc3.Angle;
				swerve.alignerror[3] = D_angle - enc4.Angle;

				if (fabs(swerve.alignerror[0])>180.0)
				{
					if((swerve.alignerror[0] > 1.0))
					{ RBMS_Set_Target_Position(&rbms1, RBMS1, rbms1.motor[RBMS1].t_pos - 0.001); }
					else if(swerve.alignerror[0] < -1.0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS1, rbms1.motor[RBMS1].t_pos + 0.001); }
					else
					{ swerve.aligndone[0] = 1; }
				}
				else
				{
					if((swerve.alignerror[0] > 1.0))
					{ RBMS_Set_Target_Position(&rbms1, RBMS1, rbms1.motor[RBMS1].t_pos + 0.001); }
					else if(swerve.alignerror[0] < -1.0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS1, rbms1.motor[RBMS1].t_pos - 0.001); }
					else
					{ swerve.aligndone[0] = 1; }
				}

				if (fabs(swerve.alignerror[1])>180.0)
				{
					if((swerve.alignerror[1] > 1.0))
					{ RBMS_Set_Target_Position(&rbms1, RBMS2, rbms1.motor[RBMS2].t_pos - 0.001); }
					else if(swerve.alignerror[1] < -1.0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS2, rbms1.motor[RBMS2].t_pos + 0.001); }
					else
					{ swerve.aligndone[1] = 1; }
				}
				else
				{
					if((swerve.alignerror[1] > 1.0))
					{ RBMS_Set_Target_Position(&rbms1, RBMS2, rbms1.motor[RBMS2].t_pos + 0.001); }
					else if(swerve.alignerror[1] < -1.0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS2, rbms1.motor[RBMS2].t_pos - 0.001); }
					else
					{ swerve.aligndone[1] = 1; }
				}

				if (fabs(swerve.alignerror[2])>180.0)
				{
					if((swerve.alignerror[2] > 1.0))
					{ RBMS_Set_Target_Position(&rbms1, RBMS3, rbms1.motor[RBMS3].t_pos - 0.001); }
					else if(swerve.alignerror[2] < -1.0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS3, rbms1.motor[RBMS3].t_pos + 0.001); }
					else
					{ swerve.aligndone[2] = 1; }
				}
				else
				{
					if((swerve.alignerror[2] > 1.0))
					{ RBMS_Set_Target_Position(&rbms1, RBMS3, rbms1.motor[RBMS3].t_pos + 0.001); }
					else if(swerve.alignerror[2] < -1.0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS3, rbms1.motor[RBMS3].t_pos - 0.001); }
					else
					{ swerve.aligndone[2] = 1; }
				}

				if (fabs(swerve.alignerror[3])>180.0)
				{
					if((swerve.alignerror[3] > 1.0))
					{ RBMS_Set_Target_Position(&rbms1, RBMS4, rbms1.motor[RBMS4].t_pos - 0.001); }
					else if(swerve.alignerror[3] < -1.0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS4, rbms1.motor[RBMS4].t_pos + 0.001); }
					else
					{ swerve.aligndone[3] = 1; }
				}
				else
				{
					if((swerve.alignerror[3] > 1.0))
					{ RBMS_Set_Target_Position(&rbms1, RBMS4, rbms1.motor[RBMS4].t_pos + 0.001); }
					else if(swerve.alignerror[3] < -1.0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS4, rbms1.motor[RBMS4].t_pos - 0.001); }
					else
					{ swerve.aligndone[3] = 1; }
				}

				if ((swerve.aligndone[0]==1) && (swerve.aligndone[1]==1) && (swerve.aligndone[2]==1) && (swerve.aligndone[3]==1))
				{
					swerve.aligned = 1;
					rbms1.motor[0].reset_pos = 1;
					rbms1.motor[1].reset_pos = 1;
					rbms1.motor[2].reset_pos = 1;
					rbms1.motor[3].reset_pos = 1;
				}
			}
		}
	}
	else if (swerve.type == TRI_SWERVE)
	{
		PWMEncoder_Angle_Update(&enc1);
		PWMEncoder_Angle_Update(&enc2);
		PWMEncoder_Angle_Update(&enc3);
		if (swerve.aligned == 0)
		{
			if (swerve.done[0] != 2 || swerve.done[1] != 2 || swerve.done[2] != 2)
			{
				if((HAL_GPIO_ReadPin(AhallsGPIOx, AhallsGPIO_Pin) != 0) && swerve.done[0] < 2)
				{
					if(rbms1.motor[RBMS1].t_pos > 0.5)
					{ swerve.done[0] = 1; }
					else if(rbms1.motor[RBMS1].t_pos < -0.5)
					{ swerve.done[0] = 2; }
					if(swerve.done[0] == 0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS1, rbms1.motor[RBMS1].t_pos + 0.006); }
					else
					{ RBMS_Set_Target_Position(&rbms1, RBMS1, rbms1.motor[RBMS1].t_pos - 0.006); }
				}
				else
				{ swerve.done[0] = 2; }

				if((HAL_GPIO_ReadPin(BhallsGPIOx, BhallsGPIO_Pin) != 0) && swerve.done[1] < 2)
				{
					if(rbms1.motor[RBMS2].t_pos > 0.5)
					{ swerve.done[1] = 1; }
					else if(rbms1.motor[RBMS2].t_pos < -0.5)
					{ swerve.done[1] = 2; }
					if(swerve.done[1] == 0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS2, rbms1.motor[RBMS2].t_pos + 0.006); }
					else
					{ RBMS_Set_Target_Position(&rbms1, RBMS2, rbms1.motor[RBMS2].t_pos - 0.006); }
				}
				else
				{ swerve.done[1] = 2; }

				if((HAL_GPIO_ReadPin(ChallsGPIOx, ChallsGPIO_Pin) != 0) && swerve.done[2] < 2)
				{
					if(rbms1.motor[RBMS3].t_pos > 0.5)
					{ swerve.done[2] = 1; }
					else if(rbms1.motor[RBMS3].t_pos < -0.5)
					{ swerve.done[2] = 2; }
					if(swerve.done[2] == 0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS3, rbms1.motor[RBMS3].t_pos + 0.006); }
					else
					{ RBMS_Set_Target_Position(&rbms1, RBMS3, rbms1.motor[RBMS3].t_pos - 0.006); }
				}
				else
				{ swerve.done[2] = 2; }
			}
			else
			{
				swerve.alignerror[0] = A_angle - enc1.Angle;
				swerve.alignerror[1] = B_angle - enc2.Angle;
				swerve.alignerror[2] = C_angle - enc3.Angle;

				if (fabs(swerve.alignerror[0])>180.0)
				{
					if((swerve.alignerror[0] > 1.0))
					{ RBMS_Set_Target_Position(&rbms1, RBMS1, rbms1.motor[RBMS1].t_pos - 0.001); }
					else if(swerve.alignerror[0] < -1.0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS1, rbms1.motor[RBMS1].t_pos + 0.001); }
					else
					{ swerve.aligndone[0] = 1; }
				}
				else
				{
					if((swerve.alignerror[0] > 1.0))
					{ RBMS_Set_Target_Position(&rbms1, RBMS1, rbms1.motor[RBMS1].t_pos + 0.001); }
					else if(swerve.alignerror[0] < -1.0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS1, rbms1.motor[RBMS1].t_pos - 0.001); }
					else
					{ swerve.aligndone[0] = 1; }
				}

				if (fabs(swerve.alignerror[1])>180.0)
				{
					if((swerve.alignerror[1] > 1.0))
					{ RBMS_Set_Target_Position(&rbms1, RBMS2, rbms1.motor[RBMS2].t_pos - 0.001); }
					else if(swerve.alignerror[1] < -1.0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS2, rbms1.motor[RBMS2].t_pos + 0.001); }
					else
					{ swerve.aligndone[1] = 1; }
				}
				else
				{
					if((swerve.alignerror[1] > 1.0))
					{ RBMS_Set_Target_Position(&rbms1, RBMS2, rbms1.motor[RBMS2].t_pos + 0.001); }
					else if(swerve.alignerror[1] < -1.0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS2, rbms1.motor[RBMS2].t_pos - 0.001); }
					else
					{ swerve.aligndone[1] = 1; }
				}

				if (fabs(swerve.alignerror[2])>180.0)
				{
					if((swerve.alignerror[2] > 1.0))
					{ RBMS_Set_Target_Position(&rbms1, RBMS3, rbms1.motor[RBMS3].t_pos - 0.001); }
					else if(swerve.alignerror[2] < -1.0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS3, rbms1.motor[RBMS3].t_pos + 0.001); }
					else
					{ swerve.aligndone[2] = 1; }
				}
				else
				{
					if((swerve.alignerror[2] > 1.0))
					{ RBMS_Set_Target_Position(&rbms1, RBMS3, rbms1.motor[RBMS3].t_pos + 0.001); }
					else if(swerve.alignerror[2] < -1.0)
					{ RBMS_Set_Target_Position(&rbms1, RBMS3, rbms1.motor[RBMS3].t_pos - 0.001); }
					else
					{ swerve.aligndone[2] = 1; }
				}

				if ((swerve.aligndone[0]==1) && (swerve.aligndone[1]==1) && (swerve.aligndone[2]==1))
				{
					swerve.aligned = 1;
					rbms1.motor[0].reset_pos = 1;
					rbms1.motor[1].reset_pos = 1;
					rbms1.motor[2].reset_pos = 1;
				}
			}
		}
	}
}
