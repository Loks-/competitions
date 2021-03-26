#pragma once

#include "common/base.h"

uint64_t PrimesCount_Table(uint64_t n);
uint64_t PrimesCount_PrimesGeneration(uint64_t n);
uint64_t PrimesCount_Legendre(uint64_t n);
uint64_t PrimesCount_Meissel(uint64_t n);
uint64_t PrimesCount_MeisselLehmer1(uint64_t n);
uint64_t PrimesCount_MeisselLehmerB(uint64_t n, unsigned b);
uint64_t PrimesCount_LagariasMillerOdlyzko(uint64_t n);
uint64_t PrimesCount_LagariasMillerOdlyzkoY(uint64_t n);
uint64_t PrimesCount_DelegliseRivat(uint64_t n);
uint64_t PrimesCount_LucyHedgehogRecursive(uint64_t n);
uint64_t PrimesCount_LucyHedgehogRecursive2(uint64_t n);
uint64_t PrimesCount_LucyHedgehogVector(uint64_t n);
uint64_t PrimesCount_PCQ(uint64_t n);
