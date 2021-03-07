#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define LICZBA_ZMIENNYCH 26
#define ASCII_A 97
#define ASCII_Z 122
#define ASCII_OTWORZ_NAWIAS 40
#define ASCII_ZAMKNIJ_NAWIAS 41
#define NOWA_LINIA '\n'
#define ZNAK_WYPISYWANIA '='
#define MAX_4_CYFRY 9999

typedef enum typ_instrukcji
{
    DODAJ,      
    SKOK,       
    PETLA,      
    ZERUJ,      
    STOP,       
    INKREMENTUJ 
} typ_instrukcji;

typedef struct instrukcja
{
    typ_instrukcji typ;
    int arg1;
    int arg2;
} instrukcja;

typedef struct liczba
{
    int rozmiar;  
    short *cyfry; 
} liczba;

int zwiekszanie(int n)
{
    return 2 * n + 1;
}

//inicjalizuje tablice zmiennych na 0
void inicjalizacja(liczba zmienne[])
{
    for (int i = 0; i < LICZBA_ZMIENNYCH; i++)
    {
        zmienne[i].rozmiar = 1;
        zmienne[i].cyfry = malloc(sizeof(short));
        zmienne[i].cyfry[0] = 0;
    }
}

//funkcja zwalniająca zaalokowaną tablice
void zwolnij_tablice(liczba tab[])
{
    for (int i = 0; i < LICZBA_ZMIENNYCH; i++)
    {
        free(tab[i].cyfry);
    }
}

//funkcja zerująca przekazaną zmienną (ZERUJ/CLR)
void zeruj(liczba *zmienne)
{
    zmienne->rozmiar = 1;
    zmienne->cyfry = realloc(zmienne->cyfry, 1 * sizeof(short));
    zmienne->cyfry[0] = 0;
}

/*
* funkcja dekrementująca przekazaną zmienną
* while sprawdza którą cyfrę można zdekrementować (nie można zdekrementować 0)
* wyznaczając jej pozycję poprzez zmienna "i"
* jeśli w danej komórce tablicy wystąpiły same 0 to po dekrementacji występuje MAX_4_CYFRY
* np jeśli moja liczba to 2 0000 to po dekrementacji będzie to 1 9999
* w przypadku gdy dekrementacja spowodowała pojawienie się w najbardziej znaczącej komórce 0
* zmniejszam jej rozmiar na potrzeby poprawności późniejszych operacji (o ile rozmiar jest różny od 1)
*/
void zdekrementuj(liczba *zmienne)
{
    int i = 0;
    while (zmienne->cyfry[i] == 0)
    {
        i++;
    }
    for (int j = 0; j < i; j++)
    {
        zmienne->cyfry[j] = MAX_4_CYFRY;
    }
    zmienne->cyfry[i]--;
    if (zmienne->cyfry[zmienne->rozmiar - 1] == 0)
    {
        if (zmienne->rozmiar != 1)
        {
            zmienne->rozmiar--;
        }
    }
}

//funkcja wykonująca kod PETLA (DJZ)
//jeśli zmienna jest różna od 0 to dekrementuj
int petla(liczba *zmienne, int arg2, int i)
{
    if (zmienne->rozmiar > 1 || zmienne->cyfry[0] > 0)
    { 
        zdekrementuj(zmienne);
        return i;
    }
    else
    {
        return arg2 - 1;
    }
}

//funkcja wykonująca kod SKOK (JMP)
int skocz(int indeks_kodu_maszynowego)
{
    return indeks_kodu_maszynowego - 1;
}

/*
* funkcja wykonująca kod DODAJ (ADD)
* jesli jest taka potrzeba to reallokuję pamięć zajmowaną przez zmienna do której chcę dodać
* aż będzie wystarczająco duża żeby dodanie odbyło się poprawnie
* następnie dodaje wedle schematu arytmetyki dużych liczb
* na samym końcu reallokuje pamiec jesli jest taka potrzeba i nadpisuje poprawny wynik
*/
void dodaj(liczba *zmienne1, liczba *zmienne2)
{
    if (zmienne1->rozmiar < zmienne2->rozmiar)
    {
        zmienne1->cyfry = realloc(zmienne1->cyfry, (long unsigned int)(zmienne2->rozmiar) * sizeof(short));
        for (int i = zmienne1->rozmiar; i < zmienne2->rozmiar; i++)
        {
            zmienne1->cyfry[i] = 0;
        }

        zmienne1->rozmiar = zmienne2->rozmiar;
    }
    short pomocnicza = 0;
    short suma;
    for (int i = 0; i < zmienne2->rozmiar; i++)
    {
        suma = (short)(zmienne1->cyfry[i] + zmienne2->cyfry[i] + pomocnicza);
        if (suma > MAX_4_CYFRY)
        {
            zmienne1->cyfry[i] = (short)(suma - (MAX_4_CYFRY + 1));
            pomocnicza = 1;
        }
        else
        {
            zmienne1->cyfry[i] = suma;
            pomocnicza = 0;
        }
    }
    if (pomocnicza == 1)
    {
        zmienne1->cyfry = realloc(zmienne1->cyfry, (long unsigned int)(zmienne1->rozmiar + 1) * sizeof(short));
        zmienne1->rozmiar++;
        zmienne1->cyfry[zmienne1->rozmiar - 1] = 1;
    }
}

