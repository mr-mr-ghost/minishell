TODOS *************************************************

TODO: Fix misc. errors in tokens (if there are any)

TODO: Fix weird ctrl+c error in heredoc

TODO: heredoc probably needs a look at

TODO: probably some memory leaks

TODO: cleanup

LINKS *************************************************

https://m4nnb3ll.medium.com/minishell-building-a-mini-bash-a-42-project-b55a10598218

https://medium.com/@abkabex/minishell-42-f680eadde592

https://medium.com/@muxanz/how-the-shell-works-internally-when-entering-a-command-42f08458870

VALGRIND READLINE SUPPRESSION **************************

valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell
