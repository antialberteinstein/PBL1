#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE* file = fopen("INPUT/DATA.INP", "r");

	char dummy;

	int n, m;
	float mat[100][100];
	fscanf(file, "%d%c", &n, &dummy);

	fscanf(file, "%d%c", &m, &dummy);

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			fscanf(file, "%f%c", &mat[i][j], &dummy);

			long int pos = ftell(file);
			if (feof(file))
				dummy = '\n';
			printf("DEBUG : %f : %c : %d : %d\n", mat[i][j], dummy, dummy, pos);
			while (dummy == ' ')	dummy = getc(file);
			if (dummy == '\n' && j < m - 1) {
				printf("thieu cot.%d\n", i + 1);
				exit(1);			} else if (dummy != '\n' && j >= m - 1) {
				printf("thua cot.%d %d\n", i + 1, dummy);
				exit(1);
			} else {
				fseek(file, pos, SEEK_SET);
			}
		}
	}
	printf("DEBUG : dummy = %d : %c\n", dummy, dummy);

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j)
			printf("%.2g\t", mat[i][j]);
		printf("\n");
	}

	fclose(file);
	return 0;
}