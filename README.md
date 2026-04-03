# ft_ls

A reimplementation of the Unix `ls` command in C, a School 42 project.

## Features

| Flag | Description |
|------|-------------|
| `-l` | Long listing format (permissions, links, owner, group, size, date) |
| `-R` | Recursively list subdirectories |
| `-a` | Show hidden files (dotfiles) |
| `-r` | Reverse sort order |
| `-t` | Sort by modification time |

Flags can be combined freely (e.g. `-lRa`).

## Build

```sh
make
```

## Example

```
$ ./ft_ls -la
total 44
drwxr-xr-x  6 charles charles  4096 Mar 15 10:32 .
drwxr-xr-x 30 charles charles  4096 Mar 14 09:11 ..
drwxr-xr-x  8 charles charles  4096 Mar 15 10:32 .git
-rw-r--r--  1 charles charles    40 Feb 20 14:05 .gitignore
-rw-r--r--  1 charles charles   100 Feb 20 14:05 .gitmodules
-rw-r--r--  1 charles charles   917 Mar 10 16:44 Makefile
-rw-r--r--  1 charles charles   512 Mar 15 10:32 README.md
drwxr-xr-x  2 charles charles  4096 Mar 10 16:44 inc
drwxr-xr-x  2 charles charles  4096 Mar 15 10:32 libft
drwxr-xr-x  2 charles charles  4096 Mar 15 10:32 src
-rw-r--r--  1 charles charles 68382 Feb 20 14:05 subject.pdf
```
