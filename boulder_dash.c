#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define GORA 'w'
#define DOL 's'
#define LEWO 'a'
#define PRAWO 'd'
#define ROCKFORD '@'
#define WYJSCIE 'X'
#define PUSTE ' '
#define DIAMENT '$'
#define KAMIEN 'O'
#define ZIEMIA '+'
#define KONIEC_RUCHU '\n'


//Funkcja wczytuje planszę z danych wejściowych.
void wczytaj(char **tab, int w, int k)
{
    char enter;
    scanf("%c", &enter);
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < k; j++)
        {
            scanf("%c", &tab[i][j]);
        }
    }
}

//Funkcja wypisuje obecny stan planszy.
void wypisz(char **tab, int w, int k)
{
    printf("%d %d\n", w, k - 1);
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < k; j++)
        {
            printf("%c", tab[i][j]);
        }
    }
}

//Funkcja zwalnia zaalokowaną pamięć.
void zwolnij(char **tab, int w)
{
    for (int i = 0; i < w; i++)
    {
        free(tab[i]);
    }
    free(tab);
}

//Funkcja sprowadza planszę do stanu stabilnego.
void stabilizuj(char **tab, int w, int k)
{
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < k - 2; j++)
        {
            if (tab[i][j] == KAMIEN && tab[i + 1][j] == PUSTE)
            //powoduje że KAMIEN pod którym jest PUSTE spada
            {
                tab[i][j] = PUSTE;
                tab[i + 1][j] = KAMIEN;
                while (tab[i - 1][j] == PUSTE)
                //pętla zapewnia mnie że każdy KAMIEŃ pod którym jest PUSTE spadnie
                //(nawet jeśli są jeden na drugim)
                {
                    i--;
                }
                i--;
            }
            if (tab[i][j] == DIAMENT && tab[i + 1][j] == PUSTE)
            //powoduje że DIAMENT pod którym jest PUSTE spada
            {
                tab[i][j] = PUSTE;
                tab[i + 1][j] = DIAMENT;
                while (tab[i - 1][j] == DIAMENT)
                //pętla zapewnia mnie że każdy DIAMENT pod którym jest PUSTE spadnie
                //(nawet jeśli są jeden na drugim)
                {
                    i--;
                }
                i--;
            }
        }
    }
}

//Funkcja znajduje współrzędną X Rockforda i ją zwraca,
//jeśli Rockforda nie ma na planszy to zwraca 0.
int znajdz_x(char **tab, int w, int k)
{
    int x = 0;
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < k - 2; j++)
        {
            if (tab[i][j] == ROCKFORD)
            {
                x = j;
            }
        }
    }
    return x;
}

//Funkcja znajduje współrzędną Y Rockforda i ją zwraca,
//jeśli Rockforda nie ma na planszy to zwraca 0.
int znajdz_y(char **tab, int w, int k)
{
    int y = 0;
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < k - 2; j++)
        {
            if (tab[i][j] == ROCKFORD)
            {
                y = i;
            }
        }
    }
    return y;
}

//Funkcja przelicza ile diamentów znajduje się na planszy i zwraca ich ilośc.
int licznik_diamentow(char **tab, int w, int k)
{
    int licznik = 0;
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < k - 2; j++)
        {
            if (tab[i][j] == DIAMENT)
            {
                licznik++;
            }
        }
    }
    return licznik;
}

