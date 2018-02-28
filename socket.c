#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>


char nombre[10];
unsigned char mac[6];
unsigned char ip[4], mask[4];
int index;

int obtenerDatos(int ds){
	struct ifreq interfaz;
	gets(nombre);
	strcpy(interfaz.ifr_name, nombre);

	if( ioctl(ds, SIOCGIFINDEX, &interfaz) == -1 ){
		perror("\nError al obtener el indice");
		exit(0);
	}
	else{
		index = interfaz.ifr_ifindex;
	}
	
	if( ioctl(ds, SIOCGIFHWADDR, &interfaz) == -1 ){
		perror("\nError al obtener el indice");
	}
	else{
		for(int i = 0; i < 6; i++)
			mac[i] = interfaz.ifr_hwaddr.sa_data[i];

    printf("La MAC:\n\n");

		for(int i = 0; i < 6; i++)
			(i==5)?printf("%02x", mac[i]):printf("%2x:", mac[i]);
		printf("\n\n\n");
	}
  
  if(ioctl(ds, SIOCGIFADDR, &interfaz) ){
    perror("\nError al obtener al address");
  }
  else{
    for(int i = 0; i < 4; i++)
      ip[i] = interfaz.ifr_addr.sa_data[i+2];

    printf("La IP: \n\n");

		for(int i = 0; i < 4; i++)
			i == 3 ? printf("%i",ip[i]):printf("%i.",ip[i]);
    printf("\n\n");
  }


  if(ioctl(ds, SIOCGIFNETMASK, &interfaz) ){
    perror("\nError al obtener al address");
  }
  else{
    for(int i = 0; i < 4; i++)
      mask[i] = interfaz.ifr_netmask.sa_data[i+2];

    printf("La mascara subred:\n\n");

		for(int i = 0; i < 4; i++)
			i == 3 ? printf("%i",mask[i]):printf("%i.",mask[i]);
      printf("\n\n");
  }
  

  unsigned char trama[50];

  for(int i = 0; i < 6; i++)
    trama[i] = 0xFF;

  for(int i = 6; i < 12; i++)
    trama[i] = mac[i-6];

  trama[12] = 0x0C;
  trama[13] = 0x0C;

  char miNombre[] = "que ondita bandita";
  for(int i = 0; i < 24; i++)
    trama[i + 14] = miNombre[i];

  enviarTrama(ds, index, trama);

	return index;
}

void enviarTrama(int ds, int indexx, unsigned char *trama){
  struct sockaddr_ll nic;
  memset(&nic, 0x00, sizeof(nic));
  nic.sll_family = AF_PACKET;
  nic.sll_protocol = htons(ETH_P_ALL); 
  nic.sll_ifindex = indexx; 
  int tam = sendto(ds, trama, 60, 0, (struct sockaddr *)&nic, sizeof(nic) );
  if(tam == -1)
    printf("Hubo un error");
  else
    printf("Se envio el mensaje\n\n\n\n\n");
}



void recibirTrama(int ds, unsigned char *bufferTrama)
{
  int tam;
  tam = recvfrom(ds, bufferTrama, 1514, 0, NULL, 0);
  if(tam == -1)
    perror("Hubo un error al recibir trama\n\n\n");
  else
    if( !memcmp(bufferTrama + 0, mac, 6) )
      imprimeTrama(bufferTrama, tam);
  
}


void imprimeTrama(unsigned char *trama, int tam_trama){
  for(int i = 0; i < tam_trama; i++ ){
    if(i%16 == 0)
      printf("\n");
    printf("%.2x ", trama[i]);
  }
}



int main(){
  unsigned char trama[1514];
 	int packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

	if(packet_socket == -1)
		perror("\nError al abrir el socket");
	else{
		perror("\nExito al abrir el socket");
		int index_1 = obtenerDatos(packet_socket);
		printf("El indice es: %d\n", index_1);
    int j = 0;
    while(1){
      j++;
      printf("Trama %i: \n", j);
      recibirTrama(packet_socket, trama);
      printf("\n\n\n");
    }
	}
	
	close(packet_socket);
	return 0;
}

