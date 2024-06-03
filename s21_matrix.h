#ifndef S21_MATRIX_H
#define S21_MATRIX_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 1
#define FAILURE 0

#define ACCURACY 1e-07

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

enum code_results { OK = 0, INVALID_MATRIX = 1, CALCULATION_ERROR = 2 };

int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);
int s21_eq_matrix(matrix_t *A, matrix_t *B);
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_mult_number(matrix_t *A, double number, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_transpose(matrix_t *A, matrix_t *result);
int s21_calc_complements(matrix_t *A, matrix_t *result);
int s21_determinant(matrix_t *A, double *result);
int s21_inverse_matrix(matrix_t *A, matrix_t *result);

// extra_functions.c
int s21_empty_matrix_check(matrix_t *A);
int negative_size(int rows, int columns);
int is_null_ptr(matrix_t *M);
int are_equal(matrix_t *A, matrix_t *B);
int of_correct_size_to_mul(matrix_t *A, matrix_t *B);
void algebraic_complements(matrix_t *A, matrix_t *result);
void record_det_matr(matrix_t *det, matrix_t *A, int minor_row,
                     int minor_column);
int nan_or_inf(matrix_t *A);
double s21_get_determinant(matrix_t *A);
void s21_get_minor_matrix(int row, int col, matrix_t *A, matrix_t *result);

#endif