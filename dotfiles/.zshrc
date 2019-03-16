# You may need to manually set your language environment
export LANG=en_US.UTF-8

export TERM="xterm-256color"
# If you come from bash you might have to change your $PATH.
# export PATH=$HOME/bin:/usr/local/bin:$PATH

# Path to your oh-my-zsh installation.
export ZSH=/home/kevin/.oh-my-zsh

# Set name of the theme to load. Optionally, if you set this to "random"
# it'll load a random theme each time that oh-my-zsh is loaded.
# See https://github.com/robbyrussell/oh-my-zsh/wiki/Themes
# ZSH_THEME="agnoster"
ZSH_THEME="powerlevel9k"

# Uncomment the following line to use case-sensitive completion.
# CASE_SENSITIVE="true"

# Uncomment the following line to use hyphen-insensitive completion. Case
# sensitive completion must be off. _ and - will be interchangeable.
# HYPHEN_INSENSITIVE="true"

# Uncomment the following line to disable bi-weekly auto-update checks.
# DISABLE_AUTO_UPDATE="true"

# Uncomment the following line to change how often to auto-update (in days).
# export UPDATE_ZSH_DAYS=13

# Uncomment the following line to disable colors in ls.
# DISABLE_LS_COLORS="true"

# Uncomment the following line to disable auto-setting terminal title.
# DISABLE_AUTO_TITLE="true"

# Uncomment the following line to enable command auto-correction.
ENABLE_CORRECTION="true"

# Uncomment the following line to display red dots whilst waiting for completion.
# COMPLETION_WAITING_DOTS="true"

# Uncomment the following line if you want to disable marking untracked files
# under VCS as dirty. This makes repository status check for large repositories
# much, much faster.
# DISABLE_UNTRACKED_FILES_DIRTY="true"

# Uncomment the following line if you want to change the command execution time
# stamp shown in the history command output.
# The optional three formats: "mm/dd/yyyy"|"dd.mm.yyyy"|"yyyy-mm-dd"
# HIST_STAMPS="mm/dd/yyyy"
HIST_STAMPS="yyyy-mm-dd"

# Would you like to use another custom folder than $ZSH/custom?
# ZSH_CUSTOM=/path/to/new-custom-folder

# Which plugins would you like to load? (plugins can be found in ~/.oh-my-zsh/plugins/*)
# Custom plugins may be added to ~/.oh-my-zsh/custom/plugins/
# Example format: plugins=(rails git textmate ruby lighthouse)
# Add wisely, as too many plugins slow down shell startup.
#plugins=(git)
plugins=(vi-mode history-substring-search git autojump zsh-syntax-highlighting zsh-autosuggestions)

source $ZSH/oh-my-zsh.sh

# User configuration

# export MANPATH="/usr/local/man:$MANPATH"

# Preferred editor for local and remote sessions
# if [[ -n $SSH_CONNECTION ]]; then
#   export EDITOR='vim'
# else
#   export EDITOR='mvim'
# fi

# Compilation flags
# export ARCHFLAGS="-arch x86_64"

# ssh
# export SSH_KEY_PATH="~/.ssh/rsa_id"

# Set personal aliases, overriding those provided by oh-my-zsh libs,
# plugins, and themes. Aliases can be placed here, though oh-my-zsh
# users are encouraged to define aliases within the ZSH_CUSTOM folder.
# For a full list of active aliases, run `alias`.
#
# Example aliases
# alias zshconfig="mate ~/.zshrc"
# alias ohmyzsh="mate ~/.oh-my-zsh"
alias cnpm="npm --registry=https://registry.npm.taobao.org \
--cache=$HOME/.npm/.cache/cnpm \
--disturl=https://npm.taobao.org/dist \
--userconfig=$HOME/.cnpmrc"
alias aria2="cd /home/kevin && aria2c --conf-path=/home/kevin/.aria2.conf"
alias open="xdg-open"
alias sublime="subl"
alias psg="ps -aux|grep"
alias kills="sudo kill -s 9"
alias update="yay -Syu"
alias cl="clear"
alias yay="yay --aururl https://aur.tuna.tsinghua.edu.cn/"
alias gc="git clone"
# vnc server the current xorg desktop environment
alias vncX="x0vncserver -display :0 -passwordfile ~/.vnc/passwd"
# use privoxy to forward socks5 to http
alias sshttp="export https_proxy='http://127.0.0.1:8118' && export http_proxy='http://127.0.0.1:8118' && export HTTPS_PROXY='http://127.0.0.1:8118' && export HTTP_PROXY='http://127.0.0.1:8118' && git config --global http.proxy socks5://127.0.0.1:1080 && git config --global https.proxy socks5://127.0.0.1:1080"
alias unsshttp="unset all_proxy && unset ALL_PROXY && unset http_proxy && unset https_proxy && unset HTTP_PROXY && unset HTTPS_PROXY && git config --global --unset http.proxy && git config --global --unset https.proxy"
alias nvidia-setting='optirun -b none nvidia-settings -c :8'
alias cudaPython='optirun python'
# alias nvidiaChromeUnstable="sudo optirun -b primus google-chrome-unstable --no-sandbox"
alias nvidiaChrome="optirun -b primus google-chrome-stable --disable-gpu-sandbox"
alias enw='emacs -nw'
# fix filezilla carsh when using wayland
alias filezilla='GDK_BACKEND=x11 filezilla' 
# yaourt
export VISUAL="vim"
# acm run
alias acmRun='_acmRun(){ g++ -std=c++11 -o $1.o -Wall $1.cpp; ./$1.o > o.txt; subl o.txt&;}; _acmRun'
alias acmDbg='_acmDbg(){g++ -std=c++11 -o $1.o -Wall $1.cpp; gdb $1.o;}; _acmDbg'
# fast Merge localte(mlocate) !
alias mFind='sudo updatedb && locate'

