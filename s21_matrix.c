#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  if (is_null_ptr(result) || negative_size(rows, columns))
    return INVALID_MATRIX;

  int res = OK;
  int ptr = 0;
  result->matrix = (double **)calloc(rows, sizeof(double *));
  if (result->matrix == NULL) {
    res = 1;
  }
  for (; ptr < rows && !res; ptr++) {
    result->matrix[ptr] = (double *)calloc(columns, sizeof(double));
  }
  if (!res) {
    result->rows = rows;
    result->columns = columns;
  } else {
    if (!result->matrix[ptr]) {
      for (int j = 0; j < ptr; j++) {
        free(result->matrix[j]);
      }
      free(result->matrix);
    }
  }
  return res;
}

void s21_remove_matrix(matrix_t *A) {
  if (A->matrix != NULL && A->rows > 0 && A->columns > 0) {
    for (int i = 0; i < A->rows; i++) {
      if (A->matrix[i] != NULL) {
        free(A->matrix[i]);
      }
    }
    free(A->matrix);
  }
  A->rows = 0;
  A->columns = 0;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  if (is_null_ptr(A) || is_null_ptr(B) || are_equal(A, B)) {
    return FAILURE;
  }
  int res = SUCCESS;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > ACCURACY) {
        res = FAILURE;
        i = A->rows;
        j = A->columns;
      }
    }
  }
  return res;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = 0;
  if (s21_empty_matrix_check(A) != 0 || s21_empty_matrix_check(B) != 0)
    error = 1;
  else if (A->rows != B->rows || A->columns != B->columns || nan_or_inf(A)) {
    error = 2;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }
  return error;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = 0;
  if (s21_empty_matrix_check(A) != 0 || s21_empty_matrix_check(B) != 0)
    error = 1;
  else if (A->rows != B->rows || A->columns != B->columns || nan_or_inf(A)) {
    error = 2;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }
  return error;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int error = 0;
  if (s21_empty_matrix_check(A) != 0)
    error = 1;
  else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }
  return error;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = 0;
  if (s21_empty_matrix_check(A) == 0 && s21_empty_matrix_check(B) == 0) {
    if (A->columns == B->rows) {
      error = s21_create_matrix(A->rows, B->columns, result);
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->columns; j++) {
          for (int k = 0; k < B->rows; k++) {
            result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
          }
        }
      }
    } else {
      error = 2;
    }
  } else {
    error = 1;
  }
  return error;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int error = 0;
  if (s21_empty_matrix_check(A) == 0) {
    error = s21_create_matrix(A->columns, A->rows, result);
    if (error == 0) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[j][i] = A->matrix[i][j];
        }
      }
    }
  } else {
    error = 1;
  }
  return error;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  if (is_null_ptr(A) || is_null_ptr(result)) {
    return INVALID_MATRIX;
  } else if (A->rows != A->columns || nan_or_inf(A)) {
    return CALCULATION_ERROR;
  }
  s21_create_matrix(A->rows, A->columns, result);
  matrix_t det;
  if (A->rows > 1) {
    s21_create_matrix(A->rows - 1, A->columns - 1, &det);
  } else {
    result->matrix[0][0] = 1.;
  }
  for (int minor_row = 0; minor_row < A->rows && A->rows > 1; minor_row++) {
    for (int minor_column = 0; minor_column < A->columns; minor_column++) {
      record_det_matr(&det, A, minor_row, minor_column);
      s21_determinant(&det, &result->matrix[minor_row][minor_column]);
    }
  }
  if (A->rows > 1) {
    s21_remove_matrix(&det);
    algebraic_complements(A, result);
  }
  return 0;
}

int s21_determinant(matrix_t *A, double *result) {
  if (is_null_ptr(A) || result == NULL) {
    return INVALID_MATRIX;
  } else if (A->rows != A->columns || nan_or_inf(A)) {
    return CALCULATION_ERROR;
  }
  *result = 0.0;
  int error = 0;
  if (s21_empty_matrix_check(A) == 0) {
    if (A->rows == A->columns) {
      *result = s21_get_determinant(A);
    }
  }

  return error;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (is_null_ptr(A) || result == NULL) {
    return INVALID_MATRIX;
  } else if (A->rows != A->columns || nan_or_inf(A)) {
    return CALCULATION_ERROR;
  }
  int error = OK;
  double deter = 0.;
  s21_determinant(A, &deter);
  if (deter) {
    matrix_t compliments, transposed;
    s21_calc_complements(A, &compliments);
    s21_transpose(A, &transposed);
    deter = 1 / deter;
    s21_mult_number(&transposed, deter, result);
    s21_remove_matrix(&compliments);
    s21_remove_matrix(&transposed);
  } else {
    error = CALCULATION_ERROR;
  }
  return error;
}
