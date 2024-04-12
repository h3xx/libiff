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

#ifndef __IFFCORE_H
#define __IFFCORE_H

#include "ifftypes.h"
#include "chunk.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads an IFF file from a given file descriptor. The resulting chunk must be freed using IFF_free().
 *
 * @param file File descriptor of the file
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return A chunk hierarchy derived from the IFF file, or NULL if an error occurs
 */
IFF_Chunk *IFF_readFdCore(FILE *file, const IFF_ChunkRegistry *chunkRegistry, IFF_IOError **error);

/**
 * Reads an IFF file from a file with the given filename. The resulting chunk must be freed using IFF_free().
 *
 * @param filename Filename of the file
 * @param chunkRegistry A registry that determine s how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return A chunk hierarchy derived from the IFF file, or NULL if an error occurs
 */
IFF_Chunk *IFF_readFileCore(const char *filename, const IFF_ChunkRegistry *chunkRegistry, IFF_IOError **error);

/**
 * Reads an IFF file from a file with the given filename or from the standard input when no filename was provided.
 * The resulting chunk must be freed using IFF_free().
 *
 * @param filename Filename of the file or NULL to read from the standard input
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return A chunk hierarchy derived from the IFF file, or NULL if an error occurs
 */
IFF_Chunk *IFF_readCore(const char *filename, const IFF_ChunkRegistry *chunkRegistry, IFF_IOError **error);

/**
 * Writes an IFF file to a given file descriptor.
 *
 * @param file File descriptor of the file
 * @param chunk A chunk hierarchy representing an IFF file
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the file has been successfully written, else FALSE
 */
IFF_Bool IFF_writeFdCore(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_IOError **error);

/**
 * Writes an IFF file to a file with the given filename.
 *
 * @param filename Filename of the file
 * @param chunk A chunk hierarchy representing an IFF file
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the file has been successfully written, else FALSE
 */
IFF_Bool IFF_writeFileCore(const char *filename, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_IOError **error);

/**
 * Writes an IFF file to a file with the given filename or to the standard output if no filename was provided.
 *
 * @param filename Filename of the file or NULL to write to the standard output
 * @param chunk A chunk hierarchy representing an IFF file
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the file has been successfully written, else FALSE
 */
IFF_Bool IFF_writeCore(const char *filename, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_IOError **error);

/**
 * Frees an IFF chunk hierarchy from memory.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_freeCore(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

IFF_QualityLevel IFF_advancedCheckCore(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_printCheckMessageFunction printCheckMessage, void *data);

/**
 * Checks whether an IFF file conforms to the IFF specification.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the IFF file conforms to the IFF specification, else FALSE
 */
IFF_QualityLevel IFF_checkCore(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Prints a textual representation of an IFF file to the given file descriptor.
 *
 * @param file File descriptor of the file
 * @param chunk A chunk hierarchy representing an IFF file
 * @param indentLevel Indent level of the textual representation
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_printFdCore(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Prints a textual representation of an IFF file to a file with a given filename.
 *
 * @param filename Filename of the file or NULL to write to the standard output
 * @param chunk A chunk hierarchy representing an IFF file
 * @param indentLevel Indent level of the textual representation
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the file has been successfully written, else FALSE
 */
IFF_Bool IFF_printFileCore(const char *filename, const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Prints a textual representation of an IFF file to a file with a given filename or to the standard output if no filename was provided.
 *
 * @param filename Filename of the file or NULL to write to the standard output
 * @param chunk A chunk hierarchy representing an IFF file
 * @param indentLevel Indent level of the textual representation
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the file has been successfully written, else FALSE
 */
IFF_Bool IFF_printCore(const char *filename, const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Checks whether two given IFF files are equal.
 *
 * @param chunk1 Chunk hierarchy to compare
 * @param chunk2 Chunk hierarchy to compare
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the given chunk hierarchies are equal, else FALSE
 */
IFF_Bool IFF_compareCore(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry);

#ifdef __cplusplus
}
#endif

#endif
