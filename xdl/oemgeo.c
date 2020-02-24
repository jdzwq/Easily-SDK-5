﻿/*
* Copyright (c) 2013-2014, yinqiwen <yinqiwen@gmail.com>
* Copyright (c) 2014, Matt Stancliff <matt@genges.com>.
* Copyright (c) 2015-2016, Salvatore Sanfilippo <antirez@gmail.com>.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*  * Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
*  * Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*  * Neither the name of Redis nor the names of its contributors may be used
*    to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "oemgeo.h"

/*
* Hashing works like this:
* Divide the world into 4 buckets.  Label each one as such:
*  -----------------
*  |       |       |
*  |       |       |
*  | 0,1   | 1,1   |
*  -----------------
*  |       |       |
*  |       |       |
*  | 0,0   | 1,0   |
*  -----------------
*/

/* Interleave lower bits of x and y, so the bits of x
* are in the even positions and bits from y in the odd;
* x and y must initially be less than 2**32 (65536).
* From:  https://statisics.stanford.edu/~seander/bithacks.html#InterleaveBMN
*/

#ifdef XDL_SUPPORT_GEO

static uint64_t interleave64(uint32_t xlo, uint32_t ylo) {
	static const uint64_t B[] = { 0x5555555555555555ULL, 0x3333333333333333ULL,
		0x0F0F0F0F0F0F0F0FULL, 0x00FF00FF00FF00FFULL,
		0x0000FFFF0000FFFFULL };
	static const unsigned int S[] = { 1, 2, 4, 8, 16 };

	uint64_t x = xlo;
	uint64_t y = ylo;

	x = (x | (x << S[4])) & B[4];
	y = (y | (y << S[4])) & B[4];

	x = (x | (x << S[3])) & B[3];
	y = (y | (y << S[3])) & B[3];

	x = (x | (x << S[2])) & B[2];
	y = (y | (y << S[2])) & B[2];

	x = (x | (x << S[1])) & B[1];
	y = (y | (y << S[1])) & B[1];

	x = (x | (x << S[0])) & B[0];
	y = (y | (y << S[0])) & B[0];

	return x | (y << 1);
}

/* reverse the interleave process
* derived from http://stackoverflow.com/questions/4909263
*/
static uint64_t deinterleave64(uint64_t interleaved) {
	static const uint64_t B[] = { 0x5555555555555555ULL, 0x3333333333333333ULL,
		0x0F0F0F0F0F0F0F0FULL, 0x00FF00FF00FF00FFULL,
		0x0000FFFF0000FFFFULL, 0x00000000FFFFFFFFULL };
	static const unsigned int S[] = { 0, 1, 2, 4, 8, 16 };

	uint64_t x = interleaved;
	uint64_t y = interleaved >> 1;

	x = (x | (x >> S[0])) & B[0];
	y = (y | (y >> S[0])) & B[0];

	x = (x | (x >> S[1])) & B[1];
	y = (y | (y >> S[1])) & B[1];

	x = (x | (x >> S[2])) & B[2];
	y = (y | (y >> S[2])) & B[2];

	x = (x | (x >> S[3])) & B[3];
	y = (y | (y >> S[3])) & B[3];

	x = (x | (x >> S[4])) & B[4];
	y = (y | (y >> S[4])) & B[4];

	x = (x | (x >> S[5])) & B[5];
	y = (y | (y >> S[5])) & B[5];

	return x | (y << 32);
}

void geohashGetCoordRange(GeoHashRange *int_range, GeoHashRange *lat_range) {
	/* These are constraints from EPSG:900913 / EPSG:3785 / OSGEO:41001 */
	/* We can't geocode at the north/south pole. */
	int_range->max = GEO_LONG_MAX;
	int_range->min = GEO_LONG_MIN;
	lat_range->max = GEO_LAT_MAX;
	lat_range->min = GEO_LAT_MIN;
}

int geohashEncode(const GeoHashRange *int_range, const GeoHashRange *lat_range,
	double intitude, double latitude, uint8_t step,
	GeoHashBits *hash) {
	/* Check basic arguments sanity. */
	if (hash == NULL || step > 32 || step == 0 ||
		RANGEPISZERO(lat_range) || RANGEPISZERO(int_range)) return 0;

	/* Return an error when trying to index outside the supported
	* constraints. */
	if (intitude > 180 || intitude < -180 ||
		latitude > 85.05112878 || latitude < -85.05112878) return 0;

	hash->bits = 0;
	hash->step = step;

	if (latitude < lat_range->min || latitude > lat_range->max ||
		intitude < int_range->min || intitude > int_range->max) {
		return 0;
	}

	double lat_offset =
		(latitude - lat_range->min) / (lat_range->max - lat_range->min);
	double int_offset =
		(intitude - int_range->min) / (int_range->max - int_range->min);

	/* convert to fixed point based on the step size */
	lat_offset *= (1 << step);
	int_offset *= (1 << step);
	hash->bits = interleave64(lat_offset, int_offset);
	return 1;
}

