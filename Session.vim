let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/github_kantog/minishell
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +27 src/parser/parser.c
badd +106 src/parser/abstract_syntax_tree_creation_fcts.c
badd +20 src/error_handler/error_checks.c
badd +54 src/executor/binaries/run_binary.c
badd +126 src/lexer/tokenizer.c
badd +72 src/cli/execute_logic.c
badd +141 src/cli/command_line_inteface.c
badd +1 src/executor/redirs/heredoc.c
badd +129 src/executor/binaries/binary_path_creation.c
badd +34 src/main/minishell_main.c
badd +114 src/executor/builtins/export.c
argglobal
%argdel
$argadd src/parser/parser.c
edit src/executor/redirs/heredoc.c
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd _ | wincmd |
vsplit
wincmd _ | wincmd |
vsplit
wincmd _ | wincmd |
vsplit
3wincmd h
wincmd w
wincmd w
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 40 + 41) / 83)
exe '2resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 2resize ' . ((&columns * 79 + 159) / 318)
exe '3resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 3resize ' . ((&columns * 79 + 159) / 318)
exe '4resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 4resize ' . ((&columns * 79 + 159) / 318)
exe '5resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 5resize ' . ((&columns * 78 + 159) / 318)
argglobal
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 42 - ((23 * winheight(0) + 20) / 40)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 42
normal! 05|
lcd ~/github_kantog/minishell
wincmd w
argglobal
if bufexists(fnamemodify("~/github_kantog/minishell/src/lexer/tokenizer.c", ":p")) | buffer ~/github_kantog/minishell/src/lexer/tokenizer.c | else | edit ~/github_kantog/minishell/src/lexer/tokenizer.c | endif
if &buftype ==# 'terminal'
  silent file ~/github_kantog/minishell/src/lexer/tokenizer.c
endif
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 126 - ((23 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 126
normal! 05|
lcd ~/github_kantog/minishell
wincmd w
argglobal
if bufexists(fnamemodify("~/github_kantog/minishell/src/main/minishell_main.c", ":p")) | buffer ~/github_kantog/minishell/src/main/minishell_main.c | else | edit ~/github_kantog/minishell/src/main/minishell_main.c | endif
if &buftype ==# 'terminal'
  silent file ~/github_kantog/minishell/src/main/minishell_main.c
endif
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 34 - ((16 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 34
normal! 022|
lcd ~/github_kantog/minishell
wincmd w
argglobal
if bufexists(fnamemodify("~/github_kantog/minishell/src/cli/execute_logic.c", ":p")) | buffer ~/github_kantog/minishell/src/cli/execute_logic.c | else | edit ~/github_kantog/minishell/src/cli/execute_logic.c | endif
if &buftype ==# 'terminal'
  silent file ~/github_kantog/minishell/src/cli/execute_logic.c
endif
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 72 - ((31 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 72
normal! 08|
lcd ~/github_kantog/minishell
wincmd w
argglobal
if bufexists(fnamemodify("~/github_kantog/minishell/src/executor/builtins/export.c", ":p")) | buffer ~/github_kantog/minishell/src/executor/builtins/export.c | else | edit ~/github_kantog/minishell/src/executor/builtins/export.c | endif
if &buftype ==# 'terminal'
  silent file ~/github_kantog/minishell/src/executor/builtins/export.c
endif
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 110 - ((31 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 110
normal! 05|
lcd ~/github_kantog/minishell
wincmd w
exe '1resize ' . ((&lines * 40 + 41) / 83)
exe '2resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 2resize ' . ((&columns * 79 + 159) / 318)
exe '3resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 3resize ' . ((&columns * 79 + 159) / 318)
exe '4resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 4resize ' . ((&columns * 79 + 159) / 318)
exe '5resize ' . ((&lines * 39 + 41) / 83)
exe 'vert 5resize ' . ((&columns * 78 + 159) / 318)
tabnext 1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let &winminheight = s:save_winminheight
let &winminwidth = s:save_winminwidth
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
set hlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
