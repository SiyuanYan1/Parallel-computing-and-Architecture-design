#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	double a, b, c, d;
	double array[3], result[4];
	char one, two, three;
	int rank, size, total, i;
	double x1r, x1i, x2r, x2i;
	FILE* fp;
	FILE* out;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status stat;
	if (rank == 0) {
		fp = fopen("quad.txt", "r");
		fscanf(fp,"%d\n", &total);
		fscanf(fp, "%s\t%s\t%s", &one, &two, &three);
	}
	MPI_Bcast(&total, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		for (i=0; i<total; i++) {
			fscanf(fp, "%lf\t%lf\t%lf", &a, &b, &c);
			d = b*b-4*a*c;
			array[0] = a;
			array[1] = b;
			array[2] = d;
			MPI_Send(array, 3, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
		}
		fclose(fp);
	}
	if (rank == 1) {
		for (i = 0; i<total; i++) {
			MPI_Recv(array, 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &stat);
			a = array[0];
			b = array[1];
			d = array[2];
			if (d >= 0) {
				x1r = (-b+sqrt(d))/(2*a);
				x2r = (-b-sqrt(d))/(2*a);
				x1i = 0;
				x2i = 0;
			} else {
				x1r = -b/(2*a);
				x1i = sqrt(-d)/(2*a);
				x2r = -b/(2*a);
				x2i = -sqrt(-d)/(2*a);
			}
			result[0] = x1r;
			result[1] = x1i;
			result[2] = x2r;
			result[3] = x2i;
			MPI_Send(&result, 4, MPI_DOUBLE, 2, 1, MPI_COMM_WORLD);
		}
	}
	if (rank == 2) {
		out = fopen("roots.txt", "w");
		fprintf(out, "%d\n", total);
		fprintf(out, "x1\tx2\tx1_real\tx1_img\tx2_real\tx2_img\n");
		for (i=0; i<total; i++) {
			MPI_Recv(&result, 4, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD, &stat);
			if (result[1] == 0) {
				fprintf(out, "%.1lf\t%.1lf\n",result[0], result[2]);
			}
			else {
				fprintf(out, "\t\t%.1lf\t%.1lf\t%.1lf\t%.1lf\n", result[0], result[1], result[2], result[3]);
			}
		}
		fclose(out);
	}
	MPI_Finalize();
	return 0;
}