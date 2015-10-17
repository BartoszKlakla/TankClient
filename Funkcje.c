#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include "struktury.h"
#include "dplotlib.h"

double Pobieraj_DATA(SOCKET ConnectSocket, char Zapytanie[10], int Argument)
{
	int Rezultat, Otrzymuj_Dlugosc = MAX_BUFOR; 
	double Data;
	char Otrzymuj_data[MAX_BUFOR] = {0};
	Rezultat = send(ConnectSocket, Zapytanie, (int)strlen(Zapytanie), 0);
	if(Rezultat != SOCKET_ERROR)
	{
		if(Argument == 0)
			printf("\nWysylanie do serwera:  OK!\n");
		Rezultat = recv(ConnectSocket, Otrzymuj_data, Otrzymuj_Dlugosc, 0);
		if(Rezultat > 0)
		{
			if(Argument == 0)
				printf("Bitow otrzymano: %d\n", Rezultat);
			if(Zapytanie == "GET_VOL")
				Otrzymuj_data[1] = '.';

			Data = atof(Otrzymuj_data);
			return Data;
		}
		else if(Rezultat == 0)
		{
			printf("Polaczenie zamkniete!\n");
			system("pause");
			return 0;
		}
		else
		{
			printf("Otrzymywanie zakonczone bledem: %d\n", WSAGetLastError());
			system("pause");
			return 0;
		}
	}
	else
	{
		printf("Wysylanie zakonczone bledem: %d", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 0;
	}		
}

void tworz_plik(struct VOL Objetosc, struct TEMP Temperatura, int Argument, int Czas_Pomiar)
{	
	FILE *Plik_rezultat;
	char Nazwa_plik[20];
	printf("\nPodaj nazwe pliku tekstowego przechowujacego dane: ");
	scanf("%s", &Nazwa_plik);
	strcat(Nazwa_plik, ".html");
	Plik_rezultat = fopen(Nazwa_plik, "w");
	fprintf(Plik_rezultat,"<html>\n  <head>\n<script type=\"text/javascript\" src=\"https://www.google.com/jsapi\"></script>\n<script type=\"text/javascript\">\n");
	fprintf(Plik_rezultat,"google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});\ngoogle.setOnLoadCallback(drawChart);\nfunction drawChart() {\n");
	fprintf(Plik_rezultat,"        var data = new google.visualization.arrayToDataTable([\n['Czas', 'Temperatura', 'Objetosc'],\n");
	for(Argument = 0; Argument < Czas_Pomiar*DZIELNIK; Argument++)
	{
		fprintf(Plik_rezultat, "[%f,  ", Objetosc.Czas[Argument]);
		fprintf(Plik_rezultat, "%f,  ", Temperatura.Wartosc[Argument]);
		fprintf(Plik_rezultat, "%f],\n", Objetosc.Wartosc[Argument]);
	}
	fprintf(Plik_rezultat,"]);    \nvar options = {\ntitle: 'Objetosc cieczy'\n};\n\n");
	fprintf(Plik_rezultat,"var chart = new google.visualization.LineChart(document.getElementById('chart_div'));\nchart.draw(data, options);\n}\n</script>\n");
	fprintf(Plik_rezultat,"  </head>\n<body>\n<div id=\"chart_div\" style=\"width: 2400px; height: 1200px;\"></div>\n</body>\n</html>\n");
	fclose(Plik_rezultat);
	system(Nazwa_plik);
}

double UsunSzumy_TEMP(struct TEMP Temperatura, int Czas_Pomiar, int Argument)
{	
	double Roznica, Wartosci_BezSzum[MAX_BUFOR];
	Roznica = Temperatura.Wartosc[Argument] - Temperatura.Wartosc[Argument+1];
	
	if(Roznica < 0)
		Roznica = Roznica*(-1);
	if(Roznica >= MAX_SKOK)
		Wartosci_BezSzum[Argument] = (Temperatura.Wartosc[Argument-1] + Temperatura.Wartosc[Argument+1])/2;
	else
		Wartosci_BezSzum[Argument] = Temperatura.Wartosc[Argument];
	
	return
		Wartosci_BezSzum[Argument];
}

double UsunSzumy_VOL(struct VOL Objetosc, int Czas_Pomiar, int Argument)
{
	double Roznica, Wartosci_BezSzum[MAX_BUFOR];
	Roznica = Objetosc.Wartosc[Argument] - Objetosc.Wartosc[Argument + 2];
		if(Roznica < 0)
			Roznica = Roznica*(-1);
		if(Roznica >= MAX_SKOK)
			Wartosci_BezSzum[Argument] = (Objetosc.Wartosc[Argument-1] + Objetosc.Wartosc[Argument+1])/2;
		else Wartosci_BezSzum[Argument] = Objetosc.Wartosc[Argument];
	return
		Wartosci_BezSzum[Argument];
}

void Tworz_DPLOT(struct VOL Objetosc, int Czas_Pomiar)
{
	DPLOT DPlot;
	char Buffer[25];
	memset(&DPlot, 0, sizeof(DPlot));
	DPlot.Version		= DPLOT_DDE_VERSION;
	DPlot.DataFormat	= DATA_XYYY;
	DPlot.MaxCurves		= 2;	
	DPlot.MaxPoints		= Czas_Pomiar;
	DPlot.NumCurves		= 2;  
	DPlot.Scale			= SCALE_LINEARX_LINEARY;
	DPlot.LegendX		= 1;
	DPlot.LegendY		= 1;
	DPlot.NP[0]			= Czas_Pomiar;
	DPlot.NP[1]			= Czas_Pomiar;
	DPlot.LineType[0]	= LINESTYLE_SOLID;
	DPlot.LineType[1]	= LINESTYLE_SOLID;
	sprintf(Buffer, "Wykres"); 
	strcpy(DPlot.Legend[1], Buffer);
	strcpy(DPlot.Title[0], "Sygnal wyslany przez DPLOTLIB.DLL");
	strcpy(DPlot.XAxis, "x");
	strcpy(DPlot.YAxis, "y");
	DPlot_Plot8(&DPlot, Objetosc.Czas, Objetosc.Wartosc, "[Caption(\"DPLOTLIB XY Test\")]""[ClearEditFlag()]");
}

/*wywo³anie tworzenia wykresów w programie DPlot*/
int __stdcall DPlot_Plot8(DPLOT *DPlot, float *array1, double *array2, LPSTR commands);