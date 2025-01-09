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
badd +78 src/executor/redirs/redir_checks.c
badd +109 src/executor/redirs/heredoc.c
badd +30 src/main/init_envvars.c
badd +77 src/libft/ft_create_dict.c
badd +54 src/main/init_data.c
badd +101 minishell.h
badd +121 src/executor/builtins/export.c
badd +18 ~/github_kantog/minishell/src/executor/pipes/pipe_functions.c
argglobal
%argdel
$argadd src/executor/redirs/redir_checks.c
edit src/executor/redirs/redir_checks.c
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
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
exe 'vert 1resize ' . ((&columns * 158 + 159) / 318)
exe 'vert 2resize ' . ((&columns * 159 + 159) / 318)
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
let s:l = 79 - ((64 * winheight(0) + 40) / 81)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 79
normal! 027|
wincmd w
argglobal
if bufexists(fnamemodify("src/executor/redirs/heredoc.c", ":p")) | buffer src/executor/redirs/heredoc.c | else | edit src/executor/redirs/heredoc.c | endif
if &buftype ==# 'terminal'
  silent file src/executor/redirs/heredoc.c
endif
balt src/executor/redirs/redir_checks.c
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
let s:l = 97 - ((63 * winheight(0) + 40) / 81)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 97
normal! 08|
wincmd w
2wincmd w
exe 'vert 1resize ' . ((&columns * 158 + 159) / 318)
exe 'vert 2resize ' . ((&columns * 159 + 159) / 318)
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
