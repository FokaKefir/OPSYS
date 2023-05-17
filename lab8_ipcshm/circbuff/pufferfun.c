/*
 * egy cirkularis puffert iro es olvaso fuggvenyek
 */

#include "pufferfun.h"

// a fuggvenyek nem ellenorzik, hogy van-e elem a pufferben 

// pufferbe ir 
char get_puff ( struct puffer * p )
{
    char c;               
    
    c = *(p->data + p->output); //kiveszi a karaktert a data tomb
                                //outputtal indexelt elemebol

    p->output++; 
                                //megnoveli a kimeneti mutatot   
    p->output = ( p->output ) % N ;
                                //ha eleri a puffer szelet, visszaviszi az elejere

    return c;
}

// a pufferbol olvas 
void put_puff ( struct puffer * p, char c )
{
    *(p->data + p->input) = c;  //beir a pufferbe az input altal
                                //indexelt helyre
    p->input++;
                                //megnoveli az input mutatot
    p->input = ( p->input ) % N ;
                                //ha eleri a puffer szelet, visszaviszi az elejere

}
