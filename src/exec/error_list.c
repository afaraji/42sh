/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 18:18:50 by sazouaka          #+#    #+#             */
/*   Updated: 2020/12/13 20:54:47 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*err_list_5(int err)
{
	static char	*s[12];

	if (!s[0])
	{
		s[95 - 95] = "EMULTIHOP (Reserved)";
		s[96 - 95] = "No message available on STREAM";
		s[97 - 95] = "ENOLINK (Reserved)";
		s[98 - 95] = "No STREAM resources";
		s[99 - 95] = "Not a STREAM";
		s[100 - 95] = "Protocol error";
		s[101 - 95] = "STREAM ioctl timeout";
		s[102 - 95] = "Operation not supported on socket";
		s[103 - 95] = "Policy not found";
		s[104 - 95] = "State not recoverable";
		s[105 - 95] = "Previous owner died";
		s[106 - 95] = "Interface output queue is full";
	}
	if (err < 12)
		return (s[err]);
	return ("Unknown.");
}

char	*err_list_4(int err)
{
	static char	*s[19];

	if (!s[0])
	{
		s[76 - 76] = "Bad procedure for program";
		s[77 - 76] = "No locks available";
		s[78 - 76] = "Function not implemented";
		s[79 - 76] = "Inappropriate file type or format";
		s[80 - 76] = "Authentication error";
		s[81 - 76] = "Need authenticator";
		s[82 - 76] = "Device power is off";
		s[83 - 76] = "Device error";
		s[84 - 76] = "Value too large to be stored in data type";
		s[85 - 76] = "Bad executable (or shared library)";
		s[86 - 76] = "Bad CPU type in executable";
		s[87 - 76] = "Shared library version mismatch";
		s[88 - 76] = "Malformed Mach-o file";
		s[89 - 76] = "Operation canceled";
		s[90 - 76] = "Identifier removed";
		s[91 - 76] = "No message of desired type";
		s[92 - 76] = "Illegal byte sequence";
		s[93 - 76] = "Attribute not found";
		s[94 - 76] = "Bad message";
	}
	return ((err < 19) ? s[err] : err_list_5(err - 19));
}

char	*err_list_3(int err)
{
	static char	*s[19];

	if (!s[0])
	{
		s[57 - 57] = "Socket is not connected";
		s[58 - 57] = "Can't send after socket shutdown";
		s[59 - 57] = "Too many references: can't splice";
		s[60 - 57] = "Operation timed out";
		s[61 - 57] = "Connection refused";
		s[62 - 57] = "Too many levels of symbolic links";
		s[63 - 57] = "File name too long";
		s[64 - 57] = "Host is down";
		s[65 - 57] = "No route to host";
		s[66 - 57] = "Directory not empty";
		s[67 - 57] = "Too many processes";
		s[68 - 57] = "Too many users";
		s[69 - 57] = "Disc quota exceeded";
		s[70 - 57] = "Stale NFS file handle";
		s[71 - 57] = "Too many levels of remote in path";
		s[72 - 57] = "RPC struct is bad";
		s[73 - 57] = "RPC version wrong";
		s[74 - 57] = "RPC prog. not avail";
		s[75 - 57] = "Program version wrong";
	}
	return ((err < 19) ? s[err] : err_list_4(err - 19));
}

char	*err_list_2(int err)
{
	static char	*s[19];

	if (!s[0])
	{
		s[38 - 38] = "Socket operation on non-socket";
		s[39 - 38] = "Destination address required";
		s[40 - 38] = "Message too long";
		s[41 - 38] = "Protocol wrong type for socket";
		s[42 - 38] = "Protocol not available";
		s[43 - 38] = "Protocol not supported";
		s[44 - 38] = "Socket type not supported";
		s[45 - 38] = "Operation not supported";
		s[46 - 38] = "Protocol family not supported";
		s[47 - 38] = "Address family not supported by protocol family";
		s[48 - 38] = "Address already in use";
		s[49 - 38] = "Can't assign requested address";
		s[50 - 38] = "Network is down";
		s[51 - 38] = "Network is unreachable";
		s[52 - 38] = "Network dropped connection on reset";
		s[53 - 38] = "Software caused connection abort";
		s[54 - 38] = "Connection reset by peer";
		s[55 - 38] = "No buffer space available";
		s[56 - 38] = "Socket is already connected";
	}
	return ((err < 19) ? s[err] : err_list_3(err - 19));
}

char	*err_list_1(int err)
{
	static char	*s[19];

	if (!s[0])
	{
		s[19 - 19] = "Operation not supported by device";
		s[20 - 19] = "Not a directory";
		s[21 - 19] = "Is a directory";
		s[22 - 19] = "Invalid argument";
		s[23 - 19] = "Too many open files in system";
		s[24 - 19] = "Too many open files";
		s[25 - 19] = "Inappropriate ioctl for device";
		s[26 - 19] = "Text file busy";
		s[27 - 19] = "File too large";
		s[28 - 19] = "No space left on device";
		s[29 - 19] = "Illegal seek";
		s[30 - 19] = "Read-only file system";
		s[31 - 19] = "Too many links";
		s[32 - 19] = "Broken pipe";
		s[33 - 19] = "Numerical argument out of domain";
		s[34 - 19] = "Result too large";
		s[35 - 19] = "Resource temporarily unavailable";
		s[36 - 19] = "Operation now in progress";
		s[37 - 19] = "Operation already in progress";
	}
	return ((err < 19) ? s[err] : err_list_2(err - 19));
}
