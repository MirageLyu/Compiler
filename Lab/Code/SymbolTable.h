#pragma once

#include "headers.h"
#include "Type.h"
#include "Function.h"

/* * *
 * AN ALTERNATIVE TO HASHING: Multiset Discrimination
 * 
 * Hashing is the method most widely used to organized a 
 * compiler's symbol table. Multiset discrimination is an
 * interesting alternative that eliminates any possible
 * of worst-case behavior. The critical insight behind multiset
 * discrimination is that the index can be constructed offline
 * in the scanner.
 * 
 * To use multiset discrimination, the compiler writer must take
 * a different approach to scanning. Instead of processing the
 * input incrementally, the compiler scans the entire program to 
 * find the complete set of identifiers. As it discovers each
 * identifier, it creates a tuple <name, position>, where name is
 * the text of the identifier and position is its ordinal position
 * in the list of classified words, or tokens. It enters all the 
 * tuples into a large set.
 * 
 * The next step sorts the set lexicographically. In effect, this 
 * creates a set of subsets, one per identifier. Each of these
 * subsets holds the tuples for all the occurence of its
 * identifier. Since each tuple refers to a specific token,
 * through its position value, the compiler can use the sorted set
 * to modify the token stream. The compiler makes a linear scan
 * over the set, processing each subset. It allocates a
 * symbol-table index for the entire subset, then rewrites the 
 * tokens to include that index. This augments the identifier
 * tokens with their symbol-table indices. If the compiler needs
 * a textual lookup function, the resulting table is ordered 
 * alphabetically for a binary search.
 * 
* * */
//all are pointers

class Symbol{
private:
    string name;
    Type type;
    int lineno;
public:
    Symbol() = default;
    
    string getName() const { return name; }
    Type getType() const { return type; }
    int getlineno() const { return lineno; }
};

class SymbolTable{
private:
    map<string, Symbol> symbols;
    map<string, Function> functions;
public:
    
};