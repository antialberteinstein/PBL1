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
#define DATA_FORMAT "%7.2f"
#define SHOW_SOLUTION(file, nghiem, i)	fprintf(file, "x%d = %-7.2g", i + 1, nghiem)
#define LOG_PATH "log.txt"
#define INPUT_PATH "DATA.INP"
#define OUTPUT_PATH "DATA.OUT"
#define OUTPUT_LABEL "=== TIM NGHIEM HE PHUONG TRINH ==="
#define TITLE "======= PBL1 - De tai 205 ======="
#define DESCRIPTION                                                            \
"   ** Tim nghiem he phuong trinh bang phuong phap Gauss. ** \n\
** Cong don phan tu sau cot he so tu do vao cot he so tu do. **\n\n"
#define INTRODUCE                                                              \
  "* Nhom 13 * \n \
\t1/. Vu Duc Minh. \n \
\t2/. Tran Nhat Nguyen. \n"

#define INVALID_OPTION_MSG "Canh bao: Lua chon khong hop le."
#define GOODBYE_MSG "Loi nhan: Hen gap lai..."
#define ENTER_TO_CONTINUE "Nhan Enter/Return de tiep tuc."

#define ERR_NE_ROW printf("Qua it dong.\n")
#define ERR_R_ROW printf("Qua nhieu dong.\n")
#define ERR_NE_COL(i) printf("Qua it du lieu tren dong %d.\n", i + 1)
#define ERR_R_COL(i) printf("Qua nhieu du lieu tren dong %d.\n", i + 1)
#define ERR_DATA_INVALID printf("Du lieu khong hop le.\n");
#define ERR_FILE_NOT_FOUND(file) printf("File %s khong ton tai.\n", file)
#define SHOW_ERROR(log_func) {													\
	log_func;	enter_to_continue();		return false;						\
}

#define RANK_LABEL "Hang cua ma tran la: "

#define COL_INP_LABEL "Nhap so dong: "
#define ROW_INP_LABEL "Nhap so cot: "
#define MAT_INP_LABEL "Nhap ma tran: "
#define FILE_PATH_INP_LABEL "Nhap duong dan file: "

#define AN_BD_COMPLETE printf("Ma tran da duoc bien doi!\n");
#define AN_SWAP_ROW(i1, i2) printf("Doi dong %d va %d.\n", i1 + 1, i2 + 1)
#define AN_CHANGE(i1, k, i2) printf("Lay dong %d tru di %g lan dong %d.\n", i2 + 1, k, i1 + 1)
#define AN_INF_SOLUTION_MSG "He phuong trinh co vo so nghiem."
#define AN_NO_SOLUTION_MSG "He phuong trinh vo nghiem."
#define AN_SOLUTION_MSG "He phuong trinh co nghiem la: "
#define OUTPUT_LABEL "=== TIM NGHIEM HE PHUONG TRINH ==="

#define MENU_1_INPUT_STDIN "Nhap ma tran tu ban phim."
#define MENU_2_INPUT_FILE "Nhap ma tran tu file."
#define MENU_3_CD "Cong don cac phan tu sau cot he so tu do."
#define MENU_4_DC "Tro ve ma tran ban dau truoc khi cong don."
#define MENU_5_GAUSS "Tim nghiem ma tran theo phuong phap Gauss."
#define MENU_6_BD "Thuc hien cac buoc bien doi ma tran."
#define MENU_7_RANK "Tim hang cua ma tran."
#define MENU_8_LEAVE "Thoat."
#define MENU_ASK_OPTION "Nhap lua chon cua ban: "

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
struct Menu;
struct Menu *menu_input, *menu_main, *menu_kcd;
FILE* log_file;
Matrix mat, mat_bk;
int n, m, n_bk, m_bk;
bool allow_input_loop = false;
void leave();  // Ham thoat khoi chuong trinh.

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

