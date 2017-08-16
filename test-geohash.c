#include <time.h>
#include <stdio.h>
#include <sys/time.h>

#include "geohash.h"

uint64_t get_current_epoch_millis()
{
    struct timeval timeValue;
    gettimeofday(&timeValue, NULL);
    uint64_t ret = ((uint64_t) timeValue.tv_sec) * 1000;
    ret += ((timeValue.tv_usec) / 1000);
    return ret;
}

int test_1()
{
    GeoHashBits hash, fast_hash;

    GeoHashRange lat_range, lon_range;
    lat_range.max = 20037726.37;
    lat_range.min = -20037726.37;
    lon_range.max = 20037726.37;
    lon_range.min = -20037726.37;
    double latitude = 9741705.20;
    double longitude = 5417390.90;

    uint32_t loop = 10000000;
    uint32_t i = 0;
    uint64_t start = get_current_epoch_millis();
    for (i = 0; i < loop; i++)
    {
        geohash_encode(lat_range, lon_range, latitude, longitude, 24, &hash);
    }

    uint64_t end = get_current_epoch_millis();
    printf("Cost %llums to encode\n", end - start);

    start = get_current_epoch_millis();
    for (i = 0; i < loop; i++)
    {
        geohash_fast_encode(lat_range, lon_range, latitude, longitude, 24, &fast_hash);
    }
    end = get_current_epoch_millis();
    printf("Cost %llums to fast encode\n", end - start);

    GeoHashArea area, area1;
    start = get_current_epoch_millis();
    for (i = 0; i < loop; i++)
    {
        geohash_decode(lat_range, lon_range, hash, &area);
    }
    end = get_current_epoch_millis();
    printf("Cost %llums to  decode\n", end - start);

    start = get_current_epoch_millis();
    for (i = 0; i < loop; i++)
    {
        geohash_fast_decode(lat_range, lon_range, hash, &area1);
    }
    end = get_current_epoch_millis();
    printf("Cost %llums to fast decode\n", end - start);

    return 0;
}

int test_2()
{
    GeoHashBits hash, fast_hash;
    GeoHashNeighbors neighbors;

    GeoHashRange lat_range, lon_range;
    lat_range.max = 20037726.37;
    lat_range.min = -20037726.37;
    lon_range.max = 20037726.37;
    lon_range.min = -20037726.37;
    double radius = 5000;
    double latitude = 9741705.20;
    double longitude = 5417390.90;
    GeoHashBits hash_min, hash_max, hash_lt, hash_gr;
    geohash_encode(lat_range, lon_range, latitude, longitude, 24, &hash);
    geohash_fast_encode(lat_range, lon_range, latitude, longitude, 24, &fast_hash);
    geohash_encode(lat_range, lon_range, latitude - radius, longitude - radius, 13, &hash_min);
    geohash_encode(lat_range, lon_range, latitude + radius, longitude + radius, 13, &hash_max);
    geohash_encode(lat_range, lon_range, latitude + radius, longitude - radius, 13, &hash_lt);
    geohash_encode(lat_range, lon_range, latitude - radius, longitude + radius, 13, &hash_gr);

    printf("## %lld\n", hash.bits);
    printf("## %lld\n", fast_hash.bits);
    geohash_get_neighbors(hash, &neighbors);
    printf("%lld\n", hash.bits);
    printf("%lld\n", neighbors.east.bits);
    printf("%lld\n", neighbors.west.bits);
    printf("%lld\n", neighbors.south.bits);
    printf("%lld\n", neighbors.north.bits);
    printf("%lld\n", neighbors.north_west.bits);
    printf("%lld\n", neighbors.north_east.bits);
    printf("%lld\n", neighbors.south_east.bits);
    printf("%lld\n", neighbors.south_west.bits);

    printf("##%lld\n", hash_min.bits);
    printf("##%lld\n", hash_max.bits);
    printf("##%lld\n", hash_lt.bits);
    printf("##%lld\n", hash_gr.bits);

    //    geohash_encode(&lat_range, &lon_range, 9741705.20, 5417390.90, 13, &hash);
    //    printf("from %lld to %lld \n", hash.bits << 2, (hash.bits+1) << 2);

    GeoHashArea area, area1;
    geohash_decode(lat_range, lon_range, hash, &area);
    geohash_fast_decode(lat_range, lon_range, hash, &area1);
    printf("%.10f %.10f\n", area.latitude.min, area.latitude.max);
    printf("%.10f %.10f\n", area.longitude.min, area.longitude.max);
    printf("%.10f %.10f\n", area1.latitude.min, area1.latitude.max);
    printf("%.10f %.10f\n", area1.longitude.min, area1.longitude.max);

    return 0;
}

void test_3()
{
    GeoHashBits hash, fast_hash;

    GeoHashRange lat_range, lon_range;
    lat_range.max = 90.0;
    lat_range.min = -90.0;
    lon_range.max = 180.0;
    lon_range.min = -180.0;

    double latitude = 12.34;
    double longitude = 56.78;

    for (int step = 1; step <= 32; step++)
    {
        geohash_encode(lat_range, lon_range, latitude, longitude, step, &hash);
        geohash_fast_encode(lat_range, lon_range, latitude, longitude, step, &fast_hash);

        printf("normal encode (%d): %llu\n", step, hash.bits);
        printf("fast encode   (%d): %llu\n", step, fast_hash.bits);
        printf("\n");
    }
}

int main()
{
    test_1();
    test_2();
    test_3();
    return 0;
}
