/*
 * CompoundHashValue.h
 *
 * Copyright (C) 2017 OpenCog Foundation
 *
 * Author: Andre Senna <https://github.com/andre-senna>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _OPENCOG_COMPOUNDHASHVALUE_H
#define _OPENCOG_COMPOUNDHASHVALUE_H

#include "utils.h"
#include <string>

class CompoundHashValueUTest; // to declare as "friend" class

namespace opencog
{

/**
 *
 */
class CompoundHashValue 
{
    friend class ::CompoundHashValueUTest; // Unit tests need to access private members

public:

    static const KBB_HASHCODE SEED;
    static const KBB_HASHCODE MULTIPLIER;

    CompoundHashValue();
    ~CompoundHashValue();

    void reset();
    void feed(unsigned long int n);
    void feed(const std::string &s);
    KBB_HASHCODE get();

private:

    static const KBB_HASHCODE INVALID;
    KBB_HASHCODE hashCode;

    // This method is private because it's unsafe as it assumes
    // that s is properly terminated with '\0' (no check is done).
    // So it's used only in this package by "friend" classes.
    void feed(char *s);
};

}

#endif // _OPENCOG_COMPOUNDHASHVALUE_H
