#define _CRT_SECURE_NO_WARNINGS

#ifndef HEADER_H
#define HEADER_H

#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

// External deklaracije za globalne varijable


// Upravljanje memorijom
void alokacijaMemorije(void);
void prosiriNizKnjiga(void);
void prosiriNizPosudbi(void);
void oslobodiMemoriju(void);

// Operacije s datotekama
void spremiSveKnjige(void);
void ucitajSveKnjige(void);
void spremiSvePosudbe(void);
void ucitajSvePosudbe(void);

//CRUD funckije 
void dodajKnjigu(void);
void prikaziKnjige(void);
void pretraziKnjige(void);
void pretraziKnjigeRekurzivno(const char* pojam, int index);
void obrisiKnjigu(void);

// Operacije s posudbama
void posudiKnjigu(void);
void vratiKnjigu(void);
void prikaziPosudbe(void);

// Pomoæne funkcije
void ocistiBuffer(void);

// Izbornik
void prikaziGlavniIzbornik(void);
void pokreniGlavniIzbornik(void);

void ocistiBuffer(void);

//Usporeðivanje po ______
int usporediKnjigePoNaslovu(const void* a, const void* b);
int usporediKnjigePoAutoru(const void* a, const void* b);
int usporediKnjigePoGodini(const void* a, const void* b);


void sortirajKnjige(void);

//Sigurnosne kopije
void sigurnosnaKopija(void);
int napraviSigurnosnuKopiju(const char* izvorniPut, const char* odredisniPut);



#endif