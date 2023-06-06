#include "tseitin.h"

#include <stdio.h>
#include <string.h>

#include "err.h"
#include "propformula.h"
#include "util.h"

/**
 * Inserts a clause with one literal into the CNF.
 *
 * @param vt   the underlying variable table
 * @param cnf  a formula
 * @param a    a literal
 */
void addUnaryClause(VarTable* vt, CNF* cnf, Literal a) {
    Clause* clause = mkTernaryClause(vt, a, 0, 0);
    addClauseToCNF(cnf, clause);
}

/**
 * Inserts a clause with two literals into the CNF.
 *
 * @param vt   the underlying variable table
 * @param cnf  a formula
 * @param a    the first literal
 * @param b    the second literal
 */
void addBinaryClause(VarTable* vt, CNF* cnf, Literal a, Literal b) {
    Clause* clause = mkTernaryClause(vt, a, b, 0);
    addClauseToCNF(cnf, clause);
}

/**
 * Inserts a clause with three literals into the CNF.
 *
 * @param vt   the underlying variable table
 * @param cnf  a formula
 * @param a    the first literal
 * @param b    the second literal
 * @param c    the third literal
 */
void addTernaryClause(VarTable* vt, CNF* cnf, Literal a, Literal b, Literal c) {
    Clause* clause = mkTernaryClause(vt, a, b, c);
    addClauseToCNF(cnf, clause);
}

/**
 * Adds clauses for a propositional formula to a CNF.
 *
 * For a propositional formula pf, clauses that are added that are equivalent to
 *
 *     x <=> pf
 *
 * where x is usually a fresh variable. This variable is also returned.
 *
 * @param vt   the underlying variable table
 * @param cnf  a formula
 * @param pf   a propositional formula
 * @return     the variable x, as described above
 */
#include "cnf.h"
#include "variables.h"
//i used chatgpt and asked it to debug my errors. Also it may have generated some code, while debugging which seemed more effiecient/compacter than mine which i then procedeed to include in my code
VarIndex addClauses(VarTable* vt, CNF* cnf, const PropFormula* pf) {
    if (pf->kind == VAR) {
        return pf->data.var;
        // Check if a variable with the same name already exists in the variable
        // tabl
    }

    // If the variable doesn't exist, add it to the variable table

    else if (pf->kind == NOT) {
        Literal a = addClauses(vt, cnf, pf->data.single_op);
        Literal x1 = (Literal)mkFreshVariable(vt);
        addBinaryClause(vt, cnf, -x1, -a);
        addBinaryClause(vt, cnf, a, x1);
        return x1;
    } else if (pf->kind == AND) {
        Literal a = addClauses(vt, cnf, pf->data.operands[0]);
        Literal b = addClauses(vt, cnf, pf->data.operands[1]);
        Literal x1 = (Literal)mkFreshVariable(vt);
        addBinaryClause(vt, cnf, -x1, a);
        addBinaryClause(vt, cnf, -x1, b);
        addTernaryClause(vt, cnf, -a, -b, x1);
        return x1;
    } else if (pf->kind == OR) {
        /*
        VarIndex x = mkFreshVariable(vt);
        for (int i = 0; i < 2; i++) {
            VarIndex xi = addClauses(vt, cnf, pf->data.operands[i]);
            Literal a = -(Literal)xi;
            Literal b = (Literal)x;
            addBinaryClause(vt, cnf, a, b);
        }
        addUnaryClause(vt, cnf, x);
        return x;
        */
        Literal a = addClauses(vt, cnf, pf->data.operands[0]);
        Literal b = addClauses(vt, cnf, pf->data.operands[1]);
        Literal x1 = (Literal)mkFreshVariable(vt);
        addTernaryClause(vt, cnf, -x1, a, b);
        addBinaryClause(vt, cnf, -a, x1);
        addBinaryClause(vt, cnf, -b, x1);
        return x1;
    } else if (pf->kind == IMPLIES) {
        Literal a = addClauses(vt, cnf, pf->data.operands[0]);
        Literal b = addClauses(vt, cnf, pf->data.operands[1]);
        Literal x1 = (Literal)mkFreshVariable(vt);
        addTernaryClause(vt, cnf, -x1, -a, b);
        addBinaryClause(vt, cnf, a, x1);
        addBinaryClause(vt, cnf, -b, x1);
        return x1;
    } else if (pf->kind == EQUIV) {
        Literal a = addClauses(vt, cnf, pf->data.operands[0]);
        Literal b = addClauses(vt, cnf, pf->data.operands[1]);
        Literal x1 = (Literal)mkFreshVariable(vt);
        addTernaryClause(vt, cnf, -x1, -a, b);
        addTernaryClause(vt, cnf, -x1, a, -b);
        addTernaryClause(vt, cnf, x1, -a, -b);
        addTernaryClause(vt, cnf, x1, a, b);
        return x1;
    } else {
        { err("Error bruv. U reached the end of the road. TURN BACK"); }
    }
}

CNF* getCNF(VarTable* vt, const PropFormula* f) {
    CNF* res = mkCNF();
    VarIndex x = addClauses(vt, res, f);
    addUnaryClause(vt, res, x);
    prettyPrintCNF(vt, res);
    printVarTable(vt);
    return res;
}
