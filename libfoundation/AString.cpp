/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "foundation/ADebug.h"
#include "foundation/AString.h"
#include <utils/Mutex.h>

namespace Inspiry {

AString::AString()
    : mData(NULL),
      mSize(0),
      mAllocSize(0) {
}

AString::AString(const char *s)
    : mData(NULL),
      mSize(0),
      mAllocSize(0) {
    setTo(s);
}

AString::AString(const char *s, size_t size)
    : mData(NULL),
      mSize(0),
      mAllocSize(0) {
    setTo(s, size);
}

AString::AString(const AString &from)
    : mData(NULL),
      mSize(0),
      mAllocSize(0) {
    setTo(from, 0, from.size());
}

AString::AString(const AString &from, size_t offset, size_t n)
    : mData(NULL),
      mSize(0),
      mAllocSize(0) {
    setTo(from, offset, n);
}

AString::~AString() {
    clear();
}

AString &AString::operator=(const AString &from) {
    if (&from != this) {
        setTo(from, 0, from.size());
    }

    return *this;
}

size_t AString::size() const {
    return mSize;
}

const char *AString::c_str() const {
    return mData;
}

bool AString::empty() const {
    return mSize == 0;
}

void AString::setTo(const char *s) {
    setTo(s, strlen(s));
}

void AString::setTo(const char *s, size_t size) {
    clear();
    append(s, size);
}

void AString::setTo(const AString &from, size_t offset, size_t n) {
    CHECK(&from != this);

    clear();
    setTo(from.mData + offset, n);
}

void AString::clear() {
    if (mData != NULL) {
        free(mData);
        mData = NULL;
    }

    mData = NULL;
    mSize = 0;
    mAllocSize = 0;
}

size_t AString::hash() const {
    size_t x = 0;
    for (size_t i = 0; i < mSize; ++i) {
        x = (x * 31) + mData[i];
    }

    return x;
}

bool AString::operator==(const AString &other) const {
    return mSize == other.mSize && !memcmp(mData, other.mData, mSize);
}

void AString::trim() {
    CHECK(mSize > 0);
    CHECK(mData != NULL);
    size_t i = 0;
    while (i < mSize && isspace(mData[i])) {
        ++i;
    }

    size_t j = mSize;
    while (j > i && isspace(mData[j - 1])) {
        --j;
    }

    memmove(mData, &mData[i], j - i);
    mSize = j - i;
    mData[mSize] = '\0';
}

void AString::erase(size_t start, size_t n) {
    CHECK(mData != NULL);
    CHECK_LT(start, mSize);
    CHECK_LE(start + n, mSize);

    memmove(&mData[start], &mData[start + n], mSize - start - n);
    mSize -= n;
    mData[mSize] = '\0';
}

void AString::append(const char *s) {
    append(s, strlen(s));
}

void AString::append(const char *s, size_t size) {
    if (mSize + size + 1 > mAllocSize) {
        mAllocSize = (mAllocSize + size + 1 + 31) & -32;
        mData = (char *)realloc(mData, mAllocSize);
        CHECK(mData != NULL);
    }

    if (mData != NULL) {
        memcpy(&mData[mSize], s, size);
        mSize += size;
        mData[mSize] = '\0';
    }
}

void AString::append(const AString &from) {
    append(from.c_str(), from.size());
}

void AString::append(const AString &from, size_t offset, size_t n) {
    append(from.c_str() + offset, n);
}

void AString::append(int x) {
    char s[16];
    memset(s, 0, sizeof(s));
    snprintf(s, sizeof(s), "%d", x);

    append(s);
}

void AString::append(unsigned x) {
    char s[16];
    memset(s, 0, sizeof(s));
    snprintf(s, sizeof(s), "%u", x);

    append(s);
}

void AString::append(long x) {
    char s[16];
    memset(s, 0, sizeof(s));
    snprintf(s, sizeof(s), "%ld", x);

    append(s);
}

void AString::append(unsigned long x) {
    char s[16];
    memset(s, 0, sizeof(s));
    snprintf(s, sizeof(s), "%lu", x);

    append(s);
}

void AString::append(long long x) {
    char s[32];
    memset(s, 0, sizeof(s));
    snprintf(s, sizeof(s), "%lld", x);

    append(s);
}

void AString::append(unsigned long long x) {
    char s[32];
    memset(s, 0, sizeof(s));
    snprintf(s, sizeof(s), "%llu", x);

    append(s);
}

void AString::append(float x) {
    char s[16];
    memset(s, 0, sizeof(s));
    snprintf(s, sizeof(s), "%f", x);

    append(s);
}

void AString::append(double x) {
    char s[16];
    memset(s, 0, sizeof(s));
    snprintf(s, sizeof(s), "%f", x);

    append(s);
}

void AString::append(void *x) {
    char s[16];
    memset(s, 0, sizeof(s));
    snprintf(s, sizeof(s), "%p", x);

    append(s);
}

ssize_t AString::find(const char *substring, size_t start) const {
    CHECK_LE(start, size());

    const char *match = strstr(mData + start, substring);

    if (match == NULL) {
        return -1;
    }

    return match - mData;
}

void AString::insert(const AString &from, size_t insertionPos) {
    insert(from.c_str(), from.size(), insertionPos);
}

void AString::insert(const char *from, size_t size, size_t insertionPos) {
    CHECK_GE(insertionPos, 0u);
    CHECK_LE(insertionPos, mSize);

    if (mSize + size + 1 > mAllocSize) {
        mAllocSize = (mAllocSize + size + 1 + 31) & -32;
        mData = (char *)realloc(mData, mAllocSize);
        CHECK(mData != NULL);
    }

    CHECK(mData != NULL);
    memmove(&mData[insertionPos + size],
            &mData[insertionPos], mSize - insertionPos + 1);

    memcpy(&mData[insertionPos], from, size);

    mSize += size;
}

bool AString::operator==(const char* s) const
{
    return ((mSize == strlen(s)) && !memcmp(mData, s, mSize));
}

bool AString::operator<(const AString &other) const {
    return compare(other) < 0;
}

bool AString::operator>(const AString &other) const {
    return compare(other) > 0;
}

int AString::compare(const AString &other) const {
    return strcmp(mData, other.mData);
}

void AString::tolower() {
    for (size_t i = 0; i < mSize; ++i) {
        mData[i] = ::tolower(mData[i]);
    }
}

bool AString::startsWith(const char *prefix) const {
    return !strncmp(mData, prefix, strlen(prefix));
}

bool AString::endsWith(const char *suffix) const {
    size_t suffixLen = strlen(suffix);

    if (mSize < suffixLen) {
        return false;
    }

    return !strcmp(mData + mSize - suffixLen, suffix);
}

AString StringPrintf(const char *format, ...) {
    va_list ap;
    va_start(ap, format);

    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    vsnprintf(buffer, 255, format, ap);

    va_end(ap);

    AString result(buffer);

    return result;
}

}  // namespace android

