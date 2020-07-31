set laststatus=2
set t_Co=256

set wildmenu
" vim 版本必须 >= 8.0, 并且 +python

" Automatic installation for vim-plug
if empty(glob('~/.vim/autoload/plug.vim'))
  silent !curl -fLo ~/.vim/autoload/plug.vim --create-dirs
    \ https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
  autocmd VimEnter * PlugInstall --sync | source $MYVIMRC
endif

call plug#begin('~/.vim/plugged')

" Add all your plugins here
" 要记得在vim中 :PlugInstall 安装这些插件

" 格式化注释
" gcb 为 box circled comment
" gcc 为普通注释
" gcu 为撤回注释, 然而垃圾的很, 就是 normal 下的 u
" FIXME: 必须设置为一个 tab 四个空格
Plug 'DCMMC/caw.vim'
" Plug 'tyru/caw.vim'
" Plug 'scrooloose/nerdcommenter'
" 缩进指示线
Plug 'Yggdroot/indentLine'

" fuzzy finder
Plug 'Yggdroot/LeaderF', { 'do': './install.sh' }

" Sublime Text 3 color scheme
Plug 'ErichDonGubler/vim-sublime-monokai'
" 自动补全括号
Plug 'jiangmiao/auto-pairs'
" 折叠代码
Plug 'tmhedberg/SimpylFold'
" Python 自动缩进
Plug 'Vimjas/vim-python-pep8-indent'
" An alternative to powerline
Plug 'vim-airline/vim-airline'
" mypy vim 扩展
" Plug 'Integralist/vim-mypy'
" Plug 'vim-scripts/indentpython.vim'
" 语法检查
" Plug 'w0rp/ale'
" shell in vim
" :Deol 打开内置终端
Plug 'Shougo/vimproc.vim', {'do' : 'make'}
Plug 'Shougo/deol.nvim'
" Plug 'Shougo/vimshell.vim'
" 文件树形结构
Plug 'scrooloose/nerdtree'
" 查看文件树形结构
Plug 'jistr/vim-nerdtree-tabs'
" 集成 git
Plug 'tpope/vim-fugitive'
" 异步 shell 执行
Plug 'skywind3000/asyncrun.vim'
" auto format
Plug 'Chiel92/vim-autoformat'
"=== vim-trailing-whitespace将代码行最后无效的空格标红 ===
Plug 'bronson/vim-trailing-whitespace'
" Async Language Server Protocol plugin for vim8
" 语法检查
Plug 'prabirshrestha/async.vim'
Plug 'prabirshrestha/vim-lsp'
" use :LspInstallServer to autoinstall and config for vim-lsp while editing
" a file with supported filetype
Plug 'mattn/vim-lsp-settings'
" Toggle window to maximum or restore
Plug 'szw/vim-maximizer'
" asynchronous completion framework for neovim/Vim8
Plug 'Shougo/deoplete.nvim'
" deoplete source for vim-lsp
Plug 'lighttiger2505/deoplete-vim-lsp'
Plug 'roxma/nvim-yarp'
Plug 'roxma/vim-hug-neovim-rpc'
Plug 'Shougo/deoplete-clangx'
" 深度学习智能补全
if has('win32') || has('win64')
  Plug 'tbodt/deoplete-tabnine', { 'do': 'powershell.exe .\install.ps1' }
else
  Plug 'tbodt/deoplete-tabnine', { 'do': './install.sh' }
endif
" 实时 md 渲染预览
Plug 'iamcco/mathjax-support-for-mkdp'
Plug 'iamcco/markdown-preview.vim'
let g:mkdp_path_to_chrome="chromium"
let g:mkdp_auto_close=0
nmap <F7> <Plug>MarkdownPreview
nmap <F8> <Plug>StopMarkdownPreview
" 更优雅的 buffer 管理
Plug 'fholgado/minibufexpl.vim'

" All of your Plugins must be added before the following line
" call vundle#end()            " required
" filetype plugin indent on    " required

call plug#end()


" 简要命令指南: 'MBEOpen' 打开管理器, ':bun'将当前 buffer 的窗口最小化, ':bw' 关闭当前 buffer 和窗口
" ':b[num]' 打开指定 num 的 buffer 为当前窗口, 注意如果当前只有一个窗口, 但是有多个 buffer, 这时候
" ':q' 会退出 vim, 所以需要用 ':bw' 来关闭
" 如果对当前窗口大小不满意, ':vertical resize+num' 宽度+[num]行,
" ':resize+num', 高度

