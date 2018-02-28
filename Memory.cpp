#include "Memory.h"



/* Constructeur du module
 */
Memory::Memory(sc_module_name name) : sc_module(name)
{
   palb = NULL;            // initialisation des pointeurs donnees image
   palv = NULL;
   palr = NULL;
   imgb = NULL;
   imgv = NULL;
   imgr = NULL;
   infimg = new int[16]; // initialisation du pointeur info image

//-------------------------------------LISTES DE SENSIBILITE A PERSONNALISER 
    SC_METHOD(pict_load);
        sensitive << reset.pos();
    SC_METHOD(pict_save);  
        sensitive << img_valid.neg();
        dont_initialize();
    SC_METHOD(mem_read); // Asynchronous memory reads
        sensitive << admemr;
    SC_METHOD(mem_write);
        sensitive << clk.pos();
}


/* Destructeur pour l'allocation dynamique
 */
Memory::~Memory() {
   delete [] infimg;
   delete [] imgb;
   delete [] imgv;
   delete [] imgr;
   if (palb!=NULL) delete [] palb;
   if (palv!=NULL) delete [] palv;
   if (palr!=NULL) delete [] palr;
   cout << "Memoire detruite\n";
}


////////////////////////////////////////////
// code des processus
////////////////////////////////////////////


/* lecture synchrone de la memoire
 */
void Memory::mem_read() {
   if ( admemr.read()<2*nbpix ) { // l'adresse memoire va de 0 a 2*nbpix-1
      data_bo.write(imgb[admemr]);
      data_vo.write(imgv[admemr]);
      data_ro.write(imgr[admemr]);
   }
}


/* ecriture synchrone dans la memoire
 */
void Memory::mem_write() { // l'adresse memoire va de 0 a 2*nbpix-1
   if(write_enable.read())
   {
      if ( admemw.read()<2*nbpix ) {
         imgb[admemw] = data_bi.read();
         imgv[admemw] = data_vi.read();
         imgr[admemw] = data_ri.read();
      }
   }
}


/* fonction qui ecrit un long (4 octets) dans un fichier
 */
void putlong(FILE *fil, unsigned int val) {
   fputc(val & 0xFF,fil);       // ecriture du premier octet
   fputc((val>>8) & 0xFF,fil);  // ecriture du deuxieme octet
   fputc((val>>16) & 0xFF,fil); // ecriture du troisieme octet
   fputc((val>>24) & 0xFF,fil); // ecriture du quatrieme octet
} 


/* fonction qui ecrit un short (2 octets) dans un fichier
 */
void putshort(FILE *fil, unsigned short val) {    
   fputc(val & 0xFF,fil);      // ecriture du premier octet
   fputc((val>>8) & 0xFF,fil); // ecriture du deuxieme octet
}  


/* sauvegarde de l'image calculee dans un nouveau fichier
 */
