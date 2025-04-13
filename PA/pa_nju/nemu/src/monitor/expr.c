#include "nemu.h"
#include "cpu/reg.h"
#include "cpu/cpu.h"
#include "memory/memory.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

uint32_t look_up_symtab(char *sym, bool *success);

enum {
    NOTYPE = 256,
    EQ = 257,
    NEQ = 258,
    NUM = 259,
    REG = 260,
    SYMB = 261,
    H = 262,
    L = 263,
    R = 264,
    EORL = 265,
    EORG = 266,
    AND = 267,
    OR = 268,
    NEG = 269,
    P = 270
};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {
    {" +", NOTYPE}, // white space
    {"\\(", '('},
    {"\\)", ')'},
    {"\\+", '+'},
    {"\\-", '-'},
    {"\\*", '*'},
    {"/", '/'},
    {"%", '%'},
    {"0[xX][0-9a-zA-Z]+", H},
    {"[0-9]+", NUM},
    {"==", EQ},
    {"!=", NEQ},
    {"\\$e[a,d,c,b]x", REG},
    {"\\$e[s,b,i]p", REG},
    {"\\$e[d,s]i", REG},
    {"[a-zA-Z]+[a-zA-Z0-9_]*", SYMB},
    {"<<", L},
    {">>", R},
    {"<=", EORL},
    {">=", EORL},
    {"<", '<'},
    {">", '>'},
    {"&&", AND},
    {"\\|\\|", OR},
    {"\\|",'|'},
    {"&",'&'},
    {"\\^",'^'},
    {"!",'!'},
    {"~",'~'},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0)
		{
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char* e) {
    int position = 0;
    int i;
    regmatch_t pmatch;

    nr_token = 0;

    while (e[position] != '\0')
    {
        /* Try all rules one by one. */
        for (i = 0; i < NR_REGEX; i++)
        {
            if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
            {
                char* substr_start = e + position;
                int substr_len = pmatch.rm_eo;
                
                position += substr_len;
                /* TODO: Now a new token is recognized with rules[i].
                 * Add codes to perform some actions with this token.
                 */
                switch (rules[i].token_type)
                {
                case NOTYPE:
                    break;
                case NUM:
                    tokens[nr_token].type = NUM;
                    strncpy(tokens[nr_token].str, substr_start, substr_len);
                    tokens[nr_token].str[substr_len] = '\0';
                    nr_token++;
                    break;
                case SYMB:
                    tokens[nr_token].type = SYMB;
                    strncpy(tokens[nr_token].str, substr_start, substr_len);
                    tokens[nr_token].str[substr_len] = '\0';
                    nr_token++;
                    break;
                case H:
                    tokens[nr_token].type = H;
                    strncpy(tokens[nr_token].str, substr_start, substr_len);
                    tokens[nr_token].str[substr_len] = '\0';
                    nr_token++;
                    break;
                case REG:
                    tokens[nr_token].type = REG;
                    strncpy(tokens[nr_token].str, substr_start, substr_len);
                    tokens[nr_token].str[substr_len] = '\0';
                    nr_token++;
                    break;
                default:
                    tokens[nr_token].type = rules[i].token_type;
                    nr_token++;
                }
                if(rules[i].token_type != NOTYPE)
                    printf("match regex[%d] at position %d with len %d: %.*s   type is %d\n", i, position, substr_len, substr_len, substr_start, tokens[nr_token - 1].type);
                break;
            }
        }
        if (i == NR_REGEX)
        {
            printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
            return false;
        }
    }
    return true;
}

bool check_parentheses(int p, int q, bool* success) {
    if (tokens[p].type != '(') {
        return false;
    }
    int count = 1;
    for (int i = p + 1; i < q; i++) {
        if (tokens[i].type == '(')
            count++;
        else if (tokens[i].type == ')')
            count--;
        if (count == 0)
            return false;
        if (count < 0) {
            *success = false;
            return false;
        }
    }
    if (tokens[q].type != ')') {
        return false;
    }
    count--;
    if (count != 0) {
        *success = false;
        printf("expr is not standard\n");
        return false;
    }
    return true;
}

int get_priority(int symble){
    switch(symble){
        case OR:
            return 11;
        case AND:
            return 10;
        case '|':
            return 9;
        case '^':
            return 8;
        case '&':
            return 7;
        case EQ: case NEQ:
            return 6;
        case '>': case '<': case EORG: case EORL:
            return 5;
        case L: case R:
            return 4;
        case '+': case '-':
            return 3;
        case '*': case '/': case '%':
            return 2;
        case '!': case '~': case NEG: case P:
            return 1;
    }
    return 0;
}

