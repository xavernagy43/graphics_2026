#include <stdio.h>
#include "matrix.h"



int main(int argc, char* argv[])
{
	float a[3][3] = {
        { 1.0f, -2.0f,  3.0f},
        { 5.0f, -3.0f,  0.0f},
        {-2.0f,  1.0f, -4.0f}
    };
	float b[3][3];
	float c[3][3];

    init_zero_matrix(b);
    b[1][1] =  8.0f;
    b[2][0] = -3.0f;
    b[2][2] =  5.0f;

    print_matrix(a);


    print_matrix(b);


    add_matrices(a, b, c);

    print_matrix(c);



    float unit[3][3];
    init_identity_matrix(unit);
    print_matrix(unit);


    float number = 2.7;
    scalar_matrix(a,number);
    print_matrix(a);


    multiply_matrices(a,b,unit);
    print_matrix(unit);

    //Rotation by 90 degree
    Point p = {1, 2, 3};
    float transform_matrix[3][3] = {
        {0, -1, 0},
        {1, 0, 0},
        {0, 0, 1}
    };

    Point transformed_point = transform_point(transform_matrix,p);
    printf("Point before transformation: (%.2f, %.2f, %.2f)\n", p.x, p.y, p.z);
    printf("Transformed point: (%.2f, %.2f, %.2f)\n", transformed_point.x, transformed_point.y, transformed_point.z);

	return 0;
}

