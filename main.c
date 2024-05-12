#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Kiem tra dang chay he dieu hanh gi.
#ifdef _WIN32
#define CLEAR_SCREEN system("cls")
#else
#define CLEAR_SCREEN system("clear")
#endif

#define MAX 100
#define LOADING_DATA_MSG "Dang load du lieu tu file DATA.INP"
#define DATA_FORMAT_CHAR "%7c"
#define DATA_FORMAT "%7.2g"
#define LOG_PATH "log.txt"
#define INPUT_PATH "DATA.INP"
#define OUTPUT_PATH "DATA.OUT"
#define OUTPUT_LABEL "=== TIM NGHIEM HE PHUONG TRINH ==="
#define TITLE "======= PBL1 - De tai 205 ======="
#define DESCRIPTION                                                            \
  "** Tim nghiem he phuong trinh bang phuong phap Gauss - ** \n\
** Cong don phan tu sau cot he so tu do vao cot he so tu do. **\n\n"
#define INTRODUCE                                                              \
  "* Nhom 13 * \n \
\t1/. Vu Duc Minh. \n \
\t2/. Tran Nhat Nguyen. \n"

#define INVALID_OPTION_MSG "Canh bao: Lua chon khong hop le."
#define ENTER_TO_CONTINUE "Nhan Enter/Return de tiep tuc."

#define ERR_NE_ROW "Qua it dong"
#define ERR_R_ROW "Qua nhieu dong"
#define ERR_NE_COL(i) "Qua it du lieu tren dong" #(i + 1)
#define ERR_R_COL(i) "Qua nhieu du lieu tren dong" #(i + 1)
#define ERR_DATA_INVALID "Du lieu khong hop le"
#define ERR_FILE_NOT_FOUND(file) "File " (file) " khong ton tai"
#define SHOW_ERROR(file, log) {																\
	if (file != NULL && file != stdin && file != stdout && file != stderr)	fclose(file)	\
	printf("%s.\n", log);																	\
	enter_to_continue();		return false;												\
}

#define RANK_LABEL "Hang cua ma tran la: "

#define NHAP_SO_DONG "Nhap so dong: "
#define NHAP_SO_COT "Nhap so cot: "
#define NHAP_MA_TRAN "Nhap ma tran: "
#define NHAP_FILE_PATH "Nhap duong dan file: "

typedef char *string;
typedef float Matrix[MAX][MAX];
typedef float Vector[MAX];
typedef unsigned char bool;
#define true 1
#define false 0
typedef void (*func);

// ========================================== //
//              KHAI BAO TOAN CUC             //
// ========================================== //
Menu menu_input, menu_main, menu_kcd;
FILE* log_file;
Matrix mat, mat_bk;
int *n, *m, *n_bk, *m_bk;

// ========================================== //
//              CAC HAM THONG DUNG            //
// ========================================== //
void enter_to_continue() {
	printf("\n%s...", ENTER_TO_CONTINUE);
	getc(stdin);	fflush(stdin);
}

// Cong don phan tu sau cot he so tu do vao cot he so tu do.
void add_last_cells(Matrix mat, int *n, int *m) {
  	for(int i = 0; i < *n; ++i)
  		for (int j = *n + 1; j < *m; ++j)
    		mat[i][*n] += mat[i][j];
  	*m = *n + 1;
}

void cpy_mat(Matrix dest, int *n_dest, int *m_dest, Matrix src, int n, int m) {
	for (int i = 0; i < n; ++i)		for (int j = 0; j < m; ++j)
		dest[i][j] = src[i][j];
	*n_dest = n;	*m_dest = m;
}

// ========================================== //
//              KHOI TAO MENU                 //
// ========================================== //
typedef struct {
	string label;
	func action;
} MenuOption;

typedef struct Menu {
	MenuOption list[MAX];
	int count;
} * Menu;

Menu create_menu() {
	Menu menu = malloc(sizeof(struct Menu));
	menu->count = 0;
	return menu;
}

void push(string label, func action, Menu menu) {
	MenuOption menu_option;
	menu_option.label = label;
	menu_option.action = action;
	menu->list[menu->count++] = menu_option;
}

void show_menu(Menu menu) {
	CLEAR_SCREEN;	printf("%s\n\n", TITLE);

	for (int i = 0; i < menu->count; ++i)
		printf("%d/. %s\n", i + 1, menu->list[i].label);
	int op = (int)(getc(stdin) - '0') - 1;	fflush(stdin);
	CLEAR_SCREEN;
	if (op >= menu->count || op < 0) {
		printf("%s\n", INVALID_OPTION_MSG);
		enter_to_continue();
	}
}

// ========================================== //
//              NHAP XUAT MT                  //
// ========================================== //

