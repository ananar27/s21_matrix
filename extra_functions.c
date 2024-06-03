#include "s21_matrix.h"

int s21_empty_matrix_check(matrix_t *A) {
  return (A == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0)
             ? INVALID_MATRIX
             : OK;
}

int negative_size(int rows, int columns) { return rows <= 0 || columns <= 0; }

int is_null_ptr(matrix_t *M) { return M == NULL; }

int are_equal(matrix_t *A, matrix_t *B) {
  return A->rows != B->rows || A->columns != B->columns;
}

int of_correct_size_to_mul(matrix_t *A, matrix_t *B) {
  return A->columns == B->rows;
}

void algebraic_complements(matrix_t *A, matrix_t *result) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if ((i + j) % 2) {
        result->matrix[i][j] *= -1;
      }
    }
  }
}

void record_det_matr(matrix_t *det, matrix_t *A, int minor_row,
                     int minor_column) {
  for (int src_row = 0, det_row = 0; src_row < A->rows; src_row++) {
    for (int src_col = 0, det_col = 0; src_col < A->columns; src_col++) {
      if (src_row != minor_row && src_col != minor_column) {
        det->matrix[det_row][det_col] = A->matrix[src_row][src_col];
        det_col++;
        if (det_col == det->columns) {
          det_row++;
          det_col = 0;
        }
      }
    }
  }
}

int nan_or_inf(matrix_t *A) {
  int res = OK;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (isinf(A->matrix[i][j]) || isnan(A->matrix[i][j])) {
        res = CALCULATION_ERROR;
        j = A->columns;
        i = A->rows;
      }
    }
  }
  return res;
}

double s21_get_determinant(matrix_t *A) {
  double result = 0;
  if (A->rows == 1) {
    result = A->matrix[0][0];
  } else {
    matrix_t temp = {0};
    s21_create_matrix(A->rows - 1, A->columns - 1, &temp);
    for (int i = 0; i < A->columns; i++) {
      s21_get_minor_matrix(0, i, A, &temp);
      double minor_det = s21_get_determinant(&temp);
      if (i % 2 == 0) {
        result += A->matrix[0][i] * minor_det;
      } else {
        result -= A->matrix[0][i] * minor_det;
      }
    }
    s21_remove_matrix(&temp);
  }
  return result;
}

void s21_get_minor_matrix(int row, int col, matrix_t *A, matrix_t *result) {
  int m_row = 0;
  int m_col = 0;
  for (int i = 0; i < A->rows; i++) {
    if (i == row) {
      continue;
    }
    m_col = 0;
    for (int j = 0; j < A->columns; j++) {
      if (j == col) {
        continue;
      }
      result->matrix[m_row][m_col] = A->matrix[i][j];
      m_col++;
    }
    m_row++;
  }
}
