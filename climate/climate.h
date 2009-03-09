/** $Id: climate.h 1182 2008-12-22 22:08:36Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file climate.h
	@addtogroup climate
	@ingroup modules
 @{
 **/

#ifndef _CLIMATE_H
#define _CLIMATE_H

#include <stdarg.h>
#include "gridlabd.h"
#include "solar_angles.h"

typedef enum{
	CP_S  = 0,
    CP_SE = 1,
	CP_SW = 2,
	CP_E  = 3,
	CP_W  = 4,
	CP_NE = 5,
	CP_NW = 6,
	CP_N  = 7,
	CP_H  = 8,
	CP_LAST = 9
} COMPASS_PTS;

typedef struct s_tmy {
	double temp; // F
	double temp_raw; // C
	double rh; // %rh
	double dnr;
	double dhr;
	double solar[CP_LAST]; // W/sf
	double windspeed;
} TMYDATA;

/**
 * This implements a Gridlab-D specific TMY2 data reader.  It was implemented
 * to pull specific information from the TMY2 raw format, including latitude
 * information contained in the TMY2 header.  Header information will be 
 * maintained for the lifetime of the reader, as it may be needed to populate
 * columns of the TMY2 structure for later use.  Leap years are handled by 
 * treating February 29th and March 1 as numerically equivalent.  IE on a
 * leap year, March 1 data is repeated for February 29th.
 */
class tmy2_reader{
private:
	// Header information
	char data_city[75];
	char data_state[3];
	int lat_degrees;
	int lat_minutes;
	int long_degrees;
	int long_minutes;

	

	FILE *fp;
	char buf[500]; // buffer to hold line data.

public:
	int tz_offset;
	tmy2_reader(){}

	void close();

	/**
	 * Open the file for reading.  This will read in the header information
	 * and position the file reader at the first data line in the file.
	 *
	 * This call will throw an exception if the file fails to open
	 *
	 * @param file the name of the TMY2 file to open
	 */
	int open(const char *file);

	/**
	 * Store the current line in a buffer for later reading by read_data
	 */
	int next();

	/**
	 * Populate the given arguments with data from the tmy2 file header
	 *
	 * @param city
	 * @param state
	 * @param degrees latitude degrees
	 * @param minutes latitude minutes
	 * @param long_deg longitude degrees
	 * @param long_min longitude minutes
	 */
	int header_info(char* city, char* state, int* degrees, int* minutes, int* long_deg, int* long_min);

	/**
	 * Populate the given arguments with data from the buffer.
	 *
	 * @param dnr - Direct Normal Radiation
	 * @param dhr - Diffuse Horizontal Radiation
	 * @param rh Relative Humidity
	 * @param tdb - Dry Bulb Temperature
	 * @param month - Month of the observation
	 * @param day - Day of the observation
	 * @param hour - hour of the observation
	 * @param wind Wind speed (optional)
	 */
	int read_data(double *dnr, double *dhr, double *tdb, double *rh, int* month, int* day, int* hour, double *wind=0);

	double calc_solar(COMPASS_PTS cpt, short doy, double lat, double sol_time, double dnr, double dhr,double vert_angle);

};



class climate {
public:
	char32 city; ///< the city
	char1024 tmyfile; ///< the TMY file name
	double temperature; ///< the temperature (degF)
	double temperature_raw; ///< the temperature (degC)
	double humidity; ///< the relative humidity (%)
	double solar_flux[CP_LAST]; ///< Solar flux array (W/sf) Elements are in order: [S, SE, SW, E, W, NE, NW, N, H]
	double solar_direct;
	double solar_diffuse;
	double wind_speed; ///< wind speed (m/s)
	double wind_dir; ///< wind direction (0-360)
	double wind_gust; ///< wind gusts (m/s)
private:
	SolarAngles *sa;
	tmy2_reader file;
	TMYDATA *tmy;
public:
	static CLASS *oclass;
	static climate *defaults;
public:
	climate(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP sync(TIMESTAMP t0);
}; ///< climate data 

#endif

/**@}*/
