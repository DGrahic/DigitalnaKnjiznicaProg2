#define _CRT_SECURE_NO_WARNINGS
#include "header.h"

// Globalne varijable (vidljive samo u ovoj datoteci)

Knjiga* knjige = NULL;
Posuditi* posudbe = NULL;
int brojKnjiga = 0;
int kapacitetKnjiga = 0;
int brojPosudbi = 0;
int kapacitetPosudbi = 0;


//==================================================================================


void prikaziGlavniIzbornik(void) {
    printf("\n=== DIGITALNA KNJIZNICA ===\n");
    printf("%d. Dodaj knjigu\n", DODAJ_KNJIGU);
    printf("%d. Pregled knjiga\n", PRIKAZI_KNJIGE);
    printf("%d. Pretrazi knjige\n", TRAZI_KNJIGE);
    printf("%d. Posudi knjigu\n", POSUDI_KNJIGU);
    printf("%d. Vrati knjigu\n", VRATI_KNJIGU);
    printf("%d. Pregled posudbi\n", PRIKAZI_POSUDBE);
    printf("%d. Obrisi knjigu\n", OBRISI_KNJIGU);
    printf("%d. Sortiraj knjige\n", SORTIRAJ_KNJIGE);
    printf("%d. Napravi sigurnosnu kopiju\n", SIGURNOSNA_KOPIJA);
    printf("%d. Izlaz\n", IZLAZ);
    printf("Odabir: ");
}


//==================================================================================


void pokreniGlavniIzbornik(void) {
    OpcijeIzbornika odabir;
    do {
        prikaziGlavniIzbornik();
        if (scanf("%d", (int*)&odabir) != 1) {
            ocistiBuffer();
            printf("Nevazeci unos!\n");
            continue;
        }
        ocistiBuffer();

        switch (odabir) {
        case DODAJ_KNJIGU: dodajKnjigu(); break;
        case PRIKAZI_KNJIGE: prikaziKnjige(); break;
        case TRAZI_KNJIGE: pretraziKnjige(); break;
        case POSUDI_KNJIGU: posudiKnjigu(); break;
        case VRATI_KNJIGU: vratiKnjigu(); break;
        case PRIKAZI_POSUDBE: prikaziPosudbe(); break;
        case OBRISI_KNJIGU: obrisiKnjigu(); break;
        case SORTIRAJ_KNJIGE: sortirajKnjige(); break;
        case SIGURNOSNA_KOPIJA: sigurnosnaKopija(); break;
        case IZLAZ:
            printf("\nIzlazim iz programa...\n");
            break;
        default:
            printf("Nevazeci odabir!\n");
        }
    } while (odabir != IZLAZ);

    oslobodiMemoriju();
}


//==================================================================================


void alokacijaMemorije(void) {
    kapacitetKnjiga = POCETNI_KAPACITET;
    kapacitetPosudbi = POCETNI_KAPACITET;

    knjige = (Knjiga*)calloc(kapacitetKnjiga, sizeof(Knjiga));
    if (!knjige) {
        perror("Greska pri alokaciji memorije za knjige");
        exit(EXIT_FAILURE);
    }

    posudbe = (Posuditi*)calloc(kapacitetPosudbi, sizeof(Posuditi));
    if (!posudbe) {
        perror("Greska pri alokaciji memorije za posudbe");
        free(knjige);
        exit(EXIT_FAILURE);
    }
}


//==================================================================================


void ucitajSveKnjige(void) {
    FILE* fp = fopen("knjige.bin", "rb");
    if (!fp) {
        if (errno != ENOENT) {
            perror("Greska pri otvaranju datoteke knjiga");
        }
        return;
    }

    fseek(fp, 0, SEEK_END);
    long velicinaDatoteke = ftell(fp);
    rewind(fp);

    brojKnjiga = velicinaDatoteke / sizeof(Knjiga);

    if (fread(knjige, sizeof(Knjiga), brojKnjiga, fp) != brojKnjiga) {
        if (!feof(fp)) {
            perror("Greska pri citanju knjiga");
        }
    }
    fclose(fp);
}


//==================================================================================


