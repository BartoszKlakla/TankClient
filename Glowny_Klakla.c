/**
 * \file   Glowny_Klakla.c
 * \author Bartosz Klakla
 * \date   02 stycznia, 2014
 */
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include "dplotlib.h"
#include "struktury.h"

/*Funkcja g³ówna main*/
int main(int argc, char *argv[])
{
	struct addrinfo *result = NULL, *pointer = NULL, hints;
	WSADATA WsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	int argument=0, Otrzymuj_Dlugosc = MAX_BUFFER, Czas_Pomiar=0;
	char Wysylaj_data[10]={0}, Otrzymuj_data[MAX_BUFFER] = {0};
	float czas;

	if(!WSAStartup(MAKEWORD(2,0), &WsaData))
		printf("\n\nInicjacja Winsock.dll: OK!\n");
	else
	{
		printf("Inicjacja Winsock.dll zakonczona bledem!");
		return 1;
	}
	
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_socktype = SOCK_STREAM;

	if(!getaddrinfo(argv[1], argv[2], &hints, &result))
		printf("Translacja nazwy hosta na adres: OK!\n");
	else
	{
		printf("Translacja zakonczona bledem: %d", GetLastError);
		WSACleanup();
		return 1;
	}
	
	pointer = result;
	
	ConnectSocket = socket(AF_INET, SOCK_DGRAM, 0);

	if(connect(ConnectSocket, pointer->ai_addr, pointer->ai_addrlen) != SOCKET_ERROR)
		printf("Polaczenie z serwerem:  OK!\n");
	else closesocket(ConnectSocket);
	
	freeaddrinfo(result);
	
	printf("\nPodaj czas pobierania danych z serwera: ");
	scanf("%d", &Czas_Pomiar);
	printf("\nRozpoczyna sie pobieranie od serwera...   \n");
	system("PAUSE");
	
	for(argument=0; argument < Czas_Pomiar*DZIELNIK; argument++)
	{	
		Sleep(1000/DZIELNIK);
		Objetosc.wartosc[argument] = Pobieraj_DATA(ConnectSocket, "GET_VOL", argument);
		czas = argument;
		Objetosc.czas[argument] = czas/DZIELNIK;
		printf("Czas pomiaru: %f  ", Objetosc.czas[argument]);
		printf("Objetosc: %f,    ", Objetosc.wartosc[argument]);
		Temperatura.wartosc[argument] = Pobieraj_DATA(ConnectSocket, "GET_TEMP", argument);
		printf("Temperatura: %f\n", Temperatura.wartosc[argument]);
	}
	tworz_plik(Objetosc, Temperatura, argument, Czas_Pomiar);
	system("pause");
	for(argument=1; argument<(Czas_Pomiar*DZIELNIK)-1; argument++)
	{
		czas = argument;
		Objetosc.wartosc[argument] = usun_szumyVOL(Objetosc, Czas_Pomiar, argument);
		Temperatura.wartosc[argument] = usun_szumyTEMP(Temperatura, Czas_Pomiar, argument);
		Objetosc.czas[argument] = czas/DZIELNIK;
		printf("Czas pomiaru: %f,  ", Objetosc.czas[argument]);
		printf("Objetosc: %f\n", Objetosc.wartosc[argument]);
	}
	tworz_plik(Objetosc, Temperatura, argument, Czas_Pomiar);
	Tworz_DPLOT(Objetosc, Czas_Pomiar);
	system("pause");
	return 0;
}
