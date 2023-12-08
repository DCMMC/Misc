# If you come from bash you might have to change your $PATH.
# export PATH=$HOME/bin:/usr/local/bin:$PATH
export LANG="en_US.UTF-8"

# Enable Powerlevel10k instant prompt. Should stay close to the top of ~/.zshrc.
# Initialization code that may require console input (password prompts, [y/n]
# confirmations, etc.) must go above this block; everything else may go below.
if [[ -r "${XDG_CACHE_HOME:-$HOME/.cache}/p10k-instant-prompt-${(%):-%n}.zsh" ]]; then
  source "${XDG_CACHE_HOME:-$HOME/.cache}/p10k-instant-prompt-${(%):-%n}.zsh"
fi

autoload -Uz compinit; compinit
source /usr/share/zsh-theme-powerlevel10k/powerlevel10k.zsh-theme
source /usr/share/zsh/plugins/fzf-tab-git/fzf-tab.plugin.zsh
source /usr/share/zsh/plugins/zsh-autosuggestions/zsh-autosuggestions.zsh
source /usr/share/zsh/plugins/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh
source /usr/share/zsh/plugins/zsh-history-substring-search/zsh-history-substring-search.zsh

bindkey "^[[A" history-substring-search-up
bindkey "^[[B" history-substring-search-down

zstyle ':completion:*' menu yes select search
# cd 时在右侧预览目录内容
zstyle ':fzf-tab:complete:cd:*' fzf-preview 'exa -1 --color=always $realpath'

# To customize prompt, run `p10k configure` or edit ~/.p10k.zsh.
[[ ! -f ~/.p10k.zsh ]] || source ~/.p10k.zsh

if [ $TILIX_ID ] || [ $VTE_VERSION ]; then
  source /etc/profile.d/vte.sh
fi

pokemonsay -p Pikachu -N 'Have a Good Day, DCMMC!'

# User configuration

# export MANPATH="/usr/local/man:$MANPATH"

# You may need to manually set your language environment
# export LANG=en_US.UTF-8

# Preferred editor for local and remote sessions
# if [[ -n $SSH_CONNECTION ]]; then
#   export EDITOR='vim'
# else
#   export EDITOR='mvim'
# fi

# Compilation flags
# export ARCHFLAGS="-arch x86_64"

# Set personal aliases, overriding those provided by oh-my-zsh libs,
# plugins, and themes. Aliases can be placed here, though oh-my-zsh
# users are encouraged to define aliases within the ZSH_CUSTOM folder.
# For a full list of active aliases, run `alias`.
#
# Example aliases
# alias zshconfig="mate ~/.zshrc"
# alias ohmyzsh="mate ~/.oh-my-zsh"

ZSH_CACHE_DIR=$HOME/.oh-my-zsh-cache
if [[ ! -d $ZSH_CACHE_DIR ]]; then
  mkdir $ZSH_CACHE_DIR
fi

source $ZSH/oh-my-zsh.sh
source /usr/share/zsh-theme-powerlevel10k/powerlevel10k.zsh-theme

# telegram on wayland
# alias telegram-desktop='env QT_QPA_PLATFORM=wayland telegram-desktop'

# HiDPI
export QT_SCALE_FACTOR=1.4
export GDK_DPI_SCALE=1.2

export EDITOR=vim
alias yay='yay --editmenu'
alias cl='clear'
alias update='yay -Syyu'
export PATH=$PATH:$HOME/bin:/opt/miktex/bin/:$(yarn global bin):${HOME}/.local/bin
alias open='xdg-open'
alias ssgit='git config --global http.proxy "socks5h://localhost:1080" && git config --global https.proxy "socks5h://localhost:1080"'
alias unssgit='git config --global --unset http.proxy && git config --global --unset https.proxy'
alias fix_audio='systemctl --user restart cros-pulse-config'

# HiDPI
# export GDK_SCALE=1.2
# export GDK_DPI_SCALE=1.2
alias starti3='unset DBUS_SESSION_BUS_ADDRESS && unset XDG_RUNTIME_DIR && Xephyr -br -ac -noreset -screen 1920x1080 -dpi 150 -resizeable :2 >/dev/null 2>&1 &; sleep 1s && DISPLAY=:2 i3 >/dev/null 2>&1 &'
alias exiti3='pkill Xephyr && pkill i3'
alias startsogou='export $(dbus-launch) && fcitx >/dev/null 2>&1 &; sogou-qimpanel >/dev/null 2>&1 &'

alias fix_net='sudo systemctl restart NetworkManager'
alias pastebin="curl -F 'vimcn=<-' https://cfp.vim-cn.com/"
alias getip='curl -i cip.cc'

# set qt5 theme for all qt apps, must install qt5ct
# export QT_QPA_PLATFORM=qt5ct
alias sshttp="export ALL_PROXY=socks5h://localhost:1080 https_proxy='http://127.0.0.1:8118' && export http_proxy='http://127.0.0.1:8118' && export HTTPS_PROXY='http://127.0.0.1:8118' && export HTTP_PROXY='http://127.0.0.1:8118' && git config --global http.proxy socks5h://127.0.0.1:1080 && git config --global https.proxy socks5h://127.0.0.1:1080"
alias unsshttp="unset all_proxy && unset ALL_PROXY && unset http_proxy && unset https_proxy && unset HTTP_PROXY && unset HTTPS_PROXY && git config --global --unset http.proxy && git config --global --unset https.proxy"
alias pdf='echo "removing old adobe user configs" && rm -r ~/.adobe/Acrobat && acroread'
alias set_cursor_breeze='gsettings set org.gnome.desktop.interface cursor-theme Breeze_Snow'
alias x11docker_konsole='x11docker --gpu --hostdisplay --user=root -- x11docker/kde-plasma konsole'
alias gc='git clone --recursive'
alias gcd1='git clone --depth=1 --recursive'
alias breeze_konsole='QT_STYLE_OVERRIDE=breeze konsole >/dev/null 2>&1 &'
alias planner="env GTK_THEME='' planner"
# You must config and run graftcp in advance.
alias proxy_run='graftcp'
# Install emoji font (e.g. JoyPixels) in advance
# yarn config set -- --emoji true
# Below is wrong alias!
# alias yarn='yarn --emoji'
function deepin-qq () {
	/usr/lib/gsd-xsettings >/dev/null 2>&1 &
	export $(dbus-launch)
	gtk-launch deepin.com.qq.im.desktop
}
alias qt5ct='QT_QPA_PLATFORMTHEME=qt5ct qt5ct'
# fix for candidate dialog disappearing when using fcitx5 on
# native wayland app: just use xwayland...
alias xwayland='sommelier -X'
# I do not know why sublime will not display until
# add flag `--debug`
alias subl='sommelier -X subl --debug >/dev/null 2>&1 &'

# To customize prompt, run `p10k configure` or edit ~/.p10k.zsh.
[[ ! -f ~/.p10k.zsh ]] || source ~/.p10k.zsh
eval $(thefuck --alias)
