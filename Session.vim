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
badd +122 src/executor/binaries/binary_path_creation.c
badd +52 src/executor/pipes/pipe_functions.c
badd +13 src/executor/binaries/run_binary.c
badd +88 src/executor/binaries/run_binary_utils.c
badd +20 src/executor/redirs/redir_checks.c
badd +203 minishell.h
badd +18 Makefile
argglobal
%argdel
$argadd src/executor/binaries/binary_path_creation.c
edit src/executor/binaries/run_binary_utils.c
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
exe 'vert 1resize ' . ((&columns * 117 + 117) / 234)
exe 'vert 2resize ' . ((&columns * 116 + 117) / 234)
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 89 - ((68 * winheight(0) + 40) / 81)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 89
normal! 08|
lcd ~/github_kantog/minishell
wincmd w
argglobal
if bufexists(fnamemodify("~/github_kantog/minishell/src/executor/binaries/run_binary.c", ":p")) | buffer ~/github_kantog/minishell/src/executor/binaries/run_binary.c | else | edit ~/github_kantog/minishell/src/executor/binaries/run_binary.c | endif
if &buftype ==# 'terminal'
  silent file ~/github_kantog/minishell/src/executor/binaries/run_binary.c
endif
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 95 - ((58 * winheight(0) + 40) / 81)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 95
normal! 08|
lcd ~/github_kantog/minishell
wincmd w
exe 'vert 1resize ' . ((&columns * 117 + 117) / 234)
exe 'vert 2resize ' . ((&columns * 116 + 117) / 234)
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
