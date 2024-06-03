#ifndef S21_MATRIX_TEST
#define S21_MATRIX_TEST

#include <stdbool.h>

#include "../s21_matrix.h"
#include "check.h"

void s21_copy_matrix(double from[][3], matrix_t *A);
void s21_copy_matrix_4(double from[][4], matrix_t *A);
void s21_print_matrix(matrix_t A);

void run_test(Suite *test);
Suite *s21_create_matrix_suite();
Suite *s21_eq_matrix_suite();
Suite *s21_sum_matrix_suite();
Suite *s21_sub_matrix_suite();
Suite *s21_mult_number_suite();
Suite *s21_mult_matrix_suite();
Suite *s21_transpose_suite();
Suite *s21_calc_complements_suite();
Suite *s21_determinant_suite();
Suite *s21_inverse_matrix_suite();

#endif