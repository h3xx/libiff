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

#include "rawchunk.h"
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "io.h"
#include "id.h"
#include "util.h"

IFF_RawChunk *IFF_createRawChunk(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    IFF_RawChunk *rawChunk = (IFF_RawChunk*)IFF_allocateChunk(chunkId, chunkSize, sizeof(IFF_RawChunk));

    if(rawChunk != NULL)
    {
        rawChunk->chunkData = (IFF_UByte*)malloc(chunkSize * sizeof(IFF_UByte));

        if(rawChunk->chunkData == NULL)
        {
            free(rawChunk);
            return NULL;
        }
    }

    return rawChunk;
}

void IFF_copyDataToRawChunkData(IFF_RawChunk *rawChunk, IFF_UByte *data)
{
    memcpy(rawChunk->chunkData, data, rawChunk->chunkSize);
}

void IFF_setRawChunkData(IFF_RawChunk *rawChunk, IFF_UByte *chunkData, IFF_Long chunkSize)
{
    rawChunk->chunkData = chunkData;
    rawChunk->chunkSize = chunkSize;
}

void IFF_setTextData(IFF_RawChunk *rawChunk, const char *text)
{
    size_t textLength = strlen(text);
    IFF_UByte *chunkData = (IFF_UByte*)malloc(textLength * sizeof(IFF_UByte));

    memcpy(chunkData, text, textLength);
    IFF_setRawChunkData(rawChunk, chunkData, textLength);
}

static IFF_Bool readRawChunkData(FILE *file, IFF_RawChunk *rawChunk)
{
    /* Read remaining bytes verbatim */
    if(fread(rawChunk->chunkData, sizeof(IFF_UByte), rawChunk->chunkSize, file) < rawChunk->chunkSize)
    {
        IFF_error("Error reading raw chunk body of chunk: '");
        IFF_errorId(rawChunk->chunkId);
        IFF_error("'\n");
        return FALSE;
    }
    else
        return TRUE;
}

IFF_Bool IFF_readRawChunkData(FILE *file, IFF_RawChunk *rawChunk, IFF_Long *bytesProcessed)
{
    IFF_Bool status = readRawChunkData(file, rawChunk);
    *bytesProcessed = *bytesProcessed + rawChunk->chunkSize;
    return status;
}

IFF_RawChunk *IFF_readRawChunk(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize)
{
    IFF_RawChunk *rawChunk = IFF_createRawChunk(chunkId, chunkSize);

    if(rawChunk != NULL)
    {
        if(!readRawChunkData(file, rawChunk))
        {
            IFF_freeChunk((IFF_Chunk*)rawChunk, 0, NULL, 0);
            return NULL;
        }
    }

    /* Return the resulting raw chunk */
    return rawChunk;
}

IFF_Bool IFF_writeRawChunk(FILE *file, const IFF_RawChunk *rawChunk)
{
    if(fwrite(rawChunk->chunkData, sizeof(IFF_UByte), rawChunk->chunkSize, file) < rawChunk->chunkSize)
    {
        IFF_error("Error writing raw chunk body of chunk '");
        IFF_errorId(rawChunk->chunkId);
        IFF_error("'\n");
        return FALSE;
    }
    else
        return TRUE;
}

IFF_Bool IFF_writeRawChunkData(FILE *file, const IFF_RawChunk *rawChunk, IFF_Long *bytesProcessed)
{
    IFF_Bool status = IFF_writeRawChunk(file, rawChunk);
    *bytesProcessed = *bytesProcessed + rawChunk->chunkSize;
    return status;
}

void IFF_freeRawChunk(IFF_RawChunk *rawChunk)
{
    free(rawChunk->chunkData);
}

void IFF_printText(const IFF_RawChunk *rawChunk, const unsigned int indentLevel)
{
    unsigned int i;

    IFF_printIndent(stdout, indentLevel, "text = '\n");
    IFF_printIndent(stdout, indentLevel + 1, "");

    for(i = 0; i < rawChunk->chunkSize; i++)
        printf("%c", rawChunk->chunkData[i]);

    printf("\n");
    IFF_printIndent(stdout, indentLevel, "';\n");
}

void IFF_printRaw(const IFF_RawChunk *rawChunk, const unsigned int indentLevel)
{
    unsigned int i;
    IFF_UByte byte;

    IFF_printIndent(stdout, indentLevel, "bytes = \n");
    IFF_printIndent(stdout, indentLevel + 1, "");

    for(i = 0; i < rawChunk->chunkSize; i++)
    {
        if(i > 0 && i % 10 == 0)
        {
            printf("\n");
            IFF_printIndent(stdout, indentLevel + 1, "");
        }

        byte = rawChunk->chunkData[i];

        /* Print extra 0 for small numbers */
        if(byte <= 0xf)
            printf("0");

        printf("%x ", byte);
    }

    printf("\n");
    IFF_printIndent(stdout, indentLevel, ";\n");
}

void IFF_printRawChunk(const IFF_RawChunk *rawChunk, unsigned int indentLevel)
{
    if(rawChunk->chunkId == IFF_ID_TEXT)
        IFF_printText(rawChunk, indentLevel);
    else
        IFF_printRaw(rawChunk, indentLevel);
}

IFF_Bool IFF_compareRawChunk(const IFF_RawChunk *rawChunk1, const IFF_RawChunk *rawChunk2)
{
    return memcmp(rawChunk1->chunkData, rawChunk2->chunkData, rawChunk1->chunkSize) == 0;
}
