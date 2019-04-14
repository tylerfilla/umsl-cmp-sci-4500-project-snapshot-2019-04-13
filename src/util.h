/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef UTIL_H
#define UTIL_H

/**
 * Stringify a preprocessor symbol.
 *
 * @param x The symbol
 * @return The symbol, stringified
 */
#define STRINGIFY(x) STRINGIFY2(x)

/**
 * @private
 */
#define STRINGIFY2(x) #x

#endif // #ifndef UTIL_H
