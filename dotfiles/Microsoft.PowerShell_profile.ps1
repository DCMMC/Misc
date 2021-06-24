# This file is located at ~\Documents\PowerShell\Microsoft.PowerShell_profile.ps1 ($PROFILE)

# Install-Module posh-git -Scope CurrentUser
# Install-Module oh-my-posh -Scope CurrentUser
# 引入 posh-git
Import-Module posh-git
# 引入 oh-my-posh
Import-Module oh-my-posh
Import-Module PSReadLine

# 设置主题为 Paradox
# Set-Theme Paradox
# https://github.com/microsoft/terminal/issues/9237#issuecomment-798913706
Set-PoshPrompt Paradox

# 设置预测文本来源为历史记录
Set-PSReadLineOption -PredictionSource History
Set-PSReadLineKeyHandler -Key UpArrow -Function HistorySearchBackward
Set-PSReadLineKeyHandler -Key DownArrow -Function HistorySearchForward
# Fish-like autosuggestions
# Ref: https://github.com/PowerShell/PSReadLine/issues/687
# `powershell -noni -nop -c "iwr -useb https://git.io/InstallPSReadlineAutocompleteBeta.ps1 | iex"`

# Set-PSReadlineKeyHandler -Key Tab -Function Complete # 设置 Tab 键补全
# 设置 Ctrl+d 为菜单补全和 Intellisense
Set-PSReadLineKeyHandler -Key Tab -Function MenuComplete
# 设置 Ctrl+z 为撤销
Set-PSReadLineKeyHandler -Key "Ctrl+z" -Function Undo
# 设置向上键为后向搜索历史记录
Set-PSReadLineKeyHandler -Key UpArrow -Function HistorySearchBackward
# 设置向下键为前向搜索历史纪录
Set-PSReadLineKeyHandler -Key DownArrow -Function HistorySearchForward

# Proxy
[system.net.webrequest]::DefaultWebProxy = new-object system.net.webproxy('http://127.0.0.1:1080')
# If you need to import proxy settings from Internet Explorer, you can replace the previous line with the: "netsh winhttp import proxy source=ie"
[system.net.webrequest]::DefaultWebProxy.credentials = [System.Net.CredentialCache]::DefaultNetworkCredentials
# You can request user credentials:
# System.Net.WebRequest]::DefaultWebProxy.Credentials = Get-Credential
# Also, you can get the user password from a saved XML file (see the article Using saved credentials in PowerShell scripts):
# System.Net.WebRequest]::DefaultWebProxy= Import-Clixml -Path C:\PS\user_creds.xml
[system.net.webrequest]::DefaultWebProxy.BypassProxyOnLocal = $true
# Check proxy using Get-ItemProperty -Path 'HKCU:\Software\Microsoft\Windows\CurrentVersion\Internet Settings' | Select-Object ProxyServer, ProxyEnable
