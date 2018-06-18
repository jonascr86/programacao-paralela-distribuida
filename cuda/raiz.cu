#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <math.h>

__global__ void adicionarKernel(double* resultado, const double* n) {
	int i = threadIdx.x;
	double a = 1, b = 0;
	double delta = pow(b, 2) - (4 * a * (n[i] * -1));
	resultado[i] = ((b * -1) + sqrt(delta)) / 2 * a;
}

cudaError_t calcularRaizes(double* resultado, const double* n, unsigned int tam) {
	double* gpu_n;
	double* gpu_resultado;
	cudaError_t status;

	status = cudaSetDevice(0);
	if (status != cudaSuccess) {
		printf("cudaSetDevice falhou!");
		goto Falha;
	}

	status = cudaMalloc((void**)&gpu_resultado, tam * sizeof(double));
	if (status != cudaSuccess) {
		printf("cudaMalloc falhou!");
		goto Falha;
	}

	status = cudaMalloc((void**)&gpu_n, tam * sizeof(double));
	if (status != cudaSuccess) {
		printf("cudaMalloc falhou!");
		goto Falha;
	}

	status = cudaMemcpy(gpu_n, n, tam * sizeof(double), cudaMemcpyHostToDevice);
	if (status != cudaSuccess) {
		printf("cudaMemcpy falhou!");
		goto Falha;
	}

	adicionarKernel<<<1, tam>>>(gpu_resultado, gpu_n);

	status = cudaGetLastError();
	if (status != cudaSuccess) {
		printf("adicionarKernel falhou: %s", cudaGetErrorString(status));
		goto Falha;
	}

	status = cudaDeviceSynchronize();
	if (status != cudaSuccess) {
		printf("cudaDeviceSynchronize falhou: %d", status);
		goto Falha;
	}

	status = cudaMemcpy(resultado, gpu_resultado, tam * sizeof(double), cudaMemcpyDeviceToHost);
	if (status != cudaSuccess) {
		printf("cudaMemcpy falhou!");
		goto Falha;
	}

Falha:
	cudaFree(gpu_resultado);
	cudaFree(gpu_n);

	return status;
}

int main()
{
	const int tam = 10;
	const double n[tam] = { 1, 4, 9, 16, 25, 36, 49, 64, 81, 100 };
	double resultado[tam] = { 0 };

	cudaError_t status = calcularRaizes(resultado, n, tam);
	if (status != cudaSuccess) {
		printf("calcularRaizes falhou!");
		return 1;
;	}

	printf("RESULTADO\n");
	for (int i = 0; i < tam; i++) {
		printf("Raiz de %f = %f\n", n[i], resultado[i]);
	}

	getchar();

	status = cudaDeviceReset();
	if (status != cudaSuccess) {
		printf("cudaDeviceReset falhou!");
		return 1;
	}

	return 0;
}