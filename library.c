#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct BOOT {
  unsigned char prog[446];
  struct partition {
    unsigned char etat;
    unsigned char chs1[3];
    unsigned char type;
    unsigned char chs2[3];
    unsigned char adrlba[4];
    unsigned char taille[4];
  }tp[4];
  unsigned char aa55[2];
};








unsigned char* Lire_Bloc(char disque_physique[], long int Num_Bloc, long int TailleBloc){
    int n,s;
    unsigned char* buffer = malloc(TailleBloc * sizeof(unsigned char)); // Allocate memory for the character array

    FILE *disk=NULL;
    disk = fopen(disque_physique, "rb"); /* Linux */
    if(disk == NULL) printf("\n Erreur le disque physique1 n'est pas ouvert\n" );
    else{ 
      s=fseek(disk, TailleBloc*Num_Bloc, SEEK_SET); /*seek par rapport au début du fichier : 0 */
      if(s!=0) printf("\n Erreur de fseek : s= %d",s);
      else{ 
        n=fread(buffer,TailleBloc, 1, disk);
        if(n<=0) printf("\n Erreur de fread = %d ",n);
        else {
          printf("\n Lecture du Bloc %ld; Nombre d’éléments lus = %ld\n",Num_Bloc,n*TailleBloc); 
          printf("Position Actuelle = %ld\n",ftell(disk));
          for(int j=0; j<TailleBloc; j++) printf("%02x", buffer[j]);
        }
      }
    }
    fclose(disk);
    return buffer;   
}

unsigned char* Lire_Secteur(char disque_physique[], long int Num_sect){
    int n,s;
    unsigned char* buffer = malloc(512 * sizeof(unsigned char)); // Allocate memory for the character array
    //long int a=0; /* secteur 10 */
    //char diskname[] = "/dev/sdb";
    FILE *disk=NULL;
    disk = fopen(disque_physique, "rb"); /* Linux */
    if(disk == NULL) printf("\n Erreur le disque physique1 n'est pas ouvert\n" );
    else{ 
      s=fseek(disk, 512*Num_sect, SEEK_SET); /*seek par rapport au début du fichier : 0 */
      if(s!=0) printf("\n Erreur de fseek : s= %d",s);
      else{ 
        n=fread(buffer,512, 1, disk);
        if(n<=0) printf("\n Erreur de fread = %d ",n);
        else {
          printf("\n Lecture du secteur 10; Nombre d’éléments lus = %d\n",n); 
          //for(int j=0; j<512; j++) printf("%02x", buffer[j]);
        }
      }
    }
    fclose(disk);
    return buffer;   
}
int LectureSequntielle(char filename[]){
    int n,s,i,j;
    unsigned char buffer[512]; /* bloc = 512 octets*/
    FILE *disk;
    //char filename[] = "/dev/sdb";

    disk = fopen(filename, "rb");
    if (disk == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }
    else {
      for( i=0; i<10; i++){
        n=fread(buffer, 512, 1, disk); /*lecture d'un enregistrement ou bloc de 512 octets*/
        if(n<=0) printf("\n Erreur de fread = %d ",n);
        else{
          printf("\n Secteur%d: Nombre d'elements lus = %d\n",i,n);
          for(j=0; j<512; j++) printf("%02x", buffer[j]);
        }
      }
    }

    // File operations here
    fclose(disk);
    return 0 ; 
}

void Afficher_Secteur (char disque_physique[], long int Num_sect){
  unsigned char* buffer = malloc(512 * sizeof(unsigned char));
  buffer = Lire_Secteur(disque_physique,Num_sect);
  for(int j=0; j<512; j++) printf("%02x", buffer[j]);
}

void Afficher_Secteur_nbOctet (char disque_physique[], long int Num_sect, int nbOctet){
  unsigned char* buffer = malloc(512 * sizeof(unsigned char));
  buffer = Lire_Secteur(disque_physique,Num_sect);
  nbOctet = nbOctet*8;
  for(int j=0; j<512; j=j+nbOctet){
    printf("\n %4d  :",j);
    for(int i=0; i<nbOctet && j+i<512; i++) printf("%x", buffer[i]);
  }
}


