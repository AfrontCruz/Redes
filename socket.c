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

/* tarea!!!!! IP, MASCARA */
/* puerta de enlace: route
   DNS: resource.conf	*/


int obtenerDatos(int ds){
	char nombre[10];
	unsigned char mac[6];
	int index;
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
		for(int i = 0; i < 6; i++)
			(i==5)?printf("%x", mac[i]):printf("%x:", mac[i]);
		printf("\n\n\n");
	}


	return index;
}

/*unsigned char* obtenerMacAddres(int ds, char nombre[10]){
	unsigned char macAddres[6];
	struct ifreq interfaz;
	strinterfaz
	if( ioctl(ds, SIOCGIFHWADDR, &interfaz) == -1 ){
		perror("\nError al obtener el indice");
	}
	
}*/

int main(){
	int packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(packet_socket == -1)
		perror("\nError al abrir el socket");
	else{
		perror("\nExito al abrir el socket");
		int index = obtenerDatos(packet_socket);
		printf("El indice es: %d\n", index);
	}
	
	close(packet_socket);
	return 0;
}


