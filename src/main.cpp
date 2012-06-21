#include <iostream>
#include <fstream>

#include <conveyor.h>
#include <weighingbelt.h>
#include <tank.h>
#include <loadinggrid.h>

#define VERBOSE 0
#define NEW_SIMULATOR 0

template <class T>
void loadObjects(T *tab, int size, const char *file)
{
    using namespace std;

    fstream plik;
    plik.open(file, ios::in | ios::binary);
    if(plik.is_open() == true) //sprawdz czy plik zostal otwarty
    {
        for(int i=0; i < size; i++)
        {
            plik >> tab[i];
        }
        plik.close();
        //delete []wydajnosc;
    }
    else cout << "plik" << file << "nie zostal otwarty" << "\n";

#if VERBOSE
    // niepotrzebne sprawdzenie czy sie wczytalo
    for (int i=0; i < size; i++)
    {
        cout << tab[i];
    }
    cout << "\n\n";
#endif
}


int main()
{
    using namespace std;

    cout << "Starting ConvSim ...\n";

    // podstawowe parametry symulacji
    int tr = 4; //czas probkowania [s]
    //int ts = 24; //czas calej symulacji [h]
    int n = 100; // ilosc cykli
    //n=ts*3600/tr;
    double v = 2.5; //predkosc przenosnikow zakladamy dla wszystkich 2,5[m/s] ALE BZDURA!!
    double przelicznik = tr / 3600.0; //przelicza t/h na t/tr czyli w rzeczywistosci ile w tym czasie tr jest w zbiorniku [t] urobku
    double odcinek=v*tr;


    ////////////////////////////////////////////////////////////////////////////
    cout << "Loading Conveyors ...\n";
    // wczytanie Conveyorow z pliku do struktury
    int ile_Conveyorow=64;// ilosc Conveyoraow
    Conveyor *tabp = new Conveyor[ile_Conveyorow];
    loadObjects<Conveyor>(tabp, ile_Conveyorow, "przenosniki.txt");


    ////////////////////////////////////////////////////////////////////////////
    cout << "Loading Tanks ...\n";
    //stworzenie tablicy struktur Tanków i wczytanie z pliku
    int ile_Tankow=16;
    Tank *tabz=new Tank[ile_Tankow]; //wskaznik na tablice w ktorej sa zapisywane struktury Tanka
    loadObjects<Tank>(tabz, ile_Tankow, "zbiorniki.txt");

    ////////////////////////////////////////////////////////////////////////////
    cout << "Loading Weighing Belts ...\n";
    //stworzenie tablicy struktur wag i wczytanie ich z pliku
    int ile_wszystkich_wag=6;
    WeighingBelt *tabw= new WeighingBelt[ile_wszystkich_wag];
    loadObjects<WeighingBelt>(tabw, ile_wszystkich_wag, "wagi.txt");


    ////////////////////////////////////////////////////////////////////////////
    cout << "Setting Weighing Belts on Conveyors ...\n";
    // przypisanie wag do przenosnikow
    for (int i=0; i<ile_Conveyorow; ++i) // Conveyor [0] lp=1
    {
        int dodatkowa=0;
        for(int j=0; j<ile_wszystkich_wag; ++j)
        {
            if(tabw[j].lp_prz == i+1) // i+1 poniewaz numerujemy w tablicy od 0
            {
                ++dodatkowa;
            }
        }
        tabp[i].ile_wag=dodatkowa;
    }

    ////////////////////////////////////////////////////////////////////////////
    cout << "Loading Loading Grids ...\n";
    //stworzenie tablicy stuktur krat i wczytanie ich z pliku
    int ile_krat=3;
    LoadingGrid *tabk = new LoadingGrid[ile_krat];
    tabk[0].loadData("kr1.txt", n);
    tabk[1].loadData("kr2.txt", n);
    tabk[2].loadData("kr3.txt", n);
#if VERBOSE
    cout << tabk[0];
    cout << tabk[1];
    cout << tabk[2];
#endif

    ////////////////////////////////////////////////////////////////////////////
    // obliczenie ile paczek bedzie w Conveyorach i zapisanie ich do struktury
    for(int i=0; i < ile_Conveyorow; ++i)
    {
        tabp[i].setIloscPaczek(tr);
    }

#if VERBOSE
    cout << "Pokazane ile paczek:\n";
    for(int i=0; i<ile_Conveyorow; i++)
    {
        cout << tabp[i].il_paczek << "\n";
    }
#endif

    ////////////////////////////////////////////////////////////////////////////
    // stworzenie odpowiedniej ilosci paczek w Tankach- tyle co cykli n i wyzerowanie ich dla wszystkich Tankow
    for (int i=0; i < ile_Tankow; ++i)
    {
        tabz[i].setZapelnianie(n);
    }

    ////////////////////////////////////////////////////////////////////////////
    // stworzenie tablicy do odczytu wag i wyzerowanie tablic

    for(int i=0; i < ile_wszystkich_wag; ++i)
    {
        tabw[i].setOdczyt(n);

    }


#if NEW_SIMULATOR

#else

    // działanie kopalni
    ////////////////////////////////////////////////////////////////////////////
    cout<<"\n";
    cout<<"\n";
    tabp[1].urobek[3]=7;

    //n=150;
    /* DZILAJACY KAWELEK KODU przrzuca miedzy Conveyorami i po nich
n=1;
for(int k=0; k<n; k++)
{
for(int i=0; i<62; i++)// ile_Conveyorow jak dam wszystki to przy ostatnim krzak
{
int end=tabp[i].il_paczek-1; // end-1..... do obliczen
for(int j=0; j<tabp[i].il_paczek-1; j++)
{
tabp[i].urobek[end-j]=tabp[i].urobek[end-j-1];
if(j==tabp[i].il_paczek-2)
{
tabp[i].urobek[0]=tabp[i+1].urobek[tabp[i+1].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje
}



}
}
}*/
    //cout<<"\n"<<"\n"<<tabp[1].il_paczek<<"\n"<<"\n";

    //n=10;
    for(int k=0; k<n; k++)
    {
        int end;


        // PRZ 22 !!!!!!!!!!!!!
        end=tabp[12].il_paczek-1; // end-1..... do obliczen
        tabz[14].zapelnianie[k]=tabz[14].zapelnianie[k]+tabp[12].urobek[end]; // odbieranie urobku
        tabz[14].stan_Tanka=tabz[14].stan_Tanka+(tabz[14].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka R III nieskonczonosc
        for(int j=0; j<tabp[12].il_paczek-1; j++)
        {
            tabp[12].urobek[end-j]=tabp[12].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[12].il_paczek-2)
            {
                tabp[12].urobek[0]=tabp[13].urobek[tabp[13].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
            }
        }






        // PRZ 23 !!!!!!!!!!!!!!
        end=tabp[13].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        for(int j=0; j<tabp[13].il_paczek-1; j++)
        {
            tabp[13].urobek[end-j]=tabp[13].urobek[end-j-1]; // przerzucanie paczek
        }
        // oproznianie Tanka po przerzuceniu paczek sypie na Conveyor
        if (tabz[1].stan_Tanka>1)
        {
            tabp[13].urobek[0]=tabz[1].wydajnosc;
            tabz[1].stan_Tanka=tabz[1].stan_Tanka-(tabz[1].wydajnosc*przelicznik);
        }
        else tabp[13].urobek[0]=0;
        tabp[13].urobek[18]=tabk[0].nadawa[k];






        // PRZ 24 !!!!!!!!!!!!!
        end=tabp[14].il_paczek-1; // end-1..... do obliczen
        tabz[1].zapelnianie[k]=tabz[1].zapelnianie[k]+tabp[14].urobek[end]; // odbieranie urobku
        tabz[1].stan_Tanka=tabz[1].stan_Tanka+(tabz[1].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka R III nieskonczonosc
        for(int j=0; j<tabp[14].il_paczek-1; j++)
        {
            tabp[14].urobek[end-j]=tabp[12].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[14].il_paczek-2)
            {
                tabp[14].urobek[0]=tabp[15].urobek[tabp[15].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
            }
        }




        // PRZ 25 !!!!!!!!!!!!!
        end=tabp[15].il_paczek-1; // end-1..... do obliczen
        for(int j=0; j<tabp[15].il_paczek-1; j++)
        {
            tabp[15].urobek[end-j]=tabp[15].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[15].il_paczek-2)
            {
                tabp[15].urobek[0]=tabp[25].urobek[tabp[25].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
            }
        }
        tabp[15].urobek[80]=tabp[20].urobek[tabp[20].il_paczek-1]; // Conveyor W POLOWIE TEGO jeszcze jeden bedzie sypał





        // PRZ 26 !!!!!!!!!!!!!!
        end=tabp[20].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        for(int j=0; j<tabp[20].il_paczek-1; j++)
        {
            tabp[20].urobek[end-j]=tabp[20].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[20].urobek[0]=tabk[0].nadawa[k];
        tabp[20].urobek[40]=tabk[0].nadawa[k];





        // PRZ 27 !!!!!!!!!!!!!
        end=tabp[25].il_paczek-1; // end-1..... do obliczen
        for(int j=0; j<tabp[25].il_paczek-1; j++)
        {
            tabp[25].urobek[end-j]=tabp[25].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[25].il_paczek-2)
            {
                tabp[25].urobek[0]=tabp[26].urobek[tabp[26].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
            }
        }





        // PRZ 28 !!!!!!!!!!!!!
        end=tabp[26].il_paczek-1; // end-1..... do obliczen
        for(int j=0; j<tabp[26].il_paczek-1; j++)
        {
            tabp[26].urobek[end-j]=tabp[26].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[26].il_paczek-2)
            {
                tabp[26].urobek[0]=tabp[27].urobek[tabp[27].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
            }
        }





        // PRZ 29 !!!!!!!!!!!!!
        end=tabp[27].il_paczek-1; // end-1..... do obliczen
        for(int j=0; j<tabp[27].il_paczek-1; j++)
        {
            tabp[27].urobek[end-j]=tabp[27].urobek[end-j-1]; // przerzucanie paczek
        }
        // oproznianie Tanka po przerzuceniu paczek
        if (tabz[6].stan_Tanka>2)
        {
            tabp[27].urobek[0]=tabz[6].wydajnosc;
            tabz[6].stan_Tanka=tabz[6].stan_Tanka-(tabz[6].wydajnosc*przelicznik);
        }
        else tabp[27].urobek[0]=0;






        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // PRZ0 !!!!!!!!!!!!!
        end=tabp[23].il_paczek-1; // end-1..... do obliczen
        tabz[15].zapelnianie[k]=tabp[23].urobek[end]; // dlaczego dodaje paczke w momencie kiedy pojawi sie na end Conveyora a nie pozniej
        tabz[15].stan_Tanka=tabz[15].stan_Tanka+(tabz[15].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka R III nieskonczonosc
        for(int j=0; j<tabp[23].il_paczek-1; j++)
        {
            tabp[23].urobek[end-j]=tabp[23].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[23].urobek[int(tabk[0].lokalizacja/odcinek)]=tabk[0].nadawa[k]; // LoadingGrid LoadingGrid sypie dopiero jak Conveyor poprzerzuca paczki gdzie lokalizacja kraty
        tabw[0].odczyt[k]=tabp[23].urobek[int(tabw[0].lokalizacja/odcinek)]; //WeighingBelt

        // PRZ1 !!!!!!!!!!!!!!
        end=tabp[24].il_paczek-1; // end-1..... do obliczen
        tabz[15].zapelnianie[k]=tabz[15].zapelnianie[k]+tabp[24].urobek[end]; //wysyp z ostatniej paczki
        tabz[15].stan_Tanka=tabz[15].stan_Tanka+(tabz[15].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka R III nieskonczonosc
        for(int j=0; j<tabp[24].il_paczek-1; j++)
        {
            tabp[24].urobek[end-j]=tabp[24].urobek[end-j-1]; // przerzucanie paczek
        }
        // oproznianie Tanka po przerzuceniu paczek
        if (tabz[6].stan_Tanka>2)
        {
            tabp[24].urobek[0]=tabz[6].wydajnosc;
            tabz[6].stan_Tanka=tabz[6].stan_Tanka-(tabz[6].wydajnosc*przelicznik);
        }
        else tabp[24].urobek[0]=0;


        // PRZ2 !!!!!!!!!!!!!!
        end=tabp[63].il_paczek-1; // end-1..... do obliczen
        tabz[6].zapelnianie[k]=tabp[63].urobek[end]; // zrzut z Conveyora
        tabz[6].stan_Tanka=tabz[6].stan_Tanka+(tabz[6].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka
        for(int j=0; j<tabp[63].il_paczek-1; j++)
        {
            tabp[63].urobek[end-j]=tabp[63].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[63].urobek[2]=tabk[0].nadawa[k]; // LoadingGrid sypie dopiero jak Conveyor poprzerzuci paczki


        // PRZ3 !!!!!!!!!!!!!
        end=tabp[28].il_paczek-1; // end-1..... do obliczen
        tabz[6].zapelnianie[k]=tabz[6].zapelnianie[k]+tabp[28].urobek[end]; // odbieranie urobku
        tabz[6].stan_Tanka=tabz[6].stan_Tanka+(tabz[6].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka R III nieskonczonosc
        for(int j=0; j<tabp[28].il_paczek-1; j++)
        {
            tabp[28].urobek[end-j]=tabp[28].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[28].il_paczek-2)
            {
                tabp[28].urobek[0]=tabp[39].urobek[tabp[39].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
            }
        }



        // PRZ4 !!!!!!!!!!!!!!
        end=tabp[29].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        for(int j=0; j<tabp[29].il_paczek-1; j++)
        {
            tabp[29].urobek[end-j]=tabp[29].urobek[end-j-1]; // przerzucanie paczek
        }
        // oproznianie Tanka po przerzuceniu paczek sypie na Conveyor
        if (tabz[10].stan_Tanka>1)
        {
            tabp[29].urobek[0]=tabz[10].wydajnosc;
            tabz[10].stan_Tanka=tabz[10].stan_Tanka-(tabz[10].wydajnosc*przelicznik);
        }
        else tabp[29].urobek[0]=0;



        // PRZ5 !!!!!!!!!!!!!
        end=tabp[30].il_paczek-1; // end-1..... do obliczen
        tabz[10].zapelnianie[k]=tabp[30].urobek[end]; //
        tabz[10].stan_Tanka=tabz[10].stan_Tanka+(tabz[10].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka
        for(int j=0; j<tabp[30].il_paczek-1; j++)
        {
            tabp[30].urobek[end-j]=tabp[30].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[30].urobek[1]=tabk[0].nadawa[k]; // LoadingGrid LoadingGrid sypie dopiero jak Conveyor poprzerzuca paczki
        tabp[30].urobek[2]=tabk[0].nadawa[k]; // LoadingGrid LoadingGrid sypie dopiero jak Conveyor poprzerzuca paczki




        // PRZ6 !!!!!!!!!!!!!
        end=tabp[44].il_paczek-1; // end-1..... do obliczen
        tabz[10].zapelnianie[k]=tabz[10].zapelnianie[k]+tabp[44].urobek[end]; // odbieranie urobku
        tabz[10].stan_Tanka=tabz[10].stan_Tanka+(tabz[10].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka R III nieskonczonosc
        for(int j=0; j<tabp[44].il_paczek-1; j++)
        {
            tabp[44].urobek[end-j]=tabp[44].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[44].il_paczek-2)
            {
                tabp[44].urobek[0]=tabp[45].urobek[tabp[45].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
            }
        }
        tabp[44].urobek[77]=tabp[42].urobek[tabp[42].il_paczek-1]; // Conveyor W POLOWIE TEGO jeszcze jeden bedzie sypał
        tabp[44].urobek[30]=tabp[46].urobek[tabp[46].il_paczek-1]; // Conveyor W POLOWIE TEGO jeszcze jeden bedzie sypał


        // PRZ7 !!!!!!!!!!!!!!
        end=tabp[42].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        for(int j=0; j<tabp[42].il_paczek-1; j++)
        {
            tabp[42].urobek[end-j]=tabp[42].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[42].il_paczek-2)
            {
                tabp[42].urobek[0]=tabp[39].urobek[tabp[39].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
            }
        }
        tabp[42].urobek[1]=tabk[0].nadawa[k]; // LoadingGrid LoadingGrid sypie dopiero jak Conveyor poprzerzuca paczki
        tabp[42].urobek[97]=tabk[0].nadawa[k]; // LoadingGrid LoadingGrid sypie dopiero jak Conveyor poprzerzuca paczki



        // PRZ8 !!!!!!!!!!!!!!
        end=tabp[39].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        for(int j=0; j<tabp[39].il_paczek-1; j++)
        {
            tabp[39].urobek[end-j]=tabp[39].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[39].il_paczek-2)
            {
                tabp[39].urobek[0]=tabp[40].urobek[tabp[40].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
            }
        }
        tabp[39].urobek[19]=tabk[0].nadawa[k]; // LoadingGrid LoadingGrid sypie dopiero jak Conveyor poprzerzuca paczki




        // PRZ9 !!!!!!!!!!!!!!
        end=tabp[40].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        for(int j=0; j<tabp[40].il_paczek-1; j++)
        {
            tabp[40].urobek[end-j]=tabp[40].urobek[end-j-1]; // przerzucanie paczek
        }
        // oproznianie Tanka po przerzuceniu paczek sypie na Conveyor
        if (tabz[11].stan_Tanka>1)
        {
            tabp[40].urobek[0]=tabz[11].wydajnosc;
            tabz[11].stan_Tanka=tabz[11].stan_Tanka-(tabz[11].wydajnosc*przelicznik);
        }
        else tabp[40].urobek[0]=0;
        tabp[40].urobek[85]=tabk[0].nadawa[k]; // LoadingGrid LoadingGrid sypie dopiero jak Conveyor poprzerzuca paczki




        // PRZ10 !!!!!!!!!!!!!
        end=tabp[41].il_paczek-1; // end-1..... do obliczen
        tabz[11].zapelnianie[k]=tabz[11].zapelnianie[k]+tabp[41].urobek[end]; // odbieranie urobku
        tabz[11].stan_Tanka=tabz[11].stan_Tanka+(tabz[11].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka R III nieskonczonosc
        for(int j=0; j<tabp[41].il_paczek-1; j++)
        {
            tabp[41].urobek[end-j]=tabp[41].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[41].il_paczek-2)
            {
                tabp[41].urobek[0]=tabp[43].urobek[tabp[43].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
            }
        }
        tabp[41].urobek[10]=tabk[0].nadawa[k];
        tabp[41].urobek[121]=tabk[0].nadawa[k];



        // PRZ11 !!!!!!!!!!!!!
        end=tabp[43].il_paczek-1; // end-1..... do obliczen
        for(int j=0; j<tabp[23].il_paczek-1; j++)
        {
            tabp[43].urobek[end-j]=tabp[43].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[43].urobek[0]=tabk[0].nadawa[k]; // LoadingGrid LoadingGrid sypie dopiero jak Conveyor poprzerzuca paczki





        // PRZ12 !!!!!!!!!!!!!!
        end=tabp[46].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        for(int j=0; j<tabp[46].il_paczek-1; j++)
        {
            tabp[46].urobek[end-j]=tabp[46].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[46].urobek[38]=tabk[0].nadawa[k]; // LoadingGrid LoadingGrid sypie dopiero jak Conveyor poprzerzuca paczki





        // PRZ13 !!!!!!!!!!!!!!
        end=tabp[45].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        for(int j=0; j<tabp[45].il_paczek-1; j++)
        {
            tabp[45].urobek[end-j]=tabp[45].urobek[end-j-1]; // przerzucanie paczek
        }
        // oproznianie Tanka po przerzuceniu paczek sypie na Conveyor
        if (tabz[2].stan_Tanka>2)
        {
            tabp[45].urobek[0]=tabz[2].wydajnosc;
            tabz[2].stan_Tanka=tabz[2].stan_Tanka-(tabz[2].wydajnosc*przelicznik);
        }
        else tabp[29].urobek[0]=0;







        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



        // PRZ 30 !!!!!!!!!!!!!
        end=tabp[0].il_paczek-1; // end-1..... do obliczen
        tabz[14].zapelnianie[k]=tabz[14].zapelnianie[k]+tabp[0].urobek[end]; // odbieranie urobku
        tabz[14].stan_Tanka=tabz[14].stan_Tanka+(tabz[14].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka R III nieskonczonosc
        for(int j=0; j<tabp[0].il_paczek-1; j++)
        {
            tabp[0].urobek[end-j]=tabp[0].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[0].il_paczek-2)
            {
                tabp[0].urobek[0]=tabp[52].urobek[tabp[52].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
            }
        }
        tabp[0].urobek[10]=tabk[0].nadawa[k];
        tabp[0].urobek[20]=tabp[47].urobek[tabp[47].il_paczek-1]; // Conveyor W POLOWIE TEGO jeszcze jeden bedzie sypał
        tabp[0].urobek[60]=tabp[18].urobek[tabp[18].il_paczek-1]; // Conveyor W POLOWIE TEGO jeszcze jeden bedzie sypał





        // PRZ 31 !!!!!!!!!!!!!
        end=tabp[18].il_paczek-1; // end-1..... do obliczen
        for(int j=0; j<tabp[18].il_paczek-1; j++)
        {
            tabp[18].urobek[end-j]=tabp[18].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[18].urobek[0]=tabk[0].nadawa[k];
        tabp[18].urobek[20]=tabk[0].nadawa[k];
        tabp[18].urobek[70]=tabk[0].nadawa[k];
        tabp[18].urobek[90]=tabk[0].nadawa[k];





        // PRZ 32 !!!!!!!!!!!!!!
        end=tabp[52].il_paczek-1;
        for(int j=0; j<tabp[52].il_paczek-1; j++)
        {
            tabp[52].urobek[end-j]=tabp[52].urobek[end-j-1]; // przerzucanie paczek
        }
        // oproznianie Tanka przed po przerzuceniu paczek sypie na Conveyor
        if (tabz[3].stan_Tanka>1)
        {
            tabp[52].urobek[0]=tabz[3].wydajnosc;
            tabz[3].stan_Tanka=tabz[3].stan_Tanka-(tabz[3].wydajnosc*przelicznik);
        }
        else tabp[52].urobek[0]=0;
        tabp[52].urobek[20]=tabp[21].urobek[tabp[21].il_paczek-1]; // Conveyor W POLOWIE TEGO jeszcze jeden bedzie sypał




        // PRZ 33 !!!!!!!!!!!!!
        end=tabp[22].il_paczek-1; // end-1..... do obliczen
        tabz[3].zapelnianie[k]=tabz[3].zapelnianie[k]+tabp[22].urobek[end]; // odbieranie urobku
        tabz[3].stan_Tanka=tabz[3].stan_Tanka+(tabz[3].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka R III nieskonczonosc
        for(int j=0; j<tabp[22].il_paczek-1; j++)
        {
            tabp[22].urobek[end-j]=tabp[22].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[22].urobek[0]=tabk[0].nadawa[k];
        tabp[22].urobek[10]=tabk[0].nadawa[k];



        // PRZ 34 !!!!!!!!!!!!!
        end=tabp[21].il_paczek-1; // end-1..... do obliczen
        for(int j=0; j<tabp[21].il_paczek-1; j++)
        {
            tabp[21].urobek[end-j]=tabp[21].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[21].urobek[0]=tabk[0].nadawa[k];
        tabp[21].urobek[30]=tabk[0].nadawa[k];

        //U7 L1


        // PRZ 35 !!!!!!!!!!!!!
        end=tabp[47].il_paczek-1; // end-1..... do obliczen
        for(int j=0; j<tabp[47].il_paczek-1; j++)
        {
            tabp[47].urobek[end-j]=tabp[47].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[47].il_paczek-2)
            {
                tabp[47].urobek[0]=tabp[48].urobek[tabp[48].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
            }
        }




        // PRZ 36 !!!!!!!!!!!!!
        end=tabp[48].il_paczek-1; // end-1..... do obliczen
        for(int j=0; j<tabp[48].il_paczek-1; j++)
        {
            tabp[48].urobek[end-j]=tabp[48].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[48].il_paczek-2)
            {
                tabp[48].urobek[0]=tabp[49].urobek[tabp[49].il_paczek-1]
                        +tabp[50].urobek[tabp[50].il_paczek-1]
                        +tabp[51].urobek[tabp[51].il_paczek-1]; // sypia 3 Conveyori na raz
            }
        }
        tabp[48].urobek[50]=tabk[0].nadawa[k];




        // PRZ 37 !!!!!!!!!!!!!
        end=tabp[51].il_paczek-1; // end-1..... do obliczen
        for(int j=0; j<tabp[51].il_paczek-1; j++)
        {
            tabp[51].urobek[end-j]=tabp[51].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[51].urobek[0]=tabk[0].nadawa[k];




        // PRZ 38 !!!!!!!!!!!!!
        end=tabp[50].il_paczek-1; // end-1..... do obliczen
        for(int j=0; j<tabp[50].il_paczek-1; j++)
        {
            tabp[50].urobek[end-j]=tabp[50].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[50].urobek[0]=tabk[0].nadawa[k];




        // PRZ 39 !!!!!!!!!!!!!!
        end=tabp[49].il_paczek-1;
        for(int j=0; j<tabp[49].il_paczek-1; j++)
        {
            tabp[49].urobek[end-j]=tabp[49].urobek[end-j-1]; // przerzucanie paczek
        }
        // oproznianie Tanka przed po przerzuceniu paczek sypie na Conveyor
        if (tabz[0].stan_Tanka>1)
        {
            tabp[49].urobek[0]=tabz[0].wydajnosc;
            tabz[0].stan_Tanka=tabz[0].stan_Tanka-(tabz[0].wydajnosc*przelicznik);
        }
        else tabp[49].urobek[0]=0;
        tabp[49].urobek[30]=tabk[0].nadawa[k];



        // PRZ 40 !!!!!!!!!!!!!
        end=tabp[54].il_paczek-1; // end-1..... do obliczen
        tabz[0].zapelnianie[k]=tabz[0].zapelnianie[k]+tabp[54].urobek[end]; // odbieranie urobku
        tabz[0].stan_Tanka=tabz[0].stan_Tanka+(tabz[0].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka R III nieskonczonosc
        for(int j=0; j<tabp[54].il_paczek-1; j++)
        {
            tabp[54].urobek[end-j]=tabp[54].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[54].il_paczek-2)
            {
                tabp[54].urobek[0]=tabp[53].urobek[tabp[53].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
            }
        }




        // PRZ 41 !!!!!!!!!!!!!
        end=tabp[53].il_paczek-1; // end-1..... do obliczen
        for(int j=0; j<tabp[53].il_paczek-1; j++)
        {
            tabp[53].urobek[end-j]=tabp[53].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[53].il_paczek-2)
            {
                tabp[53].urobek[0]=tabp[55].urobek[tabp[55].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
            }
        }
        tabp[53].urobek[60]=tabp[56].urobek[tabp[56].il_paczek-1]; // Conveyor W POLOWIE TEGO jeszcze jeden bedzie sypał





        // PRZ 42 !!!!!!!!!!!!!
        end=tabp[56].il_paczek-1; // end-1..... do obliczen
        for(int j=0; j<tabp[56].il_paczek-1; j++)
        {
            tabp[56].urobek[end-j]=tabp[56].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[56].il_paczek-2)
            {
                tabp[56].urobek[0]=tabp[57].urobek[tabp[57].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
            }
        }
        tabp[56].urobek[20]=tabk[0].nadawa[k];
        tabp[56].urobek[60]=tabk[0].nadawa[k];




        // PRZ 43 !!!!!!!!!!!!!!
        end=tabp[57].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        for(int j=0; j<tabp[57].il_paczek-1; j++)
        {
            tabp[57].urobek[end-j]=tabp[57].urobek[end-j-1]; // przerzucanie paczek
        }
        // oproznianie Tanka przed po przerzuceniu paczek sypie na Conveyor
        if (tabz[13].stan_Tanka>1)
        {
            tabp[57].urobek[0]=tabz[13].wydajnosc;
            tabz[13].stan_Tanka=tabz[13].stan_Tanka-(tabz[13].wydajnosc*przelicznik);
        }
        else tabp[57].urobek[0]=0;
        tabp[57].urobek[10]=tabk[0].nadawa[k];




        // PRZ 44 !!!!!!!!!!!!!
        end=tabp[61].il_paczek-1; // end-1..... do obliczen
        tabz[13].zapelnianie[k]=tabz[13].zapelnianie[k]+tabp[61].urobek[end]; // odbieranie urobku
        tabz[13].stan_Tanka=tabz[13].stan_Tanka+(tabz[13].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka R III nieskonczonosc
        for(int j=0; j<tabp[61].il_paczek-1; j++)
        {
            tabp[61].urobek[end-j]=tabp[61].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[61].urobek[0]=tabk[0].nadawa[k];
        tabp[61].urobek[10]=tabk[0].nadawa[k];




        // PRZ 45 !!!!!!!!!!!!!
        end=tabp[58].il_paczek-1; // end-1..... do obliczen
        tabz[13].zapelnianie[k]=tabz[13].zapelnianie[k]+tabp[58].urobek[end]; // odbieranie urobku
        tabz[13].stan_Tanka=tabz[13].stan_Tanka+(tabz[13].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka R III nieskonczonosc
        for(int j=0; j<tabp[58].il_paczek-1; j++)
        {
            tabp[58].urobek[end-j]=tabp[58].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[58].il_paczek-2)
            {
                tabp[58].urobek[0]=tabk[0].nadawa[k]; // LoadingGrid NA POCZATKU
            }
        }
        tabp[58].urobek[5]=tabp[62].urobek[tabp[62].il_paczek-1]; // Conveyor W POLOWIE TEGO jeszcze jeden bedzie sypał
        tabp[58].urobek[50]=tabk[0].nadawa[k];
        tabp[58].urobek[150]=tabk[0].nadawa[k];



        // PRZ 46 !!!!!!!!!!!!!
        end=tabp[62].il_paczek-1; // end-1..... do obliczen
        for(int j=0; j<tabp[62].il_paczek-1; j++)
        {
            tabp[62].urobek[end-j]=tabp[62].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[62].urobek[0]=tabk[0].nadawa[k];



        // PRZ 47 !!!!!!!!!!!!!!
        end=tabp[55].il_paczek-1;
        for(int j=0; j<tabp[55].il_paczek-1; j++)
        {
            tabp[55].urobek[end-j]=tabp[55].urobek[end-j-1]; // przerzucanie paczek
        }
        // oproznianie Tanka przed po przerzuceniu paczek sypie na Conveyor
        if (tabz[2].stan_Tanka>2)
        {
            tabp[55].urobek[2]=tabz[2].wydajnosc;
            tabz[2].stan_Tanka=tabz[2].stan_Tanka-(tabz[2].wydajnosc*przelicznik);
        }
        else tabp[55].urobek[0]=0;
        tabp[55].urobek[60]=tabp[59].urobek[tabp[59].il_paczek-1]; // Conveyor W POLOWIE TEGO jeszcze jeden bedzie sypał



        // PRZ 48 !!!!!!!!!!!!!!
        end=tabp[59].il_paczek-1;
        for(int j=0; j<tabp[59].il_paczek-1; j++)
        {
            tabp[59].urobek[end-j]=tabp[59].urobek[end-j-1]; // przerzucanie paczek
        }
        // oproznianie Tanka przed po przerzuceniu paczek sypie na Conveyor
        if (tabz[12].stan_Tanka>1)
        {
            tabp[59].urobek[0]=tabz[12].wydajnosc;
            tabz[12].stan_Tanka=tabz[12].stan_Tanka-(tabz[12].wydajnosc*przelicznik);
        }
        else tabp[59].urobek[0]=0;
        tabp[59].urobek[10]=tabk[0].nadawa[k];
        tabp[59].urobek[20]=tabk[0].nadawa[k];
        tabp[59].urobek[80]=tabk[0].nadawa[k];
        tabp[59].urobek[130]=tabk[0].nadawa[k];



        // PRZ 49 !!!!!!!!!!!!!
        end=tabp[60].il_paczek-1; // end-1..... do obliczen
        tabz[12].zapelnianie[k]=tabz[12].zapelnianie[k]+tabp[60].urobek[end]; // odbieranie urobku
        tabz[12].stan_Tanka=tabz[12].stan_Tanka+(tabz[12].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka R III nieskonczonosc
        for(int j=0; j<tabp[60].il_paczek-1; j++)
        {
            tabp[60].urobek[end-j]=tabp[60].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[60].urobek[0]=tabk[0].nadawa[k];


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        //H-21 L3


        // PRZ14 !!!!!!!!!!!!!!
        end=tabp[31].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        tabz[2].zapelnianie[k]=tabz[2].zapelnianie[k]+tabp[31].urobek[end]; // odbieranie urobku
        tabz[2].stan_Tanka=tabz[2].stan_Tanka+(tabz[2].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka R III nieskonczonosc
        for(int j=0; j<tabp[31].il_paczek-1; j++)
        {
            tabp[31].urobek[end-j]=tabp[31].urobek[end-j-1]; // przerzucanie paczek
        }
        // oproznianie Tanka po przerzuceniu paczek sypie na Conveyor
        if (tabz[9].stan_Tanka>1)
        {
            tabp[31].urobek[0]=tabz[9].wydajnosc;
            tabz[9].stan_Tanka=tabz[9].stan_Tanka-(tabz[9].wydajnosc*przelicznik);
        }
        else tabp[31].urobek[0]=0;
        tabp[31].urobek[10]=tabk[0].nadawa[k];



        // PRZ15 !!!!!!!!!!!!!!
        end=tabp[32].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        tabz[9].zapelnianie[k]=tabz[9].zapelnianie[k]+tabp[32].urobek[end]; // odbieranie urobku
        tabz[9].stan_Tanka=tabz[9].stan_Tanka+(tabz[9].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka R III nieskonczonosc
        for(int j=0; j<tabp[32].il_paczek-1; j++)
        {
            tabp[32].urobek[end-j]=tabp[32].urobek[end-j-1]; // przerzucanie paczek
        }
        // oproznianie Tanka przed po przerzuceniu paczek sypie na Conveyor
        if (tabz[4].stan_Tanka>1)
        {
            tabp[32].urobek[0]=tabz[4].wydajnosc;
            tabz[4].stan_Tanka=tabz[4].stan_Tanka-(tabz[4].wydajnosc*przelicznik);
        }
        else tabp[32].urobek[0]=0;
        tabp[32].urobek[140]=tabp[34].urobek[tabp[34].il_paczek-1]; // Conveyor W POLOWIE TEGO jeszcze jeden bedzie sypał
        if (tabz[7].stan_Tanka>1) // SYPIE NA Conveyor ZE TankA W POLOWIE
        {
            tabp[32].urobek[0]=tabz[7].wydajnosc;
            tabz[7].stan_Tanka=tabz[7].stan_Tanka-(tabz[7].wydajnosc*przelicznik);
        }



        // PRZ16 !!!!!!!!!!!!!!
        end=tabp[34].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        for(int j=0; j<tabp[34].il_paczek-1; j++)
        {
            tabp[34].urobek[end-j]=tabp[34].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[34].urobek[10]=tabk[0].nadawa[k];




        // PRZ17 !!!!!!!!!!!!!!
        end=tabp[35].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        tabz[7].zapelnianie[k]=tabz[7].zapelnianie[k]+tabp[35].urobek[end]; // odbieranie urobku
        tabz[7].stan_Tanka=tabz[7].stan_Tanka+(tabz[7].zapelnianie[k]*przelicznik); // monitoring stanu zapelnienia Tanka R III nieskonczonosc
        for(int j=0; j<tabp[35].il_paczek-1; j++)
        {
            tabp[35].urobek[end-j]=tabp[35].urobek[end-j-1]; // przerzucanie paczek
        }
        // oproznianie Tanka przed po przerzuceniu paczek sypie na Conveyor
        if (tabz[8].stan_Tanka>1)
        {
            tabp[35].urobek[0]=tabz[8].wydajnosc;
            tabz[8].stan_Tanka=tabz[8].stan_Tanka-(tabz[8].wydajnosc*przelicznik);
        }
        else tabp[35].urobek[0]=0;
        tabp[35].urobek[40]=tabk[0].nadawa[k];
        tabp[35].urobek[80]=tabk[0].nadawa[k];



        // PRZ18 !!!!!!!!!!!!!!
        end=tabp[36].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        tabz[8].zapelnianie[k]=tabz[8].zapelnianie[k]+tabp[36].urobek[end]; // odbieranie urobku
        tabz[8].stan_Tanka=tabz[8].stan_Tanka+(tabz[8].zapelnianie[k]*przelicznik);
        for(int j=0; j<tabp[36].il_paczek-1; j++)
        {
            tabp[36].urobek[end-j]=tabp[42].urobek[end-j-1]; // przerzucanie paczek
            if(j==tabp[36].il_paczek-2)
            {
                tabp[36].urobek[0]=tabp[38].urobek[tabp[38].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
            }
        }
        tabp[36].urobek[1]=tabk[0].nadawa[k];
        tabp[36].urobek[30]=tabk[0].nadawa[k];





        // PRZ19 !!!!!!!!!!!!!!
        end=tabp[38].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        for(int j=0; j<tabp[38].il_paczek-1; j++)
        {
            tabp[38].urobek[end-j]=tabp[38].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[38].urobek[1]=tabk[0].nadawa[k];







        // PRZ20 !!!!!!!!!!!!!!
        end=tabp[37].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        tabz[4].zapelnianie[k]=tabz[4].zapelnianie[k]+tabp[37].urobek[end]; // odbieranie urobku
        tabz[4].stan_Tanka=tabz[4].stan_Tanka+(tabz[4].zapelnianie[k]*przelicznik);
        for(int j=0; j<tabp[37].il_paczek-1; j++)
        {
            tabp[37].urobek[end-j]=tabp[37].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[37].urobek[1]=tabk[0].nadawa[k];
        tabp[37].urobek[30]=tabk[0].nadawa[k];






        // PRZ21 !!!!!!!!!!!!!!
        end=tabp[33].il_paczek-1; // end-1..... do obliczen // paczka wczesniej z end zabrana do PRZ 3
        tabz[4].zapelnianie[k]=tabz[4].zapelnianie[k]+tabp[33].urobek[end]; // odbieranie urobku
        tabz[4].stan_Tanka=tabz[4].stan_Tanka+(tabz[4].zapelnianie[k]*przelicznik);
        for(int j=0; j<tabp[33].il_paczek-1; j++)
        {
            tabp[33].urobek[end-j]=tabp[33].urobek[end-j-1]; // przerzucanie paczek
        }
        tabp[33].urobek[1]=tabk[0].nadawa[k];
        tabp[33].urobek[30]=tabk[0].nadawa[k];








    }
#endif


    //if(j==tabp[0].il_paczek-2)
    //{
    // tabp[0].urobek[0]=tabp[1].urobek[tabp[1].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje
    // }




    cout<<"\n\n";
    cout<<"\n"<<"stan zapelnienia Tanka 6 "<<tabz[6].stan_Tanka<<"\n";
    cout<<"\n"<<"stan zapelnienia Tanka 10 "<<tabz[10].stan_Tanka<<"\n";
    cout<<"\n"<<"stan zapelnienia Tanka 11 "<<tabz[11].stan_Tanka<<"\n";
    cout<<"\n"<<"stan zapelnienia R III"<<tabz[15].stan_Tanka<<"\n";


    cout<<"\n";
    tabp[23].printIloscPaczek();
    tabp[24].printIloscPaczek();
    tabp[63].printIloscPaczek();
    tabp[28].printIloscPaczek();
    tabp[29].printIloscPaczek();
    tabp[30].printIloscPaczek();
    tabp[44].printIloscPaczek();
    tabp[42].printIloscPaczek();
    tabp[39].printIloscPaczek();
    tabp[40].printIloscPaczek();
    tabp[41].printIloscPaczek();
    tabp[43].printIloscPaczek();
    tabp[46].printIloscPaczek();
    tabp[45].printIloscPaczek();

    tabp[44].printUrobek();
    tabp[42].printUrobek();
    tabp[46].printUrobek();
    tabp[41].printUrobek();

    cout<<"\n"<<"Tank 11 stan Tanka "<<tabz[11].stan_Tanka;
    cout<<"\n"<<"Tank 11 zapelnianie ";
    for(int j=0; j<n; j++)
    {
        cout<<tabz[11].zapelnianie[j]<<" ";
    }
    cout<<"\n";

    tabp[40].printUrobek();
    tabp[39].printUrobek();
    tabp[24].printUrobek();
    tabp[28].printUrobek();
    tabp[23].printUrobek();


    cout<<"\n"<<"R III"<<"\n";

    for(int j=0; j<n; j++) // sprawdzam zapelnianie R III
    {
        //cout<<tabz[15].zapelnianie[j]<<" ";
    }
    cout<<"\n";


    cout<<"\n"<<"zbiornik"<<"\n";

    for(int j=0; j<n; j++) // sprawdzam Tank
    {
        // cout<<tabz[6].zapelnianie[j]<<" ";
    }
    cout<<"\n";


    cout<<"\n"<<"waga z przenosnika 23 na 5 paczce"<<"\n";

    for(int j=0; j<n; j++) // sprawdzam Tank
    {
        //cout<<tabw[0].odczyt[j]<<" ";
    }
    cout<<"\n\n";

    float urob=800; // t/h
    float wynik = urob * przelicznik;
    cout << wynik;

    cout << "\n";

    return 0;
}
