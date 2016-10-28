#pragma once

#include "../def.h"

ACQ_NAMESPACE_START

/**
 *  This hash algorithm is based on work by Peter J. Weinberger of AT&_CT Bell Labs. The book Compilers (Principles, Techniques
 *  and Tools) by Aho, Sethi and Ulman, recommends the use of hash functions that employ the hashing methodology found in this
 *  particular algorithm.
 */ 
template<class _CT>
uint32 PJW_hash(const _CT* p_c)
{
    uint32 BitsInUnignedInt = (uint32)(sizeof(uint32) * 8);
    uint32 ThreeQuarters = (uint32)((BitsInUnignedInt  * 3) / 4);
    uint32 OneEighth = (uint32)(BitsInUnignedInt / 8);
    uint32 HighBits = (uint32)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
    uint32 hash = 0;
    uint32 test = 0;
    while (*p_c)
    {
        hash = (hash << OneEighth) + (*p_c++);
        if ((test = hash & HighBits) != 0)
        {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }
 
    return (hash & 0x7FFFFFFF);
}

/**
 *  A simple hash function from Robert Sedgwicks Algorithms in C book.
 */
template<class _CT>
uint32 JS_hash(const _CT* p_c)
{
    uint32 hash = 1315423911;
    while (*p_c)
    {
        hash ^= ((hash << 5) + (*p_c++) + (hash >> 2));
    }
    return (hash & 0x7FFFFFFF);
}

/**
 *  本算法由于在Brian Kernighan与Dennis Ritchie的《The C Programming Language》一书被展示而得名，是一种简单快捷的hash算法，
 *  也是Java目前采用的字符串的Hash算法
 */
template<class _CT>
uint32 BKDR_hash(const _CT* p_c)
{
	register uint32 hash = 0;
    uint32 seed = 131; // 31 131 1313 13131 131313 etc..
    while (*p_c)
    {
        hash = hash * seed + (*p_c++);
    } 
    return (hash & 0x7FFFFFFF);
}

/**
 * Famous hash algorithm in Unix system, also used by Microsoft in their hash_map implementation for VC++ 2005 
 * detail can be found in :http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
 */
template<class _CT>
uint32 FNV_hash(const _CT* p_c)
{
    uint32 hash = 2166136261; //offset_basis
    uint32 prime = 16777619;  //FNV_PRIME_32
    while(*p_c)
    {
        hash *= prime;
        hash ^= *p_c++;
    }
    return (hash & 0x7FFFFFFF);
}

template<class _CT>
uint32 FNV_hash2(const _CT* p_c)
{
    uint32 hash = 2166136261; //offset_basis
    uint32 prime = 16777619;  //FNV_PRIME_32
    while(*p_c != '\0')
    {
        hash ^= *p_c++;
        hash *= prime;
   }
   return (hash & 0x7FFFFFFF);
}

/**
 *  An algorithm produced by Professor Daniel J. Bernstein and shown first to the world on the 
 *  usenet newsgroup comp.lang.c. It is one of the most efficient hash functions ever published.
 */
template<class _CT>
uint32 DJB_hash(const _CT* p_c)
{
    uint32 hash = 5381; 
    while (*p_c)
    {
        hash += (hash << 5) + (*p_c++);
    }
    return (hash & 0x7FFFFFFF);
}

template<class _CT>
uint32 DJB_hash2(const _CT* s)
{
    uint32 hashvalue = 5381;
    while(*s!='\0')
    {
        hashvalue = hashvalue * 33 ^ (*s);
        s++;
    }
    return (hashvalue & 0x7FFFFFFF);
}

/**
 *  This is the algorithm of choice which is used in the open source SDBM project. 
 *  The hash function seems to have a good overall distribution for many different data 
 *  sets. It seems to work well in situations where there is a high variance in the MSBs of the
 *  elements in a data set.
 */
template<class _CT>
uint32 SDBM_hash(const _CT* p_c)
{
    uint32 hash = 0;
    while (*p_c)
    {
        // equivalent to: hash = 65599*hash + (*p_c++);
        hash = (*p_c++) + (hash << 6) + (hash << 16) - hash;
    } 
    return (hash & 0x7FFFFFFF);
}

/**
 *  An algorithm produced by me Arash Partow.
 */
template<class _CT>
uint32 AP_hash(const _CT* p_c)
{
    uint32 hash = 0;
    for (int i=0; *p_c; i++)
    {
        if ((i & 1) == 0)
        {
            hash ^= ((hash << 7) ^ (*p_c++) ^ (hash >> 3));
        }
        else
        {
            hash ^= (~((hash << 11) ^ (*p_c++) ^ (hash >> 5)));
        }
    } 
    return (hash & 0x7FFFFFFF);
}

ACQ_NAMESPACE_END