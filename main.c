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

#define TAO_THU_MUC_QUAN_LI {									\
	printf("Tao cac thu muc can thiet...\n");					\
	if (system("cd INPUT && cd .."))	system("mkdir INPUT");	\
	if (system("cd OUTPUT && cd .."))	system("mkdir OUTPUT");	\
	if (system("cd log && cd .."))		system("mkdir log");	\
	CLEAR_SCREEN;												\
}

#define MAX 100
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
#define BO_QUA_HIEN_BUOC "Nhan ki tu bat ki de bo qua hien buoc."

#define ERR_SO_DONG_LON_HON_COT(n, m) printf("So dong(%d) lon hon hoac bang so cot (%d).", n, m)
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
#define SHOW_ERROR(log_func) { log_func;		return false; }

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
#define MO_TIM_NGHIEM "Tinh."
#define MO_BIEN_DOI "Bien doi ma tran thanh dang bac thang."
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
struct Menu *menu;
FILE* log_file;
Matrix matrix, backup_matrix;
int n, m, backup_n, backup_m;
void thoat();  // Ham thoat khoi chuong trinh.
struct Menu* create_menu();  // Ham tao menu;

void init() {  // Khoi tao cac bien toan cuc.
	log_file = fopen(LOG_PATH, "w");
	menu = create_menu();
}

void destroy() {  // Giai phong cac bien toan cuc duoc cap phat.
	fclose(log_file);
	free(menu);
}

// ========================================== //
//              CAC HAM THONG DUNG            //
// ========================================== //

