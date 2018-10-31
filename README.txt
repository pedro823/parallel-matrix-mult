### Integrantes da dupla ###

- Nome: Breno Helfstein Moura  ||||  NUSP: 9790972
- Nome: Pedro Pereira          ||||  NUSP: 9778794

### Guia de execu��o ###

Para executar o ep basta executar 'make all' e executar os comandos como especificados no enunciado, isto �:

                main <implementa��o> <caminho_matr_A> <caminho_matr_B> <caminho_matr_C>

onde <implementa��o> � 'p' quando executado no modo Pthreads ou 'o' quando executado no modo OpenMP.

### Decis�es de projeto ###  

Para esse projeto foram tomadas algumas decis�es para otimizar a multiplica��o de matrizes. Seguindo as dicas do enunciado, conseguimos otimizar o cache transpondo a matriz B. Isto �, quando fazemos a multiplica��o de matrizes A * B, transpomos a matriz B para facilitar o acesso e diminuir o n�mero de "Cache Miss". Assim fizemos as seguintes mudan�as:

~ C�digo sequencial original, com muito cache miss:
for (ulli i = 0; i < A->hei; i++) {
	for (ulli j = 0; j < B->hei; j++) {
	    sum = 0;
		for (ulli k = 0; k < A->len; k++) {
			sum += A->m[i][k] * B->m[k][j];
    	}
    	mat->m[i][j] = sum;
	}
}

~ C�digo sequencial alternativo, com menos cache miss:
transpose(B);
for (ulli i = 0; i < A->hei; i++) {
	for (ulli j = 0; j < B->hei; j++) {
	    sum = 0;
		for (ulli k = 0; k < A->len; k++) {
			sum += A->m[i][k] * B->m[j][k];
    	}
    	mat->m[i][j] = sum;
	}
}

Al�m disso, paralelizamos o c�digo com OpenMP e Pthreads (como pedido no enunciado). Para paralelizar a execu��o tivemos abordagens ligeiramente diferentes com pthreads e com OpenMP. Com OpenMP optamos por paralelizar ambos os for externos, utilizando dois "#pragma omp parallel for private(sum)", para os "for" que iteram pelos indices da matriz resultado (e consequentemente pelos indices de A e B). J� com Pthreads optamos de dividir o primeiro for externo em "Peda�os" (Chunks) e separ�-los entre as threads. Chegamos a conclus�o que 32 threads era um n�mero bom para as threads.
Tamb�m foi utilizado "unsigned long long int" para a indexa��o das matrizes, assim como indicado nas dicas no enunciado.

Para a matriz que testamos, e est� sendo entregue junto com o EP, tivemos os seguintes resultados:

Time elapsed in sequential: 31.417396s
Time elapsed with transposition: 12.213758s
Time elapsed in parallel with OpenMP: 7.466603s
Time elapsed in parallel with Pthreads: 6.370319s

O computador utilizado tem as seguintes especifica��es de hardware:
- Processador: i5-6200U @ 2.3 GHz
- Mem�ria RAM: 8GB RAM DDR4 @ 2300MHz