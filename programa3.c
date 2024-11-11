#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void leermatriz(int n, double matriz[n][n], double vector[n]) {
    printf("Introduce los coeficientes de la matriz:\n");
    int i,j;
    for ( i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("Elemento [%d][%d]: ", i + 1, j + 1);
            if (scanf("%lf", &matriz[i][j]) != 1) {
                printf("Entrada inválida. Finalizando.\n");
                exit(1);
            }
        }
        printf("Introduce el valor del vector independiente en [%d]: ", i + 1);
        if (scanf("%lf", &vector[i]) != 1) {
            printf("Entrada invalida. Finalizando.\n");
            exit(1);
        }
    }
}

void mostrarmatriz(int n, double matriz[n][n], double vector[n]) {
    printf("\nMatriz:\n");
    int i,j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%8.3lf ", matriz[i][j]);
        }
        printf("| %8.3lf\n", vector[i]);
    }
}

void corregircoeficiente(int n, double matriz[n][n], double vector[n]) {
    int fila, columna;
    printf("Fila a corregir [i]: ");
    if (scanf("%d", &fila) != 1 || fila < 1 || fila > n) {
        printf("Entrada invalida. Finalizando.\n");
        exit(1);
    }
    printf("Columna a corregir [j] (o 0 para el vector independiente): ");
    if (scanf("%d", &columna) != 1 || columna < 0 || columna > n) {
        printf("Entrada inválida. Finalizando.\n");
        exit(1);
    }

    if (columna == 0) {
        printf("Introduce el nuevo valor del vector independiente en [%d]: ", fila);
        if (scanf("%lf", &vector[fila-1]) != 1) {
            printf("Entrada inválida. Finalizando.\n");
            exit(1);
        }
    } else {
        printf("Ingresar el nuevo valor para [%d][%d]: ", fila, columna);
        if (scanf("%lf", &matriz[fila-1][columna-1]) != 1) {
            printf("Entrada invalida. Finalizando.\n");
            exit(1);
        }
    }
}

int DD(int n, double matriz[n][n]) {
	int i,j;
    for (i = 0; i < n; i++) {
        double suma = 0;
        for (j = 0; j < n; j++) {
            if (i != j) {
                suma += fabs(matriz[i][j]);
            }
        }
        if (fabs(matriz[i][i]) < suma) {
            return 0;
        }
    }
    return 1;
}

double determinante(int n, double matriz[n][n]) {
    double copiamatriz[n][n];
    int i,j,k;
    for (i = 0; i < n; i++) {
        for ( j = 0; j < n; j++) {
            copiamatriz[i][j] = matriz[i][j];
        }
    }

    double det = 1;
    for (i = 0; i < n; i++) {
        for (k = i + 1; k < n; k++) {
            if (copiamatriz[i][i] == 0) return 0;
            double factor = copiamatriz[k][i] / copiamatriz[i][i];
            for (j = i; j < n; j++) {
                copiamatriz[k][j] -= factor * copiamatriz[i][j];
            }
        }
        det *= copiamatriz[i][i];
    }
    return det;
}

void jacobi(int n, double matriz[n][n], double vector[n], double x_inicial[n], int max_iter, double tol) {
    double x[n], x_new[n], error;
    int i,j,k;
    for ( i = 0; i < n; i++) {
        x[i] = x_inicial[i];
    }

    printf("\nIteracion\tVector solucion\t\tError\n");

    for (k = 1; k <= max_iter; k++) {
        for (i = 0; i < n; i++) {
            double suma = 0;
            for (j = 0; j < n; j++) {
                if (i != j) {
                    suma += matriz[i][j] * x[j];
                }
            }
            x_new[i] = (vector[i] - suma) / matriz[i][i];
        }

        error = 0;
        for ( i = 0; i < n; i++) {
            error = fmax(error, fabs(x_new[i] - x[i]));
            x[i] = x_new[i];
        }

        printf("%d\t\t", k);
        for ( i = 0; i < n; i++) {
            printf("%.6lf ", x[i]);
        }
        printf("\t%.6lf\n", error);

        if (error < tol) {
            printf("\nConvergencia alcanzada en %d iteraciones.\n", k);
            printf("Solución aproximada:\n");
            for ( i = 0; i < n; i++) {
                printf("x[%d] = %.6lf\n", i + 1, x[i]);
            }
            return;
        }
    }

    printf("\nNo se alcanzó la convergencia en el numero maximo de iteraciones.\n");
}

void menuPrincipal() {
    printf("\n\tSISTEMAS DE ECUACIONES\n");
    printf("\tMétodos numéricos - Grupo 1301\n\n");
    printf("\tAlcantar Hernández Jessica Esmeralda\n");
    printf("\tFierro Ibañez Andrea Esteph\n");
    printf("\tJaimes Molina Andrea\n");
    printf("\tLópez Ramírez Mariana Alejandra\n\n");
}

int main() {
    int n, corregir;

    menuPrincipal();

    printf("Dimension de la matriz cuadrada: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Entrada invalida para la dimension. Finalizando.\n");
        return 1;
    }

    double matriz[n][n], vector[n], x_inicial[n];
    leermatriz(n, matriz, vector);

    do {
        mostrarmatriz(n, matriz, vector);
        printf("\nEs correcta la matriz? ([SI=1], [NO=0]): ");
        if (scanf("%d", &corregir) != 1 || (corregir != 0 && corregir != 1)) {
            printf("Entrada invalida. Finalizando.\n");
            return 1;
        }
        if (!corregir) {
            corregircoeficiente(n, matriz, vector);
        }
    } while (!corregir);

    int dominante = DD(n, matriz);
    double det = determinante(n, matriz);

    if (det != 0) {
        printf("El determinante es: %lf\n", det);

        if (!dominante) {
            printf("La matriz NO es EDD. La convergencia no se garantiza.\n");
        }

        printf("Introduce el vector inicial:\n");
        int i;
        for (i = 0; i < n; i++) {
            printf("x_inicial[%d]: ", i + 1);
            if (scanf("%lf", &x_inicial[i]) != 1) {
                printf("Entrada invalida. Finalizando.\n");
                return 1;
            }
        }

        int max_iter;
        double tol;
        printf("Maximo de iteraciones: ");
        if (scanf("%d", &max_iter) != 1 || max_iter <= 0) {
            printf("Entrada invalida para el numero de iteraciones. Finalizando.\n");
            return 1;
        }
        printf("Tolerancia: ");
        if (scanf("%lf", &tol) != 1 || tol <= 0) {
            printf("Entrada inválida para la tolerancia. Finalizando.\n");
            return 1;
        }

        jacobi(n, matriz, vector, x_inicial, max_iter, tol);
    } else {
        printf("El determinante es 0. El sistema no tiene solucion unica.\n");
    }

    printf("Programa terminado.\n");
    return 0;
}
