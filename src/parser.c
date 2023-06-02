#include "parser.h"

#include "ctype.h"  //chatgpt: Which header file/function in C should i use if i want to determine if a character/string is alphanumeric/alphabetical
#include "err.h"
#include "lexer.h"
#include "list.h"
#include "propformula.h"
#include "string.h"
#include "util.h"
#include "variables.h"

/**
 * Assigns symbols to strings.
 *
 * Aborts the program with an error message if an invalid input is detected.
 *
 * @param str  a string to translate
 * @return     the resulting symbol
 */
FormulaKind toKind(const char* str) {
    size_t len = strlen(str);
    if (isalpha(str[0])) {
        for (size_t i = 0; i < len; i++) {
            if (!isalnum(str[i])) {
                err("Error bruv");
            }
        }
        return VAR;
    }
    if (strcmp(str, "!") == 0) {
        return NOT;
    } else if (strcmp(str, "&&") == 0) {
        return AND;
    } else if (strcmp(str, "||") == 0) {
        return OR;
    } else if (strcmp(str, "=>") == 0) {
        return IMPLIES;
    } else if (strcmp(str, "<=>") == 0) {
        return EQUIV;
    } else
        err("Error bruv");
    return VAR;  // not needed but oki
}

#include "list.h"

// Rest of the includes...

PropFormula* parseFormula(FILE* input, VarTable* vt) {
    List stack = mkList();
    char* token = nextToken(input);

    while (token != NULL) {
        FormulaKind kind = toKind(token);

        if (kind == VAR) {
            PropFormula* varFormula = mkVarFormula(vt, token);
            push(&stack, varFormula);
        } else if (kind == NOT) {
            if (isEmpty(&stack)) {
                err("Invalid formula: not enough operands for negation "
                    "operator");
            }

            PropFormula* operand = (PropFormula*)peek(&stack);
            pop(&stack);
            PropFormula* negationFormula = mkUnaryFormula(NOT, operand);
            push(&stack, negationFormula);
        } else {
            if (isEmpty(&stack)) {
                err("Invalid formula: not enough operands for binary operator");
            }

            PropFormula* rightOperand = (PropFormula*)peek(&stack);
            pop(&stack);
            if (isEmpty(&stack)) {
                err("Invalid formula: not enough operands for binary operator");
            }
            if (isEmpty(&stack)) {
                err("Invalid formula: not enough operands for binary operator");
            }
            PropFormula* leftOperand = (PropFormula*)peek(&stack);
            pop(&stack);
            PropFormula* binaryFormula =
                mkBinaryFormula(kind, leftOperand, rightOperand);
            push(&stack, binaryFormula);
        }
        token = nextToken(input);
    }

    if (isEmpty(&stack)) {
        err("Invalid formula: stack is empty here1");
    }

    PropFormula* formula = (PropFormula*)peek(&stack);
    pop(&stack);

    if (!isEmpty(&stack)) {
        err("Invalid formula: too many operands or operators here2");
    }

    clearList(&stack);
    return formula;
}