/*
* funkcja wykonująca kod INKREMENTUJ (INC)
* jesli jest taka potrzeba reallokuje pamięć zajmowaną przez zmienna którą chcę zinkrementować
* aż będzie wystarczająco duża żeby inkrementacja odbyła się poprawnie
* nastepnie ja inkrementuje
*/
void inkrementuj(liczba *zmienne)
{
    bool zwiekszyc_rozmiar = true;
    for (int i = zmienne->rozmiar - 1; i >= 0; i--)
    {
        if (zmienne->cyfry[i] != MAX_4_CYFRY)
        {
            zwiekszyc_rozmiar = false;
        }
    }
    if (zwiekszyc_rozmiar == true)
    {
        zmienne->cyfry = realloc(zmienne->cyfry, (long unsigned int)(zmienne->rozmiar + 1) * sizeof(short));
        for (int i = 0; i < zmienne->rozmiar; i++)
        {
            zmienne->cyfry[i] = 0;
        }
        zmienne->cyfry[zmienne->rozmiar] = 1;
        zmienne->rozmiar++;
    }
    else
    {
        int i = 0;
        while (zmienne->cyfry[i] == MAX_4_CYFRY)
        {
            zmienne->cyfry[i] = 0;
            i++;
        }
        zmienne->cyfry[i]++;
    }
}

//funkcja wypisująca zmienną (wywoływana gdy w danych wejściowych pojawia się ZNAK_WYPISWANIA ('=')
void wypisz_zmienna(liczba zmienne[])
{
    int c;
    c = getchar() - ASCII_A;
    printf("%hu", zmienne[c].cyfry[zmienne[c].rozmiar - 1]);
    for (int i = zmienne[c].rozmiar - 2; i >= 0; i--)
    {
        printf("%04hu", zmienne[c].cyfry[i]);
    }
    printf("\n");
}

//funkcja wykonująca kod maszynowy zapisany w tablicy kod_maszynowy
void wykonaj_kod_maszynowy(instrukcja **kod_maszynowy, liczba zmienne[])
{
    int i = 0;
    while ((*kod_maszynowy)[i].typ != STOP)
    {
        if ((*kod_maszynowy)[i].typ == DODAJ)
        {
            dodaj(&zmienne[(*kod_maszynowy)[i].arg1], &zmienne[(*kod_maszynowy)[i].arg2]);
        }
        else if ((*kod_maszynowy)[i].typ == SKOK)
        {
            i = skocz((*kod_maszynowy)[i].arg1);
        }
        else if ((*kod_maszynowy)[i].typ == PETLA)
        {
            i = petla(&zmienne[(*kod_maszynowy)[i].arg1], (*kod_maszynowy)[i].arg2, i);
        }
        else if ((*kod_maszynowy)[i].typ == ZERUJ)
        {
            zeruj(&zmienne[(*kod_maszynowy)[i].arg1]);
        }
        else if ((*kod_maszynowy)[i].typ == INKREMENTUJ)
        {
            inkrementuj(&zmienne[(*kod_maszynowy)[i].arg1]);
        }
        i++;
    }
}

