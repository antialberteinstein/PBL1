#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Kiem tra dang chay he dieu hanh gi.
#ifdef _WIN32
#define CLEAR_SCREEN system("cls")
#include <conio.h>
#define CLEAR_STDIN fflush(stdin)
#else
#define CLEAR_SCREEN system("clear")
#define CLEAR_STDIN while (getchar() != '\n')

int getch() {
	return getchar();
}
int getche(void) {
	return getch();
}
#endif

#define ENABLE_COLOR 1
#if ENABLE_COLOR
// Tao mau sac.
#define RED   	"\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW	"\x1B[33m"
#define BLUE	"\x1B[34m"
#define RESET 	"\x1B[0m"

#define SET_COLOR(color)	printf(color)
#define RESET_COLOR			printf(RESET)
#else
#define RED
#define GREEN
#define YELLOW
#define BLUE
#define RESET

#define SET_COLOR(color)
#define RESET_COLOR
#endif

#define TAO_THU_MUC_QUAN_LI {									\
	printf("Tao cac thu muc can thiet...\n");					\
	if (system("cd INPUT && cd .."))	system("mkdir INPUT");	\
	if (system("cd OUTPUT && cd .."))	system("mkdir OUTPUT");	\
	if (system("cd log && cd .."))		system("mkdir log");	\
	CLEAR_SCREEN;												\
}

#define HR(file) fprintf(file, "================================\
===============================\n")
#define ENTER1 10
#define ENTER2 13

#define MAX 100
#define CHAR_FORMAT "%7c"
#define DATA_FORMAT "%7.4g"
#define XUAT_NGHIEM(file, nghiem, i)	fprintf(file, "x%d = %.2g\n", i + 1, nghiem)
#define INPUT_DIR 		"INPUT/"
#define LOG_PATH 		"log/log.txt"
#define INPUT_PATH 		"DATA.INP"
#define OUTPUT_PATH 	"OUTPUT/DATA.OUT"

#define ERR_SO_DONG_LON_HON_COT(n, m)\
	printf(RED "So dong(%d) lon hon hoac bang so cot (%d)." RESET, n, m)
#define ERR_THIEU_DONG 		printf(RED "Qua it dong.\n" RESET)
#define ERR_THUA_DONG 		printf(RED "Qua nhieu dong.\n" RESET)
#define ERR_THIEU_COT(i) 	printf(RED "Qua it du lieu tren dong %d.\n" RESET, i + 1)
#define ERR_THUA_COT(i) 	printf(RED "Qua nhieu du lieu tren dong %d.\n" RESET, i + 1)
#define ERR_DL_KHONG_HOP_LE(i, j, row, col) {											\
	if (row)		printf(RED "So dong co du lieu khong hop le.\n" RESET);				\
	else if (col)	printf(RED "So cot co du lieu khong hop le.\n" RESET);				\
	else																				\
		printf(RED "Du lieu khong hop le tai dong %d, cot %d.\n" RESET, i + 1, j + 1);	\
}
#define ERR_FILE_KHONG_TON_TAI(file) printf(RED "File %s khong ton tai.\n" RESET, file)
#define SHOW_ERROR(log_func) { log_func;		return false; }

#define TB_BD_THANH_CONG 		printf(GREEN "Ma tran da duoc bien doi!\n" RESET);
#define TB_DOI_DONG(i1, i2) 	printf(YELLOW "Doi dong %d va %d.\n" RESET, i1 + 1, i2 + 1)
#define TB_BIEN_DOI(i1, k, i2) 	printf(YELLOW "Lay dong %d tru di %g lan dong %d.\n" RESET, i2 + 1, k, i1 + 1)
#define MSG_VO_SO_NGHIEM 	"He phuong trinh co vo so nghiem."
#define MSG_VO_NGHIEM 		"He phuong trinh vo nghiem."
#define MSG_CO_NGHIEM 		"He phuong trinh co nghiem la: "
#define OUTPUT_LABEL 		"\tTIM NGHIEM HE PHUONG TRINH TUYEN TINH"

typedef char *string;
typedef float Matrix[MAX][MAX];
typedef float Vector[MAX];
typedef unsigned char bool;
#define true 1
#define false 0
typedef void (*func)();