""""""""""""""""""""""""""""""
" miniBufexplorer Config
""""""""""""""""""""""""""""""
" let g:miniBufExplMapWindowNavArrows = 1
" let g:miniBufExplMapWindowNavVim = 1
" let g:miniBufExplMapCTabSwitchWindows = 1
" let g:miniBufExplMapCTabSwitchBufs = 1
" let g:miniBufExplModSelTarget = 1

" 解决FileExplorer窗口变小问题
" let g:miniBufExplForceSyntaxEnable = 1
" let g:miniBufExplorerMoreThanOne=2

" 配色
set background=dark
" 不设置这些会导致背景色为纯黑, 贼丑, 设置之后将会是终端(模拟器)的背景色,
" 所以还需要把终端主题改成 solarized
" let g:solarized_termcolors=256
" let g:solarized_termtrans=1
syntax on
" colorscheme solarized
colorscheme sublimemonokai

" disable the feature that backspace in the end of the line
" will remove eol
" ref: `:help bs`
" DCMMC: We can use `J` to join the curren line and the next line
" a.k.a remove the line breaks (e.g. \n\r) in the end of the
" current line
set bs=indent,start

" LeaderF
" vim 8.1.1615+
" set leader key from '\' to ','
let mapleader = ","
let g:Lf_WindowPosition = 'popup'
let g:Lf_PreviewInPopup = 1
let g:Lf_ShowDevIcons = 1
let g:Lf_ShortcutF = "<leader>ff"
noremap <leader>fb :<C-U><C-R>=printf("Leaderf buffer %s", "")<CR><CR>
noremap <leader>fm :<C-U><C-R>=printf("Leaderf mru %s", "")<CR><CR>
" 需要 ctags, pacman -S ctags
noremap <leader>ft :<C-U><C-R>=printf("Leaderf bufTag %s", "")<CR><CR>
noremap <leader>fl :<C-U><C-R>=printf("Leaderf line %s", "")<CR><CR>
" 需要 gtags (aka GNU global), yay -S ctags python-pygments global
" Sometimes you need `:Leaderf gtags --remove` to remove all the
" gtags cache
" https://zhuanlan.zhihu.com/p/64842373
" 启用 ctags 和 pygments 扩展支持, 该配置文件路径仅在 archlinux 上测试
" let $GTAGSCONF = '/usr/share/gtags/gtags.conf'
let g:Lf_Gtagsconf = '/usr/share/gtags/gtags.conf'
" Auto `Leaderf gtags --update`
" Max time of indexing is 3600s
let g:Lf_IndexTimeLimit = 3600
let g:Lf_GtagsAutoGenerate = 1
let g:Lf_Gtagslabel = 'native-pygments'
" 默认只对项目根目录下有这些文件(夹)的时候才使用 gtags 索引
let g:Lf_RootMarkers = ['.git', '.svn', '.hg']
noremap <leader>fr :<C-U><C-R>=printf("Leaderf! gtags -r %s --auto-jump", expand("<cword>"))<CR><CR>
noremap <leader>fd :<C-U><C-R>=printf("Leaderf! gtags -d %s --auto-jump", expand("<cword>"))<CR><CR>
noremap <leader>fo :<C-U><C-R>=printf("Leaderf! gtags --recall %s", "")<CR><CR>
noremap <leader>fn :<C-U><C-R>=printf("Leaderf gtags --next %s", "")<CR><CR>
noremap <leader>fp :<C-U><C-R>=printf("Leaderf gtags --previous %s", "")<CR><CR>
noremap <leader>fg :<C-U><C-R>=printf("Leaderf gtags --all")<CR><CR>

" set filetypes to be ignored for highlight of trailing whitespace
let g:extra_whitespace_ignored_filetypes = []

" F3 autoformat
noremap <F3> :Autoformat<CR>

" 分割方向
" :sv <filename> 纵向分割打开
" :vs <filename> 横向分割打开
set splitbelow
set splitright
" 分割快捷键切换
nnoremap <C-J> <C-W><C-J>
nnoremap <C-K> <C-W><C-K>
nnoremap <C-L> <C-W><C-L>
nnoremap <C-H> <C-W><C-H>
" 代码折叠
set foldmethod=indent
set foldlevel=99
" 空格快捷键代码折叠
" press space to fold/unfold code
nnoremap <space> za
vnoremap <space> zf
" 希望看到折叠代码的文档字符串
let g:SimpylFold_docstring_preview=1

