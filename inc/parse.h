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
# define s[0] "Undefined error: 0"
# define s[1] "Operation not permitted"
# define s[2] "No such file or directory"
# define s[3] "No such process"
# define s[4] "Interrupted system call"
# define s[5] "Input/output error"
# define s[6] "Device not configured"
# define s[7] "Argument list too long"
# define s[8] "Exec format error"
# define s[9] "Bad file descriptor"
# define s[10] "No child processes"
# define s[11] "Resource deadlock avoided"
# define s[12] "Cannot allocate memory"
# define s[13] "Permission denied"
# define s[14] "Bad address"
# define s[15] "Block device required"
# define s[16] "Resource busy"
# define s[17] "File exists"
# define s[18] "Cross-device link"
# define s[19] "Operation not supported by device"
# define s[20] "Not a directory"
# define s[21] "Is a directory"
# define s[22] "Invalid argument"
# define s[23] "Too many open files in system"
# define s[24] "Too many open files"
# define s[25] "Inappropriate ioctl for device"
# define s[26] "Text file busy"
# define s[27] "File too large"
# define s[28] "No space left on device"
# define s[29] "Illegal seek"
# define s[30] "Read-only file system"
# define s[31] "Too many links"
# define s[32] "Broken pipe"
# define s[33] "Numerical argument out of domain"
# define s[34] "Result too large"
# define s[35] "Resource temporarily unavailable"
# define s[36] "Operation now in progress"
# define s[37] "Operation already in progress"
# define s[38] "Socket operation on non-socket"
# define s[39] "Destination address required"
# define s[40] "Message too long"
# define s[41] "Protocol wrong type for socket"
# define s[42] "Protocol not available"
# define s[43] "Protocol not supported"
# define s[44] "Socket type not supported"
# define s[45] "Operation not supported"
# define s[46] "Protocol family not supported"
# define s[47] "Address family not supported by protocol family"
# define s[48] "Address already in use"
# define s[49] "Can't assign requested address"
# define s[50] "Network is down"
# define s[51] "Network is unreachable"
# define s[52] "Network dropped connection on reset"
# define s[53] "Software caused connection abort"
# define s[54] "Connection reset by peer"
# define s[55] "No buffer space available"
# define s[56] "Socket is already connected"
# define s[57] "Socket is not connected"
# define s[58] "Can't send after socket shutdown"
# define s[59] "Too many references: can't splice"
# define s[60] "Operation timed out"
# define s[61] "Connection refused"
# define s[62] "Too many levels of symbolic links"
# define s[63] "File name too long"
# define s[64] "Host is down"
# define s[65] "No route to host"
# define s[66] "Directory not empty"
# define s[67] "Too many processes"
# define s[68] "Too many users"
# define s[69] "Disc quota exceeded"
# define s[70] "Stale NFS file handle"
# define s[71] "Too many levels of remote in path"
# define s[72] "RPC struct is bad"
# define s[73] "RPC version wrong"
# define s[74] "RPC prog. not avail"
# define s[75] "Program version wrong"
# define s[76] "Bad procedure for program"
# define s[77] "No locks available"
# define s[78] "Function not implemented"
# define s[79] "Inappropriate file type or format"
# define s[80] "Authentication error"
# define s[81] "Need authenticator"
# define s[82] "Device power is off"
# define s[83] "Device error"
# define s[84] "Value too large to be stored in data type"
# define s[85] "Bad executable (or shared library)"
# define s[86] "Bad CPU type in executable"
# define s[87] "Shared library version mismatch"
# define s[88] "Malformed Mach-o file"
# define s[89] "Operation canceled"
# define s[90] "Identifier removed"
# define s[91] "No message of desired type"
# define s[92] "Illegal byte sequence"
# define s[93] "Attribute not found"
# define s[94] "Bad message"
# define s[95] "EMULTIHOP (Reserved)"
# define s[96] "No message available on STREAM"
# define s[97] "ENOLINK (Reserved)"
# define s[98] "No STREAM resources"
# define s[99] "Not a STREAM"
# define s[100] "Protocol error"
# define s[101] "STREAM ioctl timeout"
# define s[102] "Operation not supported on socket"
# define s[103] "Policy not found"
# define s[104] "State not recoverable"
# define s[105] "Previous owner died"
# define s[106] "Interface output queue is full"
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
int				insert_alias(char *key, char *sub);
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
char			*alias_check(char *s, t_alias *list);
char			*err_list_1(int err);
char			*err_list_2(int err);
char			*err_list_3(int err);
char			*err_list_4(int err);
char			*err_list_5(int err);

#endif
