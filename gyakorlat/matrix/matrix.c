#include "matrix.h"
#include <stdio.h>

void init_zero_matrix(float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = 0.0;
        }
    }
}

void print_matrix(const float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            printf("%4.4f ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void add_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}


void init_identity_matrix(float matrix[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if(i == j){
            matrix[i][j] = 1.0;
            }
            else{
                matrix[i][j] = 0.0;
            }
        }
        
    }
    
}

void scalar_matrix(float matrix[3][3], float number)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            matrix[i][j] = matrix[i][j] * number;
        }
        
    }
    
}

void multiply_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
            
        }
        
    }
    
}

Point transform_point(const float a[3][3], Point p){
    Point result;
    float homogeneous[4] = {p.x, p.y, p.z, 1.0};
    for (int i = 0; i < 3; ++i) {
        result.x += a[0][i] * homogeneous[i];
        result.y += a[1][i] * homogeneous[i];
        result.z += a[2][i] * homogeneous[i];
    }

    return result;
}

void scale(float matrix[3][3], float scale_factor) {
    for (int i = 0; i < 3; ++i) {
        matrix[i][i] *= scale_factor;
    }
}

void shift(float matrix[3][3], float shift_vector[3 - 1]) {
    for (int i = 0; i < MATRIX_SIZE - 1; ++i) {
        matrix[MATRIX_SIZE - 1][i] += shift_vector[i];
    }
}