bool enter_to_continue() {
	printf("\n%s...", ENTER_TO_CONTINUE);
	fflush(stdin);		char c = getchar();		fflush(stdin);
	return c == '\n';
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
	for (int i = 0; i < menu->count; ++i)
		printf("%d/. %s\n", i + 1, menu->list[i].label);
	printf("%s", MO_NHAP_LUA_CHON);
	int op = (int)(getc(stdin) - '0') - 1;	fflush(stdin);
	CLEAR_SCREEN;
	printf("\n");
	if (op >= menu->count || op < 0) {
		printf("%s\n", MSG_LUA_CHON_KHONG_HOP_LE);
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
  	fprintf(log_file, "%s", NHAP_SO_DONG);		fscanf(file, "%d%c", n, &dummy);
  	if (dummy != '\n' && dummy != ' ')	SHOW_ERROR(ERR_DL_KHONG_HOP_LE(0, 0, true, false));

  	fprintf(log_file, "%s", NHAP_SO_COT);		fscanf(file, "%d%c", m, &dummy);
  	if (dummy != '\n' && dummy != ' ')	SHOW_ERROR(ERR_DL_KHONG_HOP_LE(0, 0, false, true));

	if (*n  >= *m)	SHOW_ERROR(ERR_SO_DONG_LON_HON_COT(*n, *m));

  	fprintf(log_file, "%s\n", NHAP_MA_TRAN);
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
bool load_matrix(string path, bool read_path_stdin, Matrix matrix, int *n, int *m) {
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
  	bool foo = scan_matrix(matrix, n, m, file, log_file);
  	fclose(file);
  	return foo;
}

// Ghi ma tran ra file (hoac stdout).
void show_matrix(Matrix matrix, int n, int m, FILE *file) {
  	for (int i = 0; i < n; ++i) {
    	for (int j = 0; j < m - 1; ++j)
			fprintf(file, DATA_FORMAT, matrix[i][j]);
    	fprintf(file, CHAR_FORMAT, '|');
    	fprintf(file, DATA_FORMAT, matrix[i][m - 1]);
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
	show_matrix(matrix, n, m, stdout);
	show_menu(menu);
	main_window();
}

void introduce_window() {
	printf("%s\n%s\n\n%s\n\n", TITLE, DESCRIPTION, INTRODUCE);
	enter_to_continue();
}

// ========================================== //
//             CAC HAM TINH NGHIEM            //
// ========================================== //

void xuat_nghiem(FILE* file, Matrix backup_matrix, int backup_n, int backup_m,
		bool vo_nghiem, bool vo_so_nghiem, Vector nghiem) {
	const int SO_NGHIEM = m - 1;
	fprintf(file, "\n\n\n%s\n\n", OUTPUT_LABEL);
	show_matrix(backup_matrix, backup_n, backup_m, file);

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

void swap_row(Matrix matrix, int m, int i1, int i2) {
  	for (int j = 0; j < m; ++j) swap(&matrix[i1][j], &matrix[i2][j]);
}

void bien_doi_ma_tran(Matrix matrix, int i0, int j0, int n, int m, bool show_step) {
    if (i0 >= n || j0 >= m) {
		if(show_step)	TB_BD_THANH_CONG;
		return;
	}
    int k;
    if (matrix[i0][j0] == 0) {
        // Tim vi tri dau tien khac 0.
        k = i0 + 1;
        while (k < n && matrix[k][j0] == 0)	++k;
        if (k >= n)     return bien_doi_ma_tran(matrix, i0, j0 + 1, n, m, show_step);
        swap_row(matrix, m, i0, k);
    
        if (show_step) {
			TB_DOI_DONG(i0, k);
            show_matrix(matrix, n, m, stdout);
			printf("%s", BO_QUA_HIEN_BUOC);
            show_step = enter_to_continue();
        }
    }

    float tmp;
    for (int i = i0 + 1; i < n; ++i) {
        if (matrix[i][j0] == 0)    continue;
        tmp = matrix[i][j0] / matrix[i0][j0];
        for (int j = j0; j < m; ++j)    matrix[i][j] -= tmp * matrix[i0][j];

        if (show_step) {
			TB_BIEN_DOI(i0, tmp, i);
            show_matrix(matrix, n, m, stdout);
            printf("%s", BO_QUA_HIEN_BUOC);
            show_step = enter_to_continue();
        }
  	}
  	return bien_doi_ma_tran(matrix, i0 + 1, j0 + 1, n, m, show_step);
}

int rank(Matrix matrix, int n, int m) {
    int j = 0, r = 0;
    for (int i = 0; i < n; ++i) {
		while (j < m && matrix[i][j] == 0)  ++j;
		if (j < m)	++r;
	}
    return r; 
}

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

void bien_doi_Gauss(Matrix matrix, int n, int m, string output_path) {
    Vector nghiem;

    Matrix backup_matrix;
    int backup_n, backup_m;
    cpy_mat(backup_matrix, &backup_n, &backup_m, matrix, n, m);

	bien_doi_ma_tran(matrix, 0, 0, n, m, true);
	int r_mr = rank(matrix, n, m);
	int r = rank(matrix, n, m - 1);
	int so_nghiem = m - 1;

	FILE* file = fopen(output_path, "a");
	if (r != r_mr) {
		xuat_nghiem(stdout, backup_matrix, backup_n, backup_m, true, false, nghiem);
		xuat_nghiem(file, backup_matrix, backup_n, backup_m, true, false, nghiem);
	} else if (r < so_nghiem) {
		xuat_nghiem(stdout, backup_matrix, backup_n, backup_m, false, true, nghiem);
		xuat_nghiem(file, backup_matrix, backup_n, backup_m, false, true, nghiem);
	} else {
		xac_dinh_nghiem(matrix, n, m, nghiem);
		xuat_nghiem(stdout, backup_matrix, backup_n, backup_m, false, false, nghiem);
		xuat_nghiem(file, backup_matrix, backup_n, backup_m, false, false, nghiem);
	}

	fclose(file);
}

// ========================================== //
//              CAC HAM XU LI                 //
// ========================================== //

void input_from_stdin() {
	Matrix input_matrix;
	int input_n, input_m;
	if (scan_matrix(input_matrix, &input_n, &input_m, stdin, stdout))
		cpy_mat(matrix, &n, &m, input_matrix, input_n, input_m);
}

void input_from_file() {
	char path[MAX];
	Matrix input_matrix;
	int input_n, input_m;
	if (load_matrix(path, true, input_matrix, &input_n, &input_m))
		cpy_mat(matrix, &n, &m, input_matrix, input_n, input_m);
}

void cal_cong_don() {  // Cong don.
	cpy_mat(backup_matrix, &backup_n, &backup_m, matrix, n, m);
	cong_don(matrix, &n, &m);
	show_matrix(matrix, n, m, stdout);
}

void cal_huy_cong_don() {  // Huy cong don.
	cpy_mat(matrix, &n, &m, backup_matrix, backup_n, backup_m);
	show_matrix(matrix, n, m, stdout);
}

void cal_tim_nghiem() {
	Matrix cal_matrix;
	int cal_n, cal_m;
	cpy_mat(cal_matrix, &cal_n, &cal_m, matrix, n, m);
	bien_doi_Gauss(cal_matrix, cal_n, cal_m, OUTPUT_PATH);
}

void cal_bien_doi() {
	show_matrix(matrix, n, m, stdout);
	bien_doi_ma_tran(matrix, 0, 0, n, m, true);
}

void thoat() {
	printf("%s\n", GOODBYE_MSG);
}

int main() {
	TAO_THU_MUC_QUAN_LI;
	init();
	// Lay du lieu tu file DATA.INP
	load_matrix(INPUT_PATH, false, matrix, &n, &m);
	
	push(MO_INPUT_FROM_STDIN, input_from_stdin, menu);
	push(MO_INPUT_FROM_FILE, input_from_file, menu);
	push(MO_CONG_DON, cal_cong_don, menu);
	push(MO_HUY_CONG_DON, cal_huy_cong_don, menu);
	push(MO_TIM_NGHIEM, cal_tim_nghiem, menu);
	push(MO_BIEN_DOI, cal_bien_doi, menu);
	push(MO_THOAT, thoat, menu);

	introduce_window();
	main_window();

	destroy();
	return 0;
}