// ========================================== //
//              KHAI BAO TOAN CUC             //
// ========================================== //
struct MatrixRecord;
struct Menu;
struct Menu *menu;
FILE* log_file;
struct MatrixRecord current, backup;
void thoat();  // Ham thoat khoi chuong trinh.
struct Menu* create_menu();  // Ham tao menu;

typedef struct {
	bool allow;
	int pivot;  	// Chi so cua dong tru/doi dong (dong o tren).
	int dest;   	// Chi so cua dong bi tru/doi dong (dong o duoi).
	int current;	// Chi so cua dong hien tai dang bien doi.
} ColorShowing;

ColorShowing color_showing;

void set_color_showing(int pivot, int dest, int current) {
	color_showing.pivot = pivot;
	color_showing.dest = dest;
	color_showing.current = current;
}

void reset_color_showing() {
	color_showing.allow = false;
	set_color_showing(-1, -1, -1);
}

void init() {  // Khoi tao cac bien toan cuc
	log_file = fopen(LOG_PATH, "w");
	menu = create_menu();
	reset_color_showing();
}

void destroy() {  // Giai phong cac bien toan cuc duoc cap phat.
	fclose(log_file);
	free(menu);
}

// ========================================== //
//              CAC HAM THONG DUNG            //
// ========================================== //

bool enter_to_continue() {
	printf("\nNhan Enter/Return de tiep tuc...");
	getch();
}

