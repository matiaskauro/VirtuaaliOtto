/*
 * VirtuaaliOtto
 * Created by Matias on 2021-12-15.
 * matias@kauro.fi
 */

/*
 * Tama ohjelmointikoodi on toiminut palautettavana
 * lopputyona Oulun ylioposton kurssille Ohjelmointi 1. 
 *
 * Vaatimukset: simulaattorin toimimiseksi tulee samassa tiedostosijainnissa
 * koodin kanssa olla 'tilinumero.acc' tiedosto jossa 1. rivilla
 * on pinkoodi ja toisella rivilla saldo, tiedoston nimi toimii tilinumerona.
 * Tilinumero ja pinkoodi voivat sisaltaa kirjaimia ja numeroita, ja pinkoodi
 * voi itseasiassa olla jopa tyhja.
 * Tilin saldo on pakko olla kokonaisluku (kelpaa myos negatiivinen). Huomaa,
 * että ohjelman toiminta ylikirjoittaa tiedostoon noston ja muutta siten saldoa.
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include <string.h>

void lueRoskat();
void tervetuloa();
void kysyTilinumero();
void kiitosKaynnista();
void rahojenKuittaus();
void printtaaSaldo(int saldo, int nosto);
void onkoMerkkijonoNolla(char tilinumero[256]);
void lueMerkkijono(char merkkijono[], int pituus);
void printtaaSyoteSaldo(int saldo, int nostettavissa);
void jakamisAlgoritmi(int nosto, int nostoajaljella, int s50, int s20);

int lueKokonaisluku(void);
int nostonValidointi(int nosto, int saldo);
int laskePaljonkoNostettavissa(int saldo, int nostettavissa);

int main()
{
  int tunnusluku, yritysta, syote, nosto, saldo, nostettavissa, nostoajaljella, s50, s20, pituus, tulo ;
  char tilinumero[256];
  char pinkoodi[256];
  char tarkistaPinkoodi[256];
  char merkkijono[256];
  FILE *pFile;

  yritysta=3;

  printf("Huom. syöte 0 sulkee ohjelman\n");
  tervetuloa();
  for( ; ; )
  {
    printf("Syota tilinumero: \n");
    fgets(merkkijono, 256, stdin);
    lueMerkkijono(merkkijono,pituus);
    onkoMerkkijonoNolla(merkkijono);

    strcpy(tilinumero, merkkijono);
    strcat(tilinumero, ".acc");

    if ((pFile = fopen(tilinumero, "r")) != NULL)
    {
      printf("Tili loytyi!\n");
      break;
    }

    yritysta--;
    printf("Tilia ei loydy, sinulla on %d yritysta jaljella.\n", yritysta);

    if (yritysta==0)
    {
      printf("Ohjelma sulkeutuu.\n");
      exit(0);
    }
  }

  fgets(tarkistaPinkoodi, 256, pFile);
  lueMerkkijono(tarkistaPinkoodi, pituus);

  yritysta=3;
  for ( ; ; )
  {
    printf("Nappaile tunnusluku: \n");
    fgets(merkkijono, 256, stdin);
    lueMerkkijono(merkkijono,pituus);
    onkoMerkkijonoNolla(merkkijono);
    strcpy(pinkoodi, merkkijono);

    if ((tulo = strcmp(pinkoodi, tarkistaPinkoodi)) == 0)
      {
        break;
      }

      yritysta--;
      printf("Vaara tunnusluku, sinulla on %d yritysta jaljella: \n", yritysta);

      if (yritysta==0)
      {
        printf("Liian monta virheellista yritysta, ohjelma sulkeutuu.\n");
        exit(0);
      }

  }
  printf("Oikea tunnusluku!\n");

  fscanf(pFile, "%d", &saldo);

  printf("1. Otto 2. Saldo 3. Tapahtumat\n");


    printf("Syota numero: \n");
    for(;;)
    {
    syote=lueKokonaisluku();
    if(syote>0&&syote<4)
    break;
    printf("Et osunut valintoihin 1-3. Yrita uudestaan: \n");
  }

  switch (syote)
  {
    case 1:
    printf("Valitsit 1. Otto\n");
    nosto = nostonValidointi(nosto,saldo);

    printf("Haluatko tiedot\n 1.Naytolle\n 2.Kuitille\n 3.Ei kiitos\n");
    printf("Syota numero: \n");
    for(;;)
    {
    syote=lueKokonaisluku();
    if(syote>0&&syote<4)
    break;
    printf("Et osunut valintoihin 1-3. Yrita uudestaan: \n");
  };

    switch (syote)
    {
      case 1:
      printtaaSaldo(saldo, nosto);
      break;

      case 2:
      printf("Valitsit 2.kuitille\n");
      printf("Tiedot tulostetaan kuitille.\n");
      break;

      case 3:
      break;

    }

    rahojenKuittaus();
    jakamisAlgoritmi(nosto, nostoajaljella, s50, s20);
    break;

    case 2:
    printtaaSyoteSaldo(saldo, nostettavissa);
    break;

    case 3:
    printf("Valitsit 3.Tapahtumat\n");
    printf("Toiminto ei viela kaytettavissa!\n");
    break;

  }

  pFile = fopen(tilinumero,"w");
  if(pFile == NULL)
   {
      printf("Tiedostoa ei loydy!\n");
      exit(0);
   }

   fprintf(pFile,"%s\n%d",tarkistaPinkoodi,saldo-nosto);
   fclose(pFile);

   kiitosKaynnista();
   return(0);
}

/*
 * void tervetuloa, kiitosKaynnista ja rahojenKuittaus ovat funktioita
 * jotka sisaltavat erilaisia kayttajelle naytettavia printauksia. Niiden
 * tarkoitus on saastaa main funktion kokoa. Niilla ei ole parametreja, eika
 * paluuarvoja.
 */