void ucitajSvePosudbe(void) {
    FILE* fp = fopen("posudbe.bin", "rb");
    if (!fp) {
        if (errno != ENOENT) {
            perror("Greska pri otvaranju datoteke posudbi");
        }
        return;
    }

    fseek(fp, 0, SEEK_END);
    long velicinaDatoteke = ftell(fp);
    rewind(fp);

    brojPosudbi = velicinaDatoteke / sizeof(Posuditi);

    if (fread(posudbe, sizeof(Posuditi), brojPosudbi, fp) != brojPosudbi) {
        if (!feof(fp)) {
            perror("Greska pri citanju posudbi");
        }
    }
    fclose(fp);
}


//==================================================================================


void oslobodiMemoriju(void) {

    if (knjige) {
        free(knjige);
        knjige = NULL;
    }

    if (posudbe) {
        free(posudbe);
        posudbe = NULL;
    }

    brojKnjiga = brojPosudbi = 0;
    kapacitetKnjiga = kapacitetPosudbi = 0;
}


//==================================================================================


void ocistiBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


//==================================================================================


void dodajKnjigu(void) {

    Knjiga novaKnjiga;
    printf("\n--- DODAVANJE NOVE KNJIGE ---\n");

    printf("Naslov: ");
    fgets(novaKnjiga.naslov, MAX_NASLOV, stdin);
    novaKnjiga.naslov[strcspn(novaKnjiga.naslov, "\n")] = '\0';

    printf("Autor: ");
    fgets(novaKnjiga.autor, MAX_AUTOR, stdin);
    novaKnjiga.autor[strcspn(novaKnjiga.autor, "\n")] = '\0';

    printf("Genre: ");
    fgets(novaKnjiga.zanr, MAX_GENRE, stdin);
    novaKnjiga.zanr[strcspn(novaKnjiga.zanr, "\n")] = '\0';

    printf("Godina izdanja: ");
    scanf("%d", &novaKnjiga.godinaIzdanja);
    ocistiBuffer();

    novaKnjiga.status = DOSTUPNO;

    knjige[brojKnjiga++] = novaKnjiga;
    spremiSveKnjige();
    printf("Knjiga uspjesno dodana!\n");
}


//==================================================================================


void prikaziKnjige(void) {
    printf("\n--- POPIS KNJIGA (%d) ---\n", brojKnjiga);
    for (int i = 0; i < brojKnjiga; i++) {
        printf("%d. %s - %s (%s, %d) - %s\n",
            i + 1, knjige[i].naslov, knjige[i].autor,
            knjige[i].zanr, knjige[i].godinaIzdanja,
            knjige[i].status == DOSTUPNO ? "Dostupno" : "Posudjeno");
    }
}


//==================================================================================


void prikaziPosudeneKnjige(void) {
    printf("\n--- POPIS KNJIGA (%d) ---\n", brojKnjiga);
    for (int i = 0; i < brojPosudbi; i++) {
        if (knjige[i].status == POSUDJENO) {
            printf("%d. %s - %s (%s, %d) - %s\n",
                i + 1, knjige[i].naslov, knjige[i].autor,
                knjige[i].zanr, knjige[i].godinaIzdanja,
                knjige[i].status);
        }
    }
}


//==================================================================================


void spremiSveKnjige(void) {
    // Napravi backup prije spremanja
    napraviSigurnosnuKopiju("knjige.bin", "knjige_backup.bin");

    FILE* fp = fopen("knjige.bin", "wb");
    if (!fp) {
        perror("Greska pri otvaranju datoteke knjiga");
        return;
    }

    if (fwrite(knjige, sizeof(Knjiga), brojKnjiga, fp) != brojKnjiga) {
        perror("Greska pri pisanju knjiga");
        // Vrati backup ako je doslo do greske
        rename("knjige_backup.bin", "knjige.bin");
    }
    fclose(fp);
}


//==================================================================================


void pretraziKnjige(void) {
    char pojam[MAX_NASLOV];
    printf("\n--- PRETRAGA KNJIGA ---\n");
    printf("Unesite pojam za pretragu (naslov, autor ili zanr): ");

    fgets(pojam, sizeof(pojam), stdin);
    pojam[strcspn(pojam, "\n")] = '\0';

    // Pozivamo rekurzivnu funkciju za pretrazivanje
    pretraziKnjigeRekurzivno(pojam, 0);
}

