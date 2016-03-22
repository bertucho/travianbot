#include <stdio.h>
#include <string.h>
#include <winsock.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define PUERTO 80
#define HTML_LEN 25000


char* reasig(char* buffer,int tama);
int reabrir(SOCKET *clsock);
char* login(char* user,char* pass);
char* pedir(char* url,int tipo,char* post);
DWORD WINAPI construir(struct building *edif);
DWORD WINAPI enviarmaterias(struct sending *enviar);
DWORD WINAPI atacar(struct attacking *ataque);
DWORD WINAPI alarmar(struct alarming *alarma);
DWORD WINAPI desalojar(struct emptying *desalojo);
DWORD WINAPI newaldeas(char *hora);
void vegetar(char *hora,int tipo);
int aleatorio(int minimo, int maximo);
DWORD WINAPI buscar(struct casillas *casilla);
int abs(int num);
long xy2id(int x,int y);
int AnalizarCasilla(char* url,int poblado);
char* urlencode(char* c);
DWORD WINAPI EnviarIgm(struct message *mensaje);
DWORD WINAPI FakearCoord(struct faking *fake);
DWORD WINAPI FakearJugadores(struct faking *fake);


struct aldea{
       char nombre[30];
       char id[10];
       char coordenadas[12];
       int materias[4];
};

struct building{
       char newdid[10];
       char id[5];
       char hora[10];
       int naldea;
       };
       
struct sending{
       char newdid[10];
       char x[6];
       char y[6];
       char id[5];
       char materias[4][8];
       char hora[10];
       };
       
struct destroy{
       char newdid[10];
       char id[5];
       char hora[10];
       };
       
struct attacking{
       char newdid[10];
       char x1[6],y1[6],x2[6],y2[6];
       char x[6];
       char y[6];
       char hora[10];
       char t[11][8];
       char c;
       int tipo;
       };
       
struct alarming{
       char avisar;
       char desalojar;
       struct attacking *ataque;
       };
       
struct emptying{
       int total;
       struct attacking *ataque;
       };
       
struct casillas{
       int x1;
       int y1;
       int x2;
       int y2;
       };
       
struct message{
       char *texto;
       char **remitentes;
       char *topic;
       char hora[10];
       int nremitentes;
       short tipo;
       };
       
struct faking{
       int x1;
       int y1;
       int x2;
       int y2;
       int nataques;
       int habitantes;
       int njugadores;
       short tipo;
       char **jugadores;
       char hora[10];
       char *alianza;
       struct attacking *ataque;
       };
       

aldea* getaldeas(int *naldeas);



sockaddr_in sraddr;
char user[28],pueblo[16],pass[28],cookie[110]="-",server[28]="sx.travian.net";
int raza,naldeas,aldeanueva=0;
aldea* aldeas;