void Liste_Disques(){
  FILE *disk = NULL;
    char filename[] = "/dev/sd";
    char currentChar[] = "abcdefghijklmnopqrstuvwxyz";
    char charcter[2];
    charcter[1] = '\0';
    char diskname[10];
    for (int i = 0; i < 26; i++) {
        strcpy(diskname, filename);
        charcter[0] = currentChar[i];
        strcat(diskname,charcter);
        disk = fopen(diskname, "rb");
   
        if (disk == NULL) {
          printf("file %s is not connected\n",diskname);
        } else {
          printf("file %s is connected\n",diskname);
          fclose(disk);
          disk = NULL;
        }
    }
}




void Partition_etundue(long int adrlba, int *j,FILE  *disque){
  struct BOOT ebr;
  unsigned char etat ; 
  unsigned char fs; 
  unsigned  int adr ; 
  unsigned  int taille ; 
  int nl, err ; 
  double x,y,t;
 
  err =fseek(disque,adrlba*512 , SEEK_SET);
  if(err!=0){
    printf("\n Erreuer la deplacement n'est pas effectue ");
    exit(0);
  }
  nl=fread(&ebr,512,1,disque);
   if(nl<=0){
    printf("\n erreur de lecture n = %d ",nl);
    exit(0);
  }
  x=1024*1024*1024;
  adr = *(int *)&(ebr.tp[0].adrlba);
  taille =*( int *)&(ebr.tp[0].taille);
  etat = ebr.tp[0].etat ; 
  fs=ebr.tp[0].type;
  y=(double)taille*512 ; 
  t=y/x;
  
  printf("\n     sdb%d     |   x'%02x'   | x'%02x'     | %9d          | %9d            | %8.3f  |",*j, etat, fs, adr,taille,t ); printf("\n|----------------|-------------|-------------|--------------|-----------------|-------------|");
  *j = *j + 1;

  if(ebr.tp[1].type == 0x05){
    adr = *(int *)&(ebr.tp[1].adrlba);
    Partition_etundue(adr+adrlba,j, disque);
  }
}




struct BOOT Lecture_MBR(FILE *disque){
  struct BOOT br;
  int nl; 
    /* Lecture de MBR de flashdisk */
  //nl= fread(&mbr,512,1,disque);
  nl = fread(&br, 512, 1, disque);
  if(nl<=0){
    printf("\n erreur de lecture n = %d ",nl);
    exit(0);
  }
  return br;
}






void Afficher_Infos_disque(char disque_physique[]){
  unsigned char etat ; 
  unsigned char fs; 
  unsigned  int adr ; 
  unsigned  int taille ; 
  int err, i ,j ; 
  double x,y,t;
  FILE *disque ;
  struct BOOT mbr;
  
  disque = fopen(disque_physique,"rb");
  if(disque ==NULL){
    printf("le disque n'est pas ouvert ");
    exit(0);
  }
  mbr = Lecture_MBR(disque);
  
  printf("\n\n\t\t********************* Table de Partition du disque *********************\n ");
  printf("\n|----------------|--------------|-------------|--------------|--------------------|--------------|");
  printf("\n    Partition    |     Etat     |   Type SF   | Addresse LBA |  Nbres de secteurs | taille en GO |");
printf("\n|----------------|-------------|-------------|--------------|-----------------|-------------|");

  j=1 ; 
  x=1024*1024*1024;
  for(i=0 ; i<4;i++){
    //Afficher_Info_Partition(struct BOOT br,int i)
    adr=*(int *)&(mbr.tp[i].adrlba);
    taille =*( int *)&(mbr.tp[i].taille);
    etat = mbr.tp[i].etat ; 
    fs=mbr.tp[i].type;
    y=(double)taille*512 ; 
    t=y/x;
    printf("\n sdb%d       | x'%02x'     | x'%02x'     | %9d          | %9d           | %8.3f  |",j, etat, fs, adr,taille,t );
            printf("\n|----------------|-------------|-------------|--------------|-----------------|-------------|");
            
    j++ ; 
    if(fs == 0x05){
      Partition_etundue(adr,&j,disque);
    }
  }
  printf("\n =======================  FIN ============================ \n");  
  fclose(disque);  
}