void pretraziKnjigeRekurzivno(const char* pojam, int index) {
    if (index >= brojKnjiga) {
        return; // Kraj rekurzije
    }

    if (strstr(knjige[index].naslov, pojam) ||
        strstr(knjige[index].autor, pojam) ||
        strstr(knjige[index].zanr, pojam)) {

        printf("%d. %s - %s (%s, %d) - %s\n",
            index + 1, knjige[index].naslov, knjige[index].autor,
            knjige[index].zanr, knjige[index].godinaIzdanja,
            knjige[index].status == DOSTUPNO ? "Dostupno" : "Posudjeno");
    }

    if (index == brojKnjiga - 1) {
        printf("Knjiga s tim imenom ne postoji.\n");
    }

    // Rekurzivni poziv za sljedeci indeks
    pretraziKnjigeRekurzivno(pojam, index + 1);
}


//==================================================================================


void posudiKnjigu(void) {
    int odabir;
    char korisnik[MAX_KORISNIK];
    char datumPosudbe[DULJINA_DATUMA];

    printf("\n--- POSUDBA KNJIGE ---\n");
    prikaziKnjige();

    printf("Unesite broj knjige koju zelite posuditi: ");
    if (scanf("%d", &odabir) != 1 || odabir < 1 || odabir > brojKnjiga) {
        ocistiBuffer();
        printf("Neispravan unos.\n");
        return;
    }
    ocistiBuffer();
    odabir--;

    if (knjige[odabir].status == POSUDJENO) {
        printf("Knjiga je vec posudjena.\n");
        return;
    }

    printf("Unesite ime korisnika: ");
    fgets(korisnik, sizeof(korisnik), stdin);
    korisnik[strcspn(korisnik, "\n")] = '\0';

    printf("Unesite datum posudbe (npr. 23.5.2025.): ");
    fgets(datumPosudbe, sizeof(datumPosudbe), stdin);
    datumPosudbe[strcspn(datumPosudbe, "\n")] = '\0';

    if (brojPosudbi >= kapacitetPosudbi) {
        kapacitetPosudbi *= 2;
        posudbe = realloc(posudbe, kapacitetPosudbi * sizeof(Posuditi));
        if (!posudbe) {
            perror("Greska pri prosirenju memorije za posudbe");
            exit(EXIT_FAILURE);
        }
    }

    Posuditi novaPosudba;
    strncpy(novaPosudba.korisnik, korisnik, MAX_KORISNIK);
    strncpy(novaPosudba.naslovKnjige, knjige[odabir].naslov, MAX_NASLOV);
    strncpy(novaPosudba.datumPosudbe, datumPosudbe, DULJINA_DATUMA);
    novaPosudba.datumVracanja[0] = '\0'; // jos nije vraceno

    posudbe[brojPosudbi++] = novaPosudba;
    knjige[odabir].status = POSUDJENO;

    spremiSveKnjige();
    spremiSvePosudbe();

    printf("Knjiga uspjesno posudjena!\n");
}


//==================================================================================


void vratiKnjigu(void) {
    char naslov[MAX_NASLOV];
    char datumVracanja[DULJINA_DATUMA];
    bool pronadjeno = false;

    printf("\n--- VRACANJE KNJIGE ---\n");
    prikaziKnjige();

    printf("\nUnesite naslov knjige koju zelite vratiti: ");
    fgets(naslov, sizeof(naslov), stdin);
    naslov[strcspn(naslov, "\n")] = '\0';

    for (int i = 0; i < brojPosudbi; i++) {
        if (strcmp(posudbe[i].naslovKnjige, naslov) == 0 && posudbe[i].datumVracanja[0] == '\0') {
            printf("Unesite datum vracanja (npr. 22.5.2025.): ");
            fgets(datumVracanja, sizeof(datumVracanja), stdin);
            datumVracanja[strcspn(datumVracanja, "\n")] = '\0';
            strncpy(posudbe[i].datumVracanja, datumVracanja, DULJINA_DATUMA);

            // Oznaci knjigu kao dostupnu
            for (int j = 0; j < brojKnjiga; j++) {
                if (strcmp(knjige[j].naslov, naslov) == 0) {
                    knjige[j].status = DOSTUPNO;
                    break;
                }
            }

            spremiSveKnjige();
            spremiSvePosudbe();

            printf("Knjiga uspjesno vracena!\n");
            pronadjeno = true;
            break;
        }
    }

    if (!pronadjeno) {
        printf("Knjiga nije pronadjena ili je vec vracena.\n");
    }
}


