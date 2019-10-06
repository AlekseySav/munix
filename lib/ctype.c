#include "lib.h"

unsigned char _ctype[257] = { 0x00,     // EOF
    _C, _C, _C, _C, _C, _C, _C, _C,
    _C, _C | _S, _C | _S, _C | _S, _C | _S, _C | _S, _C, _C,
    _C, _C, _C, _C, _C, _C, _C, _C,
    _C, _C, _C, _C, _C, _C, _C, _C,
    _S | _SP, _P, _P, _P, _P, _P, _P, _P, 
    _P, _P, _P, _P, _P, _P, _P, _P, 
    _D, _D, _D, _D, _D, _D, _D, _D,
    _D, _D, _P, _P, _P, _P, _P, _P,
    _P, _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U,
    _U, _U, _U, _U, _U, _U, _U, _U, 
    _U, _U, _U, _U, _U, _U, _U, _U, 
    _U, _U, _U, _P, _P, _P, _P, _P, 
    _P, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L, 
    _L, _L, _L, _L, _L, _L, _L, _L, 
    _L, _L, _L, _L, _L, _L, _L, _L, 
    _L, _L, _L, _P, _P, _P, _P, _C,
    0                                   // 128 - 256
};
