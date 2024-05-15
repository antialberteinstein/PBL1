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

#define TAO_THU_MUC_QUAN_LI {										\
	printf("Tao cac thu muc can thiet...\n");					\
	if (system("cd INPUT && cd .."))	system("mkdir INPUT");	\
	if (system("cd OUTPUT && cd .."))	system("mkdir OUTPUT");	\
	if (system("cd log && cd .."))		system("mkdir log");	\
	CLEAR_SCREEN;												\
}

#define MAX 100
#define MSG_LAY_DL "Dang lay du lieu tu file DATA.INP"
#define CHAR_FORMAT "%7c"
#define DATA_FORMAT "%7.2f"
#define XUAT_NGHEIM(file, nghiem, i)	fprintf(file, "x%d = %-7.2g", i + 1, nghiem)
#define INPUT_DIR "INPUT/"
#define LOG_PATH "log/log.txt"
#define INPUT_PATH "DATA.INP"
#define OUTPUT_PATH "OUTPUT/DATA.OUT"
#define OUTPUT_LABEL "=== TIM NGHIEM HE PHUONG TRINH ==="
#define TITLE \
"            ======= PBL1 - De tai 205 ======="
#define DESCRIPTION                                                            \
"   ** Tim nghiem he phuong trinh bang phuong phap Gauss. ** \n\
** Cong don phan tu sau cot he so tu do vao cot he so tu do. **\n\n"
#define INTRODUCE                                                              \
  "\t* Nhom 13 * \n\
   1/. Vu Duc Minh. \n\
   2/. Tran Nhat Nguyen. \n"

#define MSG_LUA_CHON_KHONG_HOP_LE "Canh bao: Lua chon khong hop le."
#define GOODBYE_MSG "\tHen gap lai..."
#define ENTER_TO_CONTINUE "Nhan Enter/Return de tiep tuc."

#define ERR_THIEU_DONG printf("Qua it dong.\n")
#define ERR_THUA_DONG printf("Qua nhieu dong.\n")
#define ERR_THIEU_COT(i) printf("Qua it du lieu tren dong %d.\n", i + 1)
#define ERR_THUA_COT(i) printf("Qua nhieu du lieu tren dong %d.\n", i + 1)
#define ERR_DL_KHONG_HOP_LE(i, j, row, col) {													\
	if (row)		printf("So dong co du lieu khong hop le.\n");							\
	else if (col)	printf("So cot co du lieu khong hop le.\n");							\
	else			printf("Du lieu khong hop le tai dong %d, cot %d.\n", i + 1, j + 1);	\
}
#define ERR_FILE_KHONG_TON_TAI(file) printf("File %s khong ton tai.\n", file)
#define SHOW_ERROR(log_func) {												\
	log_func;	enter_to_continue();		return false;					\
}

#define MSG_HANG_MT "Hang cua ma tran la: "

#define NHAP_SO_DONG "Nhap so dong: "
#define NHAP_SO_COT "Nhap so cot: "
#define NHAP_MA_TRAN "Nhap ma tran: "
#define NHAP_DUONG_DAN_FILE "Nhap duong dan file: "
#define MSG_DOC_DL_THANH_CONG "Doc ma tran thanh cong!"

#define TB_BD_THANH_CONG printf("Ma tran da duoc bien doi!\n");
#define TB_DOI_DONG(i1, i2) printf("Doi dong %d va %d.\n", i1 + 1, i2 + 1)
#define TB_BIEN_DOI(i1, k, i2) printf("Lay dong %d tru di %g lan dong %d.\n", i2 + 1, k, i1 + 1)
#define MSG_VO_SO_NGHIEM "He phuong trinh co vo so nghiem."
#define MSG_VO_NGHIEM "He phuong trinh vo nghiem."
#define MSG_CO_NGHIEM "He phuong trinh co nghiem la: "
#define OUTPUT_LABEL "=== TIM NGHIEM HE PHUONG TRINH ==="

#define MO_INPUT_FROM_STDIN "Nhap ma tran tu ban phim."
#define MO_INPUT_FROM_FILE "Nhap ma tran tu file."
#define MO_CONG_DON "Cong don cac phan tu sau cot he so tu do."
#define MO_HUY_CONG_DON "Tro ve ma tran ban dau truoc khi cong don."
#define MO_TIM_NGHIEM "Tim nghiem ma tran theo phuong phap Gauss."
#define MO_BIEN_DOI "Thuc hien cac buoc bien doi ma tran."
#define MO_TIM_HANG "Tim hang cua ma tran."
#define MO_THOAT "Thoat."
#define MO_NHAP_LUA_CHON "Nhap lua chon cua ban: "

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
struct Menu *menu_main, *menu_kcd;
FILE* log_file;
Matrix mat, mat_bk;
int n, m, n_bk, m_bk;
void thoat();  // Ham thoat khoi chuong trinh.
struct Menu* create_menu();  // Ham tao menu;

