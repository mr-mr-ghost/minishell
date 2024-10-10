## TODOS:

- [ ] Fix misc. errors in tokens (if there are any)
- [ ] Fix return issues in pipes and heredoc(?)
- [ ] echo > | ls does not work properly
- [x] multiple redirects
- [x] [fgsdfg > a.txt > b.txt > c.txt] differences in bash and minishell?
- [x] echo << X > a.txt (should create an empty file)
- [x] pick first heredoc, ignore the other heredocs, but deal with all other redirs
- [ ] multiple heredocs?
- [x] builtins and heredoc
- [x] heredoc pipe (with issues)
- [x] fix builtins accepting the heredoc and stdin
- [ ] fix non builtins accepting the heredoc and stdin
- [ ] fix non builtins and heredoc not returning prompt
- [ ] builtins, nonbuiltins, heredoc, redirections and pipes working together
- [ ] cleanup the calamity of the call_pipe function in pipes_utils
- [ ] probably some memory leaks
- [ ] resolve the semi-colon question
- [ ] cleanup

## LINKS:
- https://www.codequoi.com/en/handling-a-file-by-its-descriptor-in-c/
- https://m4nnb3ll.medium.com/minishell-building-a-mini-bash-a-42-project-b55a10598218
- https://medium.com/@abkabex/minishell-42-f680eadde592
- https://medium.com/@muxanz/how-the-shell-works-internally-when-entering-a-command-42f08458870
- https://www.rozmichelle.com/pipes-forks-dups/

## VALGRIND FLAGS:
- valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all --track-fds=yes --track-origins=yes ./minishell
- --suppressions=readline.supp
- --leak-check=full
- --show-leak-kinds=all
- --track-origins=yes
- --track-fds=yes
