// odczyt z pliku.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <string>
#include <cstdlib> // do funkcji atoi konwersja string na int

#include <fstream> // operacje na plikach
using namespace std;

struct przenosnik
{
	// wczytywanie z pliku
	int lp_prz; //wykorzystywany w lokalizacji
	string oddzial;
	string nazwa;
	float L;        //dlugosc
	float B;        //szerokosc tasmy [mm]

	//		int ile_prz_syp;   // 0 pierwszy w ciagu, 1 standard, 2,3,4... sypia na okreslonym metrze przenosnika
	//		int *lp_prz_syp;   // zapisana liczba porzadkowa przenosnikow ktore sypia na niego odczytana z pliku
	//		// wsk ponizej robie z poziomu programu
	//		int *nrst_prz_syp;  // lp struktur przenosnikow sypiacych na niego
			// dalej wczytane z pliku
	//		int ile_krat_plik;                                        // jako sprawdzenie narazie nei wpisje do pliku
	//		int ile_wag_plik;                                        // jako sprawdzenie
	//		int czy_zb_odb_plik;     // 0 nie ma , 1 jest           // jako sprawdzenie
	//		int czy_zb_pop_plik;   // 0 nie ma , 1 jest            // jako sprawdzenie
			//int nr_porz_zb_odb;
			//int nr_porz_zb_pop; 

	// ponizej wyliczenia programu 
	int il_paczek; // ile paczek ma przenosnik
	float *urobek;

	//		int ile_krat;                                        // jako sprawdzenie
	int ile_wag;                                        // jako sprawdzenie
	//		int czy_zb_odb;     // 0 nie ma , 1 jest           // jako sprawdzenie
	//		int czy_zb_pop;   // 0 nie ma , 1 jest            // jako sprawdzenie

	//		int *nrst_krat; // numer porz¹dkowy krat z tablicy struktur zapisany w tablicy np. 3 elementowej (ile_krat)
	//		int *nrst_wag;   // numer porz¹dkowy wag z tablicy struktur zapisany w tablicy np. 3 elementowej (ile_wag)
	//		int nrst_zb_odb; // numer porz¹dkowy zbiornika z tablicy struktur 
	//		int nrst_zb_pop; // numer porz¹dkowy zbiornika z tablicy struktur -> najprawdopodobniej wystarczy tylko to nrst_zb_odb niekoniecznie chyba?
	
	
	//float urobek[]; // w tej tablicy bedzie przerzucany urobek 
	//float Qsr;      //wydajnosc [t/h]
	//float Qmax;
	//float V;        //predkosc tasmy
};

struct zbiornik
{
	int numer_zbiornika; //wykorzystywany w lokalizacji numer przenosnika za lub przed zbiornikiem ??????porzadkowy???????
	string odzial;
	string nazwa;
	float poj;		//pojemnosc zbiornika [t]
	float wydajnosc;
	float stan_zbiornika;
	
	//		int ile_prz_pop;
	//		int *lp_prz_pop; //numery wczytane z pliku zapisane do tablicy liczba porz¹dkowa
	//		int ile_prz_odb;
	//		int *lp_prz_odb; // numery wczytane z pliku zapisane do tablicy liczba porz¹dkowa

	// ponizej robi program

	//		int *nrst_prz_pop; // numer porz¹dkowy przensnika poprzedzajacego z tablicy struktur zapisany w tablicy np. 3 elementowej (ile_prz_odbiera)
	//int *nrst_prz_odb; // numer porz¹dkowy przenosnika odbierajacego z tablicy struktur zapisany w tablicy np. 3 elementowej (ile_prz_sypie)
	                    // odbierajacych chyba nie beda potrzebne tylko poprzedzajacych 
	float *zapelnianie;
};

struct waga
{
	int lp_prz;  // na ktorym instalujemy wage
	int lokalizacja; //na ktorym metrze przenosnika znajduje sie waga
	int gdzie;
	float *odczyt;
};

struct krata
{
	int lp_prz;
	int lokalizacja; // na ktorym metrze znajduje sie krata
	int gdzie;
	float *nadawa;
};