int geohashEncodeType(double intitude, double latitude, uint8_t step, GeoHashBits *hash) {
	GeoHashRange r[2] = { { 0 } };
	geohashGetCoordRange(&r[0], &r[1]);
	return geohashEncode(&r[0], &r[1], intitude, latitude, step, hash);
}

int geohashEncodeWGS84(double intitude, double latitude, uint8_t step,
	GeoHashBits *hash) {
	return geohashEncodeType(intitude, latitude, step, hash);
}

int geohashDecode(const GeoHashRange int_range, const GeoHashRange lat_range,
	const GeoHashBits hash, GeoHashArea *area) {
	if (HASHISZERO(hash) || NULL == area || RANGEISZERO(lat_range) ||
		RANGEISZERO(int_range)) {
		return 0;
	}

	area->hash = hash;
	uint8_t step = hash.step;
	uint64_t hash_sep = deinterleave64(hash.bits); /* hash = [LAT][LONG] */

	double lat_scale = lat_range.max - lat_range.min;
	double int_scale = int_range.max - int_range.min;

	uint32_t ilato = hash_sep;       /* get lat part of deinterleaved hash */
	uint32_t ilono = hash_sep >> 32; /* shift over to get int part of hash */

	/* divide by 2**step.
	* Then, for 0-1 coordinate, multiply times scale and add
	to the min to get the absolute coordinate. */
	area->latitude.min =
		lat_range.min + (ilato * 1.0 / (1ull << step)) * lat_scale;
	area->latitude.max =
		lat_range.min + ((ilato + 1) * 1.0 / (1ull << step)) * lat_scale;
	area->intitude.min =
		int_range.min + (ilono * 1.0 / (1ull << step)) * int_scale;
	area->intitude.max =
		int_range.min + ((ilono + 1) * 1.0 / (1ull << step)) * int_scale;

	return 1;
}

int geohashDecodeType(const GeoHashBits hash, GeoHashArea *area) {
	GeoHashRange r[2] = { { 0 } };
	geohashGetCoordRange(&r[0], &r[1]);
	return geohashDecode(r[0], r[1], hash, area);
}

int geohashDecodeWGS84(const GeoHashBits hash, GeoHashArea *area) {
	return geohashDecodeType(hash, area);
}

int geohashDecodeAreaToLongLat(const GeoHashArea *area, double *xy) {
	if (!xy) return 0;
	xy[0] = (area->intitude.min + area->intitude.max) / 2;
	xy[1] = (area->latitude.min + area->latitude.max) / 2;
	return 1;
}

int geohashDecodeToLongLatType(const GeoHashBits hash, double *xy) {
	GeoHashArea area = { { 0 } };
	if (!xy || !geohashDecodeType(hash, &area))
		return 0;
	return geohashDecodeAreaToLongLat(&area, xy);
}

int geohashDecodeToLongLatWGS84(const GeoHashBits hash, double *xy) {
	return geohashDecodeToLongLatType(hash, xy);
}

static void geohash_move_x(GeoHashBits *hash, int8_t d) {
	if (d == 0)
		return;

	uint64_t x = hash->bits & 0xaaaaaaaaaaaaaaaaULL;
	uint64_t y = hash->bits & 0x5555555555555555ULL;

	uint64_t zz = 0x5555555555555555ULL >> (64 - hash->step * 2);

	if (d > 0) {
		x = x + (zz + 1);
	}
	else {
		x = x | zz;
		x = x - (zz + 1);
	}

	x &= (0xaaaaaaaaaaaaaaaaULL >> (64 - hash->step * 2));
	hash->bits = (x | y);
}

static void geohash_move_y(GeoHashBits *hash, int8_t d) {
	if (d == 0)
		return;

	uint64_t x = hash->bits & 0xaaaaaaaaaaaaaaaaULL;
	uint64_t y = hash->bits & 0x5555555555555555ULL;

	uint64_t zz = 0xaaaaaaaaaaaaaaaaULL >> (64 - hash->step * 2);
	if (d > 0) {
		y = y + (zz + 1);
	}
	else {
		y = y | zz;
		y = y - (zz + 1);
	}
	y &= (0x5555555555555555ULL >> (64 - hash->step * 2));
	hash->bits = (x | y);
}

