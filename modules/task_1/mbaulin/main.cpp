// Copyright 2018 Nesterov Alexander
#include "../../../3rdparty/gtest-mpi/linux/gtest-mpi-listener.hpp"
#include <gtest/gtest.h>
#include <vector>
#include "./ops_mpi.h"


TEST(Parallel_Operations_MPI, Test_Max) {
    int rank;
    int rows = 50;
    int cols = 50;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    int** mat = new int* [rows];
    for (int i = 0; i < rows; i++)
        mat[i] = new int[cols];


    if (rank == 0) {
        mat = fillMatrixWithRandomNumbers(mat, rows, cols);
    }

    int global_max;
    global_max = getParallelOperations(mat, rows, cols);

    if (rank == 0) {
        int reference_max = findMax(mat, rows, cols);
        ASSERT_EQ(reference_max, global_max);
    }
}

TEST(Parallel_Operations_MPI, Test_Max_24) {
    int rank;
    int rows = 5;
    int cols = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    int** mat = new int* [rows];
    for (int i = 0; i < rows; i++)
    {
        mat[i] = new int[cols];
        for (int j = 0; j < cols; j++)
            mat[i][j] = i * cols + j;
    }

    int global_max;
    global_max = getParallelOperations(mat, rows, cols);

    if (rank == 0) {
        int reference_max = findMax(mat, rows, cols);
        ASSERT_EQ(24, global_max);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}