int main(){
    WSADATA wsadata;
    SOCKET clsock;
    char ip_server[28],accion[30],stropas[28],ttropas[3],abort,hora[10];
    char tropas[3][10][32]={
                          {"Luchadores de Porra","Lanceros","Luchadores de Hacha",
                           "Emisarios","Paladines","Jinetes Teutones","Arietes",
                           "Catapultas","Cabecillas","Colonos"},
                          {"Legionarios","Pretorianos","Imperanos",
                           "Equites Legati","Equites Imperiatoris","Equites Caesaris",
                           "Carneros","Catapultas de Fuego","Senador","Colonos"},
                          {"Falanges","Luchadores de Espada","Batidores","Rayos de Teutates",
                           "Jinetes Druidas","Jinetes Eudos","Carneros de madera",
                           "Catapultas de guerra","Caciques","Colonos"}
                          };
    hostent *hp;
    int i,j=0,idaldea,x2,y2,c,k;
    HANDLE hhilo,halarm;
    DWORD hid,halarmid;
    building *edif;
    sending *enviar;
    attacking *ataque;
    alarming *alarma;
    casillas *casilla;
    message *mensaje;
    faking *fake;
    char temp[30];
    
    printf("Servidor travian [1,2,3,4,5,s]: ");
    server[1]=getchar();
    if(server[1]=='s') strcpy(server,"tcx3.travian.cl\0");
    printf("Nombre de usuario: ");
    fflush(stdin);
    scanf("%[^\n]",user);
    printf("Contraseña: ");
    fflush(stdin);
    scanf("%s",pass);
    do{
    printf("Pueblo [germano, romano, galo]: ");
    fflush(stdin);
    scanf("%s",pueblo);
    }while(strcmp(pueblo,"germano")!=0&&strcmp(pueblo,"romano")!=0&&strcmp(pueblo,"galo")!=0);
    if(strcmp(pueblo,"germano")==0) raza=0;
    else if(strcmp(pueblo,"romano")==0) raza=1;
    else if(strcmp(pueblo,"galo")==0) raza=2;
    
    if(WSAStartup(MAKEWORD(1,1),&wsadata)!=0){printf("Error Startup");return 0;}
    
    clsock=socket(AF_INET,SOCK_STREAM,0);
    if(clsock==INVALID_SOCKET){printf("Invalid socket");return 0;}
    
    hp=gethostbyname(server);
    if(hp==NULL){printf("Error gethostbyname");return 0;}
    
   	if( hp->h_addrtype == AF_INET ) {
			strcpy(ip_server,inet_ntoa( *(in_addr*) hp->h_addr_list[0]));
			printf("%s\n",ip_server);
   }
   
   sraddr.sin_family=AF_INET;
   sraddr.sin_port=htons(PUERTO);
   sraddr.sin_addr.s_addr=inet_addr(ip_server);
   if(connect(clsock,(sockaddr*)&sraddr,sizeof(sraddr))==SOCKET_ERROR){printf("error connect");return 0;}
   
   login(user,pass);
   aldeas=getaldeas(&naldeas);
   
   
   do{
      fflush(stdout);
      printf("\naccion>> ");
      fflush(stdin);
      scanf("%[^\n]",accion);
      
      if(strcmp(accion,"construir")==0){
                    edif=(building*)malloc(sizeof(building));
                    printf("Aldea (del 1 al %d): ",naldeas);
                    fflush(stdin);
                    scanf("%d",&idaldea);
                    strcpy(edif->newdid,aldeas[idaldea-1].id);
                    edif->naldea=idaldea-1;
                    printf("id del edificio: ");
                    fflush(stdin);
                    scanf("%s",edif->id);
                    printf("Hora para construir [formato hh:mm:ss]: ");
                    fflush(stdin);
                    scanf("%s",edif->hora);
                    printf("Abortar la construiccion? (s/n): ");
                    fflush(stdin);
                    abort=getchar();
                    if(abort=='s'){ free(edif); continue;}
                    hhilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)construir,(PVOID)edif,0,&hid);
                    
                    }
   
   
      if(strcmp(accion,"enviar")==0){
                    enviar=(sending*)malloc(sizeof(sending));
                    if(naldeas>1){
                       printf("Aldea origen (del 1 al %d): ",naldeas);
                       fflush(stdin);
                       scanf("%d",&idaldea);
                       strcpy(enviar->newdid,aldeas[idaldea-1].id);
                       }
                    printf("id del mercado: ");
                    fflush(stdin);
                    scanf("%s",enviar->id);
                    printf("\nCoordenada x: ");
                    fflush(stdin);
                    scanf("%s",enviar->x);
                    printf("Coordenada y: ");
                    fflush(stdin);
                    scanf("%s",enviar->y);
                    printf("\nMaterias que quieres enviar en el orden: [leña barro hierro cereal]:\n");
                    for(i=0;i<4;i++) scanf("%s",enviar->materias[i]);
                    printf("\nHora para enviar [formato hh:mm:ss]: ");
                    fflush(stdin);
                    scanf("%s",enviar->hora);
                    printf("\nAbortar el envio? (s/n): ");
                    fflush(stdin);
                    abort=getchar();
                    if(abort=='s'){ free(enviar); continue;}
                    hhilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)enviarmaterias,(PVOID)enviar,0,&hid);
                    
                    }
   
   
      if(strcmp(accion,"atacar")==0){
                    ataque=(attacking*)malloc(sizeof(attacking));
                    ataque->tipo=0;
                    for(i=0;i<10;i++) strcpy(ataque->t[i],"0");
                    if(naldeas>1){
                       printf("Aldea origen (del 1 al %d): ",naldeas);
                       fflush(stdin);
                       scanf("%d",&idaldea);
                       strcpy(ataque->newdid,aldeas[idaldea-1].id);
                       }
                    printf("\nCoordenada x: ");
                    fflush(stdin);
                    scanf("%s",ataque->x);
                    printf("Coordenada y: ");
                    fflush(stdin);
                    scanf("%s",ataque->y);
                    do{
                    printf("\nMision:\n1. Refuerzos\n2. Ataque\n3. Atraco\nDel 1 al 3> ");
                    fflush(stdin);
                    ataque->c=getchar();
                    }while(ataque->c<49||ataque->c>51);
                    ataque->c++;
                    printf("\nHora para ataque [formato hh:mm:ss]: ");
                    fflush(stdin);
                    scanf("%s",ataque->hora);
                    printf("\nTropas. Debe estar seguro de que dispondrá de esa cantidad "
                           "de tropas en la hora especificada, de lo contrario no se mandará "
                           "el ataque. Para enviar todas las disponibles en la hora especificada "
                           "escriba \'t\'.\n\n");
                    for(i=0;i<10;i++) printf("%d. %s\n",i+1,tropas[raza][i]);
                    printf("\nTipo de tropas separados por comas (ej: 1,5,8): \n> ");
                    fflush(stdin);
                    scanf("%s",stropas);
                    for(i=0;stropas[i]!='\0';i++){
                            if(stropas[i]!=','&&j<2){
                                      ttropas[j]=stropas[i];
                                      j++;
                                      ttropas[j]='\0';
                            }
                            if(stropas[i]==','||j>=2||stropas[i+1]=='\0'){
                                      j=0;
                                      if(atoi(ttropas)>10||atoi(ttropas)<1)
                                                printf("%d es un numero incorrecto\n",atoi(ttropas));
                                      else{
                                      printf("%s: ",tropas[raza][atoi(ttropas)-1]);
                                      fflush(stdin);
                                      scanf("%s",ataque->t[atoi(ttropas)-1]);
                                      }
                            }
                    }
                    do{
                    printf("Heroe? (s/n): ");
                    fflush(stdin);
                    ataque->t[10][0]=getchar();
                    }while(ataque->t[10][0]!='s'&&ataque->t[10][0]!='n');
                    if(ataque->t[10][0]=='s') strcpy(ataque->t[10],"1");
                    else strcpy(ataque->t[10],"0");
                    printf("Abortar el ataque? (s/n): ");
                    fflush(stdin);
                    abort=getchar();
                    if(abort=='s'){ free(ataque); continue;}
                    hhilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)atacar,(PVOID)ataque,0,&hid);
                    
                    }
                    
      
      if(strncmp(accion,"alarma",6)==0){
                    alarma=(alarming*)malloc(sizeof(alarming));
                    if(strcmp(accion+strlen("alarma")," on")==0){
                       do{
                       printf("Avisar si se detecta un ataque? (s/n): ");
                       fflush(stdin);
                       alarma->avisar=getchar();
                       }while(alarma->avisar!='s'&&alarma->avisar!='n');
                       do{
                       printf("Desalojar aldea? (s/n): ");
                       fflush(stdin);
                       alarma->desalojar=getchar();
                       }while(alarma->desalojar!='s'&&alarma->desalojar!='n');
                       if(alarma->desalojar=='s'){
                          printf("Configurar el ataque de desalojo:\n");
                          ataque=(attacking*)malloc(sizeof(attacking));
                          for(i=0;i<10;i++) strcpy(ataque->t[i],"0");
                          printf("\nCoordenada x: ");
                          fflush(stdin);
                          scanf("%s",ataque->x);
                          printf("Coordenada y: ");
                          fflush(stdin);
                          scanf("%s",ataque->y);
                          do{
                          printf("\nMision:\n1. Refuerzos\n2. Ataque\n3. Atraco\nDel 1 al 3> ");
                          fflush(stdin);
                          ataque->c=getchar();
                          }while(ataque->c<49||ataque->c>51);
                          ataque->c++;
                          printf("\nTropas. Debe estar seguro de que dispondrá de esa cantidad "
                                 "de tropas en la hora especificada, de lo contrario no se mandará "
                                 "el ataque. Para enviar todas las disponibles en la hora especificada "
                                 "escriba \'t\'.\n\n");
                          for(i=0;i<10;i++) printf("%d. %s\n",i+1,tropas[raza][i]);
                          printf("\nTipo de tropas separados por comas (ej: 1,5,8): \n> ");
                          fflush(stdin);
                          scanf("%s",stropas);
                          for(i=0;stropas[i]!='\0';i++){
                                  if(stropas[i]!=','&&j<2){
                                          ttropas[j]=stropas[i];
                                          j++;
                                          ttropas[j]='\0';
                                  }
                                  if(stropas[i]==','||j>=2||stropas[i+1]=='\0'){
                                          j=0;
                                          if(atoi(ttropas)>10||atoi(ttropas)<1)
                                                printf("%d es un numero incorrecto\n",atoi(ttropas));
                                          else{
                                          printf("%s: ",tropas[raza][atoi(ttropas)-1]);
                                          fflush(stdin);
                                          scanf("%s",ataque->t[atoi(ttropas)-1]);
                                          }
                                  }
                          }
                          do{
                          printf("Heroe? (s/n): ");
                          fflush(stdin);
                          ataque->t[10][0]=getchar();
                          }while(ataque->t[10][0]!='s'&&ataque->t[10][0]!='n');
                          if(ataque->t[10][0]=='s') strcpy(ataque->t[10],"1");
                          else strcpy(ataque->t[10],"0");
                          printf("Abortar el ataque? (s/n): ");
                          fflush(stdin);
                          abort=getchar();
                          if(abort=='s'){ free(ataque); continue;}
                          alarma->ataque=(attacking*)malloc(sizeof(attacking));
                          memcpy(alarma->ataque,ataque,sizeof(attacking));
                          
                          
                          }
                       halarm=CreateThread(0,0,(LPTHREAD_START_ROUTINE)alarmar,(PVOID)alarma,0,&halarmid);
                       }
                    }
      if(strcmp(accion,"newaldeas")==0){
                    printf("Hora: ");
                    fflush(stdin);
                    scanf("%s",hora);
                    hhilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)newaldeas,(PVOID)hora,0,&hid);
                    }
                    
      if(strcmp(accion,"buscar")==0){
                    casilla=(casillas*)malloc(sizeof(casillas));
                    printf("Extremo del cuadrilatero: ");
                    printf("\nCoordenada x: ");
                    fflush(stdin);
                    scanf("%d",&casilla->x1);
                    printf("Coordenada y: ");
                    fflush(stdin);
                    scanf("%d",&casilla->y1);
                    printf("\nExtremo opuesto del cuadrilatero:");
                    printf("\nCoordenada x: ");
                    fflush(stdin);
                    scanf("%d",&casilla->x2);
                    printf("Coordenada y: ");
                    fflush(stdin);
                    scanf("%d",&casilla->y2);
                    hhilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)buscar,(PVOID)casilla,0,&hid);
                    }
                    
      if(strcmp(accion,"igm")==0){
                    mensaje=(message*)malloc(sizeof(message));
                    mensaje->tipo=0;
                    printf("Remitentes (separados por coma): ");
                    mensaje->remitentes=NULL;
                    fflush(stdin);
                    for(i=0,j=0;(c=getchar())!=EOF&&j<29;j++){
                       if(j==0)
                          mensaje->remitentes=(char**)realloc(mensaje->remitentes,i*sizeof(char*)+sizeof(char*));
                          mensaje->remitentes[i]=(char*)malloc(sizeof(char)*30);
                       if(c==','||c=='\n'){
                          temp[j]='\0';
                          strcpy(mensaje->remitentes[i],temp);
                          if(c=='\n') break;
                          else{
                             i++;
                             j=-1;
                             continue;
                          }
                       }else{
                          temp[j]=c;
                          }
                       }
                    mensaje->nremitentes=i+1;
                    for(i=0;i<mensaje->nremitentes;i++) printf("\n%s",mensaje->remitentes[i]);
                    
                    mensaje->topic=(char*)malloc(sizeof(char));
                    printf("\nAsunto: ");
                    for(i=0;(c=getchar())!='\n';i++){
                       mensaje->topic=(char*)realloc(mensaje->topic,sizeof(char)*i+sizeof(char)*2);
                       mensaje->topic[i]=c;
                       }
                    mensaje->topic[i]='\0';
                    printf("\n\n%s",mensaje->topic);
                       
                    mensaje->texto=(char*)malloc(sizeof(char));
                    printf("\nMensaje:\n\n");
                    for(i=0;(c=getchar())!=EOF;i++){
                       mensaje->texto=(char*)realloc(mensaje->texto,sizeof(char)*i+sizeof(char)*2);
                       mensaje->texto[i]=c;
                       }
                    mensaje->texto[i]='\0';
                    printf("\n\nHora para ataque [formato hh:mm:ss]: ");
                    scanf("%[1234567890:n]",mensaje->hora);
                    if(strcmp(mensaje->hora,"n")==0){ mensaje->tipo=1; strcpy(mensaje->hora,"0");}
                    
                    hhilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)EnviarIgm,(PVOID)mensaje,0,&hid);
                    }
                    
      if(strcmp(accion,"fakear coord")==0){
                    fake=(faking*)malloc(sizeof(faking));
                    fake->ataque=(attacking*)malloc(sizeof(attacking));
                    printf("Extremo del cuadrilatero: ");
                    printf("\nCoordenada x: ");
                    fflush(stdin);
                    scanf("%d",&fake->x1);
                    printf("Coordenada y: ");
                    fflush(stdin);
                    scanf("%d",&fake->y1);
                    printf("\nExtremo opuesto del cuadrilatero:");
                    printf("\nCoordenada x: ");
                    fflush(stdin);
                    scanf("%d",&fake->x2);
                    printf("Coordenada y: ");
                    fflush(stdin);
                    scanf("%d",&fake->y2);
                    printf("Minimo de habitantes: ");
                    fflush(stdin);
                    scanf("%d",&fake->habitantes);
                    printf("Alianza: ");
                    fflush(stdin);
                    fake->alianza=(char*)malloc(sizeof(char));
                    for(i=0;(c=getchar())!='\n';i++){
                       fake->alianza=(char*)realloc(fake->alianza,sizeof(char)*i+sizeof(char)*2);
                       fake->alianza[i]=c;
                       }
                    fake->alianza[i]='\0';
                    fflush(stdin);
                    printf("\n\nHora para ataque [formato hh:mm:ss]: ");
                    scanf("%[1234567890:n]",fake->ataque->hora);
                    if(strcmp(fake->ataque->hora,"n")==0){ fake->ataque->tipo=1; strcpy(fake->ataque->hora,"0");}
                    printf("Numero de ataques por aldea: ");
                    fflush(stdin);
                    scanf("%d",&fake->nataques);
                    
                    printf("Configurar el ataque:\n");
                    if(naldeas>1){
                       printf("Aldea origen (del 1 al %d): ",naldeas);
                       fflush(stdin);
                       scanf("%d",&idaldea);
                       strcpy(fake->ataque->newdid,aldeas[idaldea-1].id);
                       }
                    for(i=0;i<10;i++) strcpy(fake->ataque->t[i],"0");
                    do{
                    printf("\nMision:\n1. Refuerzos\n2. Ataque\n3. Atraco\nDel 1 al 3> ");
                    fflush(stdin);
                    fake->ataque->c=getchar();
                    }while(fake->ataque->c<49||fake->ataque->c>51);
                    fake->ataque->c++;
                    printf("\nTropas. Para enviar todas las disponibles en la hora especificada "
                           "escriba \'t\'.\n\n");
                    for(i=0;i<10;i++) printf("%d. %s\n",i+1,tropas[raza][i]);
                    printf("\nTipo de tropas separados por comas (ej: 1,5,8): \n> ");
                    fflush(stdin);
                    scanf("%s",stropas);
                    for(i=0;stropas[i]!='\0';i++){
                            if(stropas[i]!=','&&j<2){
                                    ttropas[j]=stropas[i];
                                    j++;
                                    ttropas[j]='\0';
                            }
                            if(stropas[i]==','||j>=2||stropas[i+1]=='\0'){
                                    j=0;
                                    if(atoi(ttropas)>10||atoi(ttropas)<1)
                                          printf("%d es un numero incorrecto\n",atoi(ttropas));
                                    else{
                                    printf("%s: ",tropas[raza][atoi(ttropas)-1]);
                                    fflush(stdin);
                                    scanf("%s",fake->ataque->t[atoi(ttropas)-1]);
                                    }
                            }
                    }
                    do{
                    printf("Heroe? (s/n): ");
                    fflush(stdin);
                    fake->ataque->t[10][0]=getchar();
                    }while(fake->ataque->t[10][0]!='s'&&fake->ataque->t[10][0]!='n');
                    if(fake->ataque->t[10][0]=='s') strcpy(fake->ataque->t[10],"1");
                    else strcpy(fake->ataque->t[10],"0");
                    printf("Abortar el ataque? (s/n): ");
                    fflush(stdin);
                    abort=getchar();
                    if(abort=='s'){ free(ataque); continue;}
                    hhilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)FakearCoord,(PVOID)fake,0,&hid);
                    }
                    
      if(strcmp(accion,"fakear jugadores")==0){
                    fake=(faking*)malloc(sizeof(faking));
                    fake->ataque=(attacking*)malloc(sizeof(attacking));
                    fake->jugadores=NULL;
                    printf("\nJugadores (separados por comas): ");
                    fflush(stdin);
                    for(i=0,j=0;(c=getchar())!=EOF&&j<29;j++){
                       if(j==0)
                          fake->jugadores=(char**)realloc(fake->jugadores,i*sizeof(char*)+sizeof(char*));
                          fake->jugadores[i]=(char*)malloc(sizeof(char)*30);
                       if(c==','||c=='\n'){
                          temp[j]='\0';
                          strcpy(fake->jugadores[i],temp);
                          if(c=='\n') break;
                          else{
                             if(strlen(fake->jugadores[i])>0)
                             i++;
                             j=-1;
                             continue;
                          }
                       }else{
                          temp[j]=c;
                          }
                       }
                    fake->njugadores=i+1;
                    for(i=0;i<fake->njugadores;i++) printf("\n%s",fake->jugadores[i]);
                    
                    printf("\n\nMinimo de habitantes: ");
                    fflush(stdin);
                    scanf("%d",&fake->habitantes);
                    fflush(stdin);
                    printf("\n\nHora para ataque [formato hh:mm:ss]: ");
                    scanf("%[1234567890:n]",fake->hora);
                    if(strcmp(fake->hora,"n")==0){ fake->tipo=1; strcpy(fake->hora,"0");}
                    printf("Numero de ataques por aldea: ");
                    fflush(stdin);
                    scanf("%d",&fake->nataques);
                    
                    printf("Configurar el ataque:\n");
                    if(naldeas>1){
                       printf("Aldea origen (del 1 al %d): ",naldeas);
                       fflush(stdin);
                       scanf("%d",&idaldea);
                       strcpy(fake->ataque->newdid,aldeas[idaldea-1].id);
                       }
                    for(i=0;i<10;i++) strcpy(fake->ataque->t[i],"0");
                    do{
                    printf("\nMision:\n1. Refuerzos\n2. Ataque\n3. Atraco\nDel 1 al 3> ");
                    fflush(stdin);
                    fake->ataque->c=getchar();
                    }while(fake->ataque->c<49||fake->ataque->c>51);
                    fake->ataque->c++;
                    printf("\nTropas. Para enviar todas las disponibles en la hora especificada "
                           "escriba \'t\'.\n\n");
                    for(i=0;i<10;i++) printf("%d. %s\n",i+1,tropas[raza][i]);
                    printf("\nTipo de tropas separados por comas (ej: 1,5,8): \n> ");
                    fflush(stdin);
                    scanf("%s",stropas);
                    for(i=0,j=0;stropas[i]!='\0';i++){
                            if(stropas[i]!=','&&j<2){
                                    ttropas[j]=stropas[i];
                                    j++;
                                    ttropas[j]='\0';
                            }
                            if(stropas[i]==','||j>=2||stropas[i+1]=='\0'){
                                    j=0;
                                    if(atoi(ttropas)>10||atoi(ttropas)<1)
                                          printf("%d es un numero incorrecto\n",atoi(ttropas));
                                    else{
                                    printf("%s: ",tropas[raza][atoi(ttropas)-1]);
                                    fflush(stdin);
                                    scanf("%s",fake->ataque->t[atoi(ttropas)-1]);
                                    }
                            }
                    }
                    do{
                    printf("Heroe? (s/n): ");
                    fflush(stdin);
                    fake->ataque->t[10][0]=getchar();
                    }while(fake->ataque->t[10][0]!='s'&&fake->ataque->t[10][0]!='n');
                    if(fake->ataque->t[10][0]=='s') strcpy(fake->ataque->t[10],"1");
                    else strcpy(fake->ataque->t[10],"0");
                    printf("Abortar el ataque? (s/n): ");
                    fflush(stdin);
                    abort=getchar();
                    if(abort=='s'){ free(ataque); continue;}
                    hhilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)FakearJugadores,(PVOID)fake,0,&hid);
                    }
                       
   
   }while(strcmp(accion,"salir")!=0);
}


