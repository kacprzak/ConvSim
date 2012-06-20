#include <iostream>
#include <fstream>

#include <conveyor.h>
#include <weighingbelt.h>
#include <tank.h>
#include <loadinggrid.h>

int main()
{
    using namespace std;

    cout << "ConvSim\n";


    // podstawowe parametry symulacji
    float tr; //czas probkowania [s]
    tr=4;
    int ts; //czas calej symulacji [h]
    ts=24;
    int n; // ilosc cykli
    //n=ts*3600/tr;
    n=100;
    float v; //predkosc przenosnikow zakladamy dla wszystkich 2,5[m/s]
    v=2.5;
    float przelicznik=tr/3600; //przelicza t/h na t/tr czyli w rzeczywistosci ile w tym czasie tr jest w Tanku [t] urobku
    float odcinek=v*tr;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // wczytanie kraty
    float *wydajnosc=new float[n]; //zapisujemy tutaj wydajnosc kraty w zaleznosci od ilosci cykli
    fstream plik;
    plik.open ("kr.txt", ios::in | ios::binary); // nie ma juz kr.txt
    if( plik.is_open() == true ) //sprawdz czy plik zostal otwarty
    {
        string pomin;
        for(int i=0; i<n; i++)
        {
            plik>>pomin;
            plik>>pomin;
            plik>>wydajnosc[i];
            //cout<<wydajnosc[i]<<"\n";
        }
        plik.close();
        //delete []wydajnosc;
    }
    else cout<<"plik kr.txt nie zostal otwarty"<<"\n";
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //zapis do pliku (działa)

    plik.open("zapis.txt", ios::out);
    if(plik.is_open() == true) //sprawdz czy plik zostal otwarty
    {
        cout<<"plik zostal otwarty zapisze do pliku"<<"\n";
        for(int i=0; i<n; i++)
        {
            plik<<wydajnosc[i]<<"\n";
        }
        plik.close();
        //delete []wydajnosc;
    }
    else cout<<"plik nie zostal otwarty"<<"\n";

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // wczytanie Conveyorow z pliku do struktury

    int ile_Conveyorow=64;// ilosc Conveyoraow
    Conveyor *tabp= new Conveyor[ile_Conveyorow]; // wskaznik na tablice w ktorej sa zapisywane struktury Conveyora
    plik.open ("przenosniki.txt", ios::in | ios::binary);
    if( plik.is_open() == true) //sprawdz czy plik zostal otwarty
    {
        for(int i=0; i<ile_Conveyorow; i++)
        {
            plik >> tabp[i];
        }
        plik.close();
        //delete []wydajnosc;
    }
    else cout<<"plik przenosniki.txt nie zostal otwarty"<<"\n";

    // niepotrzebne sprawdzenie czy sie wczytalo
    for (int i=0; i<ile_Conveyorow; i++)
    {
        cout << tabp[i];
    }
    cout<<"\n\n";

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //stworzenie tablicy struktur Tanków i wczytanie z pliku
    int ile_Tankow=16;
    Tank *tabz=new Tank[ile_Tankow]; //wskaznik na tablice w ktorej sa zapisywane struktury Tanka
    plik.open("zbiorniki.txt",ios::in | ios::binary);
    if(plik.is_open()==true)
    {
        for(int i=0; i<ile_Tankow; i++)
        {
            plik >> tabz[i];
        }
        plik.close();
    }
    else cout<<"nie mozna otworzyc pliku zbiorniki.txt do odczytu";

    for(int i=0; i<ile_Tankow; i++)
    {
        cout << tabz[i];
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //stworzenie tablicy struktur wag i wczytanie ich z pliku
    int ile_wszystkich_wag=6;
    WeighingBelt *tabw= new WeighingBelt[ile_wszystkich_wag];
    plik.open("wagi.txt", ios::in | ios::binary);
    if(plik.is_open()==true)
    {
        for(int i=0; i<ile_wszystkich_wag; i++)
        {
            plik >> tabw[i];
        }
        // sprawdzenie wczytania wag
        for(int i=0; i<ile_wszystkich_wag; i++)
        {
            cout<<"waga na przenisnik: " << tabw[i];
        }
        plik.close();
    }
    else cout<<"plik wagi.txt nie zostal otwarty do odczytu"<<"\n";

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // przypisanie wag do Conveyorow
    for (int i=0; i<ile_Conveyorow; i++) // Conveyor [0] lp=1
    {
        int dodatkowa=0;
        for(int j=0; j<ile_wszystkich_wag; j++)
        {
            if(tabw[j].lp_prz==i+1) // i+1 poniewaz numerujemy w tablicy od 0
            {
                dodatkowa++;
            }
        }
        tabp[i].ile_wag=dodatkowa;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //stworzenie tablicy stuktur krat i wczytanie ich z pliku
    int ile_krat=3;
    LoadingGrid *tabk= new LoadingGrid[ile_krat];
    for(int i=0; i<ile_krat; i++)
    {
        string pomin;// pomocny podczas pomijania kolumn przy wczytywaniu krat
        switch (i)
        {
        case 0:
            tabk[i].nadawa= new double[n];
            plik.open("kr1.txt",ios::in | ios::binary);
            if(plik.is_open()==true)
            {
                plik>>tabk[i].lp_prz;
                plik>>tabk[i].lokalizacja;
                tabk[i].gdzie=tabk[i].lokalizacja/odcinek;
                for(int j=0; j<n; j++)
                {
                    plik>>pomin;
                    plik>>pomin;
                    plik>>tabk[i].nadawa[j];
                }
                plik.close();
            }
            else cout<<"nie mozna otworzyc pliku kr1.txt do odczytu";

            // sprawdzenie wczytania kraty kr1
            cout<<"krata na przeniosniku nr: "<<tabk[i].lp_prz<<" zlokalizowana na metrze"<<tabk[i].lokalizacja<<"\n";
            for( int j=0; j<n; j++)
            {
                //cout<<tabk[i].nadawa[j]<<" ";
            }
            cout<<"\n\n";
            break;
        case 1:
            tabk[i].nadawa= new double[n];
            plik.open("kr2.txt",ios::in | ios::binary);
            if(plik.is_open()==true)
            {
                plik>>tabk[i].lp_prz;
                plik>>tabk[i].lokalizacja;
                for(int j=0; j<n; j++)
                {
                    plik>>pomin;
                    plik>>pomin;
                    plik>>tabk[i].nadawa[j];
                }
                plik.close();
            }
            else cout<<"nie mozna otworzyc pliku kr2.txt do odczytu";

            // sprawdzenie wczytania kraty kr1
            cout<<"waga na przenosniku nr: "<<tabk[i].lp_prz<<" zlokalizowana na metrze"<<tabk[i].lokalizacja<<"\n";
            for( int j=0; j<n; j++)
            {
                //cout<<tabk[i].nadawa[j]<<" ";
            }
            cout<<"\n\n";
            break;
        case 2:
            tabk[i].nadawa= new double[n];
            plik.open("kr3.txt",ios::in | ios::binary); // mozemy używac tego samego pliku plik6 po zamknieciu ale trzeba znowu otworzyc
            if(plik.is_open()==true)
            {
                plik>>tabk[i].lp_prz;
                plik>>tabk[i].lokalizacja;
                for(int j=0; j<n; j++)
                {
                    plik>>pomin;
                    plik>>pomin;
                    plik>>tabk[i].nadawa[j];
                }
                plik.close();
            }
            else cout<<"nie mozna otworzyc pliku kr3.txt do odczytu";

            // sprawdzenie wczytania kraty kr1
            cout<<"waga na przenosniku nr: "<<tabk[i].lp_prz<<" zlokalizowana na metrze"<<tabk[i].lokalizacja<<"\n";
            for( int j=0; j<n; j++)
            {
                //cout<<tabk[i].nadawa[j]<<" ";
            }
            cout<<"\n\n";
            break;
        }

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // obliczenie ile paczek bedzie w Conveyorach i zapisanie ich do struktury
    float t_przejazdu;
    for(int i=0; i<ile_Conveyorow; i++)
    {
        t_przejazdu=tabp[i].L/v;
        float ile_pf=t_przejazdu/tr;
        int il_p=t_przejazdu/tr;
        tabp[i].il_paczek=t_przejazdu/tr;

        if(((t_przejazdu/tr)-il_p)>0) // tworzenie paczki jezeli na koncu nie wychodzi rowno
        {
            tabp[i].il_paczek++;
        }
    }
    cout<<"\n pokazane ile paczek"<<"\n";
    for(int i=0; i<ile_Conveyorow; i++)
    {
        cout<<tabp[i].il_paczek<<"\n";
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////????????????????????????????????
    // stworzenie tablic (paczek) w Conveyorach i wyzerowanie ich
    for(int i=0; i<ile_Conveyorow; i++)
    {
        int pomocnicza=tabp[i].il_paczek; // zastanów się nad int float
        tabp[i].urobek=new float[pomocnicza];
        for(int j=0; j<tabp[i].il_paczek; j++)
        {
            tabp[i].urobek[j]=0;
        }
    }
    // wyswietlanie wyzerowanych Conveyorow

    for(int i=0; i<ile_Conveyorow; i++)
    {
        //cout<<"\n"<<"Conveyor: "<<tabp[i].nazwa<<"\t";
        for(int j=0; j<tabp[i].il_paczek; j++)
        {
            //cout<<tabp[i].urobek[j]<<" ";
        }
        //cout<<"\n";
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // stworzenie odpowiedniej ilosci paczek w Tankach- tyle co cykli n i wyzerowanie ich dla wszystkich Tankow

    for (int i=0; i<ile_Tankow; i++)
    {
        tabz[i].zapelnianie=new double[n];
        for(int j=0; j<n; j++)
        {
            tabz[i].zapelnianie[j]=0;
        }
    }
    // wyswietlanie wyzerowanych Tankow
    for (int i=0; i<ile_Tankow; i++)
    {
        cout<<"\n"<<"zbiornik: "<<tabz[i].nazwa<<"\t";
        for(int j=0; j<n; j++)
        {
            //cout<<tabz[i].zapelnianie[j]<<" ";
        }
        cout<<"\n";
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // ustawiam zapelnienie Tankow na 0,
    for (int i=0; i<ile_Tankow; i++)
    {
        tabz[i].stan_Tanka=0;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // stworzenie tablicy do odczytu wag i wyzerowanie tablic

    for(int i=0; i<ile_wszystkich_wag; i++)
    {
        tabw[i].odczyt=new double[n];
    }
    // zerowanie
    for(int i=0; i<ile_wszystkich_wag; i++)
    {
        for(int j=0; j<n; j++)
        {
            tabw[i].odczyt[j]=0;
        }
    }
    // sprawdzenie wyswietlenie
    for(int i=0; i<ile_wszystkich_wag; i++)
    {
        cout <<" waga na przenosniku nr: "<<tabw[i].lp_prz<<"\t";
        for(int j=0; j<n; j++)
        {
            //cout<<tabw[i].odczyt[j]<<" ";
        }
        cout<<"\n";
    }

    // działanie kopalni
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // petla for n-cykli dzialanie calej kopalni

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
        tabp[23].urobek[tabk[0].gdzie]=tabk[0].nadawa[k]; // LoadingGrid LoadingGrid sypie dopiero jak Conveyor poprzerzuca paczki gdzie lokalizacja kraty
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



    //if(j==tabp[0].il_paczek-2)
    //{
    // tabp[0].urobek[0]=tabp[1].urobek[tabp[1].il_paczek-1]; // tylko gdy Conveyor nie jest pierwszy w ciagu/ nikt na niego nei laduje
    // }




    cout<<"\n\n";
    cout<<"\n"<<"stan zapelnienia Tanka 6 "<<tabz[6].stan_Tanka<<"\n";
    cout<<"\n"<<"stan zapelnienia Tanka 10 "<<tabz[10].stan_Tanka<<"\n";
    cout<<"\n"<<"stan zapelnienia Tanka 11 "<<tabz[11].stan_Tanka<<"\n";
    cout<<"\n"<<"stan zapelnienia R III"<<tabz[15].stan_Tanka<<"\n";


    cout<<"\n"<<"ilos paczek Conveyora "<<tabp[23].lp_prz<<" tyle paczek -> "<<tabp[23].il_paczek<<"\n";
    cout<<"ilos paczek Conveyora "<<tabp[24].lp_prz<<" tyle paczek -> "<<tabp[24].il_paczek<<"\n";
    cout<<"ilos paczek Conveyora "<<tabp[63].lp_prz<<" tyle paczek -> "<<tabp[63].il_paczek<<"\n";
    cout<<"ilos paczek Conveyora "<<tabp[28].lp_prz<<" tyle paczek -> "<<tabp[28].il_paczek<<"\n";
    cout<<"ilos paczek Conveyora "<<tabp[29].lp_prz<<" tyle paczek -> "<<tabp[29].il_paczek<<"\n";
    cout<<"ilos paczek Conveyora "<<tabp[30].lp_prz<<" tyle paczek -> "<<tabp[30].il_paczek<<"\n";
    cout<<"ilos paczek Conveyora "<<tabp[44].lp_prz<<" tyle paczek -> "<<tabp[44].il_paczek<<"\n";
    cout<<"ilos paczek Conveyora "<<tabp[42].lp_prz<<" tyle paczek -> "<<tabp[42].il_paczek<<"\n";
    cout<<"ilos paczek Conveyora "<<tabp[39].lp_prz<<" tyle paczek -> "<<tabp[39].il_paczek<<"\n";
    cout<<"ilos paczek Conveyora "<<tabp[40].lp_prz<<" tyle paczek -> "<<tabp[40].il_paczek<<"\n";
    cout<<"ilos paczek Conveyora "<<tabp[41].lp_prz<<" tyle paczek -> "<<tabp[41].il_paczek<<"\n";
    cout<<"ilos paczek Conveyora "<<tabp[43].lp_prz<<" tyle paczek -> "<<tabp[43].il_paczek<<"\n";
    cout<<"ilos paczek Conveyora "<<tabp[46].lp_prz<<" tyle paczek -> "<<tabp[46].il_paczek<<"\n";
    cout<<"ilos paczek Conveyora "<<tabp[45].lp_prz<<" tyle paczek -> "<<tabp[45].il_paczek<<"\n";

    cout<<"\n"<<"z Conveyora 44 (WeighingBelt) ";
    for(int j=0; j<tabp[44].il_paczek; j++)
    {
        cout<<tabp[44].urobek[j]<<" ";
    }
    cout<<"\n";

    cout<<"\n"<<"z Conveyora 42 (WeighingBelt) ";
    for(int j=0; j<tabp[42].il_paczek; j++)
    {
        cout<<tabp[42].urobek[j]<<" ";
    }
    cout<<"\n";

    cout<<"\n"<<"z Conveyora 46 (WeighingBelt) ";
    for(int j=0; j<tabp[46].il_paczek; j++)
    {
        cout<<tabp[46].urobek[j]<<" ";
    }
    cout<<"\n";

    cout<<"\n"<<"z Conveyora 41 (WeighingBelt) ";
    for(int j=0; j<tabp[41].il_paczek; j++)
    {
        cout<<tabp[41].urobek[j]<<" ";
    }
    cout<<"\n";

    cout<<"\n"<<"Tank 11 stan Tanka "<<tabz[11].stan_Tanka;
    cout<<"\n"<<"Tank 11 zapelnianie ";
    for(int j=0; j<n; j++)
    {
        cout<<tabz[11].zapelnianie[j]<<" ";
    }
    cout<<"\n";

    cout<<"\n"<<"z Conveyora 40 (WeighingBelt) ";
    for(int j=0; j<tabp[40].il_paczek; j++)
    {
        cout<<tabp[40].urobek[j]<<" ";
    }
    cout<<"\n";


    cout<<"\n"<<"z Conveyora 39 (WeighingBelt) ";
    for(int j=0; j<tabp[39].il_paczek; j++)
    {
        cout<<tabp[39].urobek[j]<<" ";
    }
    cout<<"\n";


    cout<<"\n"<<"z Conveyora 24 (WeighingBelt) ";
    for(int j=0; j<tabp[24].il_paczek; j++)
    {
        cout<<tabp[24].urobek[j]<<" ";
    }
    cout<<"\n";


    cout<<"\n"<<"z Conveyora 28 (WeighingBelt) ";
    for(int j=0; j<tabp[28].il_paczek; j++)
    {
        cout<<tabp[28].urobek[j]<<" ";
    }
    cout<<"\n";

    cout<<"\n"<<"z Conveyora 23 (WeighingBelt) ";
    for(int j=0; j<tabp[23].il_paczek; j++)
    {
        cout<<tabp[23].urobek[j]<<" ";
    }
    cout<<"\n";


    cout<<"\n"<<"R III"<<"\n";

    for(int j=0; j<n; j++) // sprawdzam zapelnianie R III
    {
        //cout<<tabz[15].zapelnianie[j]<<" ";
    }
    cout<<"\n";


    cout<<"\n"<<"Tank"<<"\n";

    for(int j=0; j<n; j++) // sprawdzam Tank
    {
        // cout<<tabz[6].zapelnianie[j]<<" ";
    }
    cout<<"\n";


    cout<<"\n"<<"WeighingBelt z Conveyora 23 na 5 paczce"<<"\n";

    for(int j=0; j<n; j++) // sprawdzam Tank
    {
        //cout<<tabw[0].odczyt[j]<<" ";
    }
    cout<<"\n";



    cout<<"\n"<<"\n"<<"\n"<<"\n";

    float urob=800; // t/h

    float wynik;
    wynik=urob*przelicznik;
    cout<<wynik;


    cout << "\n";

    return 0;
}
