#include "myinclude.h"

#define MAXTIME 255
int main()
{
    struct timespec ts; //time_t ts.tv_sec a szekundumokat,
                        //long ts.tv_nsec a nanoszekundumokat tartalmazza
    long MILL = 1000000;
    long mili;
    long tmili, tmili_prev=0;
    int count = 1;
    time_t tsunix ;

    while ( count++ < 11 )
    {
        time(&tsunix);        //Unix időbélyeg
        // if (clock_gettime(CLOCK_REALTIME, &ts) < 0){  //ha ezt hagyjuk, a ts.tv_sec valós időbélyeg (rendszer idő, ugyanaz mint tsunix) 
        if (clock_gettime(CLOCK_MONOTONIC, &ts) < 0){    //ha ezt, akkor egy meghatározatlan időtől kezdődő
                                                         //de jó intervallum meghatározására
                                                         //akkor is működik, ha közben megváltoztatják a rendszer időt 
            syserr("clock");
        }
        mili = (int)floor(ts.tv_nsec / MILL);          //milliszekundumok egészben

        tmili = ts.tv_sec * 1000 + mili;               //sec * 1000 + milli

        printf("%d. unix_ts: %ld,tv_sec:%ld tv_nsec: %ld időbélyeg ms-ban %ld\n",count, tsunix,ts.tv_sec, ts.tv_nsec, tmili);

        if (tmili_prev){
            printf("két ciklus közt eltelt idő ~ %ld ms\n",tmili-tmili_prev);
        }
        tmili_prev=tmili;
        usleep(100000);
    }

    return 0;
}