MenuOption create_menu_option(string label, func action) {
	MenuOption menu_option;
	menu_option.label = label;
	menu_option.action = action;
	return menu_option;
}

typedef struct Menu {
	MenuOption list[MAX];
	int count;
} * Menu;

Menu create_menu() {
	Menu menu = malloc(sizeof(struct Menu));
	menu->count = 0;
	return menu;
}

void push(MenuOption menu_option, Menu menu) {
	menu->list[menu->count++] = menu_option;
}

// Tra ve false neu lua chon thoat chuong trinh.
bool show_menu(Menu menu) {
	for (int i = 0; i < menu->count; ++i)
		printf("%d/. %s\n", i + 1, menu->list[i].label);
	printf("%s", MENU_ASK_OPTION);
	int op = (int)(getc(stdin) - '0') - 1;	fflush(stdin);
	CLEAR_SCREEN;
	printf("\n");
	if (op >= menu->count || op < 0)
		printf("%s\n", INVALID_OPTION_MSG);
	else
		menu->list[op].action();
	enter_to_continue();
	return (menu->list[op].action != leave);
}

// ========================================== //
//              NHAP XUAT MT                  //
// ========================================== //

// Nhap ma tran tu file (hoac stdin).
bool scan_matrix(Matrix mat, int *n, int *m, FILE* file, FILE* log_file) {
  	char dummy;
  	fprintf(log_file, "%s", ROW_INP_LABEL);		fscanf(file, "%d%c", n, &dummy);
  	if (dummy != '\n' && dummy != ' ')	SHOW_ERROR(ERR_DATA_INVALID);

  	fprintf(log_file, "%s", COL_INP_LABEL);		fscanf(file, "%d%c", m, &dummy);
  	if (dummy != '\n' && dummy != ' ')	SHOW_ERROR(ERR_DATA_INVALID);

  	fprintf(log_file, "%s\n", MAT_INP_LABEL);
  	for (int i = 0; i < *n; ++i)
  		for (int j = 0;j < *m; ++j) {
    		fscanf(file, "%f%c", &mat[i][j], &dummy);
  		}
    return true;
}

