#include <stdio.h>
#include <string.h>
#include "mpi.h"

main(int argc, char** argv)
{
	int nroNodo, nroNodos;
	char hostName[MPI_MAX_PROCESSOR_NAME];
	int tamanhoHostName;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &nroNodo);
	MPI_Comm_size(MPI_COMM_WORLD, &nroNodos);
	MPI_Get_processor_name(hostName, &tamanhoHostName);

	int tamanhoVetor = 100000;
	int qtdePorNodo = (int)tamanhoVetor / nroNodos;
	int resultado = 1;

	int inicio = nroNodo * qtdePorNodo;
	int fim = inicio + qtdePorNodo - 1;

	printf("[nodo: %d@%s] %d .. %d\n", nroNodo, hostName, inicio, fim);

	int a = 0;
	for(int i = inicio; i < fim; i++) {
		resultado = resultado + 1;

		for(int j  = 0; j < 100000; j++) {
			a = (a + i) * j;
		}

	}
	printf("[nodo: %d@%s] resultado parcial: %d\n", nroNodo, hostName, resultado);

	int resultadoFinal = 0;

	MPI_Reduce(&resultado, &resultadoFinal, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (nroNodo == 0) {		
		printf("resultado final: %d\n", resultadoFinal);
	}



	MPI_Finalize();
}