int reabrir(SOCKET *clsock){
    int error;
    do{
    closesocket(*clsock);
    *clsock=socket(AF_INET,SOCK_STREAM,0);
    if((error=connect(*clsock,(sockaddr*)&sraddr,sizeof(sraddr)))==SOCKET_ERROR){printf("\nError connect");  Sleep(1000);}
    }while(error==SOCKET_ERROR);
    return 1;
}


char* login(char* user,char* pass){
      int i,len,rec;
      char *post;
      char *buffer,*ptr,slen[4];
      post=(char*)malloc(180);
      strcpy(post,"w=1024%3A768&login=\0");
      buffer=pedir("login.php",0,NULL);
      printf("\nLogeandose...\n");
      ptr=strstr(buffer,"name=\"login\" value=\"");
      if(ptr==NULL) return NULL;
      printf("a");
      ptr+=strlen("name=\"login\" value=\"");
      for(i=0;ptr[i]!='\"';i++) strncat(post,ptr+i,1);
      ptr=strstr(buffer,"<input class=\"fm fm110\" type=\"text\" name=\"");
      if(ptr==NULL) return NULL;
      printf("b");
      ptr+=strlen("<input class=\"fm fm110\" type=\"text\" name=\"");
      strcat(post,"&");
      for(i=0;ptr[i]!='\"';i++) strncat(post,ptr+i,1);
      strcat(post,"=");
      strcat(post,user);
      strcat(post,"&");
      ptr=strstr(buffer,"<input class=\"fm fm110\" type=\"password\" name=\"");
      if(ptr==NULL){ return NULL;}
      printf("c");
      ptr+=strlen("<input class=\"fm fm110\" type=\"password\" name=\"");
      for(i=0;ptr[i]!='\"';i++) strncat(post,ptr+i,1);
      strcat(post,"=");
      strcat(post,pass);
      strcat(post,"&");
      ptr=strstr(buffer,"<p align=\"center\"><input type=\"hidden\" name=\"");
      if(ptr==NULL) return NULL;
      printf("d");
      ptr+=strlen("<p align=\"center\"><input type=\"hidden\" name=\"");
      for(i=0;ptr[i]!='\"';i++) strncat(post,ptr+i,1);
      strcat(post,"=");
      ptr=strstr(ptr,"value=\"");
      if(ptr==NULL) return NULL;
      printf("e");
      ptr+=strlen("value=\"");
      for(i=0;ptr[i]!='\"';i++) strncat(post,ptr+i,1);
      strcat(post,"&s1.x=0&s1.y=0&s1=login&autologin=ja");
      len=strlen(post);
      free(buffer);
      buffer=pedir("dorf1.php",1,post);
      if((ptr=strstr(buffer,"\r\nSet-Cookie: "))!=NULL){
      ptr+=strlen("\r\nSet-Cookie: ");
      for(i=0;ptr[i-1]!=';';i++) strncpy(cookie+i,ptr+i,1);
      cookie[i]='\0';
      printf("%s\n",cookie);
      }
      free(buffer);
      free(post);
      return cookie;
      
      }
      
