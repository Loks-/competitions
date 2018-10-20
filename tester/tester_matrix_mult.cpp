#include "tester_matrix_mult.h"

bool TestMatrixMult()
{
    TesterMatrixMult<1000> tester;
    return tester.TestMultAll();
}
