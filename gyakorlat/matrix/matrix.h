#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
    float x, y, z;
} Point;
/**
 * Initializes all elements of the matrix to zero.
 */
void init_zero_matrix(float matrix[3][3]);

/**
 * Print the elements of the matrix.
 */
void print_matrix(const float matrix[3][3]);

/**
 * Add matrices.
 */
void add_matrices(const float a[3][3], const float b[3][3], float c[3][3]);


/// Initilaize elements to one 
void init_identity_matrix(float matrix[3][3]);

/// Matrix scalar 
void scalar_matrix(float matrix[3][3], float number);

/// Matrix multiple by matrix
void multiply_matrices(const float a[3][3], const float b[3][3], float c[3][3]); 

/// Transform
Point transform_point(const float a[3][3], Point p);

/// Scale  
void scale(float a[3][3], float scale_factor);

/// Shift
void shift(float a[3][3], float x, float y);

/// Rotate
void rotate(float a[3][3], float x, float y);

#endif // MATRIX_H