// Nhap ma tran tu file.
bool load_matrix(string path, bool read_path_stdin, Matrix mat, int *n, int *m) {
	if (read_path_stdin) {
		  printf("%s", FILE_PATH_INP_LABEL);
  		fgets(path, MAX, stdin);
  		path[strlen(path) - 1] = '\0';
	}

  	FILE *file = fopen(path, "r");
  	if (file == NULL) 		SHOW_ERROR(ERR_FILE_NOT_FOUND(path));
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
//              CAC CUA SO                    //
// ========================================== //

void input_window() {
	CLEAR_SCREEN;
	printf("%s\n\n", TITLE);
	show_menu(menu_input);
}

void main_window() {
	CLEAR_SCREEN;
	printf("%s\n%s\n\n", TITLE, DESCRIPTION);
	show_matrix(mat, n, m, stdout);
	bool end;
	if (n >= m)
		end = !show_menu(menu_kcd);
	else
		end = !show_menu(menu_main);
	if (!end)	main_window();
}

void introduce_window() {
	printf("%s\n%s\n\n%s\n\n", TITLE, DESCRIPTION, INTRODUCE);
	enter_to_continue();
}

// ========================================== //
//             CAC HAM TINH NGHIEM            //
// ========================================== //

void show_result(FILE* file, Matrix mat_bk, int n_bk, int m_bk,
		bool vo_nghiem, bool vo_so_nghiem, Vector nghiem) {
	int so_nghiem = m - 1;
	fprintf(file, "\n\n\n%s\n\n", OUTPUT_LABEL);
	show_matrix(mat_bk, n_bk, m_bk, file);

	if (vo_nghiem)
		fprintf(file, "%s\n", AN_NO_SOLUTION_MSG);
	else if (vo_so_nghiem)
		fprintf(file, "%s\n", AN_INF_SOLUTION_MSG);
	else {
		fprintf(file, "%s ", AN_SOLUTION_MSG);
		for (int i = 0; i < so_nghiem; ++i)
			SHOW_SOLUTION(file, nghiem[i], i);
	}
}

void swap(float *a, float *b) {
  float tmp = *a;
  *a = *b;
  *b = tmp;
}

void swap_row(Matrix mat, int m, int i1, int i2) {
  	for (int j = 0; j < m; ++j) swap(&mat[i1][j], &mat[i2][j]);
}

void bien_doi_ma_tran(Matrix mat, int i0, int j0, int n, int m, bool show_step) {
    if (i0 >= n || j0 >= m) {
		if(show_step)	AN_BD_COMPLETE;
		return;
	}
    int k;
    if (mat[i0][j0] == 0) {
        // Tim vi tri dau tien khac 0.
        k = i0 + 1;
        while (k < n && mat[k][j0] == 0)	++k;
        if (k >= n)     return bien_doi_ma_tran(mat, i0, j0 + 1, n, m, show_step);
        swap_row(mat, m, i0, k);
    
        if (show_step) {
			AN_SWAP_ROW(i0, k);
            show_matrix(mat, n, m, stdout);
            enter_to_continue();
        }
    }

    float tmp;
    for (int i = i0 + 1; i < n; ++i) {
        if (mat[i][j0] == 0)    continue;
        tmp = mat[i][j0] / mat[i0][j0];
        for (int j = j0; j < m; ++j)    mat[i][j] -= tmp * mat[i0][j];

        if (show_step) {
			AN_CHANGE(i0, tmp, i);
            show_matrix(mat, n, m, stdout);
            enter_to_continue();
        }
  	}
  	return bien_doi_ma_tran(mat, i0 + 1, j0 + 1, n, m, show_step);
}

int rank(Matrix mat, int n, int m, bool show_step) {
    int j = 0, r = 0;
    for (int i = 0; i < n; ++i) {
		while (j < m && mat[i][j] == 0)  ++j;
		if (j < m)	++r;
	}
    if (show_step) 
        printf("%s %d.\n", RANK_LABEL, r);
    return r; 
}

void xac_dinh_nghiem(Matrix mat, int n, int m, Vector nghiem) {
    nghiem[n - 1] = mat[n - 1][n]/mat[n - 1][n - 1];
    float c = 0;
    for (int i = n - 2; i >= 0; --i) {
        c = 0;
        for (int j = n - 1; j > i; --j) {
            c += mat[i][j] * nghiem[j];
        }
        nghiem[i] = (mat[i][n] - c)/mat[i][i];
        if (nghiem[i] == 0)     nghiem[i] = 0;
    }
}

void bien_doi_Gauss(Matrix mat, int n, int m, string output_path) {
    Vector nghiem;

    Matrix mat_bk;
    int n_bk, m_bk;
    cpy_mat(mat_bk, &n_bk, &m_bk, mat, n, m);

	bien_doi_ma_tran(mat, 0, 0, n, m, false);
	int r_mr = rank(mat, n, m, false);
	int r = rank(mat, n, m - 1, false);
	int so_nghiem = m - 1;

	FILE* file = fopen(output_path, "a");
	if (r != r_mr) {
		show_result(stdout, mat_bk, n_bk, m_bk, true, false, nghiem);
		show_result(file, mat_bk, n_bk, m_bk, true, false, nghiem);
	} else if (r < so_nghiem) {
		show_result(stdout, mat_bk, n_bk, m_bk, false, true, nghiem);
		show_result(file, mat_bk, n_bk, m_bk, false, true, nghiem);
	} else {
		xac_dinh_nghiem(mat, n, m, nghiem);
		show_result(stdout, mat_bk, n_bk, m_bk, false, false, nghiem);
		show_result(file, mat_bk, n_bk, m_bk, false, false, nghiem);
	}

	fclose(file);
}

// ========================================== //
//              CAC HAM XU LI                 //
// ========================================== //

void input_stdin() {
	// Neu du lieu sai va duoc cap phep thi se chay cua so nhap ma tran.
	if (!scan_matrix(mat, &n, &m, stdin, stdout) && allow_input_loop)
		input_window();
}

void input_file() {
	string path;
	// Neu du lieu sai va duoc cap phep thi se chay cua so nhap ma tran.
	if (!load_matrix(path, true, mat, &n, &m) && allow_input_loop)
		input_window();
}

void cal_cd() {  // Cong don.
	cpy_mat(mat_bk, &n_bk, &m_bk, mat, n, m);
	add_last_cells(mat, &n, &m);
	show_matrix(mat, n, m, stdout);
}

void cal_dc() {  // Huy cong don.
	cpy_mat(mat, &n, &m, mat_bk, n_bk, m_bk);
	show_matrix(mat, n, m, stdout);
}

void cal_gauss() {
	Matrix mat_t;
	int n_t, m_t;
	cpy_mat(mat_t, &n_t, &m_t, mat, n, m);
	bien_doi_Gauss(mat_t, n_t, m_t, OUTPUT_PATH);
}

void cal_bd() {
	show_matrix(mat, n, m, stdout);
	bien_doi_ma_tran(mat, 0, 0, n, m, true);
}

void cal_rank() {
	Matrix mat_r;
	int n_r, m_r;
	cpy_mat(mat_r, &n_r, &m_r, mat, n, m);
	bien_doi_ma_tran(mat_r, 0, 0, n_r, m_r, false);
	show_matrix(mat, n, m, stdout);
	rank(mat_r, n_r, m_r, true);
}

void leave() {
	printf("%s\n", GOODBYE_MSG);
}

int main() {
	log_file = fopen(LOG_PATH, "w");
	
	MenuOption mo_input_stdin = create_menu_option(MENU_1_INPUT_STDIN, input_stdin);
	MenuOption mo_input_file = create_menu_option(MENU_2_INPUT_FILE, input_file);
	MenuOption mo_cd = create_menu_option(MENU_3_CD, cal_cd);
	MenuOption mo_dc = create_menu_option(MENU_4_DC, cal_dc);
	MenuOption mo_gauss = create_menu_option(MENU_5_GAUSS, cal_gauss);
	MenuOption mo_bd = create_menu_option(MENU_6_BD, cal_bd);
	MenuOption mo_rank = create_menu_option(MENU_7_RANK, cal_rank);
	MenuOption mo_leave = create_menu_option(MENU_8_LEAVE, leave);

	menu_input = create_menu();
	push(mo_input_stdin, menu_input);	push(mo_input_stdin, menu_input);
	push(mo_leave, menu_input);

	menu_main = create_menu();
	push(mo_input_stdin, menu_main);	push(mo_input_stdin, menu_main);
	push(mo_cd, menu_main);	push(mo_dc, menu_main);	push(mo_gauss, menu_main);
	push(mo_bd, menu_main);	push(mo_rank, menu_main);	push(mo_leave, menu_main);

	menu_kcd = create_menu();
	push(mo_input_stdin, menu_kcd);	push(mo_input_stdin, menu_kcd);
	push(mo_gauss, menu_kcd);	push(mo_bd, menu_kcd);	push(mo_rank, menu_kcd);
	push(mo_leave, menu_kcd);

	introduce_window();

	// Lay du lieu ma tran tu file DATA.INP.
	CLEAR_SCREEN;
	printf("\n%s\n", LOADING_DATA_MSG);
	enter_to_continue();
	CLEAR_SCREEN;
	bool foo = load_matrix(INPUT_PATH, false, mat, &n, &m);
	if (!foo)	input_window(); 	// Neu du lieu sai thi nhap lai.

	main_window();	

	fclose(log_file);
	free(menu_main);
	free(menu_kcd);
	free(menu_input);
	return 0;
}