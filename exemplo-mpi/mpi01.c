#include <stdio.h>
#include <string.h>
#include "mpi.h"

main(int argc, char** argv)
{
	int rank, nroRanks;
	char hostName[MPI_MAX_PROCESSOR_NAME];
	int tamanhoHostName;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nroRanks);
	MPI_Get_processor_name(hostName, &tamanhoHostName);

	printf("Meu rank %d de %d executando no host %s\n", rank, nroRanks, hostName);

	MPI_Finalize();
}

