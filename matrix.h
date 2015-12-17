#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <malloc.h>

struct Matrix{
    int **data;
    int rows;
    int cols;
};

/**
* @Description Matrix constructor
*/
void matrix_init(struct Matrix *m, int rows, int cols){

    int i, j;
    //Allocate array of pointers, size is Rows
    m->data = (int **)malloc(rows * sizeof(int *));
    //If there is not enough memory for previus allocating
    if(m->data == NULL)
        exit(1);

    for(i = 0; i < rows; i++)
    {
        //Allocate array of elements, size is Cols
        m->data[i] = (int *)malloc(cols * sizeof(int));
        //If there is not enough memory for previus allocating
        if(m->data[i] == NULL)
        {
            //Clear successful allocatios
            for(j = 0; j < i; j++)
                free(m->data[i]);
            free(m->data);
            exit(1);
        }
    }

    m->rows = rows;
    m->cols = cols;
}

/**
* @Description Matrix destructor
*/
void matrix_destruct(struct Matrix *m){
    int i;

    if(m->data != NULL){
        for(i = 0; i < m->rows; i++)
            free(m->data[i]);

        free(m->data[i]);
    }

    m->data = NULL;
}

/**
* @Description Print matrix to standard output
*/
void matrix_print(struct Matrix *m){
    int i, j;

    for (i = 0; i < m->rows; i++){
        for(j = 0; j < m->cols; j++)
            printf("% 4d", m->data[i][j]);

        printf("\n");
    }
}

/**
* @Description Write matrix to file.
* First two ints are number of rows and cols respectively.
* After that ints matrix data array by matrix rows.
*/
void matrix_write_to_file(struct Matrix *m, const char *file_path){
    FILE *fp;
    int i, j;

    fp = fopen(file_path,"w");
    if (fp == NULL)
    {
        printf("Error file opening!!!\n");
        exit(1);
    }

    fprintf(fp, "%4d", m->rows);
    fprintf(fp, "%4d", m->cols);

    for (i = 0; i < m->rows; i++)
        for(j = 0; j < m->cols; j++)
            fprintf(fp, "%4d", m->data[i][j]);
}


/**
* @Description Read data from file and init matrix.
* First two ints are number of rows and cols respectively.
* After that ints matrix data array by matrix rows.
*/
void matrix_init_from_file(struct Matrix *m, const char *file_path){
    FILE *fp;
    int i, j;

    fp = fopen(file_path,"r");
    if (fp == NULL)
    {
        printf("Error file opening!!!\n");
        exit(1);
    }

    fscanf(fp, "%4d", &m->rows);
    fscanf(fp, "%4d", &m->cols);

    for (i = 0; i < m->rows; i++)
        for(j = 0; j < m->cols; j++)
            fscanf(fp, "%4d", &m->data[i][j]);

}
#endif // MATRIX_H_INCLUDED
