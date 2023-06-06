#include "dpll.h"

#include "cnf.h"
#include "err.h"
#include "list.h"
#include "util.h"
#include "variables.h"

typedef enum Reason { CHOSEN, IMPLIED } Reason;

/**
 * Struct to represent an entry in the assignment stack. Should only be created
 * and freed by pushAssignment and popAssignment.
 */
typedef struct Assignment {
    VarIndex var;
    Reason reason;
} Assignment;

/**
 * Adds a new assignment to the assignment stack.
 *
 * @param stack  an assignment stack
 * @param var    the variable to assign
 * @param r      the reason for the assignment
 */
void pushAssignment(List* stack, VarIndex var, Reason r) {
    Assignment* a = (Assignment*)malloc(sizeof(Assignment));
    a->var = var;
    a->reason = r;
    push(stack, a);
}

/**
 * Removes the head element of an assignment stack and frees it.
 *
 * @param stack  an assignment stack
 */
void popAssignment(List* stack) {
    Assignment* a = (Assignment*)peek(stack);
    free(a);
    pop(stack);
}

/**
 * Führt eine Iteration des DPLL Algorithmus aus.
 *
 * @param vt       die zugrunde liegende Variablentabelle
 * @param stack    der Zuweisungsstack
 * @param cnf      die zu prüfende Formel
 * @return         1 falls der Algorithmus mit SAT terminieren sollte,
 *                 0 falls der Algorithmus weiterlaufen sollte,
 *                 -1 falls der Algorithmus mit UNSAT terminieren sollte
 */
/**
 * Performs one iteration of the DPLL algorithm.
 *
 * @param vt       the underlying variable table
 * @param stack    an assignment stack
 * @param cnf      the formula to check
 * @return         1 if the algorithm should terminate with SAT,
 *                 0 if the algorithm should continue,
 *                -1 if the algorithm should terminate with UNSAT
 */
int iterate(VarTable* vt, List* stack, CNF* cnf) {
    ListIterator iter = mkIterator(&cnf->clauses);
    int check = 0;
    while (isValid(&iter)) {
        Clause* santa = (Clause*)getCurr(&iter);
        int check = 0;  // used to check if a reset has occured or no
        if (evalCNF(cnf) == FALSE) {
            while (isEmpty(stack) == 0 && check == 0) {
                Assignment* stalker = (Assignment*)peek(stack);
                if (stalker->reason == IMPLIED) {
                    updateVariableValue(vt, stalker->var, UNDEFINED);
                    popAssignment(stack);
                }
                if (stalker->reason == CHOSEN) {
                    VarIndex tmp = stalker->var;
                    popAssignment(stack);
                    // stalker->reason = IMPLIED;
                    // TruthValue ppls = getVariableValue(vt, stalker->var);
                    // if (ppls == TRUE)
                    updateVariableValue(vt, tmp, FALSE);
                    // else
                    // updateVariableValue(vt, stalker->var, TRUE);
                    pushAssignment(stack, tmp, IMPLIED);
                    check++;
                    break;
                }
            }
            if (check == 0) return -1;
            if (check != 0) return 0;
        }
        Literal p = 0;
        p = getUnitLiteral(vt, santa);
        if (p != 0) {
            prettyPrintCNF(vt, cnf);
            if (p > 0) {
                updateVariableValue(vt, p, TRUE);
                pushAssignment(stack, p, IMPLIED);
                updateTruthValue(vt, santa);
            }
            if (p < 0) {
                updateVariableValue(vt, -p, FALSE);
                pushAssignment(stack, -p, IMPLIED);
                updateTruthValue(vt, santa);
            }
        }

        if (p == 0) {
            VarIndex q = getNextUndefinedVariable(vt);
            if (q == 0) {
                return -1;
            }
            updateVariableValue(vt, q, TRUE);
            pushAssignment(stack, q, CHOSEN);
        }
        next(&iter);
    }
    TruthValue p = (evalCNF(cnf));
    if (p == TRUE) return 1;
}
/*{
    while(1 == 1)
    {
     ListIterator iter = mkIterator(&cnf->clauses);
     TruthValue tv = evalCNF(cnf);
     if(tv == TRUE)
        return 1;
     if(tv == FALSE)
         if(isEmpty(stack)==0)

    }
}
*/
/*
 {
    // TODO Implement me!

    // int checker = 0;
    VarIndex chap;
    Assignment* rekt;
    ListIterator iter = mkIterator((&cnf->clauses));
    TruthValue chapistik;
    TruthValue pakka_na = evalCNF(cnf);
    VarIndex tumblr;
    while (isValid(&iter) && (pakka_na == FALSE || pakka_na == UNDEFINED)) {
        Clause* curr = (Clause*)getCurr(&iter);
        pakka_na = evalCNF(cnf);
        if (pakka_na == FALSE) {
            while (isEmpty(stack) == 0) {
                rekt = (Assignment*)peek(stack);
                if (rekt->reason == IMPLIED) {
                    chap = rekt->var;
                    updateVariableValue(vt, chap, UNDEFINED);
                    popAssignment(stack);
                } else if (rekt->reason == CHOSEN) {
                    rekt->reason = IMPLIED;
                    chap = rekt->var;
                    chapistik = getVariableValue(vt, rekt->var);
                    if (chapistik == TRUE)
                        updateVariableValue(vt, chap, FALSE);
                    else if (chapistik == FALSE)
                        updateVariableValue(vt, chap, TRUE);
                }
            }
        }

        Literal ez = getUnitLiteral(vt, curr);
        if (ez != 0) {
            if (ez > 0) {
                updateVariableValue(vt, ez, TRUE);
                pushAssignment(stack, ez, IMPLIED);
            }
            if (ez < 0) {
                updateVariableValue(vt, -ez, FALSE);
                pushAssignment(stack, -ez, IMPLIED);
            }
            updateTruthValue(vt, curr);
            return 0;
        } else {
            tumblr = getNextUndefinedVariable(vt);
            updateVariableValue(vt, tumblr, TRUE);
            pushAssignment(stack, tumblr, CHOSEN);
        }
        pakka_na = evalCNF(cnf);
        next(&iter);
    }
    pakka_na = evalCNF(cnf);
    if (pakka_na == TRUE) return 1;
    if (pakka_na == FALSE || pakka_na == UNDEFINED)
        return -1;
    else
        return 0;  // annoyed at shitty wavy yellow line i hope i die
}
*/
char isSatisfiable(VarTable* vt, CNF* cnf) {
    List stack = mkList();

    int res;
    do {
        res = iterate(vt, &stack, cnf);
    } while (res == 0);

    while (!isEmpty(&stack)) {
        popAssignment(&stack);
    }

    return (res < 0) ? 0 : 1;
}
