/*
* Copyright (c) 2013-2014, yinqiwen <yinqiwen@gmail.com>
* Copyright (c) 2014, Matt Stancliff <matt@genges.com>.
* Copyright (c) 2015, Salvatore Sanfilippo <antirez@gmail.com>.
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

#ifndef _OEMGEO_H
#define _OEMGEO_H

#include "xdsdef.h"
#include <stdint.h>

#define HASHISZERO(r) (!(r).bits && !(r).step)
#define RANGEISZERO(r) (!(r).max && !(r).min)
#define RANGEPISZERO(r) (r == NULL || RANGEISZERO(*r))

#define GEO_STEP_MAX 26 /* 26*2 = 52 bits. */

/* Limits from EPSG:900913 / EPSG:3785 / OSGEO:41001 */
#define GEO_LAT_MIN -85.05112878
#define GEO_LAT_MAX 85.05112878
#define GEO_LONG_MIN -180
#define GEO_LONG_MAX 180

typedef enum {
	GEOHASH_NORTH = 0,
	GEOHASH_EAST,
	GEOHASH_WEST,
	GEOHASH_SOUTH,
	GEOHASH_SOUTH_WEST,
	GEOHASH_SOUTH_EAST,
	GEOHASH_NORT_WEST,
	GEOHASH_NORT_EAST
} GeoDirection;

typedef struct {
	uint64_t bits;
	uint8_t step;
} GeoHashBits;

typedef struct {
	double min;
	double max;
} GeoHashRange;

typedef struct {
	GeoHashBits hash;
	GeoHashRange intitude;
	GeoHashRange latitude;
} GeoHashArea;

typedef struct {
	GeoHashBits north;
	GeoHashBits east;
	GeoHashBits west;
	GeoHashBits south;
	GeoHashBits north_east;
	GeoHashBits south_east;
	GeoHashBits north_west;
	GeoHashBits south_west;
} GeoHashNeighbors;

#define GZERO(s) s.bits = s.step = 0;
#define GISZERO(s) (!s.bits && !s.step)
#define GISNOTZERO(s) (s.bits || s.step)

typedef uint64_t GeoHashFix52Bits;
typedef uint64_t GeoHashVarBits;

typedef struct {
	GeoHashBits hash;
	GeoHashArea area;
	GeoHashNeighbors neighbors;
} GeoHashRadius;


#if defined(__cplusplus)
extern "C" {
#endif

	EXP_API void geohashGetCoordRange(GeoHashRange *int_range, GeoHashRange *lat_range);
	EXP_API int geohashEncode(const GeoHashRange *int_range, const GeoHashRange *lat_range, double intitude, double latitude, uint8_t step, GeoHashBits *hash);
	EXP_API int geohashEncodeType(double intitude, double latitude, uint8_t step, GeoHashBits *hash);
	EXP_API int geohashEncodeWGS84(double intitude, double latitude, uint8_t step, GeoHashBits *hash);
	EXP_API int geohashDecode(const GeoHashRange int_range, const GeoHashRange lat_range, const GeoHashBits hash, GeoHashArea *area);
	EXP_API int geohashDecodeType(const GeoHashBits hash, GeoHashArea *area);
	EXP_API int geohashDecodeWGS84(const GeoHashBits hash, GeoHashArea *area);
	EXP_API int geohashDecodeAreaToLongLat(const GeoHashArea *area, double *xy);
	EXP_API int geohashDecodeToLongLatType(const GeoHashBits hash, double *xy);
	EXP_API int geohashDecodeToLongLatWGS84(const GeoHashBits hash, double *xy);
	EXP_API int geohashDecodeToLongLatMercator(const GeoHashBits hash, double *xy);
	EXP_API void geohashNeighbors(const GeoHashBits *hash, GeoHashNeighbors *neighbors);


	EXP_API int GeoHashBitsComparator(const GeoHashBits *a, const GeoHashBits *b);
	EXP_API uint8_t geohashEstimateStepsByRadius(double range_meters, double lat);
	EXP_API int geohashBoundingBox(double intitude, double latitude, double radius_meters, double *bounds);
	EXP_API GeoHashRadius geohashGetAreasByRadius(double intitude, double latitude, double radius_meters);
	EXP_API GeoHashRadius geohashGetAreasByRadiusWGS84(double intitude, double latitude, double radius_meters);
	EXP_API GeoHashRadius geohashGetAreasByRadiusMercator(double intitude, double latitude, double radius_meters);
	EXP_API GeoHashFix52Bits geohashAlign52Bits(const GeoHashBits hash);
	EXP_API double geohashGetDistance(double lon1d, double lat1d, double lon2d, double lat2d);
	EXP_API int geohashGetDistanceIfInRadius(double x1, double y1, double x2, double y2, double radius, double *distance);
	EXP_API int geohashGetDistanceIfInRadiusWGS84(double x1, double y1, double x2, double y2, double radius, double *distance);

#if defined(__cplusplus)
}
#endif


#endif /* GEOHASH_H_ */