char* pedir(char* url,int tipo,char* post){
      char *buffer,*datos,*buffer2,*ptr,*peticion,slen[6];
      SOCKET clsock;
      buffer2=(char*)malloc(2000);
      buffer=(char*)malloc(HTML_LEN);
      peticion=(char*)malloc(900);
      int length=0,rec,b=0;
      if(tipo==0&&post==NULL){
             strcpy(peticion,"GET /");
             strcat(peticion,url);
             strcat(peticion," HTTP/1.1\r\n"
                             "Host: ");
             strcat(peticion,server);
             strcat(peticion,"\r\n"
                             "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; es-ES; rv:1.8.1.12) Gecko/20080201 Firefox/2.0.0.12\r\n"
                             "Accept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n"
                             "Accept-Language: es-es,es;q=0.8,en-us;q=0.5,en;q=0.3\r\n"
                             "Keep-Alive: 300\r\n"
                             "Connection: keep-alive\r\n"
                             "Referer: http://s5.travian.net/dorf1.php\r\n"
                             "Cookie: ");
             strcat(peticion,cookie);
             strcat(peticion,"\r\n\r\n");
      
      }else if(tipo==1&&post!=NULL){
             strcpy(peticion,"POST /");
             strcat(peticion,url);
             strcat(peticion," HTTP/1.1\r\n"
                             "Host: ");
             strcat(peticion,server);
             strcat(peticion,"\r\n"
                             "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; es-ES; rv:1.8.1.12) Gecko/20080201 Firefox/2.0.0.12\r\n"
                             "Accept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n"
                             "Accept-Language: es-es,es;q=0.8,en-us;q=0.5,en;q=0.3\r\n"
                             "Keep-Alive: 300\r\n"
                             "Connection: keep-alive\r\n"
                             "Referer: http://s5.travian.net/dorf1.php\r\n"
                             "Cookie: ");
             strcat(peticion,cookie);
             strcat(peticion,"\r\n"
                             "Content-Type: application/x-www-form-urlencoded\r\n"
                             "Content-Length: ");
             strcat(peticion,itoa(strlen(post),slen,10));
             strcat(peticion,"\r\n\r\n");
             strcat(peticion,post);
             }
      do{
      buffer[0]='\0';
      buffer2[0]='\0';
      reabrir(&clsock);
      printf("x");
      send(clsock,peticion,strlen(peticion),0);
      do{
         rec=recv(clsock,buffer2,sizeof(buffer2)-1,0);
         buffer2[rec]='\0';
         length+=strlen(buffer2);
         strcat(buffer,buffer2);
         buffer[length]='\0';
         buffer2[0]='\0';
         }while(rec>0);
         if(strstr(buffer,"<p class=\"f9\">Para el login, tienes que activar")!=NULL&&strcmp(url,"login.php")!=0){
                      printf("z");
                      login(user,pass);
                      printf("y");
                      ptr=strstr(peticion,"Cookie: ");
                      ptr+=strlen("Cookie: ");
                      strncpy(ptr,cookie,strlen(cookie));
                      if(tipo==1&&post!=NULL){
                                strcat(ptr,"\r\n"
                                           "Content-Type: application/x-www-form-urlencoded\r\n"
                                           "Content-Length: ");
                                strcat(peticion,itoa(strlen(post),slen,10));
                                strcat(peticion,"\r\n\r\n");
                                strcat(peticion,post);
                      }else if(tipo==0&&post==NULL)
                      strcat(ptr,"\r\n\r\n");
                      b=1;
                      }else{ b=0;}
      }while(strstr(buffer,"<p class=\"f9\">Para el login, tienes que activar")!=NULL&&b==1&&strcmp(url,"login.php")!=0);
      free(buffer2);
      free(peticion);
      closesocket(clsock);
      return buffer;
      }
      

char* reasig(char* data,int tama){
      char* buffer;
      int i,len=0;
      if(data!=NULL) len=strlen(data);
      buffer=(char*)malloc(tama+1);
      for(i=0;i<tama&&i<len;i++) buffer[i]=data[i];
      buffer[i]='\0';
      free(data);
      data=buffer;
      return buffer;
      }