" Python PEP8风格缩进
" 编写python代码时会使#(注释)自动回退到行首
" au bufnewfile,bufread *.py, *.java
" \ set tabstop=4 |
" \ set softtabstop=4 |
" \ set shiftwidth=4

" web开发缩进
au BufNewFile,BufRead *.js,*.html,*.css set tabstop=2
au BufNewFile,BufRead *.js,*.html,*.css set softtabstop=2
au BufNewFile,BufRead *.js,*.html,*.css set shiftwidth=2

" Lua
au BufNewFile,BufRead *.lua set tabstop=2
au BufNewFile,BufRead *.lua set softtabstop=2
au BufNewFile,BufRead *.lua set shiftwidth=2

" yml
au BufNewFile,BufRead *.yml,*.yaml set tabstop=2
au BufNewFile,BufRead *.yml,*.yaml set softtabstop=2
au BufNewFile,BufRead *.yml,*.yaml set shiftwidth=2

" json
au BufNewFile,BufRead *.json set tabstop=2
au BufNewFile,BufRead *.json set softtabstop=2
au BufNewFile,BufRead *.json set shiftwidth=2

" C/C++
au BufRead,BufNewFile *.c,*.h,*.cpp,*hpp set expandtab
au BufRead,BufNewFile *.c,*.h,*.cpp,*.hpp set tabstop=4
au BufRead,BufNewFile *.c,*.h,*.cpp,*.hpp set shiftwidth=4
au BufRead,BufNewFile *.c,*.h,*.cpp,*.hpp set autoindent
" au BufRead,BufNewFile *.c,*.h,*.cpp,*.hpp match BadWhitespace /^\t\+/
" au BufRead,BufNewFile *.c,*.h,*.cpp,*.hpp match BadWhitespace /\s\+$/
au         BufNewFile *.c,*.h,*.cpp,*.hpp set fileformat=unix
au BufRead,BufNewFile *.c,*.h,,*.cpp,*.hpp let b:comment_leader = '/* '

" Java
au BufRead,BufNewFile *.java set expandtab
au BufRead,BufNewFile *.java set tabstop=4
au BufRead,BufNewFile *.java set shiftwidth=4
au BufRead,BufNewFile *.java set autoindent
au BufRead,BufNewFile *.java match BadWhitespace /^\t\+/
au BufRead,BufNewFile *.java match BadWhitespace /\s\+$/
au         BufNewFile *.java set fileformat=unix
au BufRead,BufNewFile *.java let b:comment_leader = '//'

" indentLine
let g:indentLine_concealcursor = ''
let g:indentLine_conceallevel = 2

" 支持 utf-8
set encoding=utf-8

set pyxversion=3
" g:python3_host_prog = "/usr/local/bin/python3"

" YCM
" 自动补全微调
" map <leader>g  :YcmCompleter GoToDefinitionElseDeclaration<CR>
" let g:ycm_complete_in_strings = 1
" set completeopt=menu,longest	"让Vim的补全菜单行为与一般IDE一致(参考VimTip1228)
" let g:ycm_add_preview_to_completeopt = 0
" autocmd InsertLeave * if pumvisible() == 0|pclose|endif	"离开插入模式后自动关闭预览窗口
" " 回车即选中当前项
" inoremap <expr> <CR>       pumvisible() ? "\<C-y>" : "\<CR>"
" " 设置在下面几种格式的文件上屏蔽ycm
" let g:ycm_filetype_blacklist = {
" 			\ 'tagbar' : 1,
" 			\ 'nerdtree' : 1,
" 			\}
" " 输入第2个字符开始补全
" let g:ycm_min_num_of_chars_for_completion=2
" " 开启语义补全
" let g:ycm_seed_identifiers_with_syntax=1
" " 在字符串输入中也能补全
" let g:ycm_complete_in_strings = 1
" " YCM 异步主动语义补全, 两个字符触发
" " https://zhuanlan.zhihu.com/p/33046090
" let g:ycm_semantic_triggers =  {
" 			\ 'c,cpp,python,java,go,erlang,perl': ['re!\w{2}'],
" 			\ 'cs,lua,javascript': ['re!\w{2}'],
" 			\ }
" let g:ycm_filetype_whitelist = {
"			\ "c":1,
"			\ "cpp":1,
"			\ "objc":1,
"			\ "sh":1,
"			\ "py": 1,
"			\ "js": 1,
"			\ "html": 1,
"			\ "vue": 1,
"			\ "ts": 1,
"			\ "zsh":1,
"			\ "zimbu":1,
"			\ }
" youcompleteme  默认tab  s-tab 和 ultisnips 冲突
" let g:ycm_key_list_select_completion = ['<Down>']
" let g:ycm_key_list_previous_completion = ['<Up>']

