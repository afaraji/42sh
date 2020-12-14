/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 23:23:11 by afaraji           #+#    #+#             */
/*   Updated: 2020/12/03 20:21:35 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H
# include "ft_21sh.h"

# define STDIN	0
# define STDOUT	1
# define STDERR	2
# define SETDFL	1
# define GETDFL	0
# define SPACE	-1
# define QUOTE	-2
# define DQUOTE	-3
# define SMCLN	-4
# define ANDLG	-5
# define ORLG	-6
# define PIP	-10
# define BGJOB	-11
# define ESCAPE	-12
# define GRT	-20
# define GRTGRT	-21
# define SML	-22
# define SMLSML	-30
# define SMLAND	-24
# define GRTAND	-25
# define WORD	-42
# define BUILTINS	"echo|export|cd|set|setenv|unset|unsetenv|env|exit|alias|"
# define BUILTINS2	"unalias|jobs|fg|bg|test|hash|fc|type"
# define RSRVDWORD	"case|do|done|elif|else|esac|fi|for|if|in|then|until|while"
# ifdef STR
# define P[0] "Undefined error: 0"
# define P[1] "Operation not permitted"
# define P[2] "No such file or directory"
# define P[3] "No such process"
# define P[4] "Interrupted system call"
# define P[5] "Input/output error"
# define P[6] "Device not configured"
# define P[7] "Argument list too long"
# define P[8] "Exec format error"
# define P[9] "Bad file descriptor"
# define P[10] "No child processes"
# define P[11] "Resource deadlock avoided"
# define P[12] "Cannot allocate memory"
# define P[13] "Permission denied"
# define P[14] "Bad address"
# define P[15] "Block device required"
# define P[16] "Resource busy"
# define P[17] "File exists"
# define P[18] "Cross-device link"
# define P[19] "Operation not supported by device"
# define P[20] "Not a directory"
# define P[21] "Is a directory"
# define P[22] "Invalid argument"
# define P[23] "Too many open files in system"
# define P[24] "Too many open files"
# define P[25] "Inappropriate ioctl for device"
# define P[26] "Text file busy"
# define P[27] "File too large"
# define P[28] "No space left on device"
# define P[29] "Illegal seek"
# define P[30] "Read-only file system"
# define P[31] "Too many links"
# define P[32] "Broken pipe"
# define P[33] "Numerical argument out of domain"
# define P[34] "Result too large"
# define P[35] "Resource temporarily unavailable"
# define P[36] "Operation now in progress"
# define P[37] "Operation already in progress"
# define P[38] "Socket operation on non-socket"
# define P[39] "Destination address required"
# define P[40] "Message too long"
# define P[41] "Protocol wrong type for socket"
# define P[42] "Protocol not available"
# define P[43] "Protocol not supported"
# define P[44] "Socket type not supported"
# define P[45] "Operation not supported"
# define P[46] "Protocol family not supported"
# define P[47] "Address family not supported by protocol family"
# define P[48] "Address already in use"
# define P[49] "Can't assign requested address"
# define P[50] "Network is down"
# define P[51] "Network is unreachable"
# define P[52] "Network dropped connection on reset"
# define P[53] "Software caused connection abort"
# define P[54] "Connection reset by peer"
# define P[55] "No buffer space available"
# define P[56] "Socket is already connected"
# define P[57] "Socket is not connected"
# define P[58] "Can't send after socket shutdown"
# define P[59] "Too many references: can't splice"
# define P[60] "Operation timed out"
# define P[61] "Connection refused"
# define P[62] "Too many levels of symbolic links"
# define P[63] "File name too long"
# define P[64] "Host is down"
# define P[65] "No route to host"
# define P[66] "Directory not empty"
# define P[67] "Too many processes"
# define P[68] "Too many users"
# define P[69] "Disc quota exceeded"
# define P[70] "Stale NFS file handle"
# define P[71] "Too many levels of remote in path"
# define P[72] "RPC struct is bad"
# define P[73] "RPC version wrong"
# define P[74] "RPC prog. not avail"
# define P[75] "Program version wrong"
# define P[76] "Bad procedure for program"
# define P[77] "No locks available"
# define P[78] "Function not implemented"
# define P[79] "Inappropriate file type or format"
# define P[80] "Authentication error"
# define P[81] "Need authenticator"
# define P[82] "Device power is off"
# define P[83] "Device error"
# define P[84] "Value too large to be stored in data type"
# define P[85] "Bad executable (or shared library)"
# define P[86] "Bad CPU type in executable"
# define P[87] "Shared library version mismatch"
# define P[88] "Malformed Mach-o file"
# define P[89] "Operation canceled"
# define P[90] "Identifier removed"
# define P[91] "No message of desired type"
# define P[92] "Illegal byte sequence"
# define P[93] "Attribute not found"
# define P[94] "Bad message"
# define P[95] "EMULTIHOP (Reserved)"
# define P[96] "No message available on STREAM"
# define P[97] "ENOLINK (Reserved)"
# define P[98] "No STREAM resources"
# define P[99] "Not a STREAM"
# define P[100] "Protocol error"
# define P[101] "STREAM ioctl timeout"
# define P[102] "Operation not supported on socket"
# define P[103] "Policy not found"
# define P[104] "State not recoverable"
# define P[105] "Previous owner died"
# define P[106] "Interface output queue is full"
# endif

int				main_parse(char *line);
int				ft_or(int m, int a, int b, int c);
int				ft_print(int fd, char *format, ...);
int				ft_exit(char **av);
void			exit_status(int status);
char			**env_to_tab(t_variable *var, int all);
char			**paths_from_env(void);
int				run_cmd(char **cmd);
int				builtins(char *cmd, char **av, char **env);
int				cd_builtin(char **av, char **env);
t_variable		*get_env(char *s, int typ);
char			*get_var_from_tab(char **env, char *str);
char			*ft_appendstr(char *s1, char *s2);
char			*ft_strsub_delimit(char *s, char c);
int				alias_infinit_loop(char *str, char *s, t_alias *aliases);
int				alias_sub(t_list_token *word, t_alias *aliases);
int				is_reserved(char *str);
void			insert_alias(char *key, char *sub);
t_list_token	*ft_tokenize(char *str);
void			token_print(t_list_token *node);
void			parse_and_replace(t_list_token **cmd_token, t_list_token *node);
int				keywords_alias_sub(t_list_token **cmd_token);
char			*fetch_variables(char *key, int typ);
int				is_assword(char *str);
int				tilde_sub(t_list_token **cmd_token);
int				is_dollar(char *s);
int				end_dollar_word(char *s, int start);
char			*get_dollar_var(char *s, int start, int end);
char			*str_dollar_sub(char *str);
int				dollar_sub(t_list_token **cmd_token);
char			*get_pwd(char **env);
int				lexer(t_list_token **cmd_token);
void			bg_jobs(void);
int				ft_alias(char **av);
int				ft_unalias(char **av);
void			ft_signal(void);
int				init_shell(char **env);
t_hist			*create_history(void);
char			*delete_escape(char *str);
int				system_calls(char *func, int ret, int failure_value);
void			replace_node(t_list_token **dst, t_list_token **src);
char			*fc_history_remove(void);
void			fc_history_add(char *s, int l);
void			*ft_malloc(size_t	size);
int				is_char_in_str(char c, char *s);
int				list_count(t_l *list);
char			**strsplit_str(char const *s, char *c);
char			*free_remove_quot(char *s);

#endif