# CUDA
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/cuda-8.0/opt/cuda/lib64"
export CUDA_HOME=/usr/local/cuda-8.0/opt/cuda
export PATH="$PATH:/home/kevin/go/bin:/usr/local/cuda-8.0/opt/cuda/bin:/home/kevin/Android/Sdk/platform-tools:$HOME/.cargo/bin:/home/kevin/.local/bin:/usr/local/texlive/2018/bin/x86_64-linux:/home/kevin/.gem/ruby/2.6.0/bin"
# fix qt application crash in gnome on wayland
export QT_QPA_PLATFORM="xcb"

# 给 man 添加漂亮的语法高亮
export LESS_TERMCAP_mb=$'\E[1m\E[32m'
export LESS_TERMCAP_mh=$'\E[2m'
export LESS_TERMCAP_mr=$'\E[7m'
export LESS_TERMCAP_md=$'\E[1m\E[36m'
export LESS_TERMCAP_ZW=""
export LESS_TERMCAP_us=$'\E[4m\E[1m\E[37m'
export LESS_TERMCAP_me=$'\E(B\E[m'
export LESS_TERMCAP_ue=$'\E[24m\E(B\E[m'
export LESS_TERMCAP_ZO=""
export LESS_TERMCAP_ZN=""
export LESS_TERMCAP_se=$'\E[27m\E(B\E[m'
export LESS_TERMCAP_ZV=""
export LESS_TERMCAP_so=$'\E[1m\E[33m\E[44m'

# fix AVD manager
export ANDROID_EMULATOR_USE_SYSTEM_LIBS=1
 
eval $(thefuck --alias)
# You can use whatever you want as an alias, like for Mondays:
eval $(thefuck --alias FUCK)

# powerline
export TERM='xterm-256color'

POWERLINE_SCRIPT=/usr/share/powerline/bindings/bash/powerline.sh
if [ -f $POWERLINE_SCRIPT ]; then
  source $POWERLINE_SCRIPT
fi

if [ $TILIX_ID ] || [ $VTE_VERSION ]; then
        source /etc/profile.d/vte.sh
fi

# Fbtrem auto exec after login in tty console
# LANG=zh_CN.UTF-8 fbterm
# case "$TERM" in
#    xterm*')
#        if [ -e /usr/share/terminfo/x/xterm-256color ]; then
#            export TERM=xterm-256color
#        elif [ -e /usr/share/terminfo/x/xterm-color ]; then
#            export TERM=xterm-color;
#        else
#            export TERM=xterm
#        fi
#        ;;
#    linux)
#        [ -n "$FBTERM" ] && export TERM=fbterm
#        ;;
# esac

# Flash
export MOZ_PLUGIN_PATH="/usr/lib/mozilla/plugins"

#THIS MUST BE AT THE END OF THE FILE FOR SDKMAN TO WORK!!!
export SDKMAN_DIR="/home/kevin/.sdkman"
[[ -s "/home/kevin/.sdkman/bin/sdkman-init.sh" ]] && source "/home/kevin/.sdkman/bin/sdkman-init.sh"

# set zsh to vi mode
set -o vi

# use hfs+ disk partition as data exchange partition among ArchLinux, macOS and Windows.
function hfs_disk () { 
	if mount | grep /mnt/hfs_disk > /dev/null; then
		sudo umount /dev/hfs_disk
	fi
	sudo mkdir -p /mnt/hfs_disk && sudo sudo mount -t hfsplus -o force --uuid 8b199521-1001-3e27-a635-dc65658c059c /mnt/hfs_disk && sudo mount -t hfsplus -o remount,force,rw --uuid 8b199521-1001-3e27-a635-dc65658c059c /mnt/hfs_disk 
}

# wrap these commands for interactive use to avoid accidental overwrites.
function rm() { command rm -i "$@"; }
function cp() { command cp -i "$@"; }
function mv() { command mv -i "$@"; }

# set qt5 theme as `qgnomeplatform` if running GNOME
if [ "$XDG_CURRENT_DESKTOP" = "GNOME" ]; then
	export QT_QPA_PLATFORMTHEME='gnome'
fi
# fix krunner not autostart in kde
if [[ "$XDG_CURRENT_DESKTOP" == "KDE" && `pgrep krunner` == '' ]]; then
	nohup krunner &
fi

# fix deepin-wine BadWindow error in KDE and other non-gnome DE
if [[ "$XDG_CURRENT_DESKTOP" == "KDE" && `pgrep gsd-xsettings` == '' ]]; then
	nohup /usr/lib/gsd-xsettings &
fi
# replace chromium icons to chrome style for Paper-icon-theme-git and chromium
function replace_chromium_icons () {
	for i in `yay -Ql paper-icon-theme-git | grep 'chromium.*\.png$'`; if [ $i != 'paper-icon-theme-git' ]; then sudo cp -rvf ${i%%chromium*.png}'google-chrome.png' ${i}; fi
}
# for lua packages installed locally by luarocks
eval "$(luarocks path)"