// Cong don phan tu sau cot he so tu do vao cot he so tu do.
void cong_don(Matrix matrix, int *n, int *m) {
  	for(int i = 0; i < *n; ++i)
  		for (int j = *n + 1; j < *m; ++j)
    		matrix[i][*n] += matrix[i][j];
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

typedef struct MatrixRecord {
	Matrix matrix;
	int n;
	int m;
} MatrixRecord;

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

// Tra ve false neu lua chon khong hop le.
void show_menu(Menu menu) {
	printf("\n");
	
	int size = strlen(menu->list[0].label) + 6;
	printf("+");
	for (int i = 1; i < size - 1; ++i)
		printf("-");
	printf("+\n");
	
	for (int i = 0; i < menu->count; ++i)
		printf("| %d/. %s\n", i + 1, menu->list[i].label);
	
	printf("+");
	for (int i = 1; i < size - 1; ++i)
		printf("-");
	printf("+\n\n");
	printf("%s", "Nhap lua chon cua ban: ");
	int op = (int)getchar();
	// Sua loi troi tren UNIX.
	if (op != ENTER1 && op != ENTER2)
		CLEAR_STDIN;
	op -= '0' + 1;
	CLEAR_SCREEN;
	printf("\n");
	if (op >= menu->count || op < 0) {
		printf(RED "%s\n" RESET, "Canh bao: Lua chon khong hop le.");
	}
	else
		menu->list[op].action();
	enter_to_continue();
	if (menu->list[op].action == thoat) {
		// Neu user chon thoat thi giai phong bo nho va thoat chuong trinh.
		destroy();
		exit(0);
	}
}

// ========================================== //
//              NHAP XUAT MT                  //
// ========================================== //

// Nhap ma tran tu file (hoac stdin).
bool scan_matrix(Matrix matrix, int *n, int *m, FILE* file, FILE* log_file) {
  	char dummy;
  	fprintf(log_file, "%s", "Nhap so dong: ");		fscanf(file, "%d%c", n, &dummy);
  	if (dummy != '\n' && dummy != ' ')	SHOW_ERROR(ERR_DL_KHONG_HOP_LE(0, 0, true, false));

  	fprintf(log_file, "%s", "Nhap so cot: ");		fscanf(file, "%d%c", m, &dummy);
  	if (dummy != '\n' && dummy != ' ')	SHOW_ERROR(ERR_DL_KHONG_HOP_LE(0, 0, false, true));

	if (*n  >= *m)	SHOW_ERROR(ERR_SO_DONG_LON_HON_COT(*n, *m));

  	fprintf(log_file, "%s\n", "Nhap ma tran: ");
  	for (int i = 0; i < *n; ++i)
  		for (int j = 0;j < *m; ++j) {
    		fscanf(file, "%f%c", &matrix[i][j], &dummy);
    		if (feof(file))			dummy = '\n';
			if (dummy != '\n' && dummy != ' ')	SHOW_ERROR(ERR_DL_KHONG_HOP_LE(i, j, false, false));
			if (file == stdin)	continue;
			const long int POSITION = ftell(file);
			while (dummy == ' ')	dummy = getc(file);
			if (dummy == '\n' && j < *m - 1) {
				if (j <= 0)
					SHOW_ERROR(ERR_THIEU_DONG)
				else
					SHOW_ERROR(ERR_THIEU_COT(i));
			}
			else if (dummy != '\n' && j >= *m - 1)
				SHOW_ERROR(ERR_THUA_COT(i))
			else
				fseek(file, POSITION, SEEK_SET);
  		}
	float tmp;
	if (file != stdin && !feof(file) && fscanf(file, "%f", &tmp) == 1)
		SHOW_ERROR(ERR_THUA_DONG);

	printf(GREEN "%s\n" RESET, "Doc ma tran thanh cong!");
    return true;
}

// Nhap ma tran tu file.
bool load_matrix(string path, bool read_path_stdin, Matrix matrix, int *n, int *m) {
	if (read_path_stdin) {
		printf("%s", "Nhap duong dan file: ");
  		fgets(path, MAX, stdin);
  		path[strlen(path) - 1] = '\0';
	}

  	FILE *file = fopen(path, "r");
  	if (file == NULL) {
		char temp[MAX];
		strcpy(temp, INPUT_DIR);
		strcat(temp, path);
		file = fopen(temp, "r");
		if (file == NULL) {
			ERR_FILE_KHONG_TON_TAI(path);
			SHOW_ERROR(ERR_FILE_KHONG_TON_TAI(temp));
		}
	}
  	bool foo = scan_matrix(matrix, n, m, file, log_file);
  	fclose(file);
  	return foo;
}

// Ghi ma tran ra file (hoac stdout).
void show_matrix(Matrix matrix, int n, int m, FILE *file) {
  	for (int i = 0; i < n; ++i) {
    	for (int j = 0; j < m; ++j){
			if (j == m - 1)
				fprintf(file, CHAR_FORMAT, '|');
			if (matrix[i][j] == 0 && i < current_row && allow_color_showing_in_matrix)
				SET_COLOR(GREEN);
			if (allow_color_showing_in_matrix && i == pivot_row)
				SET_COLOR(BLUE);
			if (allow_color_showing_in_matrix && i == dest_row)
				SET_COLOR(RED);
			fprintf(file, DATA_FORMAT, matrix[i][j]);
			RESET_COLOR;
		}
    	fprintf(file, "\n");
  	}
  	fprintf(file, "\n");
}

void hien_thi_hpt(Matrix matrix, int so_nghiem, FILE* file) {
	int pivot;
	for (int i = 0; i < so_nghiem; ++i) {
		pivot = -1;
		for (int j = 0; j < so_nghiem; ++j) {
			if (matrix[i][j] == 0) {
				fprintf(file, "%10c", ' ');
			} else {
				if (pivot == -1)
					pivot = j;
				float fabs_value = fabs(matrix[i][j]);
				if (j == pivot)
					if (fabs_value != 1)
						fprintf(file, "%6.2g", matrix[i][j], j + 1);
					else
						fprintf(file, "%6c", ' ');
				else {
					fprintf(file, "  %c", ((matrix[i][j] > 0) ? '+' : '-'));
					if (fabs_value != 1)
						fprintf(file, "%3.2g", fabs_value);
					else
						fprintf(file, "%3c", ' ');
				}
				fprintf(file, "[x%d]", j + 1);
			}
		}
		fprintf(file, "  =%3.2g\n", matrix[i][so_nghiem]);
	}
}

// ========================================== //
//              CAC CUA SO                    //
// ========================================== //

void main_window() {
	cong_don(current.matrix, &current.n, &current.m);
	CLEAR_SCREEN;
	SET_COLOR(YELLOW);
	printf("                     PBL1: De tai 205\n\n");
	RESET_COLOR;
	show_matrix(current.matrix, current.n, current.m, stdout);
	show_menu(menu);
	main_window();
}

void introduce_window() {
	SET_COLOR(YELLOW);
	printf("                     PBL1: De tai 205\n");
	RESET_COLOR;
	printf("      +) Tim nghiem he phuong trinh bang phuong phap Gauss.\n");
	printf("      +) Cong don phan tu sau cot he so tu do vao cot he so tu do.\n\n");
	SET_COLOR(YELLOW);
	printf("\n                   Nhom 13:\n");
	RESET_COLOR;
	printf("              +) Vu Duc Minh.\n");
	printf("              +) Tran Nhat Nguyen.\n");
	enter_to_continue();
}

// ========================================== //
//             CAC HAM TINH NGHIEM            //
// ========================================== //

void xac_dinh_nghiem(Matrix matrix, int n, int m, Vector nghiem) {
    nghiem[n - 1] = matrix[n - 1][n]/matrix[n - 1][n - 1];
    float c = 0;
    for (int i = n - 2; i >= 0; --i) {
        c = 0;
        for (int j = n - 1; j > i; --j) {
            c += matrix[i][j] * nghiem[j];
        }
        nghiem[i] = (matrix[i][n] - c)/matrix[i][i];
        if (nghiem[i] == 0)     nghiem[i] = 0;
    }
}

void xuat_nghiem(FILE* file, MatrixRecord backup,
		Matrix matrix, int r, int r_mr, Vector nghiem) {
	const int SO_NGHIEM = backup.m - 1;
	fprintf(file, "\n\n\n%s\n\n", OUTPUT_LABEL);
	HR(file);
	hien_thi_hpt(backup.matrix, SO_NGHIEM, file);
	
	HR(file);
	fprintf(file, "%s\n", "Ma tran duoc bien doi la:");
	show_matrix(matrix, backup.n, backup.m, file);
	HR(file);

	if (r != r_mr)
		fprintf(file, "%s\n", MSG_VO_NGHIEM);
	else if (r < r_mr)
		fprintf(file, "%s\n", MSG_VO_SO_NGHIEM);
	else {
		xac_dinh_nghiem(matrix, backup.n, backup.m, nghiem);
		fprintf(file, "%s\n", MSG_CO_NGHIEM);
		for (int i = 0; i < SO_NGHIEM; ++i)
			XUAT_NGHIEM(file, nghiem[i], i);
	}
}

void swap(float *a, float *b) {
  	float tmp = *a;
  	*a = *b;
  	*b = tmp;
}

void swap_row(Matrix matrix, int m, int i1, int i2) {
  	for (int j = 0; j < m; ++j) swap(&matrix[i1][j], &matrix[i2][j]);
}

void bien_doi_ma_tran(Matrix matrix, int i0, int j0, int n, int m, bool show_step, int step) {
    if (i0 >= n || j0 >= m) {
		if(show_step)	TB_BD_THANH_CONG;
		pivot_row = -1;		dest_row = -1;
		current_row = n + 1;
		if (show_step)
			show_matrix(matrix, n, m, stdout);
		allow_color_showing_in_matrix = false;
		current_row = -1;
		return;
	}
	color_showing.allow = show_step;
    int k;
    if (matrix[i0][j0] == 0) {
        // Tim vi tri dau tien khac 0.
        k = i0 + 1;
        while (k < n && matrix[k][j0] == 0)	++k;
        if (k >= n)
			return bien_doi_ma_tran(matrix, i0, j0 + 1, n, m, show_step, step);
        swap_row(matrix, m, i0, k);
    
        if (show_step) {
			printf("\n\n");		HR(stdout);
			printf("+)BUOC %d:\n", step++);
			TB_DOI_DONG(i0, k);
			set_color_showing(i0, k,
				(color_showing.current < color_showing.dest) ? dest_row : current_row);
			pivot_row = i0;		dest_row = k;
			current_row = ;
            show_matrix(matrix, n, m, stdout);
        }
    }

    float tmp;
    for (int i = i0 + 1; i < n; ++i) {
        if (matrix[i][j0] == 0)    continue;
        tmp = matrix[i][j0] / matrix[i0][j0];
        for (int j = j0; j < m; ++j)    matrix[i][j] -= tmp * matrix[i0][j];

        if (show_step) {
			printf("\n\n");	HR(stdout);
			printf("+) BUOC %d:\n", step++);
			TB_BIEN_DOI(i0, tmp, i);
			pivot_row = i0;		dest_row = i;
			current_row = (current_row < dest_row) ? dest_row : current_row;
            show_matrix(matrix, n, m, stdout);
        }
  	}
  	return bien_doi_ma_tran(matrix, i0 + 1, j0 + 1, n, m, show_step, step);
}

int rank(Matrix matrix, int n, int m) {
    int j = 0, r = 0;
    for (int i = 0; i < n; ++i) {
		while (j < m && matrix[i][j] == 0)  ++j;
		if (j < m)	++r;
	}
    return r; 
}

void bien_doi_Gauss(Matrix matrix, int n, int m, string output_path, bool check) {
    Vector nghiem;

    MatrixRecord backup;
    cpy_mat(backup.matrix, &backup.n, &backup.m, matrix, n, m);

	printf("Hien cac buoc bien doi?\n");
	printf("+------------------------------+\n");
	printf("| Chon Enter/Return de dong y. |\n");
	printf("| Chon phim bat ki de tu choi. |\n");
	printf("+------------------------------+\n\n");
	printf("Chon: ");

	char c = getche();
	// Sua loi troi tren UNIX
	bool show_step = (c == ENTER1 || c == ENTER2);
	if (!show_step)
		CLEAR_STDIN;

	bien_doi_ma_tran(matrix, 0, 0, n, m, show_step, 1);
	int r_mr = rank(matrix, n, m);
	int r = rank(matrix, n, m - 1);
	int so_nghiem = m - 1;

	FILE* file = fopen(output_path, "a");
	xuat_nghiem(stdout, backup, matrix, r, r_mr, nghiem);
	if (check)
		xuat_nghiem(file, backup, matrix, r, r_mr, nghiem);
	fclose(file);
}

// ========================================== //
//              CAC HAM XU LI                 //
// ========================================== //

void nhap_tu_stdin() {
	MatrixRecord input;
	if (scan_matrix(input.matrix, &input.n, &input.m, stdin, stdout))
		cpy_mat(current.matrix, &current.n, &current.m, input.matrix, input.n, input.m);
}

void nhap_tu_file() {
	char path[MAX];
	MatrixRecord input;
	if (load_matrix(path, true, input.matrix, &input.n, &input.m))
		cpy_mat(current.matrix, &current.n, &current.m, input.matrix, input.n, input.m);
}

void xuat() {
	MatrixRecord cal;
	cpy_mat(cal.matrix, &cal.n, &cal.m, current.matrix, current.n, current.m);
	bien_doi_Gauss(cal.matrix, cal.n, cal.m, OUTPUT_PATH, true);
	printf("\nXuat thanh cong vao file DATA.OUT!\n");
}

void bien_doi() {
	printf("+) Ma tran ban dau:\n");
	show_matrix(current.matrix, current.n, current.m, stdout);
	MatrixRecord cal;
	cpy_mat(cal.matrix, &cal.n, &cal.m, current.matrix, current.n, current.m);
	bien_doi_ma_tran(cal.matrix, 0, 0, cal.n, cal.m, true, 1);
}

void tim_nghiem() {
	MatrixRecord cal;
	cpy_mat(cal.matrix, &cal.n, &cal.m, current.matrix, current.n, current.m);

	bien_doi_Gauss(cal.matrix, cal.n, cal.m, OUTPUT_PATH, false);
}

void thoat() {
	printf("%s\n", "\tHen gap lai...");
}

int main() {
	TAO_THU_MUC_QUAN_LI;
	init();
	// Lay du lieu tu file DATA.INP
	load_matrix(INPUT_PATH, false, current.matrix, &current.n, &current.m);
	CLEAR_SCREEN;
	
	push("Nhap ma tran tu ban phim.                     |", nhap_tu_stdin, menu);
	push("Nhap ma tran tu file.                         |", nhap_tu_file, menu);
	push("Bien doi ma tran thanh dang bac thang.        |", bien_doi, menu);
	push("Tinh nghiem cua he phuong trinh tuyen tinh.   |", tim_nghiem, menu);
	push("Xuat ket qua.                                 |", xuat, menu);
	push("Thoat.                                        |", thoat, menu);

	introduce_window();
	main_window();

	destroy();
	return 0;
}