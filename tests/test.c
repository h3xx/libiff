/*
 * Copyright (c) 2012 Sander van der Burg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "test.h"
#include "iff.h"

IFF_Chunk *TEST_read(const char *filename, IFF_IOError **error)
{
    return IFF_read(filename, &TEST_chunkRegistry, error);
}

IFF_Bool TEST_write(const char *filename, const IFF_Chunk *chunk, IFF_IOError **error)
{
    return IFF_write(filename, chunk, &TEST_chunkRegistry, error);
}

void TEST_free(IFF_Chunk *chunk)
{
    IFF_free(chunk, &TEST_chunkRegistry);
}

IFF_QualityLevel TEST_check(const IFF_Chunk *chunk)
{
    return IFF_check(chunk, &TEST_chunkRegistry);
}

void TEST_printFd(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    IFF_printFd(file, chunk, indentLevel, &TEST_chunkRegistry);
}

IFF_Bool TEST_compare(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    return IFF_compare(chunk1, chunk2, &TEST_chunkRegistry);
}