void init() {  // Khoi tao cac bien toan cuc.
	log_file = fopen(LOG_PATH, "w");
	menu_main = create_menu();
	menu_kcd = create_menu();
}

void destroy() {  // Giai phong cac bien toan cuc duoc cap phat.
	fclose(log_file);
	free(menu_main);
	free(menu_kcd);
}

// ========================================== //
//              CAC HAM THONG DUNG            //
// ========================================== //

void clear_stdin(){
	int c;
	do c = getchar();	while (c != '\n' && c != EOF);
}

void enter_to_continue() {
	printf("\n%s...", ENTER_TO_CONTINUE);
	clear_stdin();
}

// Cong don phan tu sau cot he so tu do vao cot he so tu do.
void cong_don(Matrix mat, int *n, int *m) {
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

// Tra ve false neu lua chon khong hop le.
void show_menu(Menu menu) {
	printf("\n");
	for (int i = 0; i < menu->count; ++i)
		printf("%d/. %s\n", i + 1, menu->list[i].label);
	printf("%s", MO_NHAP_LUA_CHON);
	int op = (int)(getc(stdin) - '0') - 1;	fflush(stdin);
	CLEAR_SCREEN;
	printf("\n");
	if (op >= menu->count || op < 0) {
		printf("%s\n", MSG_LUA_CHON_KHONG_HOP_LE);
		enter_to_continue();
		return;
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
bool scan_matrix(Matrix mat, int *n, int *m, FILE* file, FILE* log_file) {
  	char dummy;
  	fprintf(log_file, "%s", NHAP_SO_DONG);		fscanf(file, "%d%c", n, &dummy);
  	if (dummy != '\n' && dummy != ' ')	SHOW_ERROR(ERR_DL_KHONG_HOP_LE(0, 0, true, false));

  	fprintf(log_file, "%s", NHAP_SO_COT);		fscanf(file, "%d%c", m, &dummy);
  	if (dummy != '\n' && dummy != ' ')	SHOW_ERROR(ERR_DL_KHONG_HOP_LE(0, 0, false, true));

  	fprintf(log_file, "%s\n", NHAP_MA_TRAN);
  	for (int i = 0; i < *n; ++i)
  		for (int j = 0;j < *m; ++j) {
    		fscanf(file, "%f%c", &mat[i][j], &dummy);
    		if (feof(file))			dummy = '\n';
			if (dummy != '\n' && dummy != ' ')	SHOW_ERROR(ERR_DL_KHONG_HOP_LE(i, j, false, false));
			if (file == stdin)	continue;
			const long int POSITION = ftell(file);
			while (dummy == ' ')	dummy = getc(file);
			if (dummy == '\n' && j < *m - 1) {
				if (j <= 0)
					SHOW_ERROR(ERR_THIEU_DONG)
				else
					SHOW_ERROR(ERR_THUA_COT(i));
			}
			else if (dummy != '\n' && j >= *m - 1)
				SHOW_ERROR(ERR_THUA_COT(i))
			else
				fseek(file, POSITION, SEEK_SET);
  		}
	float tmp;
	if (file != stdin && !feof(file) && fscanf(file, "%f", &tmp) == 1)
		SHOW_ERROR(ERR_THUA_DONG);

	printf("%s\n", MSG_DOC_DL_THANH_CONG);
    return true;
}

// Nhap ma tran tu file.
bool load_matrix(string path, bool read_path_stdin, Matrix mat, int *n, int *m) {
	if (read_path_stdin) {
		printf("%s", NHAP_DUONG_DAN_FILE);
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
  	bool foo = scan_matrix(mat, n, m, file, log_file);
  	fclose(file);
  	return foo;
}

// Ghi ma tran ra file (hoac stdout).
void show_matrix(Matrix mat, int n, int m, FILE *file) {
  	for (int i = 0; i < n; ++i) {
    	for (int j = 0; j < m - 1; ++j)
			fprintf(file, DATA_FORMAT, mat[i][j]);
    	fprintf(file, CHAR_FORMAT, '|');
    	fprintf(file, DATA_FORMAT, mat[i][m - 1]);
    	fprintf(file, "\n");
  	}
  	fprintf(file, "\n");
}

// ========================================== //
//              CAC CUA SO                    //
// ========================================== //

void main_window() {
	CLEAR_SCREEN;
	printf("%s\n%s\n\n", TITLE, DESCRIPTION);
	show_matrix(mat, n, m, stdout);
	if (n >= m - 1)
		show_menu(menu_kcd);
	else
		show_menu(menu_main);
	main_window();
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
	const int SO_NGHIEM = m - 1;
	fprintf(file, "\n\n\n%s\n\n", OUTPUT_LABEL);
	show_matrix(mat_bk, n_bk, m_bk, file);

	if (vo_nghiem)
		fprintf(file, "%s\n", MSG_VO_NGHIEM);
	else if (vo_so_nghiem)
		fprintf(file, "%s\n", MSG_VO_SO_NGHIEM);
	else {
		fprintf(file, "%s ", MSG_CO_NGHIEM);
		for (int i = 0; i < SO_NGHIEM; ++i)
			XUAT_NGHEIM(file, nghiem[i], i);
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
		if(show_step)	TB_BD_THANH_CONG;
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
			TB_DOI_DONG(i0, k);
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
			TB_BIEN_DOI(i0, tmp, i);
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
        printf("%s %d.\n", MSG_HANG_MT, r);
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

void input_from_stdin() {
	Matrix mat_i;
	int n_i, m_i;
	if (scan_matrix(mat_i, &n_i, &m_i, stdin, stdout))
		cpy_mat(mat, &n, &m, mat_i, n_i, m_i);
}

void input_from_file() {
	char path[MAX];
	Matrix mat_i;
	int n_i, m_i;
	if (load_matrix(path, true, mat_i, &n_i, &m_i))
		cpy_mat(mat, &n, &m, mat_i, n_i, m_i);
}

void cal_cong_don() {  // Cong don.
	cpy_mat(mat_bk, &n_bk, &m_bk, mat, n, m);
	cong_don(mat, &n, &m);
	show_matrix(mat, n, m, stdout);
}

void cal_huy_cong_don() {  // Huy cong don.
	cpy_mat(mat, &n, &m, mat_bk, n_bk, m_bk);
	show_matrix(mat, n, m, stdout);
}

void cal_tim_nghiem() {
	Matrix mat_t;
	int n_t, m_t;
	cpy_mat(mat_t, &n_t, &m_t, mat, n, m);
	bien_doi_Gauss(mat_t, n_t, m_t, OUTPUT_PATH);
}

void cal_bien_doi() {
	show_matrix(mat, n, m, stdout);
	bien_doi_ma_tran(mat, 0, 0, n, m, true);
}

void cal_tim_hang() {
	Matrix mat_r;
	int n_r, m_r;
	cpy_mat(mat_r, &n_r, &m_r, mat, n, m);
	bien_doi_ma_tran(mat_r, 0, 0, n_r, m_r, false);
	show_matrix(mat, n, m, stdout);
	rank(mat_r, n_r, m_r, true);
}

void thoat() {
	printf("%s\n", GOODBYE_MSG);
}

int main() {
	TAO_THU_MUC_QUAN_LI;
	init();
	// Lay du lieu tu file DATA.INP
	load_matrix(INPUT_PATH, false, mat, &n, &m);
	
	MenuOption mo_input_from_stdin = create_menu_option(MO_INPUT_FROM_STDIN, input_from_stdin);
	MenuOption mo_input_from_file = create_menu_option(MO_INPUT_FROM_FILE, input_from_file);
	MenuOption mo_cong_don = create_menu_option(MO_CONG_DON, cal_cong_don);
	MenuOption mo_huy_cong_don = create_menu_option(MO_HUY_CONG_DON, cal_huy_cong_don);
	MenuOption mo_tim_nghiem = create_menu_option(MO_TIM_NGHIEM, cal_tim_nghiem);
	MenuOption mo_bien_doi = create_menu_option(MO_BIEN_DOI, cal_bien_doi);
	MenuOption mo_tim_hang = create_menu_option(MO_TIM_HANG, cal_tim_hang);
	MenuOption mo_thoat = create_menu_option(MO_THOAT, thoat);

	push(mo_input_from_stdin, menu_main);	push(mo_input_from_file, menu_main);
	push(mo_cong_don, menu_main);	push(mo_huy_cong_don, menu_main);	push(mo_tim_nghiem, menu_main);
	push(mo_bien_doi, menu_main);	push(mo_tim_hang, menu_main);	push(mo_thoat, menu_main);

	push(mo_input_from_stdin, menu_kcd);	push(mo_input_from_file, menu_kcd);
	push(mo_tim_nghiem, menu_kcd);	push(mo_bien_doi, menu_kcd);	push(mo_tim_hang, menu_kcd);
	push(mo_thoat, menu_kcd);

	introduce_window();
	main_window();

	destroy();
	return 0;
}