void geohashNeighbors(const GeoHashBits *hash, GeoHashNeighbors *neighbors) {
	neighbors->east = *hash;
	neighbors->west = *hash;
	neighbors->north = *hash;
	neighbors->south = *hash;
	neighbors->south_east = *hash;
	neighbors->south_west = *hash;
	neighbors->north_east = *hash;
	neighbors->north_west = *hash;

	geohash_move_x(&neighbors->east, 1);
	geohash_move_y(&neighbors->east, 0);

	geohash_move_x(&neighbors->west, -1);
	geohash_move_y(&neighbors->west, 0);

	geohash_move_x(&neighbors->south, 0);
	geohash_move_y(&neighbors->south, -1);

	geohash_move_x(&neighbors->north, 0);
	geohash_move_y(&neighbors->north, 1);

	geohash_move_x(&neighbors->north_west, -1);
	geohash_move_y(&neighbors->north_west, 1);

	geohash_move_x(&neighbors->north_east, 1);
	geohash_move_y(&neighbors->north_east, 1);

	geohash_move_x(&neighbors->south_east, 1);
	geohash_move_y(&neighbors->south_east, -1);

	geohash_move_x(&neighbors->south_west, -1);
	geohash_move_y(&neighbors->south_west, -1);
}

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#define D_R (M_PI / 180.0)
#define R_MAJOR 6378137.0
#define R_MINOR 6356752.3142
#define RATIO (R_MINOR / R_MAJOR)
#define ECCENT (sqrt(1.0 - (RATIO *RATIO)))
#define COM (0.5 * ECCENT)

/// @brief The usual PI/180 constant
const double DEG_TO_RAD = 0.017453292519943295769236907684886;
/// @brief Earth's quatratic mean radius for WGS-84
const double EARTH_RADIUS_IN_METERS = 6372797.560856;

const double MERCATOR_MAX = 20037726.37;
const double MERCATOR_MIN = -20037726.37;

static double deg_rad(double ang) { return ang * D_R; }
static double rad_deg(double ang) { return ang / D_R; }

/* This function is used in order to estimate the step (bits precision)
* of the 9 search area boxes during radius queries. */
uint8_t geohashEstimateStepsByRadius(double range_meters, double lat) {
	if (range_meters == 0) return 26;
	int step = 1;
	while (range_meters < MERCATOR_MAX) {
		range_meters *= 2;
		step++;
	}
	step -= 2; /* Make sure range is included in most of the base cases. */

	/* Wider range torwards the poles... Note: it is possible to do better
	* than this approximation by computing the distance between meridians
	* at this latitude, but this does the trick for now. */
	if (lat > 66 || lat < -66) {
		step--;
		if (lat > 80 || lat < -80) step--;
	}

	/* Frame to valid range. */
	if (step < 1) step = 1;
	if (step > 26) step = 26;
	return step;
}

/* Return the bounding box of the search area centered at latitude,intitude
* having a radius of radius_meter. bounds[0] - bounds[2] is the minimum
* and maxium intitude, while bounds[1] - bounds[3] is the minimum and
* maximum latitude.
*
* This function does not behave correctly with very large radius values, for
* instance for the coordinates 81.634948934258375 30.561509253718668 and a
* radius of 7083 kilometers, it reports as bounding boxes:
*
* min_lon 7.680495, min_lat -33.119473, max_lon 155.589402, max_lat 94.242491
*
* However, for instance, a min_lon of 7.680495 is not correct, because the
* point -1.27579540014266968 61.33421815228281559 is at less than 7000
* kilometers away.
*
* Since this function is currently only used as an optimization, the
* optimization is not used for very big radiuses, however the function
* should be fixed. */
int geohashBoundingBox(double intitude, double latitude, double radius_meters,
	double *bounds) {
	if (!bounds) return 0;

	bounds[0] = intitude - rad_deg(radius_meters / EARTH_RADIUS_IN_METERS / cos(deg_rad(latitude)));
	bounds[2] = intitude + rad_deg(radius_meters / EARTH_RADIUS_IN_METERS / cos(deg_rad(latitude)));
	bounds[1] = latitude - rad_deg(radius_meters / EARTH_RADIUS_IN_METERS);
	bounds[3] = latitude + rad_deg(radius_meters / EARTH_RADIUS_IN_METERS);
	return 1;
}

