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
#include "io.h"
#include "id.h"
#include "util.h"

IFF_Chunk *IFF_createRawChunk(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    IFF_RawChunk *rawChunk = (IFF_RawChunk*)IFF_createChunk(chunkId, chunkSize, sizeof(IFF_RawChunk));

    if(rawChunk != NULL)
    {
        rawChunk->chunkData = (IFF_UByte*)malloc(chunkSize * sizeof(IFF_UByte));

        if(rawChunk->chunkData == NULL)
        {
            free(rawChunk);
            return NULL;
        }
    }

    return (IFF_Chunk*)rawChunk;
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

IFF_Bool IFF_readRawChunk(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    IFF_RawChunk *rawChunk = (IFF_RawChunk*)chunk;

    if(fread(rawChunk->chunkData, sizeof(IFF_UByte), rawChunk->chunkSize, file) < rawChunk->chunkSize)
    {
        *error = IFF_createDataIOError(file, rawChunk->chunkSize, attributePath, "chunkData", "raw data", rawChunk->chunkId);
        return FALSE;
    }
    else
    {
        *bytesProcessed = *bytesProcessed + rawChunk->chunkSize;
        return TRUE;
    }
}

IFF_Bool IFF_writeRawChunk(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    const IFF_RawChunk *rawChunk = (const IFF_RawChunk*)chunk;

    if(fwrite(rawChunk->chunkData, sizeof(IFF_UByte), rawChunk->chunkSize, file) < rawChunk->chunkSize)
    {
        *error = IFF_createDataIOError(file, rawChunk->chunkSize, attributePath, "chunkData", "raw data", rawChunk->chunkId);
        return FALSE;
    }
    else
    {
        *bytesProcessed = *bytesProcessed + rawChunk->chunkSize;
        return TRUE;
    }
}

IFF_QualityLevel IFF_checkRawChunk(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    return IFF_QUALITY_PERFECT;
}

void IFF_freeRawChunk(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_RawChunk *rawChunk = (IFF_RawChunk*)chunk;
    free(rawChunk->chunkData);
}

void IFF_printText(FILE *file, const IFF_RawChunk *chunk, const unsigned int indentLevel)
{
    const IFF_RawChunk *rawChunk = (const IFF_RawChunk*)chunk;

    IFF_Long i;

    IFF_printIndent(file, indentLevel, ".chunkData = \"");

    for(i = 0; i < rawChunk->chunkSize; i++)
        fputc(rawChunk->chunkData[i], file);

    fprintf(file, "\",\n");
}

void IFF_printRaw(FILE *file, const IFF_RawChunk *rawChunk, const unsigned int indentLevel)
{
    IFF_Long i;
    IFF_UByte byte;

    IFF_printIndent(file, indentLevel, ".chunkData = {\n");
    IFF_printIndent(file, indentLevel + 1, "");

    for(i = 0; i < rawChunk->chunkSize; i++)
    {
        if(i > 0)
            fputs(", ", file);

        if(i > 0 && i % 10 == 0)
        {
            fprintf(file, "\n");
            IFF_printIndent(file, indentLevel + 1, "");
        }

        byte = rawChunk->chunkData[i];

        fputs("0x", file);

        /* Print extra 0 for small numbers */

        if(byte <= 0xf)
            fputs("0", file);

        fprintf(file, "%x", byte);
    }

    fprintf(file, "\n");
    IFF_printIndent(file, indentLevel, "},\n");
}

void IFF_printRawChunk(FILE *file, const IFF_Chunk *chunk, unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    const IFF_RawChunk *rawChunk = (IFF_RawChunk*)chunk;

    if(rawChunk->chunkId == IFF_ID_TEXT)
        IFF_printText(file, rawChunk, indentLevel);
    else
        IFF_printRaw(file, rawChunk, indentLevel);
}

IFF_Bool IFF_compareRawChunk(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    const IFF_RawChunk *rawChunk1 = (const IFF_RawChunk*)chunk1;
    const IFF_RawChunk *rawChunk2 = (const IFF_RawChunk*)chunk2;

    return memcmp(rawChunk1->chunkData, rawChunk2->chunkData, rawChunk1->chunkSize) == 0;
}