aldea* getaldeas(int* naldeas){
       char *buffer,*ptr;
       int i,x;
       aldea *aldeas;
       buffer=pedir("dorf1.php",0,NULL);
       *naldeas=0;
       ptr=buffer;
       if(strstr(ptr,"</span>&nbsp; <a href=\"?newdid=")==NULL){
                    aldeas=(aldea*)malloc(sizeof(aldea));
                    strcpy(aldeas->id,"0");
                    strcpy(aldeas->nombre,"Unknown");
                    strcpy(aldeas->coordenadas,"|");
                    (*naldeas)++;
                    }else{
       for(i=0;(ptr=strstr(ptr,"</span>&nbsp; <a href=\"?newdid="))!=NULL;i++)
                    ptr+=strlen("</span>&nbsp; <a href=\"?newdid=");
       ptr=buffer;
       aldeas=(aldea*)malloc(i*sizeof(aldea));}
       while((ptr=strstr(ptr,"</span>&nbsp; <a href=\"?newdid="))!=NULL){
                    ptr+=strlen("</span>&nbsp; <a href=\"?newdid=");
                    for(i=0;ptr[i]!='\"'&&i<9;i++) strncpy(aldeas[*naldeas].id+i,ptr+i,1);
                    aldeas[*naldeas].id[i]='\0';
                    ptr=strstr(ptr,">");
                    ptr+=strlen(">");
                    for(i=0;ptr[i]!='<'&&i<29;i++) strncpy(aldeas[*naldeas].nombre+i,ptr+i,1);
                    aldeas[*naldeas].nombre[i]='\0';
                    ptr=strstr(ptr,"<td class=\"right dlist1\">");
                    ptr+=strlen("<td class=\"right dlist1\">");
                    for(i=0;ptr[i]!='<'&&i<11;i++) strncpy(aldeas[*naldeas].coordenadas+i,ptr+i,1);
                    strncpy(aldeas[*naldeas].coordenadas+i,"|",1);
                    i++;
                    ptr=strstr(ptr,"<td class=\"left dlist3\">");
                    ptr+=strlen("<td class=\"left dlist3\">");
                    for(x=0;ptr[x]!='<'&&i<11;i++,x++) strncpy(aldeas[*naldeas].coordenadas+i,ptr+x,1);
                    aldeas[*naldeas].coordenadas[i]='\0';
                    printf("\n\nAldea %d:",(*naldeas)+1);
                    printf("\nNombre: %s\n",aldeas[*naldeas].nombre);
                    printf("id: %s\n",aldeas[*naldeas].id);
                    printf("Coord: %s\n",aldeas[*naldeas].coordenadas);
                    (*naldeas)++;
                    }
       free(buffer);
       free(ptr);
       return aldeas;

}



DWORD WINAPI construir(struct building *edif){
      char *buffer,*ptr,opcion[2][32]={"<a href=\"dorf1.php?a=","<a href=\"dorf2.php?a="};
      char url[110]="build.php?newdid=",hora[10];
      int i,tipo,b=0;
      if(atoi(edif->id)<19) tipo=0; else tipo=1;
      strcat(url,edif->newdid);
      strcat(url,"&id=");
      strcat(url,edif->id);
      strcpy(hora,edif->hora);
      vegetar(hora,0);
      if(aldeanueva==1){
         ptr=strstr(url,"newdid=");
         ptr+=strlen("newdid=");
         strcpy(ptr,aldeas[edif->naldea].id);
         strcat(ptr,"&id=");
         strcat(url,edif->id);
         }
      do{
         buffer=pedir(url,0,NULL);
         if((ptr=strstr(buffer,opcion[tipo]))!=NULL){
                 ptr+=strlen("<a href=\"");
                 for(i=0;ptr[i]!='\"';i++) strncpy(url+i,ptr+i,1);
                 url[i]='\0';
                 free(buffer);
                 buffer=pedir(url,0,NULL);
                 b=1;
                 }
         if(b==0) Sleep(120000);
         }while(b==0);
      free(buffer);
      
}


void vegetar(char *hora,int tipo){
    int i;
    char temp[3], *ptr;
    
    if(tipo==1) Sleep(atoi(hora)*1000);
    else{
    for(i=0;hora[i]!=':'&&hora[i]!='\0'&&i<2;i++){
                         temp[i]=hora[i];
                         }
    ptr=&hora[++i];
    temp[i]='\0';
    int horas=atoi(temp);
    horas=horas*60*60;
    memset(temp,'\0',sizeof(temp));
    for(i=0;ptr[i]!=':'&&ptr[i]!='\0'&&i<2;i++){
                         temp[i]=ptr[i];
                         }
    ptr=&ptr[++i];
    temp[i]='\0';
    int minutos=atoi(temp);
    minutos=minutos*60;
    memset(temp,'\0',sizeof(temp));
    for(i=0;ptr[i]!='\0'&&i<2;i++) temp[i]=ptr[i];
    temp[i]='\0';
    int segundos=atoi(temp);
    int total=horas+minutos+segundos;
    long long hora24=24*60*60;
    time_t secs=0;
    time(&secs);
    tm *fecha=localtime(&secs);
    long long ahora=fecha->tm_hour*60*60+fecha->tm_min*60+fecha->tm_sec;
    long long dif;
    if(ahora<total){
                    dif=total-ahora;
                    }else{
                    dif=hora24-ahora+total;
                    }
    Sleep(dif*1000);
    }
    
}


DWORD WINAPI enviarmaterias(struct sending *enviar){
      char *datos,*post,*ptr,*p,hora[10],url[110]="dorf1.php";
      int i;
      post=(char*)malloc(240);
      strcpy(hora,enviar->hora);
      if(naldeas>1){
         strcat(url,"?newdid=");
         strcat(url,enviar->newdid);
      }
         
      
      strcpy(post,"id=");
      strcat(post,enviar->id);
      strcat(post,"&r1=");
      strcat(post,enviar->materias[0]);
      strcat(post,"&r2=");
      strcat(post,enviar->materias[1]);
      strcat(post,"&r3=");
      strcat(post,enviar->materias[2]);
      strcat(post,"&r4=");
      strcat(post,enviar->materias[3]);
      strcat(post,"&x=");
      strcat(post,enviar->x);
      strcat(post,"&y=");
      strcat(post,enviar->y);
      strcat(post,"&dname=&s1.x=0&s1.y=0&s1=ok");
      
      free(enviar);
      vegetar(hora,0);
      datos=pedir(url,0,NULL);
      strcpy(url,"build.php\0");
      free(datos);
      datos=pedir(url,1,post);
      ptr=strstr(post,"&x=");
      strcpy(ptr,"&a=");
      p=strstr(datos,"<input type=\"hidden\" name=\"a\" value=\"");
      p+=strlen("<input type=\"hidden\" name=\"a\" value=\"");
      for(i=0;p[i]!='\"';i++) strncat(ptr,p+i,1);
      strcat(ptr,"&sz=");
      p=strstr(datos,"<input type=\"hidden\" name=\"sz\" value=\"");
      p+=strlen("<input type=\"hidden\" name=\"sz\" value=\"");
      for(i=0;p[i]!='\"';i++) strncat(ptr,p+i,1);
      printf("yyy");
      strcat(ptr,"&kid=");
      p=strstr(datos,"<input type=\"hidden\" name=\"kid\" value=\"");
      p+=strlen("<input type=\"hidden\" name=\"kid\" value=\"");
      for(i=0;p[i]!='\"';i++) strncat(ptr,p+i,1);
      strcat(ptr,"&s1.x=0&s1.y=0&s1=ok");
      free(datos);
      datos=pedir(url,1,post);
      free(datos);
      free(post);
      }


