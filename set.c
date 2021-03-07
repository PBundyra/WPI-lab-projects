#include <stdio.h>
#include <stdbool.h>
#define WIELKOSC_TALII 81
#define POCZATKOWE_KARTY 12
#define DODATKOWE_KARTY 3
#define SUMA_1_CECHY 6
#define LICZBA_CECH_KARTY 4
#define PODSTAWA_POTEGI 10
#define DZIELNIK_MODULO 10

//funkcja ktora zwraca karte tworzaca set z 2 wejsciowymi kartami
int potrzebna_karta(int a, int b)
{
    int karta1 = a, karta2 = b, karta_do_seta = 0;
    for (int i = 0; i < LICZBA_CECH_KARTY; i++)
    {
        int potega = 1;
        for (int j = 0; j < i; j++)
        {
            potega *= PODSTAWA_POTEGI;
        }   
        if (((karta1 / potega % DZIELNIK_MODULO) == (karta2 / potega % DZIELNIK_MODULO)))
        {
            karta_do_seta += karta1 / potega % DZIELNIK_MODULO * potega;
        }
        else
        {
            karta_do_seta += (SUMA_1_CECHY - (karta1 / potega % DZIELNIK_MODULO) - (karta2 / potega % DZIELNIK_MODULO)) * potega;
        }
    }
    return karta_do_seta;
}

//funkcja ktora wypisuje pierwszy napotkany set o ile taki istnieje
//i zwraca informajce czy set byl na stole
bool znajdz_set(int *talia, int odkryte_karty)
{
    for (int i = 0; i < odkryte_karty; i++)
    {
        for (int j = i + 1; j < odkryte_karty; j++)
        {
            int karta1 = talia[i], karta2 = talia[j];
            int karta3 = potrzebna_karta(karta1, karta2);
            for (int k = j + 1; k < odkryte_karty; k++)
            {
                if (talia[k] == karta3 && karta3 != 0)
                {
                    printf("- %d %d %d\n", karta1, karta2, karta3);
                    talia[i] = 0;
                    talia[j] = 0;
                    talia[k] = 0;
                    return true;
                }
            }
        }
    }
    return false;
}

//funkcja wypisujaca stan stolu zgodnie ze wzorem
//i zwracająca ile kart wypisala
int wypisz_stol(int *talia, int odkryte_karty)
{
    putchar('=');
    int ile_kart_na_stole = 0;
    for (int i = 0; i < odkryte_karty; i++)
    {
        if (talia[i] != 0)
        {
            printf(" %d", talia[i]);
            ile_kart_na_stole++;
        }
    }
    printf("\n");
    return ile_kart_na_stole;
}

//na poczatku wczytuje dane wejsciowe i umieszczam je w tablicy
//następnie sprawdzam ile mam nadmiarowych kart
//(tych których nie wykładam na stół za pierwszym razem)
//warunek (if (pozostale_karty == 0)) zapewnia mnie ze jesli
// nie mam pozostałych kart to nie wejde do if'a znajdującego się w linii 118
int main(void)
{
    int licznik_kart = 0;
    int talia[WIELKOSC_TALII];
    for (int i = 0; i < WIELKOSC_TALII; i++)
    {
        talia[i] = 0;
        scanf("%d", &talia[i]);
        if (talia[i] != 0)
        {
            licznik_kart++;
        }
    }
    if (licznik_kart <= POCZATKOWE_KARTY)
    {
        licznik_kart = POCZATKOWE_KARTY;
    }
    int pozostale_karty = licznik_kart - POCZATKOWE_KARTY;
    int odkryte_karty = POCZATKOWE_KARTY;
    int ile_kart_na_stole = wypisz_stol(talia, odkryte_karty);
    while (pozostale_karty >= 0)
    {
        while (znajdz_set(talia, odkryte_karty))
        {
            if (pozostale_karty >= DODATKOWE_KARTY && ile_kart_na_stole <= POCZATKOWE_KARTY)
            {
                odkryte_karty += DODATKOWE_KARTY;
                pozostale_karty -= DODATKOWE_KARTY;
            }
            ile_kart_na_stole = wypisz_stol(talia, odkryte_karty);
        }
        if (pozostale_karty == 0)
        {
            pozostale_karty--;
        }
        if (!znajdz_set(talia, odkryte_karty) && pozostale_karty >= 0)
        {
            printf("+\n");
            odkryte_karty += DODATKOWE_KARTY;
            ile_kart_na_stole = wypisz_stol(talia, odkryte_karty);
            pozostale_karty -= DODATKOWE_KARTY;
        }
    }
    return 0;
}