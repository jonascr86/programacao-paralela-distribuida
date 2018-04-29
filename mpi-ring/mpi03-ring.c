#include <stdio.h>
#include <string.h>
#include "mpi.h"

main(int argc, char** argv)
{
	int rank, nroRanks;
	char hostName[MPI_MAX_PROCESSOR_NAME];
	int tamanhoHostName;
	char msg[1000];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nroRanks);
	MPI_Get_processor_name(hostName, &tamanhoHostName);
	

	if (rank == 0) {
		//printf("node 0 enviando para nodo 1\n");		
		sprintf(msg, "[%d@%s]", rank, hostName);
		MPI_Send(msg,strlen(msg)+1,MPI_CHAR,1,0,MPI_COMM_WORLD);
     //printf("node 0 recebendo do nodo %d\n", nroRanks-1);		
		MPI_Recv(msg, 1000, MPI_CHAR, nroRanks-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Processo %d recebeu: %s\n", rank, msg);
	} else {
      //printf("node %d recebendo do nodo %d\n", rank, rank-1);	
		MPI_Recv(msg, 1000, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);      
		sprintf(msg, "%s->[%d@%s]", msg, rank, hostName);
		//printf("node %d enviando para o nodo %d\n", rank, rank+1);	
		MPI_Send(msg,strlen(msg)+1,MPI_CHAR,(rank+1)%nroRanks,0,MPI_COMM_WORLD);
	}

	MPI_Finalize();
}

