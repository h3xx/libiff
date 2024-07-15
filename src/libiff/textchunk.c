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

#include "textchunk.h"
#include <string.h>
#include <stdlib.h>
#include "rawchunk.h"

IFF_ChunkInterface IFF_textChunkInterface = {&IFF_createRawChunk, &IFF_readRawChunkContents, &IFF_writeRawChunkContents, &IFF_checkRawChunkContents, &IFF_clearRawChunkContents, &IFF_printTextChunkContents, &IFF_compareRawChunkContents, NULL, NULL};

void IFF_setTextData(IFF_TextChunk *textChunk, const char *text)
{
    size_t textLength = strlen(text);
    IFF_UByte *chunkData = (IFF_UByte*)malloc(textLength * sizeof(IFF_UByte));

    memcpy(chunkData, text, textLength);
    IFF_setRawChunkData((IFF_RawChunk*)textChunk, chunkData, textLength);
}

static void printChunkDataText(FILE *file, const void *value, const unsigned int indentLevel)
{
    const IFF_TextChunk *textChunk = (const IFF_TextChunk*)value;
    IFF_Long i;

    fputc('"', file);

    for(i = 0; i < textChunk->chunkSize; i++)
    {
        char character = textChunk->chunkData[i];

        if(character == '"')
            fputs("\\\"", file);
        else
            fputc(character, file);
    }

    fputc('"', file);
}

void IFF_printTextChunkContents(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_printField(file, indentLevel, "chunkData", (const IFF_RawChunk*)chunk, printChunkDataText);
}
