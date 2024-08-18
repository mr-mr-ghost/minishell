## TODOS:
- [ ] Convert environment into array of strings
- [ ] Check and update environment array each nonbuiltin command call
- [ ] Find nonbuiltin command pathway in $PATH and update command word
- [ ] FIX nonbuiltin command redirection
- [ ] Pipes
- [ ] Heredoc
- [ ] FIX memory error in using command exit (conditional jump in free_env)

## LINKS:
- https://www.codequoi.com/en/handling-a-file-by-its-descriptor-in-c/
- https://m4nnb3ll.medium.com/minishell-building-a-mini-bash-a-42-project-b55a10598218
- https://medium.com/@abkabex/minishell-42-f680eadde592
- https://medium.com/@muxanz/how-the-shell-works-internally-when-entering-a-command-42f08458870

## VALGRIND FLAGS:
- --suppressions=readline.supp
- --leak-check=full
- --show-leak-kinds=all
- --track-origins=yes
- --track-fds=yes
