#define _CRT_SECURE_NO_WARNINGS

#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stdbool.h>

#define MAX_NASLOV 100
#define MAX_AUTOR 50
#define MAX_GENRE 30
#define MAX_KORISNIK 50
#define DULJINA_DATUMA 11
#define POCETNI_KAPACITET 10

typedef enum {
    DOSTUPNO,
    POSUDJENO
} StatusKnjige;

typedef enum {
    DODAJ_KNJIGU,
    PRIKAZI_KNJIGE,
    TRAZI_KNJIGE,
    POSUDI_KNJIGU,
    VRATI_KNJIGU,
    PRIKAZI_POSUDBE,
    OBRISI_KNJIGU,
    SORTIRAJ_KNJIGE,
    SIGURNOSNA_KOPIJA,
    IZLAZ
} OpcijeIzbornika;

typedef enum {
    SORTIRAJ_PO_NASLOVU,
    SORTIRAJ_PO_AUTORU,
    SORTIRAJ_PO_GODINI
} NacinSortiranja;

typedef struct {
    char naslov[MAX_NASLOV];
    char autor[MAX_AUTOR];
    char zanr[MAX_GENRE];
    int godinaIzdanja;
    StatusKnjige status;
} Knjiga;

typedef struct {
    char korisnik[MAX_KORISNIK];
    char naslovKnjige[MAX_NASLOV];
    char datumPosudbe[POCETNI_KAPACITET];
    char datumVracanja[DULJINA_DATUMA];
} Posuditi;

// External deklaracije za globalne varijable
extern Knjiga* knjige;
extern Posuditi* posudbe;
extern int brojKnjiga;
extern int kapacitetKnjiga;
extern int brojPosudbi;
extern int kapacitetPosudbi;

#endif