DWORD WINAPI atacar(struct attacking *ataque){
      char *datos,*post,*post2,*ptr,*url,itos[5],*buscar,todos[8];
      int i,j;
      buscar=(char*)malloc(60);
      url=(char*)malloc(80);
      post=(char*)malloc(200);
      post2=(char*)malloc(100);
      strcpy(url,"a2b.php");
      if(naldeas>1){
         strcat(url,"?newdid=");
         strcat(url,ataque->newdid);
      }
      strcpy(post,"b=1&");
      if(ataque->tipo!=1)
      vegetar(ataque->hora,ataque->tipo);
      datos=pedir(url,0,NULL);
      for(i=0;i<11;i++){
              buscar[0]='\0';
              if(strcmp(ataque->t[i],"0")==0) sprintf(post,"%st%d=0&",post,i+1);
              else{
                   sprintf(buscar,"<a href=\"#\" onClick=\"document.snd.t%d.value=",i+1);
                   if((ptr=strstr(datos,buscar))!=NULL){
                           ptr+=strlen(buscar);
                           for(j=0;ptr[j]!=';'&&j<8;j++) strncpy(todos+j,ptr+j,1);
                           todos[j]='\0';
                           if(atoi(ataque->t[i])<=atoi(todos)&&atoi(ataque->t[i])>0&&strcmp(ataque->t[i],"t")!=0)
                                     sprintf(post,"%st%d=%s&",post,i+1,ataque->t[i]);
                           if(atoi(ataque->t[i])<0&&strcmp(ataque->t[i],"t")!=0){
                                     sprintf(post,"%st%d=0&",post,i+1);
                                     continue;
                                     }
                           if(atoi(ataque->t[i])>atoi(todos)&&strcmp(ataque->t[i],"t")!=0)
                                     strcpy(ataque->t[i],"t");
                           if(strcmp(ataque->t[i],"t")==0) 
                           sprintf(post,"%st%d=%s&",post,i+1,todos);
                           }else{
                                 sprintf(post,"%st%d=0&",post,i+1);
                                 continue;
                                 }
                   }
              }
      ptr=post+4;
      strcpy(post2,ptr);
      sprintf(post,"%sc=%c&dname=&x=%s&y=%s&s1.x=0&s1.y=0&s1=ok",post,ataque->c,ataque->x,ataque->y);
      
      free(datos);
      datos=pedir(url,1,post);
      if(strstr(datos,"No existe ninguna aldea en ")!=NULL){
                          printf("\nCoordenadas invalidas"); return 0;}
      
      ptr=datos;
      strcpy(post,"id=39&a=");
      ptr=strstr(ptr,"<input type=\"hidden\" name=\"a\" value=\"");
      if(ptr==NULL){return 0;}
      ptr+=strlen("<input type=\"hidden\" name=\"a\" value=\"");
      for(i=0;ptr[i]!='\"';i++) strncat(post,ptr+i,1);
      strcat(post,"&c=");
      ptr=strstr(ptr,"<input type=\"hidden\" name=\"c\" value=\"");
      if(ptr==NULL){printf("Error");return 0;}
      ptr+=strlen("<input type=\"hidden\" name=\"c\" value=\"");
      for(i=0;ptr[i]!='\"';i++) strncat(post,ptr+i,1);
      strcat(post,"&kid=");
      ptr=strstr(ptr,"<input type=\"hidden\" name=\"kid\" value=\"");
      if(ptr==NULL){printf("Error");return 0;}
      ptr+=strlen("<input type=\"hidden\" name=\"kid\" value=\"");
      for(i=0;ptr[i]!='\"';i++) strncat(post,ptr+i,1);
      sprintf(post,"%s&%s&s1.x=0&s1.y=0&s1=ok",post,post2);
      free(datos);
      datos=pedir(url,1,post);
      free(ataque);
      free(datos);
      free(post);
      free(post2);
      free(url);
      free(buscar);
      }
      

DWORD WINAPI alarmar(struct alarming *alarma){
      char *datos,*url,*stiempo,temp[4],stotal[10],*ptr;
      int i,j,total=0,total2=0,activar=0,dormir;
      time_t *llegada1,*llegada2;
      HANDLE hhilo;
      DWORD hid;
      emptying *desalojo;
      desalojo=(emptying*)malloc(sizeof(emptying));
      
      desalojo->ataque=(attacking*)malloc(sizeof(attacking));
      memcpy(desalojo->ataque,alarma->ataque,sizeof(attacking));
      llegada1=(time_t*)malloc(sizeof(time_t)*naldeas);
      llegada2=(time_t*)malloc(sizeof(time_t)*naldeas);
      for(i=0;i<naldeas;i++) llegada2[i]=0;
      stiempo=(char*)malloc(12);
      url=(char*)malloc(60);
      strcpy(url,"dorf1.php");
      
      while(1){
      if(naldeas>1){
         for(i=0;i<naldeas;i++){
             strcpy(url,"dorf1.php?newdid=");
             strcat(url,aldeas[i].id);
             printf("b");
             datos=pedir(url,0,NULL);
             printf("a");
             if((ptr=strstr(datos,"<img src=\"img/un/a/att1.gif\""))!=NULL){
                printf("d");
                if(alarma->desalojar=='s'){
                   time(&llegada1[i]);
                   ptr=strstr(ptr,"<span id=timer");
                   ptr=strstr(ptr,">");
                   ptr+=strlen(">");
                   for(j=0;ptr[j]!='<'&&j<11;j++) strncpy(stiempo+j,ptr+j,1);
                   stiempo[j]='\0';
                   printf("\nLLega en %s",stiempo);
                   for(j=0;stiempo[j]!=':'&&stiempo[j]!='\0';j++) strncpy(temp+j,stiempo+j,1);
                   temp[j]='\0';
                   stiempo=&stiempo[++j];
                   total=atoi(temp)*3600;
                   for(j=0;stiempo[j]!=':'&&stiempo[j]!='\0';j++) strncpy(temp+j,stiempo+j,1);
                   temp[j]='\0';
                   stiempo=&stiempo[++j];
                   total+=atoi(temp)*60;
                   for(j=0;stiempo[j]!='\0';j++) strncpy(temp+j,stiempo+j,1);
                   temp[j]='\0';
                   total+=atoi(temp);
                   llegada1[i]+=total;
                   printf("  %d",llegada1[i]);
                   if(llegada1[i]-llegada2[i]<-3||llegada2[i]==0){
                      printf("\nnuevo ataque?");
                      printf("\n%d\n%d",llegada1[i],llegada2[i]);
                      printf("ff");
                      desalojo->total=total;
                      strcpy(desalojo->ataque->newdid,aldeas[i].id);
                      hhilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)desalojar,(PVOID)desalojo,0,&hid);
                      llegada2[i]=llegada1[i];
                      activar=0;
                      }
                   if(llegada1[i]-llegada2[i]>10) llegada2[i]=0;
                   }
                if(alarma->avisar=='s'&&activar==0){
                   printf("\a\a\a");
                   Sleep(1000);
                   printf("\a\a\a\a");
                   Sleep(1000);
                   printf("\a\a\a\a\a");
                   activar=1;
                   }
                }
                
             }
      }else{
         datos=pedir(url,0,NULL);
         if((ptr=strstr(datos,"<img src=\"img/un/a/att1.gif\""))!=NULL){
                if(alarma->desalojar=='s'){
                   time(&llegada1[0]);
                   ptr=strstr(ptr,"<span id=timer");
                   ptr=strstr(ptr,">");
                   ptr+=strlen(">");
                   for(j=0;ptr[j]!='<';j++) strncpy(stiempo+j,ptr+j,1);
                   stiempo[j]='\0';
                   printf("\nLLega en %s",stiempo);
                   for(j=0;stiempo[j]!=':'&&stiempo[j]!='\0';j++) strncpy(temp+j,stiempo+j,1);
                   temp[j]='\0';
                   stiempo=&stiempo[++j];
                   total=atoi(temp)*3600;
                   for(j=0;stiempo[j]!=':'&&stiempo[j]!='\0';j++) strncpy(temp+j,stiempo+j,1);
                   temp[j]='\0';
                   stiempo=&stiempo[++j];
                   total+=atoi(temp)*60;
                   for(j=0;stiempo[j]!='\0';j++) strncpy(temp+j,stiempo+j,1);
                   temp[j]='\0';
                   total+=atoi(temp);
                   llegada1[0]+=total;
                   printf("  %d",llegada1[0]);
                   if(llegada1[0]-llegada2[0]<-3||llegada2[0]==0){
                      printf("\nnuevo ataque?");
                      printf("\n%d\n%d",llegada1[0],llegada2[0]);
                      printf("ff");
                      desalojo->total=total;
                      printf("aa");
                      hhilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)desalojar,(PVOID)desalojo,0,&hid);
                      llegada2[0]=llegada1[0];
                      activar=0;
                      }
                   if(llegada1[0]-llegada2[0]>10) llegada2[0]=0;
                   }
                if(alarma->avisar=='s'&&activar==0){
                   printf("\a\a\a");
                   Sleep(1000);
                   printf("\a\a\a\a");
                   Sleep(1000);
                   printf("\a\a\a\a\a");
                   
                   activar=1;
                   }
                }
                
         }
      free(datos);
      dormir=aleatorio(60,250);
      Sleep(dormir*1000);
      }
      free(url);
      free(stiempo);
      }


DWORD WINAPI desalojar(struct emptying *desalojo){
      int total;
      HANDLE hhilo;
      DWORD hid;
      time_t ppio,final;
      attacking* attack;
      attack=(attacking*)malloc(sizeof(attacking));
      printf("hh");
      strcpy(desalojo->ataque->hora,"0");
      desalojo->ataque->tipo=1;
      memcpy(attack,desalojo->ataque,sizeof(attacking));
      total=desalojo->total-40;
      printf("\nDormira %d segundos",total);
      Sleep(total*1000);
      hhilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)atacar,(PVOID)attack,0,&hid);
      Sleep(33*1000);
      hhilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)atacar,(PVOID)attack,0,&hid);
      Sleep(5*1000);
      time(&ppio);
      final=ppio+3;
      for(;final-ppio>0;){
           hhilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)atacar,(PVOID)attack,0,&hid);
           Sleep(150);
           time(&ppio);
           }
      }