// Nhap ma tran tu file (hoac stdin).
bool scan_matrix(Matrix mat, int *n, int *m, FILE *file, FILE *log_file) {
  	char dummy;
  	fprintf(log_file, "%s", NHAP_SO_DONG);		fscanf(file, "%d%c", n, &dummy);
  	if (dummy != '\n' && dummy != ' ')	SHOW_ERROR(ERR_DATA_INVALID, false, 0);
  	
  	fprintf(log_file, "%s", NHAP_SO_COT);		fscanf(file, "%d%c", m, &dummy);
  	if (dummy != '\n' && dummy != ' ')	SHOW_ERROR(ERR_DATA_INVALID, false, 0);

  	int row_count = 0;
  	fprintf(log_file, "%s\n", NHAP_MA_TRAN);
  	for (int i = 0; i < *n; ++i)
  		for (int j = 0;j < *m; ++j) {
    		fscanf(file, "%f%c", &mat[i][j], &dummy);
    		if (dummy != '\n' && dummy != ' ')	SHOW_ERROR(ERR_DATA_INVALID, false, 0);
    		if (dummy == '\n')	++row_count;
  		}
  	if (row_count < *n)		SHOW_ERROR(file, ERR_DATA_INVALID);
  	if (row_count > *n)		SHOW_ERROR(file, ERR_DATA_INVALID);

  	char buf[MAX], num[MAX];
  	bool in_space = false;
  	int size, count = 0;
  	for (int i = 0; i < *n; ++i) {
    	count = 0;
    	fgets(buf, MAX, file);
    	size = strlen(buf);
    	for (int k = 0; i < size; ++i) {
      		if (buf[k] != ' ') {
        		in_space = false;
        		++count;
      		}
      		if (buf[k] == ' ' && !in_space)
        	in_space = true;
    	}
    	if (count < *m) 	SHOW_ERROR(file, ERR_NE_ROW(i));
    	if (count > *m) 	SHOW_ERROR(file, ERR_NE_ROW(i));
    }
    return true;
}

// Nhap ma tran tu file.
bool load_matrix(string path, bool read_path_stdin, Matrix mat, int *n, int *m) {
	if (read_path_stdin) {
		  printf("%s", NHAP_FILE_PATH);
  		fgets(path, MAX, stdin);
  		path[strlen(path) - 1] = '\0';
	}

  	FILE *file = fopen(path, "r");
  	if (file == NULL) 		SHOW_ERROR(file, ERR_FILE_NOT_FOUND(path));
  	bool foo = scan_matrix(mat, n, m, file, log_file);
  	fclose(file);
  	return foo;
}

// Ghi ma tran ra file (hoac stdout).
void show_matrix(Matrix mat, int n, int m, FILE *file) {
  	for (int i = 0; i < n; ++i) {
    	for (int j = 0; j < m - 1; ++j) { fprintf(file, DATA_FORMAT, mat[i][j]); }
    	fprintf(file, DATA_FORMAT_CHAR, '|');
    	fprintf(file, DATA_FORMAT, mat[i][m - 1]);
    	fprintf(file, "\n");
  	}
  	fprintf(file, "\n");
}

// ========================================== //
//             CAC HAM TINH NGHIEM            //
// ========================================== //

void swap(float *a, float *b) {
  float tmp = *a;
  *a = *b;
  *b = tmp;
}

void swap_row(Matrix mat, int m, int i1, int i2) {
  	for (int j = 0; j < m; ++j) swap(&mat[i1][j], &mat[i2][j]);
}

// Tra ve true neu bien doi thanh cong (ve dang tam giac tren).
// Tra ve false neu bien doi khong thanh cong => vo so nghiem.
bool bien_doi_ma_tran(Matrix mat, int i0, int j0, int n, int m, bool show_step) {
    if (i0 >= n || j0 >= m)
      return true;
    int k;
    if (mat[i0][j0] == 0) {
        // Tim vi tri dau tien khac 0.
        k = i0 + 1;
        while (k < n && mat[k][j0] == 0)	++k;
        if (k >= n)     return (i0 == m);
        swap_row(mat, m, i0, k);
    
        // printf(TB_DOI_DONG(i0, k));
        if (show_step) {
            show_matrix(mat, n, m, stdout);
            enter_to_continue();
            CLEAR_SCREEN;
        }
    }

    float tmp;
    for (int i = i0 + 1; i < n; ++i) {
        if (mat[i][j0] == 0)    continue;
        tmp = mat[i][j0] / mat[i0][j0];
        for (int j = j0; j < m; ++j)    mat[i][j] -= tmp * mat[i0][j];

        // printf(TB_BIEN_DOI(i0, tmp, i));
        if (show_step) {
            show_matrix(mat, n, m, stdout);
            enter_to_continue();
            CLEAR_SCREEN;
        }
  }
  return bien_doi_ma_tran(mat, i0 + 1, j0 + 1, n, m, show_step);
}

int rank(Matrix mat, int n, int m, bool show_step) {
    int i, j = 0, r;
    for (i = n - 1; i < n; ++i) {
        j = 0;
        while (mat[i][j] == 0 && j < m)    ++j;
        if (j < m)  r = ((j > n) ? n : j);
    } 
    if (show_step) 
        printf("%s %d.\n", RANK_LABEL, r);
    return r; 
}

void bien_doi_Gauss(Matrix mat, int n, int m, string output_path) {
    Vector nghiem;

    Matrix mat_bk;
    int *n_bk, *m_bk;
    cpy_mat(mat_bk, n_bk, m_bk, mat, n, m);
}

// ========================================== //
//              CAC HAM XU LI                 //
// ========================================== //

void input_stdin() {
	scan_matrix(mat, n, m, stdin, stdout);
}

void input_file() {
	string path;
	load_matrix(path, true, mat, n, m);
}

void cal_cd() {  // Cong don.
	cpy_mat(mat_bk, n_bk, m_bk, mat, *n, *m);
	add_last_cells(mat, n, m);
	show_matrix(mat, *n, *m, stdout);
}

void cal_dc() {  // Huy cong don.
	cpy_mat(mat, n, m, mat_bk, *n_bk, *m_bk);
	show_matrix(mat, *n, *m, stdout);
}

void cal_gauss() {
	//
}

int main() {
	FILE* file = fopen(LOG_PATH, "w");

  printf("%s\n", ERR_R_COL(5));

	fclose(file);
	return 0;
}