//==================================================================================


void spremiSvePosudbe(void) {
    // Napravi backup prije spremanja
    napraviSigurnosnuKopiju("posudbe.bin", "posudbe_backup.bin");

    FILE* fp = fopen("posudbe.bin", "wb");
    if (!fp) {
        perror("Greska pri otvaranju datoteke posudbi");
        return;
    }

    if (fwrite(posudbe, sizeof(Posuditi), brojPosudbi, fp) != brojPosudbi) {
        perror("Greska pri pisanju posudbi");
        // Vrati backup ako je doslo do greske
        rename("posudbe_backup.bin", "posudbe.bin");
    }
    fclose(fp);
}


//==================================================================================


void obrisiKnjigu(void) {
    prikaziKnjige();
    int indeks;
    printf("Odaberite broj knjige koji zelite obrisati: ");
    scanf("%d", &indeks);
    indeks--;

    if (indeks < 0 || indeks >= brojKnjiga) {
        printf("Nevazeci indeks knjige!\n");
        return;
    }

    // Provjeri je li knjiga trenutno posudena
    if (knjige[indeks].status == POSUDJENO) {
        printf("Knjiga je trenutno posudena i ne moze se obrisati!\n");
        return;
    }

    // Pomakni sve knjige iza ove za jedno mjesto unatrag
    for (int i = indeks; i < brojKnjiga - 1; i++) {
        knjige[i] = knjige[i + 1];
    }

    brojKnjiga--;

    // Azuriraj posudbe koje se odnose na ovu knjigu
    for (int i = 0; i < brojPosudbi; i++) {
        if (strcmp(posudbe[i].naslovKnjige, knjige[indeks].naslov) == 0) {
            // Pomakni sve posudbe iza ove za jedno mjesto unatrag
            for (int j = i; j < brojPosudbi - 1; j++) {
                posudbe[j] = posudbe[j + 1];
            }
            brojPosudbi--;
            i--; // Vrati se jedan korak jer smo smanjili niz
        }
    }

    spremiSveKnjige();
    spremiSvePosudbe();
    printf("Knjiga uspjesno obrisana!\n");
}


//==================================================================================


void prikaziPosudbe(void) {
    if (brojPosudbi == 0) {
        printf("\nNema evidentiranih posudbi.\n");
        return;
    }

    printf("\n--- SVE POSUDBE (%d) ---\n", brojPosudbi);
    for (int i = 0; i < brojPosudbi; i++) {
        printf("%d. %s - %s\n", i + 1, posudbe[i].korisnik, posudbe[i].naslovKnjige);
        printf("   Posudjeno: %s\n", posudbe[i].datumPosudbe);
        if (posudbe[i].datumVracanja[0] != '\0') {
            printf("   Vraceno: %s\n", posudbe[i].datumVracanja);
        }
        else {
            printf("   Status: U posudi\n");
        }
        printf("--------------------------------\n");
    }
}


//==================================================================================


void sortirajKnjige() {
    if (!knjige || brojKnjiga == 0) {
        printf("Nema knjiga za sortiranje!\n");
        return;
    }

    printf("\n--- SORTIRANJE KNJIGA ---\n");
    printf("1. Po naslovu\n");
    printf("2. Po autoru\n");
    printf("3. Po godini izdanja\n");
    printf("Odabir: ");

    int odabir;
    if (scanf("%d", &odabir) != 1 || odabir < 1 || odabir > 3) {
        ocistiBuffer();
        printf("Neispravan odabir!\n");
        return;
    }
    ocistiBuffer();

    // Debug: Print before sorting
    printf("\n--- Prije sortiranja ---\n");
    for (int i = 0; i < brojKnjiga; i++) {
        printf("%d. %s - %s (%d)\n", i + 1, knjige[i].naslov, knjige[i].autor, knjige[i].godinaIzdanja);
    }

    switch (odabir) {
    case 1:
        qsort(knjige, brojKnjiga, sizeof(Knjiga), usporediKnjigePoNaslovu);
        printf("\nKnjige sortirane po naslovu.\n");
        break;
    case 2:
        qsort(knjige, brojKnjiga, sizeof(Knjiga), usporediKnjigePoAutoru);
        printf("\nKnjige sortirane po autoru.\n");
        break;
    case 3:
        qsort(knjige, brojKnjiga, sizeof(Knjiga), usporediKnjigePoGodini);
        printf("\nKnjige sortirane po godini izdanja.\n");
        break;
    }

    // Debug: Print after sorting
    printf("\n--- Nakon sortiranja ---\n");
    for (int i = 0; i < brojKnjiga; i++) {
        printf("%d. %s - %s (%d)\n", i + 1, knjige[i].naslov, knjige[i].autor, knjige[i].godinaIzdanja);
    }

    spremiSveKnjige();
}