void tervetuloa()
{

  printf("Tervetuloa!\n");
  printf("Syota kortti, ole hyva.\n");
}
void kiitosKaynnista()
{
  printf("Kiitos kaynnista.\n");
  printf("Tervetuloa uudelleen!\n");
}
void rahojenKuittaus()
{
  printf("Ota kortti, ole hyva.\n");
  printf("Rahat tulossa...\n");
  printf("Ota rahat, ole hyva!\n");
}

/*
 * void printtaaSyoteSaldo tulostaa kayttajalle tilin saldotiedot
 * void printtaSaldo tulostaa kayttajalle tilin saldotiedot ennen ja noston jalkeen
 *
 * parametrit:
 * paluuarvo: tulosteet (int) saldosta, nostettavissa nostorajasta tai saldosta
 * ja nostosta
 */

void printtaaSyoteSaldo(int saldo, int nostettavissa)
{
  nostettavissa=laskePaljonkoNostettavissa(saldo,nostettavissa);
  printf("Valitsit 2.Saldo\n");
  printf("Tilin saldo: %d euroa\n",saldo);
  printf("Tililta nostettavissa: %d euroa\n",nostettavissa);
}

void printtaaSaldo(int saldo, int nosto)
{
  printf("Valitsit 1.Naytolle\n");
  printf("Tilin saldo enenn nostoa on: %d \n",saldo);
  printf("Noston suuruus: %d \n",nosto);
  printf("Tilin saldo noston jalkeen: %d \n",saldo-nosto);
}

/*
 * nostonValidointi sisaltaa loopin josta paasee pois joko syottamalla 0 tai
 * antamalla validin syotteen. Validi syote tarkoittaa summaa minka automaatin
 * on mahdollista "tulostaa" 20EUR ja 50EUR seteleilla, eika ylita automaatin
 * maksimi nostoa 1000EUR. Taman jalkeen nostoa verrataan viela tilin saldoon
 * silla jos nosto on suurempi kuin tilin saldo ohjataan kayttaja pois ohjelmasta.

 * parametri : nosto (int) ja saldo(int)
 * paluuarvo : tulosteet riippuvaisia tilanteesta, jos nosto sallittu palaa
 * nosto main funktioon sellaisenaan
 */