" 支持Virtualenv虚拟环境
" py << EOF
" import os
" import sys
" if 'VIRTUAL_ENV' in os.environ:
" 	project_base_dir = os.environ['VIRTUAL_ENV']
" 	activate_this = os.path.join(project_base_dir, 'bin/activate_this.py')
" 	execfile(activate_this, dict(__file__=activate_this))
" EOF

" tabnine
call deoplete#custom#var('tabnine', {
\ 'line_limit': 700,
\ 'max_num_results': 10,
\ })

" ale 异步语法检查
" 语法高亮
" 让 ale 对 py 文件使用 flake8
" 记得 sudo pip install flake8
" let g:ale_linters = { 'python': ['flake8'], }
" Auto-check file for errors on write:
let g:PyFlakeOnWrite = 1
" let g:flake8_show_in_gutter = 1
" 错误表示在整行而不是侧边
let g:flake8_show_in_file = 1
let python_highlight_all=1

" 在终端 vim 中默认打开tabs
let g:nerdtree_tabs_open_on_console_startup=1
" 隐藏文件树形结构中的 .pyc
let NERDTreeIgnore=['\.pyc$', '\~$', '__pycache__'] "ignore files in NERDTree

" 显示行号
set nu

" alias of Deol command
cnoreabbrev vimshell Deol

" Python 代码格式化
" 需要 sudo pip install yapf
autocmd FileType python nnoremap <LocalLeader>= :0,$!yapf(" --style pep8")<CR>

" 一键执行
map <F5> :call CompileRun()<CR>
func! CompileRun()
	exec "w"
	if &filetype == 'c'
		exec "! gcc  % -o %<; time ./%<"
	elseif &filetype == 'cpp'
		exec "! g++ -std=c++11 % -o %<; time ./%<"
	elseif &filetype == 'java'
		exec "! javac %; time java %<"
	elseif &filetype == 'sh'
		exec "! time bash %"
	elseif &filetype == 'python'
		exec "! time python %"
	endif
endfunc

" 用 ,. 组合键代替 Esc
inoremap ,. <esc>
vnoremap ,. <esc>
nnoremap ,. <esc>

" vim-lsp 配置
let g:lsp_auto_enable = 1
if executable('pyls')
    " pip install python-language-server
    au User lsp_setup call lsp#register_server({
        \ 'name': 'pyls',
        \ 'cmd': {server_info->['pyls']},
        \ 'whitelist': ['python'],
        \ })
endif
" Register ccls C++ lanuage server.
if executable('ccls')
   au User lsp_setup call lsp#register_server({
      \ 'name': 'ccls',
      \ 'cmd': {server_info->['ccls']},
      \ 'root_uri': {server_info->lsp#utils#path_to_uri(lsp#utils#find_nearest_parent_file_directory(lsp#utils#get_buffer_path(), 'compile_commands.json'))},
      \ 'initialization_options': {'cache': {'directory': '/tmp/ccls/cache' }},
      \ 'whitelist': ['c', 'cpp', 'objc', 'objcpp', 'cc'],
      \ })
endif

" key binding for vim-lsp
function! s:on_lsp_buffer_enabled() abort
    setlocal omnifunc=lsp#complete
    setlocal signcolumn=yes
    nmap <buffer> ld <plug>(lsp-definition)
    nmap <buffer> lr <plug>(lsp-rename)
    " refer to doc to add more commands
endfunction

augroup lsp_install
    au!
    " call s:on_lsp_buffer_enabled only for languages that has the server registered.
    autocmd User lsp_buffer_enabled call s:on_lsp_buffer_enabled()
augroup END

" Use deoplete.
let g:deoplete#enable_at_startup = 1

" Toggle window to maximum or restore
let g:maximizer_set_default_mapping = 0
nnoremap <S-M> :MaximizerToggle!<CR>

" exit terminal mode in deol buffer
" tnoremap <ESC>   <C-\><C-n>
tnoremap ,.  <C-\><C-n>