//==================================================================================


int usporediKnjigePoNaslovu(const void* a, const void* b) {
    const Knjiga* knjigaA = (const Knjiga*)a;
    const Knjiga* knjigaB = (const Knjiga*)b;
    return strcmp(knjigaA->naslov, knjigaB->naslov);
}

int usporediKnjigePoAutoru(const void* a, const void* b) {
    const Knjiga* knjigaA = (const Knjiga*)a;
    const Knjiga* knjigaB = (const Knjiga*)b;
    return strcmp(knjigaA->autor, knjigaB->autor);
}

int usporediKnjigePoGodini(const void* a, const void* b) {
    const Knjiga* knjigaA = (const Knjiga*)a;
    const Knjiga* knjigaB = (const Knjiga*)b;
    return knjigaA->godinaIzdanja - knjigaB->godinaIzdanja;
}


//==================================================================================


int preimenujKnjigu(const char* oldTitle, const char* newTitle) {
    // Implement the rename functionality using rename()
    char oldFilename[MAX_NASLOV];
    char newFilename[MAX_NASLOV];
    snprintf(oldFilename, sizeof(oldFilename), "%s.bin", oldTitle);
    snprintf(newFilename, sizeof(newFilename), "%s.bin", newTitle);
    return rename(oldFilename, newFilename);
}


//==================================================================================


int napraviSigurnosnuKopiju(const char* izvorniPut, const char* odredisniPut) {
    FILE* izvor = NULL, * odrediste = NULL;
    char buffer[8192]; // 8KB buffer
    size_t procitano;

    // Otvori izvornu datoteku
    izvor = fopen(izvorniPut, "rb");
    if (izvor == NULL) {
        perror("Greska pri otvaranju izvorne datoteke (ili se datoteka otvara prvi put)");
        return -1;
    }

    // Otvori odredisnu datoteku
    odrediste = fopen(odredisniPut, "wb");
    if (odrediste == NULL) {
        perror("Greska pri otvaranju odredisne datoteke");
        fclose(izvor);
        return -1;
    }

    // Kopiraj podatke
    while ((procitano = fread(buffer, 1, sizeof(buffer), izvor)) > 0) {
        if (fwrite(buffer, 1, procitano, odrediste) != procitano) {
            perror("Greska pri pisanju u odredisnu datoteku");
            fclose(izvor);
            fclose(odrediste);
            remove(odredisniPut); // Obrisi nepotpunu kopiju
            return -1;
        }
    }

    // Provjeri greske nakon citanja
    if (ferror(izvor)) {
        perror("Greska pri citanju izvorne datoteke");
        fclose(izvor);
        fclose(odrediste);
        remove(odredisniPut);
        return -1;
    }

    // Zatvori datoteke
    if (fclose(izvor) != 0) {
        perror("Greaka pri zatvaranju izvorne datoteke");
    }

    if (fclose(odrediste) != 0) {
        perror("Greska pri zatvaranju odredisne datoteke");
        remove(odredisniPut);
        return -1;
    }

    return 0;
}


//==================================================================================


void sigurnosnaKopija() {
    if (napraviSigurnosnuKopiju("knjige.bin", "knjige_backup.bin") == 0 &&
        napraviSigurnosnuKopiju("posudbe.bin", "posudbe_backup.bin") == 0) {
        printf("Sigurnosna kopija uspjesno kreirana!\n");
    }
    else {
        printf("Greska pri kreiranju sigurnosne kopije!\n");
    }
}
