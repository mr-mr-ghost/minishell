## TODOS:

- [x] multiple redirects
- [x] [fgsdfg > a.txt > b.txt > c.txt] differences in bash and minishell?
- [x] echo << X > a.txt (should create an empty file)
- [x] pick first heredoc, ignore the other heredocs, but deal with all other redirs
- [x] builtins and heredoc
- [x] builtins, nonbuiltins, heredoc, redirections and pipes working together
- [ ] take the last heredoc instead, as in bash?
- [ ] multiple heredocs?
- [x] cleanup the calamity of the call_pipe function in pipes_utils
- [x] ls | cat << X > t.txt | tr 'a' 'A' | pwd		 (writes ls to txt when heredoc NULL - it shouldn't)
- [ ] fix minishell_tester fails
- [ ] probably some memory leaks
- [ ] resolve the semi-colon question
- [ ] cleanup

## minishel tester fails:

- [ ] echo "$" (should return $)
- [ ] echo hi < a.txt bye bye (should ignore redirection and repeat words after redir)
- [x] cat <.filename ./filename (should ignore the first file)
- [ ] echo hi > b.txt bye (should write "hi bye" in the file)
- [ ] some bad $? during builtin redirection stuff maybe?

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