DWORD WINAPI newaldeas(char *hora){
      aldeanueva=1;
      vegetar(hora,0);
      free(aldeas);
      aldeas=getaldeas(&naldeas);
      }

int aleatorio(int minimo, int maximo){
    Sleep(152);
    srand(clock());
    int dif=maximo-minimo;
    int random=rand()%(dif+1);
    random+=minimo;
    return random;
    }
    
DWORD WINAPI buscar(struct casillas *casilla){
      int i,j,k,l,z,pasosx,pasosy,x,y,granjas=0,centrox,centroy,poblado;
      FILE *bd,*rbd;
      char *datos,*post,*ptr,*ptr2,*link,c[4],aldea[30],jugador[30],alianza[30],habitantes[8];
      long id;
      
      post=(char*)malloc(40);
      link=(char*)malloc(50);
      
      bd=fopen("datos.txt","a+");
      
      pasosx=abs(casilla->x1-casilla->x2)/7+1;
      pasosy=abs(casilla->y1-casilla->y2)/7+1;
      centrox=casilla->x1+3;
      centroy=casilla->y1-3;
      for(j=0;j<=pasosy;j++){
            Sleep(aleatorio(3,12)*100);
            if(j!=0) centroy-=7;
            centrox=casilla->x1+3;
            for(i=0;i<=pasosx;i++){
                  if(i!=0) centrox+=7;
                  sprintf(post,"xp=%d&yp=%d&s1.x=0&s1.y=0&s1=ok",centrox,centroy);
                  printf("\n%d %d",centrox,centroy);
                  datos=pedir("karte.php",1,post);
                  for(k=0;k<7;k++){
                        for(l=0;l<7;l++){
                            Sleep(aleatorio(50,150));
                            x=casilla->x1+i*7+l;
                            y=casilla->y1-j*7-k;
                            printf("\n%d %d %d %d %d %d",j,i,k,l,x,y);
                            if(y<casilla->y2){
                                 printf("a");
                                 if(x+1>casilla->x2){
                                 l=7;
                                 k=7;
                                 i=pasosx;
                                 j=pasosy;
                                 break;
                                 }else{
                                 k=7;
                                 l=7;
                                 break;
                                 }
                            }
                            if(x>casilla->x2){
                                 printf("b");
                                 if(y-1<casilla->y2){
                                 l=7;
                                 k=7;
                                 i=pasosx;
                                 j=pasosy;
                                 break;
                                 }else{
                                 l=7;
                                 break;
                                 }
                            }
                            id=xy2id(x,y);
                            sprintf(link,"href=\"karte.php?d=%d&c=",id);
                            printf("\n%s\n",link);
                            ptr=strstr(datos,link);
                            if(ptr==NULL){ printf("coordenada no encontrada"); break;}
                            ptr+=strlen(link);
                            for(z=0;ptr[z]!='\"'&&z<2;z++) strncpy(c+z,ptr+z,1);
                            c[z]='\0';
                            ptr2=strstr(ptr,">");
                            if(strstr(ptr,"shape=\"poly\"alt=\"")!=NULL&&ptr<ptr2){
                               poblado=1;
                               }else poblado=0;
                            sprintf(link,"karte.php?d=%d&c=%s",id,c);
                            printf("\n%s\n",link);
                            granjas=AnalizarCasilla(link,poblado);
                            if(granjas>0){
                               if(poblado==1){
                                  ptr=strstr(ptr," onmouseover=\"map(\'");
                                  if(ptr==NULL||ptr>ptr2){ printf("Poblado2?"); poblado=0;
                                  }else{
                                    ptr+=strlen(" onmouseover=\"map(\'");
                                    for(z=0;ptr[z]!='\''&&z<29;z++) strncpy(aldea+z,ptr+z,1);
                                    aldea[z]='\0';
                                    ptr=strstr(ptr,"\',\'");
                                    ptr+=strlen("\',\'");
                                    for(z=0;ptr[z]!='\''&&z<29;z++) strncpy(jugador+z,ptr+z,1);
                                    jugador[z]='\0';
                                    ptr=strstr(ptr,"\',\'");
                                    ptr+=strlen("\',\'");
                                    for(z=0;ptr[z]!='\''&&z<7;z++) strncpy(habitantes+z,ptr+z,1);
                                    habitantes[z]='\0';
                                    ptr=strstr(ptr,"\',\'");
                                    ptr+=strlen("\',\'");
                                    for(z=0;ptr[z]!='\''&&z<29;z++) strncpy(alianza+z,ptr+z,1);
                                    alianza[z]='\0';
                                    fprintf(bd,"%dc\t%d|%d\t%s\t%s\t%s\t%s\n",granjas,x,y,jugador,aldea,alianza,habitantes);
                                  }
                               }
                               if(poblado==0){
                                  fprintf(bd,"%dc\t%d|%d\n",granjas,x,y);
                               }
                            }
                        }
                  }
            }
      }
}
                        
      
      
      
long xy2id(int x,int y){
     return 1+(x+400)+(801*abs(y-400));
     }


int abs(int num){
    if(num<0) return -num; else return num;
}


int AnalizarCasilla(char* url,int poblado){
     char *datos,*ptr,cereales[4],f[4];
     int i,crop;
     
     datos=pedir(url,0,NULL);
     if(poblado==0){
        ptr=strstr(datos,"<td><img class=\"res\" src=\"img/un/r/4.gif\"></td>\n<td class=\"s7 b\">");
        if(ptr==NULL){ printf("Parece no poblado"); return -1;}
        ptr+=strlen("<td><img class=\"res\" src=\"img/un/r/4.gif\"></td>\n<td class=\"s7 b\">");
        for(i=0;ptr[i]!='<'&&i<3;i++) strncpy(cereales+i,ptr+i,1);
        cereales[i]='\0';
        free(datos);
        if(atoi(cereales)==9||atoi(cereales)==15) return atoi(cereales);
        else return 0;
     }else if(poblado==1){
        ptr=strstr(datos,"<div id=\"f");
        if(ptr==NULL){ printf("No parece poblado"); return 0;}
        ptr+=strlen("<div id=\"f");
        for(i=0;ptr[i]!='\"'&&i<3;i++) strncpy(f+i,ptr+i,1);
        f[i]='\0';
        free(datos);
        if(strcmp(f,"1")==0) crop=9;
        else if(strcmp(f,"6")==0) crop=15;
        else crop=0;
        return crop;
        }
     
     }
        


char* urlencode(char* c)
{
	int i,j;
	char *h = "0123456789abcdef";
	char *resul;
	resul=(char*)malloc(strlen(c)*3);

	for(i=0,j=0;i<strlen(c);i++,j++){
		if( 'a' <= c[i] && c[i] <= 'z'
		|| 'A' <= c[i] && c[i] <= 'Z'
		|| '0' <= c[i] && c[i] <= '9'
		|| c[i] == '-' || c[i] == '_' || c[i] == '.' )
			resul[j]=c[i];
		else if( c[i] == ' ' )
			resul[j]='+';
		else {
            resul[j++]='%';
			resul[j++]=h[c[i] >> 4];
			resul[j]=h[c[i] & 0x0f];
		}
	}
	resul[j]='\0';
	return resul;
}



