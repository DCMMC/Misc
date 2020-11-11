# If you come from bash you might have to change your $PATH.
# export PATH=$HOME/bin:/usr/local/bin:$PATH
export LANG="en_US.UTF-8"

cowsay 'Welcome, DCMMC!'

# Path to your oh-my-zsh installation.
export ZSH=/usr/share/oh-my-zsh

# Set name of the theme to load --- if set to "random", it will
# load a random theme each time oh-my-zsh is loaded, in which case,
# to know which specific one was loaded, run: echo $RANDOM_THEME
# See https://github.com/robbyrussell/oh-my-zsh/wiki/Themes
ZSH_THEME="agnoster"

# Set list of themes to pick from when loading at random
# Setting this variable when ZSH_THEME=random will cause zsh to load
# a theme from this variable instead of looking in ~/.oh-my-zsh/themes/
# If set to an empty array, this variable will have no effect.
# ZSH_THEME_RANDOM_CANDIDATES=( "robbyrussell" "agnoster" )

# Uncomment the following line to use case-sensitive completion.
# CASE_SENSITIVE="true"

# Uncomment the following line to use hyphen-insensitive completion.
# Case-sensitive completion must be off. _ and - will be interchangeable.
# HYPHEN_INSENSITIVE="true"

# Uncomment the following line to disable bi-weekly auto-update checks.
DISABLE_AUTO_UPDATE="true"

# Uncomment the following line to automatically update without prompting.
# DISABLE_UPDATE_PROMPT="true"

# Uncomment the following line to change how often to auto-update (in days).
# export UPDATE_ZSH_DAYS=13

# Uncomment the following line if pasting URLs and other text is messed up.
# DISABLE_MAGIC_FUNCTIONS=true

# Uncomment the following line to disable colors in ls.
# DISABLE_LS_COLORS="true"

# Uncomment the following line to disable auto-setting terminal title.
# DISABLE_AUTO_TITLE="true"

# Uncomment the following line to enable command auto-correction.
# ENABLE_CORRECTION="true"

# Uncomment the following line to display red dots whilst waiting for completion.
# COMPLETION_WAITING_DOTS="true"

# Uncomment the following line if you want to disable marking untracked files
# under VCS as dirty. This makes repository status check for large repositories
# much, much faster.
# DISABLE_UNTRACKED_FILES_DIRTY="true"

# Uncomment the following line if you want to change the command execution time
# stamp shown in the history command output.
# You can set one of the optional three formats:
# "mm/dd/yyyy"|"dd.mm.yyyy"|"yyyy-mm-dd"
# or set a custom format using the strftime function format specifications,
# see 'man strftime' for details.
# HIST_STAMPS="mm/dd/yyyy"

# Would you like to use another custom folder than $ZSH/custom?
# ZSH_CUSTOM=/path/to/new-custom-folder

# Which plugins would you like to load?
# Standard plugins can be found in ~/.oh-my-zsh/plugins/*
# Custom plugins may be added to ~/.oh-my-zsh/custom/plugins/
# Example format: plugins=(rails git textmate ruby lighthouse)
# Add wisely, as too many plugins slow down shell startup.
plugins=(git history-substring-search)
source /usr/share/zsh/plugins/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh
source /usr/share/zsh/plugins/zsh-autosuggestions/zsh-autosuggestions.zsh


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