uint32_t eval(int p, int q, bool* success) {
    uint32_t ans = 0;
    if(p > q)
        return 0;
    else if (p == q) {
        if (tokens[p].type == H)
        {
            uint32_t result = 0;
            sscanf(tokens[p].str, "%x", &result);
            ans = result;
        }
        else if (tokens[p].type == NUM)
        {
            uint32_t result = 0;
            sscanf(tokens[p].str, "%d", &result);
            ans = result;
        }
        else if (tokens[p].type == REG)
        {
            if (tokens[p].str[2] == 'a' && tokens[p].str[3] == 'x')
                ans = cpu.eax;
            else if (tokens[p].str[2] == 'b' && tokens[p].str[3] == 'x')
                ans = cpu.ebx;
            else if (tokens[p].str[2] == 'c' && tokens[p].str[3] == 'x')
                ans = cpu.ecx;
            else if (tokens[p].str[2] == 'd' && tokens[p].str[3] == 'x')
                ans = cpu.edx;
            else if (tokens[p].str[2] == 's' && tokens[p].str[3] == 'i')
                ans = cpu.esi;
            else if (tokens[p].str[2] == 'd' && tokens[p].str[3] == 'i')
                ans = cpu.edi;
            else if (tokens[p].str[2] == 's' && tokens[p].str[3] == 'p')
                ans = cpu.esp;
            else if (tokens[p].str[2] == 'b' && tokens[p].str[3] == 'p')
                ans = cpu.ebp;
            else if (tokens[p].str[2] == 'i' && tokens[p].str[3] == 'p')
                ans = cpu.eip;
        }
        else
            ans = look_up_symtab(tokens[p].str, success);
    }
    else if (check_parentheses(p, q, success) == true) {
        if(!(*success))
            return 0;
        ans = eval(p + 1, q - 1, success);
    }
    else {
        int op = p - 1, pri = -1, count;
        for (count = p; count <= q; count++) {
            if (tokens[count].type == '(') {
                int flag = 1;
                while (count <= q && flag > 0) {
                    count++;
                    if (tokens[count].type == '(')
                        flag++;
                    else if (tokens[count].type == ')')
                        flag--;
                }
            }
            else if (tokens[count].type != NUM && tokens[count].type != REG && tokens[count].type != SYMB && tokens[count].type != H) {
                int pri_next = get_priority(tokens[count].type);
                if(pri_next==0){
                    *success=false;
                    printf("Can not find operator\n");
                    return 0;
                }
                if (pri_next >= pri) {
                    pri = pri_next;
                    op = count;
                }
            }
        }
        if (op == p - 1) {
            *success = false;
            printf("Expr is not standard\n");
            return 0;
        }
        if (op == p && tokens[op].type != NEG && tokens[op].type != P) {
             *success = false;
            printf("Expr is not standard\n");
            return 0;
        }
        uint32_t val1 = eval(p, op - 1, success), val2 = eval(op + 1, q, success);
        switch (tokens[op].type) {
        case '+':
            ans = val1 + val2;
            break;
        case '-':
            ans = val1 - val2;
            break;
        case '*':
            ans = val1 * val2;
            break;
        case '/':
            ans = val1 / val2;
            break;
        case '%':
            ans = val1 % val2;
            break;
        case L:
            ans = val1 << val2;
            break;
        case R:
            ans = val1 >> val2;
            break;
        case '>':
            ans = val1 > val2;
            break;
        case '<':
            ans = val1 < val2;
            break;
        case EORG:
            ans = val1 <= val2;
            break;
        case EORL:
            ans = val1 >= val2;
            break;
        case EQ:
            ans = val1 == val2;
            break;
        case NEQ:
            ans = val1 != val2;
            break;
        case '&':
            ans = val1 & val2;
            break;
        case '^':
            ans = val1 ^ val2;
            break;
        case '|':
            ans = val1 | val2;
            break;
        case AND:
            ans = val1 && val2;
            break;
        case OR:
            ans = val1 || val2;
            break;
        case '!':
            ans = !val2;
            break;
        case '~':
            ans = ~val2;
            break;
        case P:
            if(val2 < 0){
                printf("Expr is not standard");
                return 0;
            }
            uint32_t tmp0=hw_mem[val2],tmp1=hw_mem[val2+1],tmp2=hw_mem[val2+2],tmp3=hw_mem[val2+3];
            ans = tmp0 + (tmp1<<8) + (tmp2<<16) + (tmp3<<24);
            break;
        case NEG:
            ans = -val2;
            break;
        }
    }
    return ans;
}

uint32_t expr(char* e, bool* success) {
    if (!make_token(e)) {
        *success = false;
        printf("Can not match with rules\n");
        return 0;
    }
    printf("The num of nr_token is %d\n",nr_token);
    int i;
    for (i = 0; i < nr_token; i++)
    {
        if (tokens[i].type == '*' && (i == 0 || (tokens[i - 1].type != NUM && tokens[i - 1].type != REG && tokens[i - 1].type != SYMB && tokens[i - 1].type != H)))
            tokens[i].type = P;
        else if (tokens[i].type == '-' && (i == 0 || (tokens[i - 1].type != NUM && tokens[i - 1].type != REG && tokens[i - 1].type != SYMB && tokens[i - 1].type != H)))
            tokens[i].type = NEG;
    }
    int ans = eval(0, nr_token - 1, success);
    return ans;
}