DWORD WINAPI EnviarIgm(struct message *mensaje){
      int i,j,nremitentes;
      char *igm,*topic,*post,*datos,*ptr,**remitentes;
      remitentes=(char**)malloc((mensaje->nremitentes)*sizeof(char*));
      for(i=0;i<mensaje->nremitentes;i++){
         remitentes[i]=(char*)malloc(sizeof(char)*30);
         strcpy(remitentes[i],mensaje->remitentes[i]);
         }
      nremitentes=mensaje->nremitentes;
      igm=urlencode(mensaje->texto);
      printf("\n\n%s",igm);
      topic=urlencode(mensaje->topic);
      printf("\n\n%s",topic);
      post=(char*)malloc(strlen(igm)+strlen(topic)+200);
      vegetar(mensaje->hora,mensaje->tipo);
      for(j=0;j<nremitentes;j++){
         if(j%5==0&&j!=0){
            Sleep(65*1000);
         }
         printf("\n%s",remitentes[j]);
         strcpy(post,"c=");
         datos=pedir("nachrichten.php?t=1",0,NULL);
         if((ptr=strstr(datos,"<input type=\"hidden\" name=\"c\" value=\""))==NULL){
            printf("\nError en la pagina\n");
            return 0;
         }else{
            ptr+=strlen("<input type=\"hidden\" name=\"c\" value=\"");
            for(i=0;ptr[i]!='\"';i++) post[2+i]=ptr[i];
            post[2+i]='\0';
            }
         strcat(post,"&an=");
         strcat(post,remitentes[j]);
         strcat(post,"&be=");
         strcat(post,topic);
         strcat(post,"&message=");
         strcat(post,igm);
         strcat(post,"&t=2&s1.x=38&s1.y=12");
         free(datos);
         Sleep(1000);
         datos=pedir("nachrichten.php",1,post);
         Sleep(10*1000);
         }
}
         
         
DWORD WINAPI FakearCoord(struct faking *fake){
      int i,j,k,l,z,pasosx,pasosy,x,y,granjas=0,centrox,centroy,poblado;
      char *datos,*post,*ptr,*ptr2,*link,c[4],aldea[30],jugador[30],alianza[30],habitantes[8],*sx,*sy;
      long id;
      attacking *ataque;
      HANDLE hhilo;
      DWORD hid;
      
      post=(char*)malloc(40);
      link=(char*)malloc(50);
      
      
      pasosx=abs(fake->x1-fake->x2)/7+1;
      pasosy=abs(fake->y1-fake->y2)/7+1;
      centrox=fake->x1+3;
      centroy=fake->y1-3;
      for(j=0;j<=pasosy;j++){
            if(j!=0) centroy-=7;
            centrox=fake->x1+3;
            for(i=0;i<=pasosx;i++){
                  if(i!=0) centrox+=7;
                  sprintf(post,"xp=%d&yp=%d&s1.x=0&s1.y=0&s1=ok",centrox,centroy);
                  printf("\n%d %d",centrox,centroy);
                  datos=pedir("karte.php",1,post);
                  for(k=0;k<7;k++){
                        for(l=0;l<7;l++){
                            Sleep(100);
                            x=fake->x1+i*7+l;
                            y=fake->y1-j*7-k;
                            if(y<fake->y2){
                                 printf("a");
                                 if(x+1>fake->x2){
                                 l=7;
                                 k=7;
                                 i=pasosx;
                                 j=pasosy;
                                 break;
                                 }else{
                                 k=7;
                                 l=7;
                                 break;
                                 }
                            }
                            if(x>fake->x2){
                                 printf("b");
                                 if(y-1<fake->y2){
                                 l=7;
                                 k=7;
                                 i=pasosx;
                                 j=pasosy;
                                 break;
                                 }else{
                                 l=7;
                                 break;
                                 }
                            }
                            printf("\n%d/%d ==> ",x,y);
                            id=xy2id(x,y);
                            sprintf(link,"<area href=\"karte.php?d=%d&amp;c=",id);
                            ptr=strstr(datos,link);
                            if(ptr==NULL){ printf("coordenada no encontrada"); break;}
                            ptr+=strlen(link);
                            for(z=0;ptr[z]!='\"'&&z<2;z++) strncpy(c+z,ptr+z,1);
                            c[z]='\0';
                            ptr2=strstr(ptr,">");
                            if(strstr(ptr,"shape=\"poly\"alt=\"")!=NULL&&ptr<ptr2){
                               poblado=1;
                               }else poblado=0;
                            sprintf(link,"karte.php?d=%d&c=%s",id,c);
                            if(poblado==1){
                                  ptr=strstr(ptr," onmouseover=\"map(\'");
                                  if(ptr==NULL||ptr>ptr2){ printf("Poblado2?"); poblado=0;
                                  }else{
                                    ptr+=strlen(" onmouseover=\"map(\'");
                                    for(z=0;ptr[z]!='\''&&z<29;z++) strncpy(aldea+z,ptr+z,1);
                                    aldea[z]='\0';
                                    ptr=strstr(ptr,"\',\'");
                                    ptr+=strlen("\',\'");
                                    for(z=0;ptr[z]!='\''&&z<29;z++) strncpy(jugador+z,ptr+z,1);
                                    jugador[z]='\0';
                                    ptr=strstr(ptr,"\',\'");
                                    ptr+=strlen("\',\'");
                                    for(z=0;ptr[z]!='\''&&z<7;z++) strncpy(habitantes+z,ptr+z,1);
                                    habitantes[z]='\0';
                                    ptr=strstr(ptr,"\',\'");
                                    ptr+=strlen("\',\'");
                                    for(z=0;ptr[z]!='\''&&z<29;z++) strncpy(alianza+z,ptr+z,1);
                                    alianza[z]='\0';
                                    if(strcmp(alianza,fake->alianza)==0&&atoi(habitantes)>=fake->habitantes){
                                       printf("\n%d/%d => ",x,y);
                                       ataque=(attacking*)malloc(sizeof(attacking));
                                       memcpy(ataque,fake->ataque,sizeof(attacking));
                                       itoa(x,ataque->x,10);
                                       itoa(y,ataque->y,10);
                                       for(i=0;i<fake->nataques;i++){
                                          hhilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)atacar,(PVOID)ataque,0,&hid);
                                          }
                                    Sleep(1000);
                                    }
                                  }
                            }
                        }
                  }
            }
      }
}


DWORD WINAPI FakearJugadores(struct faking *fake){
      int i,j;
      char *buffer,*post,*ptr,*url,habitantes[8];
      attacking *ataque,*attack;
      HANDLE hhilo;
      DWORD hid;
      
      fake->ataque->tipo=1;
      strcpy(fake->ataque->hora,"0");
      post=(char*)malloc(sizeof(char)*80);
      url=(char*)malloc(sizeof(char)*50);
      
      printf("aa");
      if(fake->tipo!=1)
      vegetar(fake->hora,fake->tipo);
      buffer=pedir("dorf1.php",0,NULL);
      printf("\n%d",fake->njugadores);
      for(i=0;i<fake->njugadores;i++){
         printf("\n\n%s: ",fake->jugadores[i]);
         sprintf(post,"spieler=%s&s1.x=0&s1.y=0&s1=ok",fake->jugadores[i]);
         buffer=pedir("statistiken.php",1,post);
         ptr=strstr(buffer,"<td class=\"s7 ou\"><a href=\"");
         if(ptr==NULL){printf("\nJugador no encontrado"); return 0;}
         ptr+=strlen("<td class=\"s7 ou\"><a href=\"");
         for(j=0;ptr[j]!='\"';j++) strncpy(url+j,ptr+j,1);
         url[j]='\0';
         free(buffer);
         buffer=pedir(url,0,NULL);
         ptr=strstr(buffer,"<td width=\"25%\">Coordenadas</td>");
         if(ptr==NULL){printf("\nFallo al buscar las aldeas"); return 0;}
         ptr+=strlen("<td width=\"25%\">Coordenadas</td>");
         while((ptr=strstr(ptr,"<td class=\"s7\"><a href=\"karte.php?d="))!=NULL){
             ataque=(attacking*)malloc(sizeof(attacking));
             memcpy(ataque,fake->ataque,sizeof(attacking));
             ptr=strstr(ptr,"<td>");
             if(ptr==NULL){ printf("Fallo al buscar las aldeas 2"); return 0;}
             ptr+=strlen("<td>");
             for(j=0;ptr[j]!='<';j++) strncpy(habitantes+j,ptr+j,1);
             habitantes[j]='\0';
             if(atoi(habitantes)>=fake->habitantes){
                ptr=strstr(ptr,"<td>");
                if(ptr==NULL){ printf("Y las coordenadas?"); return 0;}
                ptr+=strlen("<td>");
                ptr++;
                for(j=0;ptr[j]!='|';j++) strncpy(ataque->x+j,ptr+j,1);
                ataque->x[j]='\0';
                ptr+=j+1;
                for(j=0;ptr[j]!=')';j++) strncpy(ataque->y+j,ptr+j,1);
                ataque->y[j]='\0';
                printf("\n(%s|%s)",ataque->x,ataque->y);
                for(j=0;j<fake->nataques;j++){
                   attack=(attacking*)malloc(sizeof(attacking));
                   memcpy(attack,ataque,sizeof(attacking));
                   hhilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)atacar,(PVOID)attack,0,&hid);
                   Sleep(500);
                }
             Sleep(1000);
             }else break;
         }
      }
}
      




