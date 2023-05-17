
#define	    N      3    /* a puffer hossza */

/* a cirkularis puffer */

struct puffer {
    int input;          //elso szabad byte-ra mutat
    int output;         //elsonek beirt kimeneti byte-ra mutat
    char data[N];       // N karaktert tartalmaz
};

/* a cirkularis pufferbe iro es olvaso fuggvenyek */

char get_puff ( struct puffer * p ); 
void put_puff ( struct puffer * p, char c ); 


