// $Id: triplex_meter.h 942 2008-09-19 20:03:17Z dchassin $
//	Copyright (C) 2008 Battelle Memorial Institute

#ifndef _TRIPLEXMETER_H
#define _TRIPLEXMETER_H

#include "powerflow.h"
#include "triplex_node.h"

class triplex_meter : public triplex_node
{
public:
	complex measured_voltage[3];	///< measured voltage
	complex measured_current[3];	///< measured current
	double measured_real_energy;	///< metered real energy consumption
	double measured_reactive_energy;///< metered reactive energy consumption
	complex measured_power; //< metered power
	complex indiv_measured_power[3]; //individual phase power
	double measured_demand; //< metered demand (peak of power)
	double measured_real_power; //< metered real power
	double measured_reactive_power; //< metered reactive power
	TIMESTAMP next_time;
	TIMESTAMP dt;
	TIMESTAMP last_t;

#ifdef SUPPORT_OUTAGES
	int16 sustained_count;	//reliability sustained event counter
	int16 momentary_count;	//reliability momentary event counter
	int16 total_count;		//reliability total event counter
	int16 s_flag;			//reliability flag that gets set if the meter experienced more than n sustained interruptions
	int16 t_flag;			//reliability flage that gets set if the meter experienced more than n events total
	complex pre_load;		//the load prior to being interrupted
#endif

	double hourly_acc;
	double previous_monthly_bill;	//Total monthly bill for the previous month
	double monthly_bill;			//Accumulator for the current month's bill
	double monthly_fee;				//Once a month flat fee for customer hook-up
	double monthly_energy;			//Accumulator for the current month's energy
	double last_energy;				//Meter reading at end of previous month
	typedef enum {
		BM_NONE,
		BM_UNIFORM,
		BM_TIERED,
		BM_HOURLY
	} BILLMODE;
	BILLMODE bill_mode;
	OBJECT *power_market;
	PROPERTY *price_prop;
	int32 bill_day;					//Day bill is to be processed (assumed to occur at midnight of that day)
	int last_bill_month;			//Keeps track of which month we are looking at
	double price;					//Standard uniform pricing
	double tier_price[3], tier_energy[3];  //Allows for additional tiers of pricing over the standard price in TIERED

	double process_bill(TIMESTAMP t1);
	int check_prices();

public:
	static CLASS *oclass;
	static CLASS *pclass;
public:
	triplex_meter(MODULE *mod);
	inline triplex_meter(CLASS *cl=oclass):triplex_node(cl){};
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP postsync(TIMESTAMP t0, TIMESTAMP t1);
	int isa(char *classname);
};

#endif // _TRIPLEXMETER_H

