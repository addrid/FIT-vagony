#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
TVAN
je struktura, která reprezentuje jeden nákladní vagón. Má složky m_Next, která odkazuje na pøípadný další vagón v øadì (musí být NULL pro poslední vagón), m_To, 
která udává jméno cílového seøaïovacího nádraží a m_Cargo, která udává náklad vagónu (do nákladu jako pøepravce nezasahujete, jeho obsah Vás nemusí zajímat).

TTRAIN
je struktura popisující vlak (lokálku i vlak jedoucí po koridoru). Má složky m_Start, která je odkazem na první vagón (NULL, pokud jede samotná lokomotiva bez vagónù) 
a m_Count udávající poèet vagónù v tomto vlaku (musí korespondovat s délkou spojového seznamu vagónù).

borrowVan
je funkce pro zapùjèení vagónu. V naší implementaci funkce vytvoøí novou strukturu TVAN a vyplní její složky. Složky m_Next a m_Cargo nastaví na NULL a složku m_To 
vyplní podle parametru.

returnVans
je funkce, která slouží k vrácení zapùjèených vagónù. V naší reprezentaci funkce uvolní pamì alokovanou pro reprezentaci vagónù. Dále upraví složky ve struktuøe vlaku 
(nastaví NULL/0). Složkou m_Cargo se zabývat nemusíte, bude nastavena na NULL (zákazníci si náklad pøed odevzdáním vagónu vyloží).

connectVan
je funkce, která zapojí vagón do vlaku. Funkce má 3 parametry: vlak, kam má být vagón zapojen, vagón, který má být zapojen a pøíznak, zda má být vagón zapojen na zaèátek vlaku
 (parametr toStart nenulový) nebo na konec vlaku (parametr toStart nulový). Funkce odpovídajícím zpùsobem upraví spojový seznam a reprezentaci vlaku.

route
je funkce, která simuluje vlastní seøaïovací nádraží. Parametrem je jméno seøaïovacího nádraží (podle toho vybírá vagóny z vlaku), vlak train, který pøijel po koridoru 
a místní lokálka localTrain. Funkce zajistí správné pøepojení vagónù mezi vlaky, tj. odpovídající úpravy struktur, které vlaky a vagóny reprezentují.
*/
typedef struct TVan
 {
   struct TVan   * m_Next;
   char          * m_To;
   void          * m_Cargo;
 } TVAN;

typedef struct TTrain
 {
   TVAN          * m_Start;
   int             m_Count;
 } TTRAIN;
#endif /* __PROGTEST__ */

TVAN * borrowVan ( const char * to )
 { 
    int delka = strlen(to);
    
    TVAN * vagon = ( TVAN * ) malloc ( sizeof ( *vagon ) );
    vagon->m_Next = NULL;
    vagon->m_Cargo = NULL;
    vagon->m_To = (char*)malloc(delka + 1); 
    strncpy(vagon->m_To, to, delka + 1); 
    //vagon->m_To = (char*)to;
    printf("Vagon byl vytvoren\n");
    
    return vagon;
 }
void   returnVans ( TTRAIN * train )
 {
     TVAN * dalsi = train->m_Start;    
     TVAN * temp = NULL;
     while(dalsi) 
     {
         temp=dalsi->m_Next;
         free(dalsi);
         dalsi=temp;
     }
 } 
void   connectVan ( TTRAIN * train, TVAN * van, int toStart )
 {
     TVAN * dalsi = train->m_Start;
     if(toStart != 0)
     {  /* Vagon chci zapojit na zacatek */
        dalsi = ( TVAN * ) malloc ( sizeof ( *van ) );
        dalsi -> m_Next = train->m_Start;
        train -> m_Start = dalsi;
        
        printf("A pripojen na zacatek\n");
     }
     else
     {  /* Vagon chci zapojit na konec */
     
        /*if ( !*start )
        {
             *start = insertStart( val, *start ); /*vyuzijeme funkci, kterou uz mame
             return;
        }*/
 
        while ( dalsi -> m_Next )
        {
             dalsi = dalsi -> m_Next; /*Dokud muzu, prelezu na dalsi polozku*/
        }
        
        dalsi -> m_Next = ( TVAN * ) malloc ( sizeof ( *van ) );
        dalsi -> m_Next -> m_Next = NULL;
        //
        dalsi -> m_Next = van;
        //
        dalsi -> m_Next -> m_Cargo = NULL;
        dalsi -> m_Next -> m_To = NULL;
        printf("A pripojen na konec\n");
     }
     
 }                                                             
void   route ( const char * stationName, TTRAIN * train, TTRAIN * localTrain )
 {
   TVAN * van = localTrain->m_Start;
   int i;    
   while(van->m_Next)//for(i=0; i<localTrain->m_Count; i++)
   {
       if(van->m_To != stationName)
       {
           van = ( TVAN * ) malloc ( sizeof ( *van ) );
           //van = localTrain->m_Start;
           connectVan ( train, van, 0 );
           van = van -> m_Next;
           printf("prepojeno z lokalky");
       }
   }
   van = train->m_Start;
   
   printf("1");
   while(van->m_Next != NULL)
   {printf("2");
      if(van->m_To == stationName)
       {
           //van = train->m_Start;
           connectVan ( localTrain, van, 0 );
           van = van -> m_Next;
           printf("prepojeno z vlaku");
       }               
   }
   
   
 }                                                             
#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
 {
   /* tests */
   TTRAIN a = { NULL, 0 }, b = { NULL, 0 }, c = { NULL, 0 };
   char   tmpStr[100];

   connectVan ( &a, borrowVan ( "Praha" ), 1 );
   connectVan ( &a, borrowVan ( "Brno" ), 1 );
   connectVan ( &a, borrowVan ( "Ostrava" ), 0 );
   connectVan ( &a, borrowVan ( "Brno" ), 0 );
   /* a: train with 4 vans: [Brno] -> [Praha] -> [Ostrava] -> [Brno] */
   connectVan ( &b, borrowVan ( "Praha" ), 1 );
   connectVan ( &b, borrowVan ( "Plzen" ), 1 );
   connectVan ( &b, borrowVan ( "Ostrava" ), 0 );
   connectVan ( &b, borrowVan ( "Praha" ), 0 );
   /* b: train with 4 vans: [Plzen] -> [Praha] -> [Ostrava] -> [Praha] */
   route ( "Brno", &a, &b  );
   /* a: train with 6 vans: [Praha] -> [Ostrava] -> [Plzen] -> [Praha] -> [Ostrava] -> [Praha] */
   /* b: train with 2 vans: [Brno] -> [Brno] */
   returnVans ( &a );
   returnVans ( &b );

   
   
   system("PAUSE");
   return 0;
 }
#endif  /* __PROGTEST__ */
