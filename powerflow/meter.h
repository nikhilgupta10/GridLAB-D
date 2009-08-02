// $Id: meter.h 1182 2008-12-22 22:08:36Z dchassin $
//	Copyright (C) 2008 Battelle Memorial Institute

#ifndef _METER_H
#define _METER_H

#include "powerflow.h"
#include "node.h"

class meter : public node
{
public:
	complex measured_voltage[3];	///< measured voltage
	complex measured_current[3];	///< measured current
	double measured_real_energy;	///< metered real energy consumption
	double measured_reactive_energy;///< metered reactive energy consumption
	complex measured_power;			///< metered power
	double measured_demand;			///< metered demand (peak of power)
	double measured_real_power;		///< metered real power
	double measured_reactive_power; ///< metered reactive power
	complex indiv_measured_power[3];///< metered power on each phase
	TIMESTAMP dt;
	TIMESTAMP last_t;

public:
	static CLASS *oclass;
	static CLASS *pclass;
public:
	meter(MODULE *mod);
	inline meter(CLASS *cl=oclass):node(cl){};
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP postsync(TIMESTAMP t0, TIMESTAMP t1);
	int isa(char *classname);
};

#endif // _METER_H