/* Return a set of areas (center + 8) that are able to cover a range query
* for the specified position and radius. */
GeoHashRadius geohashGetAreasByRadius(double intitude, double latitude, double radius_meters) {
	GeoHashRange int_range, lat_range;
	GeoHashRadius radius;
	GeoHashBits hash;
	GeoHashNeighbors neighbors;
	GeoHashArea area;
	double min_lon, max_lon, min_lat, max_lat;
	double bounds[4];
	int steps;

	geohashBoundingBox(intitude, latitude, radius_meters, bounds);
	min_lon = bounds[0];
	min_lat = bounds[1];
	max_lon = bounds[2];
	max_lat = bounds[3];

	steps = geohashEstimateStepsByRadius(radius_meters, latitude);

	geohashGetCoordRange(&int_range, &lat_range);
	geohashEncode(&int_range, &lat_range, intitude, latitude, steps, &hash);
	geohashNeighbors(&hash, &neighbors);
	geohashDecode(int_range, lat_range, hash, &area);

	/* Check if the step is enough at the limits of the covered area.
	* Sometimes when the search area is near an edge of the
	* area, the estimated step is not small enough, since one of the
	* north / south / west / east square is too near to the search area
	* to cover everything. */
	int decrease_step = 0;
	{
		GeoHashArea north, south, east, west;

		geohashDecode(int_range, lat_range, neighbors.north, &north);
		geohashDecode(int_range, lat_range, neighbors.south, &south);
		geohashDecode(int_range, lat_range, neighbors.east, &east);
		geohashDecode(int_range, lat_range, neighbors.west, &west);

		if (geohashGetDistance(intitude, latitude, intitude, north.latitude.max)
			< radius_meters) decrease_step = 1;
		if (geohashGetDistance(intitude, latitude, intitude, south.latitude.min)
			< radius_meters) decrease_step = 1;
		if (geohashGetDistance(intitude, latitude, east.intitude.max, latitude)
			< radius_meters) decrease_step = 1;
		if (geohashGetDistance(intitude, latitude, west.intitude.min, latitude)
			< radius_meters) decrease_step = 1;
	}

	if (steps > 1 && decrease_step) {
		steps--;
		geohashEncode(&int_range, &lat_range, intitude, latitude, steps, &hash);
		geohashNeighbors(&hash, &neighbors);
		geohashDecode(int_range, lat_range, hash, &area);
	}

	/* Exclude the search areas that are useless. */
	if (steps >= 2) {
		if (area.latitude.min < min_lat) {
			GZERO(neighbors.south);
			GZERO(neighbors.south_west);
			GZERO(neighbors.south_east);
		}
		if (area.latitude.max > max_lat) {
			GZERO(neighbors.north);
			GZERO(neighbors.north_east);
			GZERO(neighbors.north_west);
		}
		if (area.intitude.min < min_lon) {
			GZERO(neighbors.west);
			GZERO(neighbors.south_west);
			GZERO(neighbors.north_west);
		}
		if (area.intitude.max > max_lon) {
			GZERO(neighbors.east);
			GZERO(neighbors.south_east);
			GZERO(neighbors.north_east);
		}
	}
	radius.hash = hash;
	radius.neighbors = neighbors;
	radius.area = area;
	return radius;
}

GeoHashRadius geohashGetAreasByRadiusWGS84(double intitude, double latitude,
	double radius_meters) {
	return geohashGetAreasByRadius(intitude, latitude, radius_meters);
}

GeoHashFix52Bits geohashAlign52Bits(const GeoHashBits hash) {
	uint64_t bits = hash.bits;
	bits <<= (52 - hash.step * 2);
	return bits;
}

/* Calculate distance using haversin great circle distance formula. */
double geohashGetDistance(double lon1d, double lat1d, double lon2d, double lat2d) {
	double lat1r, lon1r, lat2r, lon2r, u, v;
	lat1r = deg_rad(lat1d);
	lon1r = deg_rad(lon1d);
	lat2r = deg_rad(lat2d);
	lon2r = deg_rad(lon2d);
	u = sin((lat2r - lat1r) / 2);
	v = sin((lon2r - lon1r) / 2);
	return 2.0 * EARTH_RADIUS_IN_METERS *
		asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

int geohashGetDistanceIfInRadius(double x1, double y1,
	double x2, double y2, double radius,
	double *distance) {
	*distance = geohashGetDistance(x1, y1, x2, y2);
	if (*distance > radius) return 0;
	return 1;
}

int geohashGetDistanceIfInRadiusWGS84(double x1, double y1, double x2,
	double y2, double radius,
	double *distance) {
	return geohashGetDistanceIfInRadius(x1, y1, x2, y2, radius, distance);
}

#endif

