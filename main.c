#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "library.h"


struct MASTER_BOOT {
  unsigned char Instruction_de_saut[3];
  unsigned char Nom_fabricant[8];
  struct Bloc_de_parametres {
    unsigned char Nombre_secteur[2];
    unsigned char Nombre_secteurs_cluster;
    unsigned char Nombre_secteurs_ser[2];
    unsigned char Nombre_FAT;
    unsigned char Nombre_entrées[2]
    unsigned char Nombre_de_secteurs[2];
    unsigned char Descripteur_du_support;
    unsigned char Nombre_de_secteurs_par_FAT[2];
    unsigned char Nombre_secteurs_par_piste[2];
    unsigned char Nombre_de_tetes[2];
    unsigned char Nombre_de_secteurs_avant_partition[4];
    unsigned char Nombre_secteurs_la_partition[4];
  }Bp;
  struct Section_FAT {
    unsigned char Nombre_secteur[4];
    unsigned char Flags[2];
    unsigned char Version[2];
    unsigned char cluster[4];
    unsigned char Numero_de_Secteur[2]
    unsigned char Copie_Secteur[2];
    unsigned char octets_reserves[12];
    unsigned char BIOS_Drive;
    unsigned char Reserve;
    unsigned char Extension;
    unsigned char Numero_de_serie_du_volume[4];
    unsigned char Label_volume[11];
    unsigned char Système_fichier[8];
  }Sfat;
  unsigned char bootstrap[420];
  unsigned char Signature[2];
};





int main() {

   //Afficher_Infos_disque("/dev/sdb");
    Afficher_Secteur_nbOctet("/dev/sdb",2,1);
    //Lire_Bloc("/dev/sdb", 3, 1280);
    return 0;
    
}
