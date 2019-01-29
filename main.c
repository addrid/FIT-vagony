#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
TVAN
je struktura, kter� reprezentuje jeden n�kladn� vag�n. M� slo�ky m_Next, kter� odkazuje na p��padn� dal�� vag�n v �ad� (mus� b�t NULL pro posledn� vag�n), m_To, 
kter� ud�v� jm�no c�lov�ho se�a�ovac�ho n�dra�� a m_Cargo, kter� ud�v� n�klad vag�nu (do n�kladu jako p�epravce nezasahujete, jeho obsah V�s nemus� zaj�mat).

TTRAIN
je struktura popisuj�c� vlak (lok�lku i vlak jedouc� po koridoru). M� slo�ky m_Start, kter� je odkazem na prvn� vag�n (NULL, pokud jede samotn� lokomotiva bez vag�n�) 
a m_Count ud�vaj�c� po�et vag�n� v tomto vlaku (mus� korespondovat s d�lkou spojov�ho seznamu vag�n�).

borrowVan
je funkce pro zap�j�en� vag�nu. V na�� implementaci funkce vytvo�� novou strukturu TVAN a vypln� jej� slo�ky. Slo�ky m_Next a m_Cargo nastav� na NULL a slo�ku m_To 
vypln� podle parametru.

returnVans
je funkce, kter� slou�� k vr�cen� zap�j�en�ch vag�n�. V na�� reprezentaci funkce uvoln� pam� alokovanou pro reprezentaci vag�n�. D�le uprav� slo�ky ve struktu�e vlaku 
(nastav� NULL/0). Slo�kou m_Cargo se zab�vat nemus�te, bude nastavena na NULL (z�kazn�ci si n�klad p�ed odevzd�n�m vag�nu vylo��).

connectVan
je funkce, kter� zapoj� vag�n do vlaku. Funkce m� 3 parametry: vlak, kam m� b�t vag�n zapojen, vag�n, kter� m� b�t zapojen a p��znak, zda m� b�t vag�n zapojen na za��tek vlaku
 (parametr toStart nenulov�) nebo na konec vlaku (parametr toStart nulov�). Funkce odpov�daj�c�m zp�sobem uprav� spojov� seznam a reprezentaci vlaku.

route
je funkce, kter� simuluje vlastn� se�a�ovac� n�dra��. Parametrem je jm�no se�a�ovac�ho n�dra�� (podle toho vyb�r� vag�ny z vlaku), vlak train, kter� p�ijel po koridoru 
a m�stn� lok�lka localTrain. Funkce zajist� spr�vn� p�epojen� vag�n� mezi vlaky, tj. odpov�daj�c� �pravy struktur, kter� vlaky a vag�ny reprezentuj�.
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