void Memory::pict_save() {   
   FILE *file2;
   
   if ( !reset.read() ) {
         cout << "Creation fichier ecriture\n";
         file2 = fopen("IMG2.bmp", "wb");
         if ( !file2 )  cout << "Erreur d'ouverture fichier ecriture\n";
         else           cout << "Fichier ecriture ouvert\n";
         
         putshort(file2,infimg[0]);   // signature
         putlong(file2,54+nbpix*3);   // taille totale
         putlong(file2,infimg[2]);    // reserve
         putlong(file2,54);           // offset
         putlong(file2,infimg[4]);    // taille entete
         putlong(file2,infimg[5]);    // largeur image
         putlong(file2,infimg[6]);    // hauteur image
         putshort(file2,infimg[7]);   // nb plans
         putshort(file2,24);          // codage couleur 24 bits
         putlong(file2,infimg[9]);    // compression
         putlong(file2,nbpix*3);      // taille totale  (on passe de 4 a 24 bits)
         putlong(file2,infimg[11]);   // resolution horizontale 
         putlong(file2,infimg[12]);   // resolution verticale
         putlong(file2,infimg[13]);   // nb couleurs palette
         putlong(file2,infimg[14]);   // nb couleurs importantes
         
         for (int i=0;i<nbpix;i++) { // ecriture contenu pixels
            fputc(imgb[i+nbpix],file2);
            fputc(imgv[i+nbpix],file2);
            fputc(imgr[i+nbpix],file2);
         }
         
         fclose(file2);
         cout << "Fichier ecriture ferme \n" ;
   }  
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            


/* fonction qui lit un long (4 octets) dans un fichier
 */
int getlong(FILE *fil) {
   int i;
   
   i =  fgetc(fil);            // lecture du premier octet
   i += 0x100* fgetc(fil);     // lecture du deuxieme octet
   i += 0x10000* fgetc(fil);   // lecture du troisieme octet
   i += 0x1000000* fgetc(fil); // lecture du quatrieme octet
   return(i);
}  


/* fonction qui lit un short (2 octets) dans un fichier
 */
short getshort(FILE *fil) {
   short i;
   
   i =  fgetc(fil);        // lecture du premier octet
   i += 0x100* fgetc(fil); // lecture du deuxieme octet
   return(i);
}    


/* processus de chargement de l'image lance une fois au demarrage
 */
void Memory::pict_load() {
   unsigned char ch;
   unsigned short ind;
   int i;
   int lng;
   FILE *file;         // pointeur sur le fichier ouvert
   
   // ouverture du fichier
   file = fopen("lena.bmp", "rb");                      
   if ( !file )  cout << "Erreur d'ouverture\n";
   else          cout << "Fichier lecture ouvert\n";
   
   // entête
   infimg[0] =  getshort(file);
   if ( infimg[0]==0x4D42 )    cout << "Fichier d'image trouve\n";
   else                        cout << "Ce fichier n'est pas un fichier d'image\n";
   
   // taille
   infimg[1] =  getlong(file);
   cout << "taille fichier : " << infimg[1] << "\n";
   
   // reserve
   infimg[2] =  getlong(file);
   
   // offset
   infimg[3] =  getlong(file);
   cout << "offset : " << infimg[3] << "\n";
   
   // taille entete
   infimg[4] =  getlong(file);
   cout << "taille entete : " << infimg[4] << "\n";
   
   // largeur
   infimg[5] =  getlong(file);
   if ((infimg[5] & 3) != 0) infimg[5] = (infimg[5]&0xFFFFFFFC)+4 ;
   cout << "largeur : " << infimg[5] << "\n";
   ncol = infimg[5] ;
   
   // hauteur
   infimg[6] =  getlong(file);
   cout << "hauteur : " << infimg[6] << "\n";
   nlig = infimg[6];
   
   nbpix = infimg[5]*infimg[6];
   cout << "nombre de pixels : " << nbpix << "\n";
   
   // nbplans
   infimg[7] =  getshort(file);
   
   // profondeur
   infimg[8] =  getshort(file);
   cout << "profondeur : " << infimg[8] << "\n";
   
   // compression
   infimg[9] =  getlong(file);
   cout << "compression : " << infimg[9] << "\n";
   
   // taille totale de l'image
   infimg[10] =  getlong(file);
   cout << "taille totale : " << infimg[10] << "\n";
   
   // resolution horizontale
   infimg[11] =  getlong(file);
   cout << "resolution horizontale : " << infimg[11] << "\n";

   // resolution verticale
   infimg[12] =  getlong(file);
   cout << "resolution horizontale : " << infimg[12] << "\n";
   
   // nb couleurs
   infimg[13] =  getlong(file);
   cout << "nombre de couleurs de la palette : " << infimg[13] << "\n";
   
   // nb couleurs importantes
   infimg[14] =  getlong(file);
   cout << "nombre de couleurs importantes de la palette : " << infimg[14] << "\n";
   
   // lecture de la palette
   if (infimg[8]<24) {
      // calcul de la longueur de la palette
      // TODO: A simplifier avec math.h
      lgpal = 0;
      if      (infimg[8]==1)  lgpal = 2;
      else if (infimg[8]==4)  lgpal = 16;
      else if (infimg[8]==8)  lgpal = 256;
      else if (infimg[8]==16) lgpal = 65536;
      cout << "longueur palette : " << lgpal << "\n";
      
      // si les tableaux palette existent deja on les supprime et on les realloue
      if (palb!=NULL) delete [] palb;
      if (palv!=NULL) delete [] palv;
      if (palr!=NULL) delete [] palr;
      palb = new unsigned char [lgpal];
      palv = new unsigned char [lgpal];
      palr = new unsigned char [lgpal];
      
      // lit la palette
      for (i=0;i<lgpal;i++) {
         palb[i] = (unsigned char) getc(file);
         palv[i] = (unsigned char) getc(file);
         palr[i] = (unsigned char) getc(file);   
         getc(file);
      }
   }    
   
   lng = 0;
   
   // si les tableaux image existent deja on les supprime et on les realloue
   if (imgb!=NULL)  delete [] imgb;
   if (imgv!=NULL)  delete [] imgv;
   if (imgr!=NULL)  delete [] imgr;
   imgb = new unsigned char [2*nbpix];
   imgv = new unsigned char [2*nbpix];
   imgr = new unsigned char [2*nbpix];
   
   // remplissage des tableaux image
   while (!feof(file) && (lng<nbpix)) {  // boucle sur chaque pixel
      if (infimg[8]==1) {
      }
      else if (infimg[8]==4) { // quand il y a la palette on l'utilise
         ch = (unsigned char) getc(file);
         imgb[lng] = palb[ch >> 4];
         imgv[lng] = palv[ch >> 4];
         imgr[lng++] = palr[ch >> 4];
         imgb[lng] = palb[ch & 0x0F];
         imgv[lng] = palv[ch & 0x0F];
         imgr[lng++] = palr[ch & 0x0F];
      }
      else if (infimg[8]==8) {
         ch = (unsigned char) getc(file);
         imgb[lng] = palb[ch];
         imgv[lng] = palv[ch];
         imgr[lng++] = palr[ch];
      }
      else if (infimg[8]==16) {
         ind = (unsigned char) getc(file);
         ind += (unsigned char) getc(file)*0x100;
         imgb[lng] = palb[ind];
         imgv[lng] = palv[ind];
         imgr[lng++] = palr[ind];
      }
      else if (infimg[8]==24) { // en mode 24 bits, on lit directement les pixels
         imgb[lng] = getc(file);
         imgv[lng] = getc(file);
         imgr[lng++] = getc(file);
      }
   }
   
   // fermeture du fichier
   fclose(file);
   cout << "longueur des data : " << lng << "\n";
   cout << "Fichier lecture ferme\n";
}
