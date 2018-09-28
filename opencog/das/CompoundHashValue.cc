#include "CompoundHashValue.h"
#include <stdexcept>

using namespace opencog;
using namespace std;

// Public static values
const KBB_HASHCODE CompoundHashValue::SEED = 47; // any prime number should work
const KBB_HASHCODE CompoundHashValue::MULTIPLIER = 13; // any prime number should work

// hash code == 0 means that no value have been feeded to compute it
const KBB_HASHCODE CompoundHashValue::INVALID = 0;

CompoundHashValue::CompoundHashValue() 
{
    reset();
}

CompoundHashValue::~CompoundHashValue() 
{
}

void CompoundHashValue::reset()
{
    hashCode = INVALID;
}

void CompoundHashValue::feed(unsigned long int n)
{
    if (hashCode == INVALID) hashCode = SEED;
    hashCode = hashCode * MULTIPLIER + n;
    if (hashCode == INVALID) throw runtime_error("Invalid compound hash code");
}

void CompoundHashValue::feed(const std::string &s)
{
    for (const char& c : s) {
        feed((unsigned long int) c);
    }
}

void CompoundHashValue::feed(char *s)
{
    unsigned int i = 0;
    // Unsafe. That's why this method is private (used by 'friend' classes)
    while (s[i] != '\0') {
        feed((unsigned long int) s[i]);
        i++;
    }
}

KBB_HASHCODE CompoundHashValue::get()
{
    return hashCode;
}