//funkcja zwiększająca zajmowaną pamięc przez tablice kod_maszynowy
void powieksz_kod_maszynowy(instrukcja **kod_maszynowy, int iteracja, int *rozmiar_kod)
{
    while (*rozmiar_kod <= iteracja)
    {
        *rozmiar_kod = zwiekszanie(*rozmiar_kod);
        *kod_maszynowy = realloc((*kod_maszynowy), (long unsigned int)*rozmiar_kod * sizeof *(*kod_maszynowy));
        for (int j = *rozmiar_kod / 2; j < *rozmiar_kod; j++)
        {
            (*kod_maszynowy)[j].typ = 0;
            (*kod_maszynowy)[j].arg1 = 0;
            (*kod_maszynowy)[j].arg2 = 0;
        }
    }
}
/*
* funkcja zajmująca się translacją danych wejściowych na kod maszynowy rozumiany przez program pętlik
* jeśli pierwszy if jest spełniony tonastępuje wywołanie rekurencyjne funkcji kompiluj, która zwraca
* indeks w tablicy kod_maszynowy od którego powinniśmy zacząć dalsze nadpisywanie tablicy
* jeśli spełniony zostaje else if to następuję optymalizacja kodu maszynowego i zapisanie go w tablicy kod_maszynowy
* lub nadpisanie tablicy instrukcją SKOK
* optymalizacja zachodzi jeśli zostają spełnione 2 warunki wymienione w poleceniu zadania tj.
* i) jeśli nie występuje zagnieżdżona instrukcja Powtarzaj (czyli między '(' i ')' nie ma żadnego innego nawiasu)
* ii) jeśli między '(' i ')' nie występuje zmienna(inna niż zaraz po '(') będąca pierwszym argumentem instrukcji DJZ
* jeśli któryś z powyższych warunków nie jest spełniony to do tablicy kod_maszynowy zostaje wpisana instrukcja SKOK
* jeśli żaden z powyższych ifów nie jest spełniony to następuja inkrementacja zmiennej podanej w danych wejściowych
* po wyjściu z pętli while, pamięc zajmowana przez tablice kod_maszynowy zostaje zwiekszona żeby mieć pewność
* że ostatnia instrukcja (STOP) zostanie poprawnie nadpisana
*/
int kompiluj(instrukcja **kod_maszynowy, liczba zmienne[], int poczatek_iteracji, int *rozmiar_kod)
{
    int c;
    bool petla_w_srodku = false;
    int indeks_powrotu = poczatek_iteracji - 1;
    int iteracja = poczatek_iteracji;
    while ((c = getchar()) != NOWA_LINIA)
    {
        powieksz_kod_maszynowy(kod_maszynowy, iteracja, rozmiar_kod);
        if (c == ASCII_OTWORZ_NAWIAS)
        {
            petla_w_srodku = true;
            c = getchar();
            (*kod_maszynowy)[iteracja].typ = PETLA;
            (*kod_maszynowy)[iteracja].arg1 = c - ASCII_A;
            iteracja = kompiluj(kod_maszynowy, zmienne, iteracja + 1, rozmiar_kod);

        }
        else if (c == ASCII_ZAMKNIJ_NAWIAS)
        {
            bool warunek_optymalizacji2 = false;
            if (!petla_w_srodku)
            { 

                for (int i = poczatek_iteracji; i < iteracja; i++)
                { 
                    if ((*kod_maszynowy)[i].arg1 == (*kod_maszynowy)[indeks_powrotu].arg1)
                    {
                        warunek_optymalizacji2 = true;
                    }
                }
                if (!warunek_optymalizacji2)
                {
                    int zmienna_do_wyzerowania = (*kod_maszynowy)[indeks_powrotu].arg1;
                    for (int i = poczatek_iteracji - 1; i < iteracja - 1; i++)
                    {
                        (*kod_maszynowy)[i].typ = DODAJ;
                        (*kod_maszynowy)[i].arg1 = (*kod_maszynowy)[i + 1].arg1;
                        (*kod_maszynowy)[i].arg2 = zmienna_do_wyzerowania;
                    }
                    (*kod_maszynowy)[iteracja - 1].typ = ZERUJ;
                    (*kod_maszynowy)[iteracja - 1].arg1 = zmienna_do_wyzerowania;
                    return iteracja;
                }
            }
            if (petla_w_srodku || warunek_optymalizacji2)
            {
                (*kod_maszynowy)[iteracja].typ = SKOK;
                (*kod_maszynowy)[iteracja].arg1 = indeks_powrotu;
                (*kod_maszynowy)[iteracja].arg2 = 0;
                (*kod_maszynowy)[indeks_powrotu].arg2 = iteracja + 1;
                return iteracja + 1;
            }
        }
        else
        {
            (*kod_maszynowy)[iteracja].typ = INKREMENTUJ;
            (*kod_maszynowy)[iteracja].arg1 = c - ASCII_A;
            (*kod_maszynowy)[iteracja].arg2 = 0;
            iteracja++;
        }
    }
    powieksz_kod_maszynowy(kod_maszynowy, iteracja, rozmiar_kod);
    (*kod_maszynowy)[iteracja].typ = STOP;
    return -1;
}

void wykonaj_program(liczba zmienne[])
{
    instrukcja *kod_maszynowy = NULL;
    kod_maszynowy = realloc(kod_maszynowy, 1 * sizeof *kod_maszynowy);
    kod_maszynowy[0].arg2 = 0;
    kod_maszynowy[0].arg1 = 0;
    int x = 1;
    kompiluj(&kod_maszynowy, zmienne, 0, &x);
    wykonaj_kod_maszynowy(&kod_maszynowy, zmienne);
    free(kod_maszynowy);
}

int main(void)
{
    liczba zmienne[LICZBA_ZMIENNYCH];
    inicjalizacja(zmienne);
    int c;
    while ((c = getchar()) != EOF)
    {
        if (c == ZNAK_WYPISYWANIA)
        {
            wypisz_zmienna(zmienne);
        }
        else
        {
            ungetc(c, stdin);
            wykonaj_program(zmienne);
        }
    }
    zwolnij_tablice(zmienne);
    return 0;
}