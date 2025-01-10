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
badd +13 src/executor/redirs/heredoc.c
badd +24 src/executor/builtins/exit.c
badd +251 minishell.h
badd +87 src/main/sighandler.c
badd +13 src/main/sighandler_utils.c
badd +36 /usr/include/x86_64-linux-gnu/bits/sigaction.h
argglobal
%argdel
$argadd src/executor/redirs/heredoc.c
edit src/executor/redirs/heredoc.c
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
wincmd _ | wincmd |
vsplit
2wincmd h
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
exe 'vert 1resize ' . ((&columns * 115 + 173) / 347)
exe 'vert 2resize ' . ((&columns * 115 + 173) / 347)
exe 'vert 3resize ' . ((&columns * 115 + 173) / 347)
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
let s:l = 13 - ((12 * winheight(0) + 42) / 84)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 13
normal! 02|
wincmd w
argglobal
if bufexists(fnamemodify("src/main/sighandler.c", ":p")) | buffer src/main/sighandler.c | else | edit src/main/sighandler.c | endif
if &buftype ==# 'terminal'
  silent file src/main/sighandler.c
endif
balt src/executor/redirs/heredoc.c
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
let s:l = 96 - ((50 * winheight(0) + 42) / 84)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 96
normal! 08|
wincmd w
argglobal
if bufexists(fnamemodify("src/main/sighandler_utils.c", ":p")) | buffer src/main/sighandler_utils.c | else | edit src/main/sighandler_utils.c | endif
if &buftype ==# 'terminal'
  silent file src/main/sighandler_utils.c
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
let s:l = 27 - ((26 * winheight(0) + 42) / 84)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 27
normal! 02|
lcd ~/github_kantog/minishell
wincmd w
3wincmd w
exe 'vert 1resize ' . ((&columns * 115 + 173) / 347)
exe 'vert 2resize ' . ((&columns * 115 + 173) / 347)
exe 'vert 3resize ' . ((&columns * 115 + 173) / 347)
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