int nostonValidointi(int nosto, int saldo)
{
  for(; ;)
  {
    printf("Syota nostettava summa: \n");
    nosto=lueKokonaisluku();

    while (nosto>1000)
    {
      printf("Maksimi kateisnosto on 1000EUR.\n");
      printf("Syota nostettava summa uudelleen: \n");
      nosto=lueKokonaisluku();

    }

    if(nosto == 20 || (nosto>=30 && nosto % 10 == 0))
    {
      break;
    }

    printf("Summaa %d ei voi nostaa automaatista silla summaa ei voida jakaa 20EUR ja 50EUR seteleilla.\n",nosto);
    printf("Yrita uudelleen\n...\n");
  }

  if (nosto>saldo)
  {
    printf("Tililla ei tarpeeksi saldoa, ohjelma sulkeutuu.\n");
    exit(0);
  }

  return(nosto);
}

 /*
  * jakamisAlgoritmi saa kayttajalta syotetun noston. Funktion tarkoituksena on
  * laskea se maara seteleita mita tarvitaan tulostettavaksi automaatista
  * vastaamaan kayttajan syottamaa nostoa.

  * parametri: nosto(int)
  * paluuarvo: s50(int) ja s20(int) jotka kertovat kuinka monta 50EUR ja
  * 20 EUR setelia kayttajalle annetaan. Funktio antaa mahdollisimman monta
  * 50 EUR setelia niin, etta kuitenkin koko nosto summa saadaan tulostettua.
  * funktio kuitenkin itsessaan jo tulostaa kyseisen lauseen eika paluuarvoja
  * sinansa palauteta main funktioon.
  */

void jakamisAlgoritmi(int nosto, int nostoajaljella, int s50, int s20)
{
  nostoajaljella=nosto;
  s50=0;
  s20=0;

    while (nostoajaljella>0)
    {
      if (nostoajaljella%50==0)
      {
        while (nostoajaljella>=50)
        {
          nostoajaljella=nostoajaljella-50;
          s50++;
        }
      }
      if (nostoajaljella>=20)
      {
        nostoajaljella=nostoajaljella-20;
        s20++;
      }
    }
    printf("50EUR seteleiden maara:%d\n20EUR seteleiden maara:%d\n", s50, s20);
}

/*
 *lueRoskat tyhjentaa puskurin. Funktio saatu tehtavanannossa.
 */

void lueRoskat(void)
{
  while (fgetc(stdin) != '\n');
}

 /*
  * lueKokonaisluku tarkistaa kayttajan syotteen, jos syote on muuta kuinka
  * kokonaisluku pyydetaan uusi syote. Jos syote on pelkka nolla ohjelma
  * sulkeutuu. Funktio saatu tehtavanannossa.
  */

int lueKokonaisluku(void)
{
  int luku;
  char merkki;
  int tila;

  while ((tila = scanf("%d%c", &luku, &merkki)) == 0 || (2 == tila && merkki != '\n'))
  {
    lueRoskat();
    printf("Ei numero! Yrita uudestaan: ");
  }

  if (luku==0)
  {
    printf("Suljit ohjelman\n");
    exit(0);
  }
  return luku;
}

 /*
  * lueMerkkijono ottaa kayttajan syottaman merkkijonon ja validoi sen
  * kaytettavaan muotoon. Funktio saatu tehtavanannossa.
  */

void lueMerkkijono(char merkkijono[], int pituus)
{
  //fgets(merkkijono, pituus, stdin);

  if (merkkijono[strlen(merkkijono) - 1] == '\n')
  merkkijono[strlen(merkkijono) - 1] = '\0';

  else
  lueRoskat();
}

 /*
  * onkoMerkkijonoNolla tarkistaa onko merkkijono pelkastaa yksi nolla, jos
  * on ohjelma sulkeutuu.
  */

void onkoMerkkijonoNolla(char merkkijono[256])
{
  if(merkkijono[0]=='0' && strlen(merkkijono)==1)
  {
    printf("Suljit ohjelman\n");
    exit(0);
  }
}

/*
 * laskePaljonkoNostettavissa laskee nostettavissa maaraa jos saldo on pienempi
 * kuin 1000EUR
 */

int laskePaljonkoNostettavissa(int saldo, int nostettavissa)
{
  nostettavissa=1000;

  if (saldo<nostettavissa)
  nostettavissa=saldo;

  return(nostettavissa);
}
