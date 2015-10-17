#ifndef STRUKTURY_H
#define STRUKTURY_H
#define MAX_BUFOR 1024
#define MAX_SKOK 2
#define DZIELNIK 4
#define dPlot_Plot8 DPlot_Plot8
#pragma comment(lib, "Ws2_32.lib")

/*Struktura VOL zawieraj¹ca dane dotycz¹ce objêtoœci zbiornika*/
struct VOL
{
	float Czas[MAX_BUFOR];
	double Wartosc[MAX_BUFOR];
} Objetosc;

/*Struktura TEMP zawieraj¹ca dane dotycz¹ce temperatury zbiornika*/
struct TEMP
{
	int Czas[MAX_BUFOR];
	double Wartosc[MAX_BUFOR];
} Temperatura;

/**
 * Funkcja  Pobieraj_DATA:         Pobiera dane z serwera, na podstawie okreslonego zapytania
 *\param    SOCKET ConnectSocket:  SOCKETY utworzone w funkcji main
 *\param    Zapytanie[10]:         Tablica znaków bêd¹ca jednym z ustalonych zapytañ. GET_VOL, lub GET_TEMP
 *\param    Argument:              Argument z funkcji main, wskazuj¹cy miejsce w tablicy wartosc[MAX_BUFFER], pobierana z pêtli for
 * \return  Data:				   Zmienna typu Double bêd¹ca wartoœci¹ pobran¹ z serwera, jest ona zapisywana do jednej ze struktur
 */
double Pobieraj_DATA(SOCKET ConnectSocket, char Zapytanie[10], int Argument);

/**
 * Funkcja  TworzPlik:	        Tworzy plik html bêd¹cy wykresem dwóch funkcji
 *\param    VOL:				Struktura zawieraj¹ca dane pobrane z serwera
 *\param    TEMP:				Struktura zawieraj¹ca dane pobrane z serwera
 *\param    Argument:           Argument z funkcji main, wskazuj¹cy miejsce w tablicy wartosc[MAX_BUFFER], pobierana z pêtli for
 *\param	Czas_Pomiar:		Czas pobierania danych z serwera okreslony przez u¿ytkownika
 * \return  Void		
 */
void TworzPlik(struct VOL Objetosc, struct TEMP Temperatura, int Argument, int Czas_Pomiar);

/**
 * Funkcja  UsunSzumy_TEMP:     Usuwa szumy z tablicy temperatury zbiornka
 *\param    TEMP:				Struktura zawieraj¹ca dane pobrane z serwera
 *\param    Argument:           Argument z funkcji main, wskazuj¹cy miejsce w tablicy wartosc[MAX_BUFFER], pobierana z pêtli for
 *\param	Czas_Pomiar:		Czas pobierania danych z serwera okreslony przez u¿ytkownika
 * \return  Wartosci_BezSzum:	Tablica wartosci temperatury zbiornika nie zawieraj¹ca szumów	
 */
double UsunSzumy_TEMP(struct TEMP Temperatura, int Czas_Pomiar, int Argument);

/**
 * Funkcja  UsunSzumy_VOL:		Usuwa szumy z tablicy objêtoœci zbiornka
 *\param    VOL:				Struktura zawieraj¹ca dane pobrane z serwera
 *\param    Argument:           Argument z funkcji main, wskazuj¹cy miejsce w tablicy wartosc[MAX_BUFFER], pobierana z pêtli for
 *\param	Czas_Pomiar:		Czas pobierania danych z serwera okreslony przez u¿ytkownika
 * \return  Wartosci_BezSzum:	Tablica wartosci objêtoœci zbiornika nie zawieraj¹ca szumów	
 */
double UsunSzumy_VOL(struct VOL Objetosc, int Czas_Pomiar, int Argument);

/**
 * Funkcja  Tworz_DPLOT:		Tworzy i uruchamia wykres danych, pobranych z serwera, w programie DPlot
 *\param    VOL:				Struktura zawieraj¹ca dane pobrane z serwera
 *\param	Czas_Pomiar:		Czas pobierania danych z serwera okreslony przez u¿ytkownika
 * \return  Void
 */
void Tworz_DPLOT(struct VOL Objetosc, int Czas_Pomiar);



#endif