//int _tmain(int argc, _TCHAR* argv[])
int main()
{
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
	float przelicznik=tr/3600; //przelicza t/h na t/tr czyli w rzeczywistosci ile w tym czasie tr jest w zbiorniku [t] urobku
	float odcinek=v*tr;

////////////////////////////////////////////////////////////////////////////////////////////////
	// wczytanie kraty
	float *wydajnosc=new float[n]; //zapisujemy tutaj wydajnosc kraty w zaleznosci od ilosci cykli
	fstream plik;
	plik.open ("kr.txt", ios::in | ios::binary);  // nie ma juz kr.txt
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
	 else  cout<<"plik kr.txt nie zostal otwarty"<<"\n";
///////////////////////////////////////////////////////////////////////////////////////////////
	 //zapis do pliku (dzia³a)
	 	
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
	 else  cout<<"plik nie zostal otwarty"<<"\n";

////////////////////////////////////////////////////////////////////////////////////////////////
// wczytanie przenosnikow z pliku do struktury

	int ile_przenosnikow=64;// ilosc przenosnikaow
	przenosnik *tabp= new przenosnik[ile_przenosnikow]; // wskaznik na tablice w ktorej sa zapisywane struktury przenosnika
	plik.open ("przenosniki.txt", ios::in | ios::binary);
	if( plik.is_open() == true) //sprawdz czy plik zostal otwarty
    {
		for(int i=0; i<ile_przenosnikow; i++)
		{
			plik>>tabp[i].lp_prz;
			plik>>tabp[i].oddzial;
			plik>>tabp[i].nazwa;
			plik>>tabp[i].L;
			plik>>tabp[i].B;
		}
        plik.close();
		//delete []wydajnosc;
	}
	 else  cout<<"plik przenosniki.txt nie zostal otwarty"<<"\n";
// niepotrzebne sprawdzenie czy sie wczytalo
	for (int i=0; i<ile_przenosnikow; i++)
	{
		cout<<tabp[i].lp_prz<<"\t\t";
		cout<<tabp[i].oddzial<<"\t\t";
		cout<<tabp[i].nazwa<<"\t\t";
		cout<<tabp[i].L<<"\t\t";
		cout<<tabp[i].B<<"\n";
	}
		cout<<"\n\n";

///////////////////////////////////////////////////////////////////////////////////////////////
	//stworzenie tablicy struktur zbiorników i wczytanie z pliku
	int ile_zbiornikow=16;
	zbiornik *tabz=new zbiornik[ile_zbiornikow];  //wskaznik na tablice w ktorej sa zapisywane struktury zbiornika
	plik.open("zbiorniki.txt",ios::in | ios::binary);
	if(plik.is_open()==true)
	{
		for(int i=0; i<ile_zbiornikow; i++)
		{
			plik>>tabz[i].numer_zbiornika;
			plik>>tabz[i].odzial;
			plik>>tabz[i].nazwa;
			plik>>tabz[i].poj;
			plik>>tabz[i].wydajnosc;
		}
		plik.close();
	}
	else cout<<"nie mozna otworzyc pliku zbiorniki.txt do odczytu";

	for(int i=0; i<ile_zbiornikow; i++)
	{
		//cout<<tabz[i].numer_zbiornika<<"\t";
		//cout<<tabz[i].odzial<<"\t";
		//cout<<tabz[i].nazwa<<"\t";
		//cout<<tabz[i].poj<<"\n";
		//cout<<tabz[i].wydajnosc<<"\n";
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//stworzenie tablicy struktur wag i wczytanie ich z pliku
	int ile_wszystkich_wag=6;
	waga *tabw= new waga[ile_wszystkich_wag];
	plik.open("wagi.txt", ios::in | ios::binary);
	if(plik.is_open()==true)
	{
		for(int i=0; i<ile_wszystkich_wag; i++)
		{
			plik>>tabw[i].lp_prz;
			plik>>tabw[i].lokalizacja;
			tabw[i].gdzie=tabw[i].lokalizacja/odcinek;
		}
		// sprawdzenie wczytania wag
		for(int i=0; i<ile_wszystkich_wag; i++)
		{
			cout<<"waga na przenosnik: "<<tabw[i].lp_prz<<"\t";
			cout<<tabw[i].lokalizacja<<"\n";
		}
		plik.close();
	}
	else  cout<<"plik wagi.txt nie zostal otwarty do odczytu"<<"\n";

//////////////////////////////////////////////////////////////////////////////////////////////////////
	// przypisanie wag do przenosnikow
	for (int i=0; i<ile_przenosnikow; i++)  // przenosnik [0] lp=1
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
		//cout<<"ile wag na przenosnikach"<<tabp[i].ile_wag<<"\n";
		//		tabp[i].nrst_wag=new int[tabp[i].ile_wag]; //alokacja pamieci na numery struktur wag 
													//wystepujacych na danym przenosniku 

		///////////////////////////////////////////////////////////////////////////////// na dole zle popraw
		/*for (int k=0; k<tabp[i].ile_wag; k++)
		{
			int pomoc=0;
			for(int j=0; j<ile_wszystkich_wag; j++)        
			{
				if(tabw[j].lp_prz==i+1) // i+1 poniewaz numerujemy w tablicy od 0
					{
						if(pomoc==0)
						{
							tabp[i].nrst_wag[k]=j;  // do tablicy z numerami struktur wag przypisujemy numery wagi
							pomoc++;
						}
					}
				
			}        
			// pod nrst_wag[] mamy zapisane numery struktur wag na tym przenosniku, 
			//tyle numerów ile wag na przenosniku	
		}*/
	}

	// sprawdzenie przypisywania wag do przenosnikow
	/*
	for (int i=0; i<ile_przenosnikow; i++)
	{
		cout<<"L.P. przenosnika "<<tabp[i].lp_prz<<" ile wag na przenosniku "<<tabp[i].ile_wag<<"\n";
		for (int j=0; j<tabp[i].ile_wag; j++)
			{
				cout<<" kolejne wagi: "<<"[]"<<j<<"\t"<<tabp[i].nrst_wag[j]<<"\n"; 
			}
	}
	*/


		
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//stworzenie tablicy stuktur krat i wczytanie ich z pliku
	int ile_krat=3;
	krata *tabk= new krata[ile_krat];
	for(int i=0; i<ile_krat; i++)
	{
		string pomin;// pomocny podczas pomijania kolumn przy wczytywaniu krat
			switch (i)
			{
				case 0: 
					tabk[i].nadawa= new float[n];
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
					else  cout<<"nie mozna otworzyc pliku kr1.txt do odczytu";

					// sprawdzenie wczytania kraty kr1
					cout<<"krata na przenosniku nr: "<<tabk[i].lp_prz<<" zlokalizowana na metrze"<<tabk[i].lokalizacja<<"\n";
					for( int j=0; j<n; j++)
					{
						//cout<<tabk[i].nadawa[j]<<" ";
					}
					cout<<"\n\n";
					break;
				case 1:
					tabk[i].nadawa= new float[n];
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
					else  cout<<"nie mozna otworzyc pliku kr2.txt do odczytu";

					// sprawdzenie wczytania kraty kr1
					cout<<"krata na przenosniku nr: "<<tabk[i].lp_prz<<" zlokalizowana na metrze"<<tabk[i].lokalizacja<<"\n";
					for( int j=0; j<n; j++)
					{
						//cout<<tabk[i].nadawa[j]<<" ";
					}
					cout<<"\n\n";
						break;
				case 2:                                                  
					tabk[i].nadawa= new float[n];
					plik.open("kr3.txt",ios::in | ios::binary);  // mozemy u¿ywac tego samego pliku plik6 po zamknieciu ale trzeba znowu otworzyc
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
					else  cout<<"nie mozna otworzyc pliku kr3.txt do odczytu";

					// sprawdzenie wczytania kraty kr1
					cout<<"krata na przenosniku nr: "<<tabk[i].lp_prz<<" zlokalizowana na metrze"<<tabk[i].lokalizacja<<"\n";
					for( int j=0; j<n; j++)
					{
						//cout<<tabk[i].nadawa[j]<<" ";
					}
					cout<<"\n\n";
					break;
			}

	}

////////////////////////////////////////////////////////////////////////////////////////////////////
	// obliczenie ile paczek bedzie w przenosnikach i zapisanie ich do struktury
	float t_przejazdu;
	for(int i=0; i<ile_przenosnikow; i++)
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
		for(int i=0; i<ile_przenosnikow; i++)
	{
		cout<<tabp[i].il_paczek<<"\n";
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////????????????????????????????????
		// stworzenie tablic (paczek) w przenosnikach i wyzerowanie ich
		for(int i=0; i<ile_przenosnikow; i++)
		{
			int pomocnicza=tabp[i].il_paczek;     // zastanów siê nad int float
			tabp[i].urobek=new float[pomocnicza];
			for(int j=0; j<tabp[i].il_paczek; j++)
			{
				tabp[i].urobek[j]=0;
			}
		}
		// wyswietlanie wyzerowanych przenosnikow
		
		for(int i=0; i<ile_przenosnikow; i++)
		{
			//cout<<"\n"<<"przenosnik: "<<tabp[i].nazwa<<"\t";
			for(int j=0; j<tabp[i].il_paczek; j++)
			{
				//cout<<tabp[i].urobek[j]<<" ";
			}
			//cout<<"\n";
		}
/////////////////////////////////////////////////////////////////////////////////////////////////////
		// stworzenie odpowiedniej ilosci paczek w zbiornikach- tyle co cykli n i wyzerowanie ich dla wszystkich zbiornikow

		for (int i=0; i<ile_zbiornikow; i++)
		{
			tabz[i].zapelnianie=new float[n];
			for(int j=0; j<n; j++)
			{
				tabz[i].zapelnianie[j]=0;
			}
		}
		// wyswietlanie wyzerowanych zbiornikow
		for (int i=0; i<ile_zbiornikow; i++)
		{
			cout<<"\n"<<"zbiornik: "<<tabz[i].nazwa<<"\t";
			for(int j=0; j<n; j++)
			{
				//cout<<tabz[i].zapelnianie[j]<<" ";
			}
			cout<<"\n";
		}

/////////////////////////////////////////////////////////////////////////////////////////////////////// 
		// ustawiam zapelnienie zbiornikow na 0,
		for (int i=0; i<ile_zbiornikow; i++)
		{
			tabz[i].stan_zbiornika=0;
		}
///////////////////////////////////////////////////////////////////////////////////////////////////////
		// stworzenie tablicy do odczytu wag i wyzerowanie tablic

		for(int i=0; i<ile_wszystkich_wag; i++)
		{
			tabw[i].odczyt=new float[n];
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
			cout <<" waga na przenosniku"<<tabw[i].lp_prz<<"\t";
			for(int j=0; j<n; j++)
			{
				//cout<<tabw[i].odczyt[j]<<" ";
			}
			cout<<"\n";
		}

		// dzia³anie kopalni
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
		/* DZILAJACY KAWELEK KODU przrzuca miedzy przenosnikami i po nich
		n=1;
		for(int k=0; k<n; k++)
		{
			for(int i=0; i<62; i++)// ile_przenosnikow jak dam wszystki to przy ostatnim krzak
			{
				int end=tabp[i].il_paczek-1;  // end-1..... do obliczen
				for(int j=0; j<tabp[i].il_paczek-1; j++)
				{
					tabp[i].urobek[end-j]=tabp[i].urobek[end-j-1];
					
					if(j==tabp[i].il_paczek-2)
					{
						tabp[i].urobek[0]=tabp[i+1].urobek[tabp[i+1].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje
					}



				}
			}
		}*/
		//cout<<"\n"<<"\n"<<tabp[1].il_paczek<<"\n"<<"\n";
			

		//n=10;
		for(int k=0; k<n; k++)
		{
			int end;


			//  PRZ 22 !!!!!!!!!!!!!
			end=tabp[12].il_paczek-1;  // end-1..... do obliczen										
			tabz[14].zapelnianie[k]=tabz[14].zapelnianie[k]+tabp[12].urobek[end];  // odbieranie urobku
			tabz[14].stan_zbiornika=tabz[14].stan_zbiornika+(tabz[14].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika R III nieskonczonosc
			for(int j=0; j<tabp[12].il_paczek-1; j++)
			{
				tabp[12].urobek[end-j]=tabp[12].urobek[end-j-1];  // przerzucanie paczek
					if(j==tabp[12].il_paczek-2)
					{
						tabp[12].urobek[0]=tabp[13].urobek[tabp[13].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
					}
			}






			// PRZ 23	!!!!!!!!!!!!!!
			end=tabp[13].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3									 
			for(int j=0; j<tabp[13].il_paczek-1; j++)
			{
				tabp[13].urobek[end-j]=tabp[13].urobek[end-j-1];  // przerzucanie paczek
			}
			// oproznianie zbiornika po przerzuceniu paczek sypie na przenosnik
			if (tabz[1].stan_zbiornika>1)
			{
				tabp[13].urobek[0]=tabz[1].wydajnosc;
				tabz[1].stan_zbiornika=tabz[1].stan_zbiornika-(tabz[1].wydajnosc*przelicznik);
			}
			else tabp[13].urobek[0]=0;
			tabp[13].urobek[18]=tabk[0].nadawa[k];






			//  PRZ 24 !!!!!!!!!!!!!
			end=tabp[14].il_paczek-1;  // end-1..... do obliczen										
			tabz[1].zapelnianie[k]=tabz[1].zapelnianie[k]+tabp[14].urobek[end];  // odbieranie urobku
			tabz[1].stan_zbiornika=tabz[1].stan_zbiornika+(tabz[1].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika R III nieskonczonosc
			for(int j=0; j<tabp[14].il_paczek-1; j++)
			{
				tabp[14].urobek[end-j]=tabp[12].urobek[end-j-1];  // przerzucanie paczek
				if(j==tabp[14].il_paczek-2)
				{
					tabp[14].urobek[0]=tabp[15].urobek[tabp[15].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
				}
			}




			//  PRZ 25 !!!!!!!!!!!!!
			end=tabp[15].il_paczek-1;  // end-1..... do obliczen										
			for(int j=0; j<tabp[15].il_paczek-1; j++)
			{
				tabp[15].urobek[end-j]=tabp[15].urobek[end-j-1];  // przerzucanie paczek
					if(j==tabp[15].il_paczek-2)
					{
						tabp[15].urobek[0]=tabp[25].urobek[tabp[25].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
					}
			}
			tabp[15].urobek[80]=tabp[20].urobek[tabp[20].il_paczek-1];  // PRZENOSNIK W POLOWIE TEGO jeszcze jeden bedzie sypa³





			// PRZ 26	!!!!!!!!!!!!!!
			end=tabp[20].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3									 
			for(int j=0; j<tabp[20].il_paczek-1; j++)
			{
				tabp[20].urobek[end-j]=tabp[20].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[20].urobek[0]=tabk[0].nadawa[k];
			tabp[20].urobek[40]=tabk[0].nadawa[k];





			//  PRZ 27 !!!!!!!!!!!!!
			end=tabp[25].il_paczek-1;  // end-1..... do obliczen										
			for(int j=0; j<tabp[25].il_paczek-1; j++)
			{
				tabp[25].urobek[end-j]=tabp[25].urobek[end-j-1];  // przerzucanie paczek
					if(j==tabp[25].il_paczek-2)
					{
						tabp[25].urobek[0]=tabp[26].urobek[tabp[26].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
					}
			}





			//  PRZ 28 !!!!!!!!!!!!!
			end=tabp[26].il_paczek-1;  // end-1..... do obliczen										
			for(int j=0; j<tabp[26].il_paczek-1; j++)
			{
				tabp[26].urobek[end-j]=tabp[26].urobek[end-j-1];  // przerzucanie paczek
					if(j==tabp[26].il_paczek-2)
					{
						tabp[26].urobek[0]=tabp[27].urobek[tabp[27].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
					}
			}





			//  PRZ 29 !!!!!!!!!!!!!
			end=tabp[27].il_paczek-1;  // end-1..... do obliczen										
			for(int j=0; j<tabp[27].il_paczek-1; j++)
			{
				tabp[27].urobek[end-j]=tabp[27].urobek[end-j-1];  // przerzucanie paczek
			}
			// oproznianie zbiornika po przerzuceniu paczek
			if (tabz[6].stan_zbiornika>2)
			{
				tabp[27].urobek[0]=tabz[6].wydajnosc;
				tabz[6].stan_zbiornika=tabz[6].stan_zbiornika-(tabz[6].wydajnosc*przelicznik);
			}
			else tabp[27].urobek[0]=0;






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//  PRZ0 !!!!!!!!!!!!!
			end=tabp[23].il_paczek-1;  // end-1..... do obliczen										
			tabz[15].zapelnianie[k]=tabp[23].urobek[end];  // dlaczego dodaje paczke w momencie kiedy pojawi sie na end przenosnika a nie pozniej
			tabz[15].stan_zbiornika=tabz[15].stan_zbiornika+(tabz[15].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika R III nieskonczonosc
			for(int j=0; j<tabp[23].il_paczek-1; j++)
			{
				tabp[23].urobek[end-j]=tabp[23].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[23].urobek[tabk[0].gdzie]=tabk[0].nadawa[k];  // krata krata sypie dopiero jak przenosnik poprzerzuca paczki gdzie lokalizacja kraty
			tabw[0].odczyt[k]=tabp[23].urobek[tabw[0].gdzie];  //waga

			// PRZ1	!!!!!!!!!!!!!!
			end=tabp[24].il_paczek-1;  // end-1..... do obliczen											 
			tabz[15].zapelnianie[k]=tabz[15].zapelnianie[k]+tabp[24].urobek[end];  //wysyp z ostatniej paczki
			tabz[15].stan_zbiornika=tabz[15].stan_zbiornika+(tabz[15].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika R III nieskonczonosc
			for(int j=0; j<tabp[24].il_paczek-1; j++)
			{
				tabp[24].urobek[end-j]=tabp[24].urobek[end-j-1];  // przerzucanie paczek
			}
			// oproznianie zbiornika po przerzuceniu paczek
			if (tabz[6].stan_zbiornika>2)
			{
				tabp[24].urobek[0]=tabz[6].wydajnosc;
				tabz[6].stan_zbiornika=tabz[6].stan_zbiornika-(tabz[6].wydajnosc*przelicznik);
			}
			else tabp[24].urobek[0]=0;
				

			// PRZ2	!!!!!!!!!!!!!!				
			end=tabp[63].il_paczek-1;  // end-1..... do obliczen										
			tabz[6].zapelnianie[k]=tabp[63].urobek[end];  // zrzut z przenosnika
			tabz[6].stan_zbiornika=tabz[6].stan_zbiornika+(tabz[6].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika
			for(int j=0; j<tabp[63].il_paczek-1; j++)
			{
				tabp[63].urobek[end-j]=tabp[63].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[63].urobek[2]=tabk[0].nadawa[k];  // krata sypie dopiero jak przenosnik poprzerzuci paczki


			//  PRZ3 !!!!!!!!!!!!!
			end=tabp[28].il_paczek-1;  // end-1..... do obliczen										
			tabz[6].zapelnianie[k]=tabz[6].zapelnianie[k]+tabp[28].urobek[end];  // odbieranie urobku
			tabz[6].stan_zbiornika=tabz[6].stan_zbiornika+(tabz[6].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika R III nieskonczonosc
			for(int j=0; j<tabp[28].il_paczek-1; j++)
			{
				tabp[28].urobek[end-j]=tabp[28].urobek[end-j-1];  // przerzucanie paczek
					if(j==tabp[28].il_paczek-2)
					{
						tabp[28].urobek[0]=tabp[39].urobek[tabp[39].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
					}
			}



			// PRZ4	!!!!!!!!!!!!!!
			end=tabp[29].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3									 
			for(int j=0; j<tabp[29].il_paczek-1; j++)
			{
				tabp[29].urobek[end-j]=tabp[29].urobek[end-j-1];  // przerzucanie paczek
			}
			// oproznianie zbiornika po przerzuceniu paczek sypie na przenosnik
			if (tabz[10].stan_zbiornika>1)
			{
				tabp[29].urobek[0]=tabz[10].wydajnosc;
				tabz[10].stan_zbiornika=tabz[10].stan_zbiornika-(tabz[10].wydajnosc*przelicznik);
			}
			else tabp[29].urobek[0]=0;



			//  PRZ5 !!!!!!!!!!!!!
			end=tabp[30].il_paczek-1;  // end-1..... do obliczen										
			tabz[10].zapelnianie[k]=tabp[30].urobek[end];  // 
			tabz[10].stan_zbiornika=tabz[10].stan_zbiornika+(tabz[10].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika 
			for(int j=0; j<tabp[30].il_paczek-1; j++)
			{
				tabp[30].urobek[end-j]=tabp[30].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[30].urobek[1]=tabk[0].nadawa[k];  // krata krata sypie dopiero jak przenosnik poprzerzuca paczki
			tabp[30].urobek[2]=tabk[0].nadawa[k];  // krata krata sypie dopiero jak przenosnik poprzerzuca paczki




			//  PRZ6 !!!!!!!!!!!!!
			end=tabp[44].il_paczek-1;  // end-1..... do obliczen										
			tabz[10].zapelnianie[k]=tabz[10].zapelnianie[k]+tabp[44].urobek[end];  // odbieranie urobku
			tabz[10].stan_zbiornika=tabz[10].stan_zbiornika+(tabz[10].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika R III nieskonczonosc
			for(int j=0; j<tabp[44].il_paczek-1; j++)
			{
				tabp[44].urobek[end-j]=tabp[44].urobek[end-j-1];  // przerzucanie paczek
				if(j==tabp[44].il_paczek-2)
				{
					tabp[44].urobek[0]=tabp[45].urobek[tabp[45].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
				}
			}
			tabp[44].urobek[77]=tabp[42].urobek[tabp[42].il_paczek-1];  // PRZENOSNIK W POLOWIE TEGO jeszcze jeden bedzie sypa³
			tabp[44].urobek[30]=tabp[46].urobek[tabp[46].il_paczek-1];  // PRZENOSNIK W POLOWIE TEGO jeszcze jeden bedzie sypa³


			// PRZ7	!!!!!!!!!!!!!!
			end=tabp[42].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3									 
			for(int j=0; j<tabp[42].il_paczek-1; j++)
			{
				tabp[42].urobek[end-j]=tabp[42].urobek[end-j-1];  // przerzucanie paczek
				if(j==tabp[42].il_paczek-2)
				{
					tabp[42].urobek[0]=tabp[39].urobek[tabp[39].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
				}
			}
			tabp[42].urobek[1]=tabk[0].nadawa[k];  // krata krata sypie dopiero jak przenosnik poprzerzuca paczki
			tabp[42].urobek[97]=tabk[0].nadawa[k];  // krata krata sypie dopiero jak przenosnik poprzerzuca paczki
			


			// PRZ8	!!!!!!!!!!!!!!
			end=tabp[39].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3									 
			for(int j=0; j<tabp[39].il_paczek-1; j++)
			{
				tabp[39].urobek[end-j]=tabp[39].urobek[end-j-1];  // przerzucanie paczek
				if(j==tabp[39].il_paczek-2)
				{
					tabp[39].urobek[0]=tabp[40].urobek[tabp[40].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
				}
			}
			tabp[39].urobek[19]=tabk[0].nadawa[k];  // krata krata sypie dopiero jak przenosnik poprzerzuca paczki




			// PRZ9	!!!!!!!!!!!!!!
			end=tabp[40].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3									 
			for(int j=0; j<tabp[40].il_paczek-1; j++)
			{
				tabp[40].urobek[end-j]=tabp[40].urobek[end-j-1];  // przerzucanie paczek
			}
			// oproznianie zbiornika po przerzuceniu paczek sypie na przenosnik
			if (tabz[11].stan_zbiornika>1)
			{
				tabp[40].urobek[0]=tabz[11].wydajnosc;
				tabz[11].stan_zbiornika=tabz[11].stan_zbiornika-(tabz[11].wydajnosc*przelicznik);
			}
			else tabp[40].urobek[0]=0;
			tabp[40].urobek[85]=tabk[0].nadawa[k];  // krata krata sypie dopiero jak przenosnik poprzerzuca paczki




			//  PRZ10 !!!!!!!!!!!!!
			end=tabp[41].il_paczek-1;  // end-1..... do obliczen										
			tabz[11].zapelnianie[k]=tabz[11].zapelnianie[k]+tabp[41].urobek[end];  // odbieranie urobku
			tabz[11].stan_zbiornika=tabz[11].stan_zbiornika+(tabz[11].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika R III nieskonczonosc
			for(int j=0; j<tabp[41].il_paczek-1; j++)
			{
				tabp[41].urobek[end-j]=tabp[41].urobek[end-j-1];  // przerzucanie paczek
				if(j==tabp[41].il_paczek-2)
				{
					tabp[41].urobek[0]=tabp[43].urobek[tabp[43].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
				}
			}
			tabp[41].urobek[10]=tabk[0].nadawa[k]; 
			tabp[41].urobek[121]=tabk[0].nadawa[k]; 



			//  PRZ11 !!!!!!!!!!!!!
			end=tabp[43].il_paczek-1;  // end-1..... do obliczen										
			for(int j=0; j<tabp[23].il_paczek-1; j++)
			{
				tabp[43].urobek[end-j]=tabp[43].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[43].urobek[0]=tabk[0].nadawa[k];  // krata krata sypie dopiero jak przenosnik poprzerzuca paczki





			// PRZ12	!!!!!!!!!!!!!!
			end=tabp[46].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3									 
			for(int j=0; j<tabp[46].il_paczek-1; j++)
			{
				tabp[46].urobek[end-j]=tabp[46].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[46].urobek[38]=tabk[0].nadawa[k];  // krata krata sypie dopiero jak przenosnik poprzerzuca paczki





			// PRZ13	!!!!!!!!!!!!!!
			end=tabp[45].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3									 
			for(int j=0; j<tabp[45].il_paczek-1; j++)
			{
				tabp[45].urobek[end-j]=tabp[45].urobek[end-j-1];  // przerzucanie paczek
			}
			// oproznianie zbiornika po przerzuceniu paczek sypie na przenosnik
			if (tabz[2].stan_zbiornika>2)
			{
				tabp[45].urobek[0]=tabz[2].wydajnosc;
				tabz[2].stan_zbiornika=tabz[2].stan_zbiornika-(tabz[2].wydajnosc*przelicznik);
			}
			else tabp[29].urobek[0]=0;




			


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



			//  PRZ 30 !!!!!!!!!!!!!
			end=tabp[0].il_paczek-1;  // end-1..... do obliczen										
			tabz[14].zapelnianie[k]=tabz[14].zapelnianie[k]+tabp[0].urobek[end];  // odbieranie urobku
			tabz[14].stan_zbiornika=tabz[14].stan_zbiornika+(tabz[14].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika R III nieskonczonosc
			for(int j=0; j<tabp[0].il_paczek-1; j++)
			{
				tabp[0].urobek[end-j]=tabp[0].urobek[end-j-1];  // przerzucanie paczek
					if(j==tabp[0].il_paczek-2)
					{
						tabp[0].urobek[0]=tabp[52].urobek[tabp[52].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
					}
			}
			tabp[0].urobek[10]=tabk[0].nadawa[k];
			tabp[0].urobek[20]=tabp[47].urobek[tabp[47].il_paczek-1];  // PRZENOSNIK W POLOWIE TEGO jeszcze jeden bedzie sypa³
			tabp[0].urobek[60]=tabp[18].urobek[tabp[18].il_paczek-1];  // PRZENOSNIK W POLOWIE TEGO jeszcze jeden bedzie sypa³





			//  PRZ 31 !!!!!!!!!!!!!
			end=tabp[18].il_paczek-1;  // end-1..... do obliczen										
			for(int j=0; j<tabp[18].il_paczek-1; j++)
			{
				tabp[18].urobek[end-j]=tabp[18].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[18].urobek[0]=tabk[0].nadawa[k];
			tabp[18].urobek[20]=tabk[0].nadawa[k];
			tabp[18].urobek[70]=tabk[0].nadawa[k];
			tabp[18].urobek[90]=tabk[0].nadawa[k];





			// PRZ 32	!!!!!!!!!!!!!!
			end=tabp[52].il_paczek-1;	
			for(int j=0; j<tabp[52].il_paczek-1; j++)
			{
				tabp[52].urobek[end-j]=tabp[52].urobek[end-j-1];  // przerzucanie paczek
			}
			// oproznianie zbiornika przed po przerzuceniu paczek sypie na przenosnik
			if (tabz[3].stan_zbiornika>1)
			{
				tabp[52].urobek[0]=tabz[3].wydajnosc;
				tabz[3].stan_zbiornika=tabz[3].stan_zbiornika-(tabz[3].wydajnosc*przelicznik);
			}
			else tabp[52].urobek[0]=0;
			tabp[52].urobek[20]=tabp[21].urobek[tabp[21].il_paczek-1];  // PRZENOSNIK W POLOWIE TEGO jeszcze jeden bedzie sypa³




			//  PRZ 33 !!!!!!!!!!!!!
			end=tabp[22].il_paczek-1;  // end-1..... do obliczen										
			tabz[3].zapelnianie[k]=tabz[3].zapelnianie[k]+tabp[22].urobek[end];  // odbieranie urobku
			tabz[3].stan_zbiornika=tabz[3].stan_zbiornika+(tabz[3].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika R III nieskonczonosc
			for(int j=0; j<tabp[22].il_paczek-1; j++)
			{
				tabp[22].urobek[end-j]=tabp[22].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[22].urobek[0]=tabk[0].nadawa[k];
			tabp[22].urobek[10]=tabk[0].nadawa[k];



			//  PRZ 34 !!!!!!!!!!!!!
			end=tabp[21].il_paczek-1;  // end-1..... do obliczen										
			for(int j=0; j<tabp[21].il_paczek-1; j++)
			{
				tabp[21].urobek[end-j]=tabp[21].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[21].urobek[0]=tabk[0].nadawa[k];
			tabp[21].urobek[30]=tabk[0].nadawa[k];

			//U7 L1


			//  PRZ 35 !!!!!!!!!!!!!
			end=tabp[47].il_paczek-1;  // end-1..... do obliczen										
			for(int j=0; j<tabp[47].il_paczek-1; j++)
			{
				tabp[47].urobek[end-j]=tabp[47].urobek[end-j-1];  // przerzucanie paczek
					if(j==tabp[47].il_paczek-2)
					{
						tabp[47].urobek[0]=tabp[48].urobek[tabp[48].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
					}
			}




			//  PRZ 36 !!!!!!!!!!!!!
			end=tabp[48].il_paczek-1;  // end-1..... do obliczen										
			for(int j=0; j<tabp[48].il_paczek-1; j++)
			{
				tabp[48].urobek[end-j]=tabp[48].urobek[end-j-1];  // przerzucanie paczek
					if(j==tabp[48].il_paczek-2)
					{
						tabp[48].urobek[0]=tabp[49].urobek[tabp[49].il_paczek-1]
										  +tabp[50].urobek[tabp[50].il_paczek-1]
										  +tabp[51].urobek[tabp[51].il_paczek-1];   // sypia 3 przenosniki na raz
					}
			}
			tabp[48].urobek[50]=tabk[0].nadawa[k];




			//  PRZ 37 !!!!!!!!!!!!!
			end=tabp[51].il_paczek-1;  // end-1..... do obliczen										
			for(int j=0; j<tabp[51].il_paczek-1; j++)
			{
				tabp[51].urobek[end-j]=tabp[51].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[51].urobek[0]=tabk[0].nadawa[k];




			//  PRZ 38 !!!!!!!!!!!!!
			end=tabp[50].il_paczek-1;  // end-1..... do obliczen										
			for(int j=0; j<tabp[50].il_paczek-1; j++)
			{
				tabp[50].urobek[end-j]=tabp[50].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[50].urobek[0]=tabk[0].nadawa[k];




			// PRZ 39	!!!!!!!!!!!!!!
			end=tabp[49].il_paczek-1;	
			for(int j=0; j<tabp[49].il_paczek-1; j++)
			{
				tabp[49].urobek[end-j]=tabp[49].urobek[end-j-1];  // przerzucanie paczek
			}
			// oproznianie zbiornika przed po przerzuceniu paczek sypie na przenosnik
			if (tabz[0].stan_zbiornika>1)
			{
				tabp[49].urobek[0]=tabz[0].wydajnosc;
				tabz[0].stan_zbiornika=tabz[0].stan_zbiornika-(tabz[0].wydajnosc*przelicznik);
			}
			else tabp[49].urobek[0]=0;
			tabp[49].urobek[30]=tabk[0].nadawa[k];



			//  PRZ 40 !!!!!!!!!!!!!
			end=tabp[54].il_paczek-1;  // end-1..... do obliczen										
			tabz[0].zapelnianie[k]=tabz[0].zapelnianie[k]+tabp[54].urobek[end];  // odbieranie urobku
			tabz[0].stan_zbiornika=tabz[0].stan_zbiornika+(tabz[0].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika R III nieskonczonosc
			for(int j=0; j<tabp[54].il_paczek-1; j++)
			{
				tabp[54].urobek[end-j]=tabp[54].urobek[end-j-1];  // przerzucanie paczek
				if(j==tabp[54].il_paczek-2)
				{
					tabp[54].urobek[0]=tabp[53].urobek[tabp[53].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
				}
			}




			//  PRZ 41 !!!!!!!!!!!!!
			end=tabp[53].il_paczek-1;  // end-1..... do obliczen										
			for(int j=0; j<tabp[53].il_paczek-1; j++)
			{
				tabp[53].urobek[end-j]=tabp[53].urobek[end-j-1];  // przerzucanie paczek
				if(j==tabp[53].il_paczek-2)
				{
					tabp[53].urobek[0]=tabp[55].urobek[tabp[55].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
				}
			}
			tabp[53].urobek[60]=tabp[56].urobek[tabp[56].il_paczek-1];  // PRZENOSNIK W POLOWIE TEGO jeszcze jeden bedzie sypa³





			//  PRZ 42 !!!!!!!!!!!!!
			end=tabp[56].il_paczek-1;  // end-1..... do obliczen										
			for(int j=0; j<tabp[56].il_paczek-1; j++)
			{
				tabp[56].urobek[end-j]=tabp[56].urobek[end-j-1];  // przerzucanie paczek
					if(j==tabp[56].il_paczek-2)
					{
						tabp[56].urobek[0]=tabp[57].urobek[tabp[57].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
					}
			}
			tabp[56].urobek[20]=tabk[0].nadawa[k];
			tabp[56].urobek[60]=tabk[0].nadawa[k];




			// PRZ 43	!!!!!!!!!!!!!!
			end=tabp[57].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3			
			for(int j=0; j<tabp[57].il_paczek-1; j++)
			{
				tabp[57].urobek[end-j]=tabp[57].urobek[end-j-1];  // przerzucanie paczek
			}
			// oproznianie zbiornika przed po przerzuceniu paczek sypie na przenosnik
			if (tabz[13].stan_zbiornika>1)
			{
				tabp[57].urobek[0]=tabz[13].wydajnosc;
				tabz[13].stan_zbiornika=tabz[13].stan_zbiornika-(tabz[13].wydajnosc*przelicznik);
			}
			else tabp[57].urobek[0]=0;
			tabp[57].urobek[10]=tabk[0].nadawa[k];
		



			//  PRZ 44 !!!!!!!!!!!!!
			end=tabp[61].il_paczek-1;  // end-1..... do obliczen										
			tabz[13].zapelnianie[k]=tabz[13].zapelnianie[k]+tabp[61].urobek[end];  // odbieranie urobku
			tabz[13].stan_zbiornika=tabz[13].stan_zbiornika+(tabz[13].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika R III nieskonczonosc
			for(int j=0; j<tabp[61].il_paczek-1; j++)
			{
				tabp[61].urobek[end-j]=tabp[61].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[61].urobek[0]=tabk[0].nadawa[k];
			tabp[61].urobek[10]=tabk[0].nadawa[k];




			//  PRZ 45 !!!!!!!!!!!!!
			end=tabp[58].il_paczek-1;  // end-1..... do obliczen										
			tabz[13].zapelnianie[k]=tabz[13].zapelnianie[k]+tabp[58].urobek[end];  // odbieranie urobku
			tabz[13].stan_zbiornika=tabz[13].stan_zbiornika+(tabz[13].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika R III nieskonczonosc
			for(int j=0; j<tabp[58].il_paczek-1; j++)
			{
				tabp[58].urobek[end-j]=tabp[58].urobek[end-j-1];  // przerzucanie paczek
				if(j==tabp[58].il_paczek-2)
				{
					tabp[58].urobek[0]=tabk[0].nadawa[k];   // KRATA NA POCZATKU
				}
			}
			tabp[58].urobek[5]=tabp[62].urobek[tabp[62].il_paczek-1];  // PRZENOSNIK W POLOWIE TEGO jeszcze jeden bedzie sypa³
			tabp[58].urobek[50]=tabk[0].nadawa[k];
			tabp[58].urobek[150]=tabk[0].nadawa[k];



			//  PRZ 46 !!!!!!!!!!!!!
			end=tabp[62].il_paczek-1;  // end-1..... do obliczen										
			for(int j=0; j<tabp[62].il_paczek-1; j++)
			{
				tabp[62].urobek[end-j]=tabp[62].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[62].urobek[0]=tabk[0].nadawa[k];



			// PRZ 47	!!!!!!!!!!!!!!
			end=tabp[55].il_paczek-1;	
			for(int j=0; j<tabp[55].il_paczek-1; j++)
			{
				tabp[55].urobek[end-j]=tabp[55].urobek[end-j-1];  // przerzucanie paczek
			}
			// oproznianie zbiornika przed po przerzuceniu paczek sypie na przenosnik
			if (tabz[2].stan_zbiornika>2)
			{
				tabp[55].urobek[2]=tabz[2].wydajnosc;
				tabz[2].stan_zbiornika=tabz[2].stan_zbiornika-(tabz[2].wydajnosc*przelicznik);
			}
			else tabp[55].urobek[0]=0;
			tabp[55].urobek[60]=tabp[59].urobek[tabp[59].il_paczek-1];  // PRZENOSNIK W POLOWIE TEGO jeszcze jeden bedzie sypa³



			// PRZ 48	!!!!!!!!!!!!!!
			end=tabp[59].il_paczek-1;	
			for(int j=0; j<tabp[59].il_paczek-1; j++)
			{
				tabp[59].urobek[end-j]=tabp[59].urobek[end-j-1];  // przerzucanie paczek
			}
			// oproznianie zbiornika przed po przerzuceniu paczek sypie na przenosnik
			if (tabz[12].stan_zbiornika>1)
			{
				tabp[59].urobek[0]=tabz[12].wydajnosc;
				tabz[12].stan_zbiornika=tabz[12].stan_zbiornika-(tabz[12].wydajnosc*przelicznik);
			}
			else tabp[59].urobek[0]=0;
			tabp[59].urobek[10]=tabk[0].nadawa[k];
			tabp[59].urobek[20]=tabk[0].nadawa[k];
			tabp[59].urobek[80]=tabk[0].nadawa[k];
			tabp[59].urobek[130]=tabk[0].nadawa[k];



			//  PRZ 49 !!!!!!!!!!!!!
			end=tabp[60].il_paczek-1;  // end-1..... do obliczen										
			tabz[12].zapelnianie[k]=tabz[12].zapelnianie[k]+tabp[60].urobek[end];  // odbieranie urobku
			tabz[12].stan_zbiornika=tabz[12].stan_zbiornika+(tabz[12].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika R III nieskonczonosc
			for(int j=0; j<tabp[60].il_paczek-1; j++)
			{
				tabp[60].urobek[end-j]=tabp[60].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[60].urobek[0]=tabk[0].nadawa[k];


			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


			//H-21 L3


			// PRZ14	!!!!!!!!!!!!!!
			end=tabp[31].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3			
			tabz[2].zapelnianie[k]=tabz[2].zapelnianie[k]+tabp[31].urobek[end];  // odbieranie urobku
			tabz[2].stan_zbiornika=tabz[2].stan_zbiornika+(tabz[2].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika R III nieskonczonosc
			for(int j=0; j<tabp[31].il_paczek-1; j++)
			{
				tabp[31].urobek[end-j]=tabp[31].urobek[end-j-1];  // przerzucanie paczek
			}
			// oproznianie zbiornika po przerzuceniu paczek sypie na przenosnik
			if (tabz[9].stan_zbiornika>1)
			{
				tabp[31].urobek[0]=tabz[9].wydajnosc;
				tabz[9].stan_zbiornika=tabz[9].stan_zbiornika-(tabz[9].wydajnosc*przelicznik);
			}
			else tabp[31].urobek[0]=0;
			tabp[31].urobek[10]=tabk[0].nadawa[k];



			// PRZ15	!!!!!!!!!!!!!!
			end=tabp[32].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3			
			tabz[9].zapelnianie[k]=tabz[9].zapelnianie[k]+tabp[32].urobek[end];  // odbieranie urobku
			tabz[9].stan_zbiornika=tabz[9].stan_zbiornika+(tabz[9].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika R III nieskonczonosc
			for(int j=0; j<tabp[32].il_paczek-1; j++)
			{
				tabp[32].urobek[end-j]=tabp[32].urobek[end-j-1];  // przerzucanie paczek
			}
			// oproznianie zbiornika przed po przerzuceniu paczek sypie na przenosnik
			if (tabz[4].stan_zbiornika>1)
			{
				tabp[32].urobek[0]=tabz[4].wydajnosc;
				tabz[4].stan_zbiornika=tabz[4].stan_zbiornika-(tabz[4].wydajnosc*przelicznik);
			}
			else tabp[32].urobek[0]=0;
			tabp[32].urobek[140]=tabp[34].urobek[tabp[34].il_paczek-1];  // PRZENOSNIK W POLOWIE TEGO jeszcze jeden bedzie sypa³
			if (tabz[7].stan_zbiornika>1)                                // SYPIE NA PRZENOSNIK ZE ZBIORNIKA W POLOWIE
			{
				tabp[32].urobek[0]=tabz[7].wydajnosc;
				tabz[7].stan_zbiornika=tabz[7].stan_zbiornika-(tabz[7].wydajnosc*przelicznik);
			}



			// PRZ16	!!!!!!!!!!!!!!
			end=tabp[34].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3									 
			for(int j=0; j<tabp[34].il_paczek-1; j++)
			{
				tabp[34].urobek[end-j]=tabp[34].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[34].urobek[10]=tabk[0].nadawa[k];




			// PRZ17	!!!!!!!!!!!!!!
			end=tabp[35].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3			
			tabz[7].zapelnianie[k]=tabz[7].zapelnianie[k]+tabp[35].urobek[end];  // odbieranie urobku
			tabz[7].stan_zbiornika=tabz[7].stan_zbiornika+(tabz[7].zapelnianie[k]*przelicznik);  // monitoring stanu zapelnienia zbiornika R III nieskonczonosc
			for(int j=0; j<tabp[35].il_paczek-1; j++)
			{
				tabp[35].urobek[end-j]=tabp[35].urobek[end-j-1];  // przerzucanie paczek
			}
			// oproznianie zbiornika przed po przerzuceniu paczek sypie na przenosnik
			if (tabz[8].stan_zbiornika>1)
			{
				tabp[35].urobek[0]=tabz[8].wydajnosc;
				tabz[8].stan_zbiornika=tabz[8].stan_zbiornika-(tabz[8].wydajnosc*przelicznik);
			}
			else tabp[35].urobek[0]=0;
			tabp[35].urobek[40]=tabk[0].nadawa[k];
			tabp[35].urobek[80]=tabk[0].nadawa[k];



			// PRZ18	!!!!!!!!!!!!!!
			end=tabp[36].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3	
			tabz[8].zapelnianie[k]=tabz[8].zapelnianie[k]+tabp[36].urobek[end];  // odbieranie urobku
			tabz[8].stan_zbiornika=tabz[8].stan_zbiornika+(tabz[8].zapelnianie[k]*przelicznik);
			for(int j=0; j<tabp[36].il_paczek-1; j++)
			{
				tabp[36].urobek[end-j]=tabp[42].urobek[end-j-1];  // przerzucanie paczek
				if(j==tabp[36].il_paczek-2)
				{
					tabp[36].urobek[0]=tabp[38].urobek[tabp[38].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje, a on odbiera
				}
			}
			tabp[36].urobek[1]=tabk[0].nadawa[k];
			tabp[36].urobek[30]=tabk[0].nadawa[k];





			// PRZ19	!!!!!!!!!!!!!!
			end=tabp[38].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3									 
			for(int j=0; j<tabp[38].il_paczek-1; j++)
			{
				tabp[38].urobek[end-j]=tabp[38].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[38].urobek[1]=tabk[0].nadawa[k];







			// PRZ20	!!!!!!!!!!!!!!
			end=tabp[37].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3	
			tabz[4].zapelnianie[k]=tabz[4].zapelnianie[k]+tabp[37].urobek[end];  // odbieranie urobku
			tabz[4].stan_zbiornika=tabz[4].stan_zbiornika+(tabz[4].zapelnianie[k]*przelicznik);
			for(int j=0; j<tabp[37].il_paczek-1; j++)
			{
				tabp[37].urobek[end-j]=tabp[37].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[37].urobek[1]=tabk[0].nadawa[k];
			tabp[37].urobek[30]=tabk[0].nadawa[k];






			// PRZ21	!!!!!!!!!!!!!!
			end=tabp[33].il_paczek-1;  // end-1..... do obliczen		// paczka wczesniej z end zabrana do PRZ 3	
			tabz[4].zapelnianie[k]=tabz[4].zapelnianie[k]+tabp[33].urobek[end];  // odbieranie urobku
			tabz[4].stan_zbiornika=tabz[4].stan_zbiornika+(tabz[4].zapelnianie[k]*przelicznik);
			for(int j=0; j<tabp[33].il_paczek-1; j++)
			{
				tabp[33].urobek[end-j]=tabp[33].urobek[end-j-1];  // przerzucanie paczek
			}
			tabp[33].urobek[1]=tabk[0].nadawa[k];
			tabp[33].urobek[30]=tabk[0].nadawa[k];








		}
			


									//if(j==tabp[0].il_paczek-2)
					//{
					//	tabp[0].urobek[0]=tabp[1].urobek[tabp[1].il_paczek-1];   // tylko gdy przenosnik nie jest pierwszy w ciagu/ nikt na niego nei laduje
				//	}	
		
		
		
		
			cout<<"\n\n";
			cout<<"\n"<<"stan zapelnienia zbiornika 6  "<<tabz[6].stan_zbiornika<<"\n";
			cout<<"\n"<<"stan zapelnienia zbiornika 10  "<<tabz[10].stan_zbiornika<<"\n";
			cout<<"\n"<<"stan zapelnienia zbiornika 11  "<<tabz[11].stan_zbiornika<<"\n";
			cout<<"\n"<<"stan zapelnienia R III"<<tabz[15].stan_zbiornika<<"\n";

		
			cout<<"\n"<<"ilos paczek przenosnika  "<<tabp[23].lp_prz<<" tyle paczek ->  "<<tabp[23].il_paczek<<"\n";
			cout<<"ilos paczek przenosnika  "<<tabp[24].lp_prz<<" tyle paczek ->  "<<tabp[24].il_paczek<<"\n";
			cout<<"ilos paczek przenosnika  "<<tabp[63].lp_prz<<" tyle paczek ->  "<<tabp[63].il_paczek<<"\n";
			cout<<"ilos paczek przenosnika  "<<tabp[28].lp_prz<<" tyle paczek ->  "<<tabp[28].il_paczek<<"\n";
			cout<<"ilos paczek przenosnika  "<<tabp[29].lp_prz<<" tyle paczek ->  "<<tabp[29].il_paczek<<"\n";
			cout<<"ilos paczek przenosnika  "<<tabp[30].lp_prz<<" tyle paczek ->  "<<tabp[30].il_paczek<<"\n";
			cout<<"ilos paczek przenosnika  "<<tabp[44].lp_prz<<" tyle paczek ->  "<<tabp[44].il_paczek<<"\n";
			cout<<"ilos paczek przenosnika  "<<tabp[42].lp_prz<<" tyle paczek ->  "<<tabp[42].il_paczek<<"\n";
			cout<<"ilos paczek przenosnika  "<<tabp[39].lp_prz<<" tyle paczek ->  "<<tabp[39].il_paczek<<"\n";
			cout<<"ilos paczek przenosnika  "<<tabp[40].lp_prz<<" tyle paczek ->  "<<tabp[40].il_paczek<<"\n";
			cout<<"ilos paczek przenosnika  "<<tabp[41].lp_prz<<" tyle paczek ->  "<<tabp[41].il_paczek<<"\n";
			cout<<"ilos paczek przenosnika  "<<tabp[43].lp_prz<<" tyle paczek ->  "<<tabp[43].il_paczek<<"\n";
			cout<<"ilos paczek przenosnika  "<<tabp[46].lp_prz<<" tyle paczek ->  "<<tabp[46].il_paczek<<"\n";
			cout<<"ilos paczek przenosnika  "<<tabp[45].lp_prz<<" tyle paczek ->  "<<tabp[45].il_paczek<<"\n";

						cout<<"\n"<<"z przenosnika 44  (waga) ";
		for(int j=0; j<tabp[44].il_paczek; j++)
				{
					cout<<tabp[44].urobek[j]<<" ";
				}
				cout<<"\n";

							cout<<"\n"<<"z przenosnika 42  (waga) ";
		for(int j=0; j<tabp[42].il_paczek; j++)
				{
					cout<<tabp[42].urobek[j]<<" ";
				}
				cout<<"\n";

							cout<<"\n"<<"z przenosnika 46  (waga) ";
		for(int j=0; j<tabp[46].il_paczek; j++)
				{
					cout<<tabp[46].urobek[j]<<" ";
				}
				cout<<"\n";

			cout<<"\n"<<"z przenosnika 41  (waga) ";
		for(int j=0; j<tabp[41].il_paczek; j++)
				{
					cout<<tabp[41].urobek[j]<<" ";
				}
				cout<<"\n";

				cout<<"\n"<<"zbiornik 11  stan zbiornika "<<tabz[11].stan_zbiornika;
		cout<<"\n"<<"zbiornik 11  zapelnianie ";
		for(int j=0; j<n; j++)
				{
					cout<<tabz[11].zapelnianie[j]<<" ";
				}
				cout<<"\n";

						cout<<"\n"<<"z przenosnika 40  (waga) ";
		for(int j=0; j<tabp[40].il_paczek; j++)
				{
					cout<<tabp[40].urobek[j]<<" ";
				}
				cout<<"\n";


							cout<<"\n"<<"z przenosnika 39  (waga) ";
		for(int j=0; j<tabp[39].il_paczek; j++)
				{
					cout<<tabp[39].urobek[j]<<" ";
				}
				cout<<"\n";


			cout<<"\n"<<"z przenosnika 24  (waga) ";
		for(int j=0; j<tabp[24].il_paczek; j++)
				{
					cout<<tabp[24].urobek[j]<<" ";
				}
				cout<<"\n";
								

		cout<<"\n"<<"z przenosnika 28  (waga) ";
		for(int j=0; j<tabp[28].il_paczek; j++)
				{
					cout<<tabp[28].urobek[j]<<" ";
				}
				cout<<"\n";

					cout<<"\n"<<"z przenosnika 23  (waga) ";
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


				cout<<"\n"<<"zbiornik"<<"\n";

							for(int j=0; j<n; j++) // sprawdzam zbiornik
			{
			//	cout<<tabz[6].zapelnianie[j]<<" ";
			}
			cout<<"\n";


			cout<<"\n"<<"waga z przenosnika 23 na 5 paczce"<<"\n";

			for(int j=0; j<n; j++) // sprawdzam zbiornik
			{
				//cout<<tabw[0].odczyt[j]<<" ";
			}
			cout<<"\n";


		
	cout<<"\n"<<"\n"<<"\n"<<"\n";

	float urob=800;      //   t/h
	
	float wynik;
	wynik=urob*przelicznik;
	cout<<wynik;


	cout<<"\n"<<"\n"<<"\n"<<"\n";

	system("pause");
	return 0;
}

/*
if(i!=0)                            // jezeli to nie jest pierwszy przenosnik na trasie to wez z poprzedniego i wrzuc na nastepny
					{
						if(j==0)
						{
						int pomocnicza;
						pomocnicza=tabp[i-1].il_paczek-1;
						cout<<"pomocnicza" <<pomocnicza;
						tabp[i].urobek[j]=tabp[i-1].urobek[pomocnicza];
						}
						else  tabp[i].urobek[j]=tabp[i].urobek[j-1];
					}                                  // koniec przerzutu miedzy przenosnikami
					else  
						if(j==0)
						{
							tabp[i].urobek[j]=tabk[i].nadawa[k];
						}
						tabp[i].urobek[j]=tabp[i].urobek[j-1];
						*/
/*
		// stworzenie tablic (paczek) w przenosnikach i wyzerowanie ich
		for(int i=0; i<ile_przenosnikow; i++)
		{
			int pomocnicza=tabp[i].il_paczek;
			tabp[i].urobek=new float[pomocnicza];
			for(int j=0; j<tabp[i].il_paczek; j++)
			{
				tabp[i].urobek[j]=0;
			}
		}
*/

/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for(int i=0;i<250; i++)           //ilos przerzutow na calej kopalni
	{
		przenosnik[0]=urobek[i];
		for (int j=0; j<l; j++)    // przezucanie paczek po jednym przenosniku l jest o jeden mniejsze niz liczba paczek w przenosniku
		{
			przenosnik[l-j]=przenosnik[l-j-1];
		}
		tabz[i]=przenosnik[49]; // zapelnianie zbiornika z historia o czas probkowania
		//cout<<tabz[i];

		waga[i]=przenosnik[20];
		//cout<<waga[i];
	
		w=w+tabz[i];// aktualny stan zapelnienia zbiornika
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//przenosnik za zbiornikiem

		if(w>=1)  // dozownik-> opró¿nianie zbiornika
		{
			prz[0]=1;
			w=w-1;
		}
		else prz[0]=0;
		//cout<< prz[0];

//////////////////////// przenoœnik odbieraj¹cy

		for (int j=0; j<l; j++)    // przezucanie paczek po jednym przenosniku l jest o jeden mniejsze niz liczba paczek w przenosniku
		{
			prz[l-j]=prz[l-j-1];
		}
		tabz1[i]=prz[49]; // zapelnianie zbiornika z historia o czas probkowania
		
		waga1[i]=prz[39];
		//cout<<waga1[i];
	
		w1=w1+tabz1[i];// aktualny stan zapelnienia zbiornika
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////// koniec symulacji
	// waga wyswieta
	for(int i=0; i<250; i++)
	{	if(i==0)
		{
		cout<<"\n"<<"waga: ";
		}
			cout<<waga[i];
	}
	//////////////////////////////////////////////////// 2 waga wyswietla
		for(int i=0; i<250; i++)
	{	if(i==0)
		{
		cout<<"\n"<<"waga1: ";
		}
			cout<<waga1[i];
	}
//////////////////////////////////////////////////////////// wyswietla zbiorniki
				for(int i=0; i<250; i++)
	{	if(i==0)
		{
		cout<<"\n"<<"jak sie zape³nia³ zbiornik  : ";
		}
			cout<<tabz[i];
			tabzw[i]=tabz[i]+tabzw[i-1];
	}

		
				
				for(int i=0; i<250; i++)
	{	if(i==0)
		{
		cout<<"\n"<<"jak sie zape³nia³ zbiornik 1: ";
		}
			cout<<tabz1[i];

	}

cout<<"aktualny stan zape³nienia zbiornika  : "<<w<<"\n";
cout<<"aktualny stan zape³nienia zbiornika 1: "<<w1<<"\n";
}
*/
