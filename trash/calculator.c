#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "../libft/libft.h"

#define O_BRACKET	-1
#define C_BRACKET	-2
#define MULT		-3
#define DIV			-4
#define MOD			-5
#define ADD			-6
#define SUB			-7
#define ISEQUL		-8
#define NOTEQUL		-9
#define GRT			-10
#define GRTEQUL		-11
#define SML			-12
#define SMLEQUL		-13
#define OR			-14
#define AND			-15
#define EQUL		-16
#define PLUSPLUS	-17
#define MINUSMINUS	-18
#define NUM			-41
#define WORD		-42


typedef	long long int t_int;

typedef	struct		s_tok
{
	int				typ;
	char			*word;
	t_int			n;
	struct s_tok	*right;
	struct s_tok	*left;
}					t_tok;

t_tok	*g_tok_list = NULL;

char	*tok_to_a(int op)
{
	if (op == O_BRACKET)
		return("(");
	if (op == C_BRACKET)
		return(")");
	if (op == MULT)
		return ("*");
	if (op == DIV)
		return ("/");
	if (op == MOD)
		return ("\%");
	if (op == PLUSPLUS)
		return ("++");
	if (op == ADD)
		return ("+");
	if (op == MINUSMINUS)
		return ("--");
	if (op == SUB)
		return ("-");
	if (op == ISEQUL)
		return ("==");
	if (op == NOTEQUL)
		return ("!=");
	if (op == GRTEQUL)
		return (">=");
	if (op == GRT)
		return (">");
	if (op == SMLEQUL)
		return ("<=");
	if (op == SML)
		return ("<");
	if (op == OR)
		return ("||");
	if (op == AND)
		return ("&&");
	if (op == EQUL)
		return ("=");
	return ("???");
}

int		is_op(char *s)
{
	if (*s == '(')
		return(O_BRACKET);
	if (*s == ')')
		return(C_BRACKET);
	if (*s == '*')
		return (MULT);
	if (*s == '/')
		return (DIV);
	if (*s == '%')
		return (MOD);
	if (*s == '+' && *(s + 1) == '+' && )
		return (PLUSPLUS);
	if (*s == '+')
		return (ADD);
	if (*s == '-' && *(s + 1) == '-')
		return (MINUSMINUS);
	if (*s == '-')
		return (SUB);
	if (*s == '=' && *(s + 1) == '=')
		return (ISEQUL);
	if (*s == '!' && *(s + 1) == '=')
		return (NOTEQUL);
	if (*s == '>' && *(s + 1) == '=')
		return (GRTEQUL);
	if (*s == '>')
		return (GRT);
	if (*s == '<' && *(s + 1) == '=')
		return (SMLEQUL);
	if (*s == '<')
		return (SML);
	if (*s == '|' && *(s + 1) == '|')
		return (OR);
	if (*s == '&' && *(s + 1) == '&')
		return (AND);
	if (*s == '=')
		return (EQUL);
	if (ft_isdigit(*s))
		return (NUM);
	return (WORD);
}

char	*push_word(char *s, int typ)
{
	int		i;
	char	*word;
	t_tok	*node;

	i = 0;

	if (typ == NUM)
		while (s[i] && ft_isdigit(s[i]))
			i++;
	else
		while (s[i] && ft_isalnum(s[i]))
			i++;
	if (i == 0)
	{
		printf("error: syntax undefined '%s'\n", s);
		return ("");
	}
	word = ft_strsub(s, 0, i);
	if (g_tok_list == NULL)
	{
		g_tok_list = (t_tok *)malloc(sizeof(t_tok));
		g_tok_list->left = NULL;
		node = g_tok_list;
	}
	else
	{
		node = g_tok_list;
		while (node->right)
			node = node->right;
		node->right = (t_tok *)malloc(sizeof(t_tok));
		node->right->left = node;
		node = node->right;
	}
	node->right = NULL;
	node->typ = typ;
	node->word = word;
	return (s + i);
}

void	push_operator(int op)
{
	t_tok	*node;

	if (g_tok_list == NULL)
	{
		g_tok_list = (t_tok *)malloc(sizeof(t_tok));
		g_tok_list->left = NULL;
		node = g_tok_list;
	}
	else
	{
		node = g_tok_list;
		while (node->right)
			node = node->right;
		node->right = (t_tok *)malloc(sizeof(t_tok));
		node->right->left = node;
		node = node->right;
	}
	node->right = NULL;
	node->typ = op;
	node->word = NULL;
}

void	parse_tok(char *s)
{
	int		op;

	op = 0;
	while (*s)
	{
		while (*s && *s == ' ')
			s++;
		if (*s == '\0')
			break;
		if ((op = is_op(s)) == WORD || op == NUM)
		{
			s = push_word(s, op);
			continue ;
		}
		else
			push_operator(op);
		if (op == -17 || op == -18 || op == -8 || op == -9 || op == -11 ||
											op == -13 || op == -14 || op == -15)
			s++;
		s++;
	}
}

void	print_tok(t_tok *list)
{
	t_tok	*node;

	node = list;
	while (node)
	{
		if (node->typ == WORD || node->typ == NUM)
			printf(" %s", node->word);
		else
			printf(" %s", tok_to_a(node->typ));
		node = node->right;
	}
	printf("\n");
}

void	print_tok_inverse(t_tok *list)
{
	t_tok	*node;

	node = list;
	while (node->right)
		node = node->right;
	while (node)
	{
		if (node->typ == WORD || node->typ == NUM)
			printf(" %s", node->word);
		else
			printf(" %s", tok_to_a(node->typ));
		node = node->left;
	}
	printf("\n");
}

t_int	calc(char *s)
{
	parse_tok(s);
	if (g_tok_list == NULL)
		return (0);
	print_tok(g_tok_list);
	// print_tok_inverse(g_tok_list);
	return (0);
}

int		main(int ac, char **av)
{
	int		i;

	if (ac < 2)
		printf("Usage: ./calc \"5 + 2 - 8 / 4 * 2\"\n");
	i = 1;
	while (av[i])
	{
		printf("%s ===> [%lld]\n", av[i], calc(av[i]));
		i++;
	}
	return (0);
}