/*Funkcja powoduje przesunięcię się Rockforda na planszy, w zależności od wartości
* przekazywanej zmiennej "nastepny_ruch", dodatkowo do funkcji przekazywane są
* aktualne współrzedne Rockforda na planszy, ilość pozostałych diamentów, oraz zmienna "koniec ruchów"
* która określa czy Rockford może się poruszyć (Rockford nie może się ruszać jeśli nie ma go na planszy)
* to jak zmieni się położenie Rockforda determinują zmienne pion i poziom oraz pole na które chce poruszyć
* się Rockford. Zmienne pion i poziom determinowane są przez zmienną "następny ruch". Rockford może poruszyć
* się tylko w określonych przypadkach, które są opisane w treści zadania.
*/
void ruch(char **tab, int *rockf_x, int *rockf_y, char nastepny_ruch, int *diamenty, int *koniec_ruchow)
{
    int pion = 0, poziom = 0, x = *rockf_x, y = *rockf_y, diamenty_ruch = *diamenty, mozliwosc_ruchu = *koniec_ruchow;
    if (x == 0 || y == 0)
    {
        mozliwosc_ruchu = 1;
    }
    if (mozliwosc_ruchu == 0)
    {
        if (nastepny_ruch == LEWO)
        {
            pion = 0;
            poziom = -1;
        }
        if (nastepny_ruch == GORA)
        {
            pion = -1;
            poziom = 0;
        }
        if (nastepny_ruch == DOL)
        {
            pion = 1;
            poziom = 0;
        }
        if (nastepny_ruch == PRAWO)
        {
            pion = 0;
            poziom = 1;
        }
    }
    //pozycja Rockforda to tab[y][x]
    //jego docelowe położenie będzie miało postać tab[y+pion][x+poziom],
    //(o ile zajdzie któryś z warunków zmiany położenia)

    //warunek zmiany położenia dotyczący ZIEMIA, DIAMENT, PUSTE
    if (tab[y + pion][x + poziom] == ZIEMIA || tab[y + pion][x + poziom] == DIAMENT || tab[y + pion][x + poziom] == PUSTE)
    {
        if (tab[y + pion][x + poziom] == DIAMENT)
        {
            diamenty_ruch--;
        }
        tab[y][x] = PUSTE;
        x += poziom;
        y += pion;
        tab[y][x] = ROCKFORD;
    }

    //warunek zmiany położenia dotyczący KAMIEN w poziomie
    else if (tab[y][x + poziom] == KAMIEN && tab[y][x + 2 * poziom] == PUSTE)
    {
        tab[y][x] = PUSTE;
        x += poziom;
        tab[y][x] = ROCKFORD;
        tab[y][x + poziom] = KAMIEN;
    }
    //warunek zmiany polożenia dotyczący WYJSCIE
    else if (diamenty_ruch == 0 && tab[y + pion][x + poziom] == WYJSCIE)
    {
        tab[y][x] = PUSTE;
        mozliwosc_ruchu = 1;
    }

    //nadpisujemy aktualne położenie Rockforda, ilość pozostałych na planszy diamentów
    //oraz zmienną mówiącą o możliwości zmiany położenia przez Rockforda
    *rockf_x = x;
    *rockf_y = y;
    *diamenty = diamenty_ruch;
    *koniec_ruchow = mozliwosc_ruchu;
}

/*Funkcja wczytuje kolejne dane wejściowe i zmienia położenie Rockforda
* (o ile zostaną spełnione warunki z treści zadania) lub wypisuje plansze.
* Funkcja kończy swoje działanie gdy nie ma więcej danych wejściowych.
*/
void graj(char **tab, int w, int k)
{
    stabilizuj(tab, w, k);
    wypisz(tab, w, k);
    int diamenty = licznik_diamentow(tab, w, k);
    int rockf_x = znajdz_x(tab, w, k);
    int rockf_y = znajdz_y(tab, w, k);
    int koniec_ruchow = 0;
    char nastepny_ruch;
    while (scanf("%c", &nastepny_ruch) != EOF)
    {
        if (nastepny_ruch == LEWO || nastepny_ruch == GORA || nastepny_ruch == DOL || nastepny_ruch == PRAWO)
        {
            ruch(tab, &rockf_x, &rockf_y, nastepny_ruch, &diamenty, &koniec_ruchow);
            stabilizuj(tab, w, k);
        }
        if (nastepny_ruch == KONIEC_RUCHU)
        {
            wypisz(tab, w, k);
        }
    }
}

int main(void)
{
    int w, k;
    scanf("%d %d", &w, &k);
    k++;
    //inkrementacja jest potrzebna, ponieważ na końcu każdego
    //wiersza w danych wejściowych znajduje się '\n'
    char **tab = malloc((size_t)w * sizeof(char *));
    for (int i = 0; i < w; i++)
    {
        tab[i] = malloc((size_t)k * sizeof(char));
    }
    wczytaj(tab, w, k);
    graj(tab, w, k);
    zwolnij(tab, w);
    return 0;
}