# 番茄钟打包脚本
Write-Host "开始打包番茄钟应用..." -ForegroundColor Green

# 设置路径
$projectRoot = $PSScriptRoot
$deployDir = Join-Path $projectRoot "deploy"
$exePath = Join-Path $projectRoot "build\windows\x64\release\pomodoro.exe"

# 清理并创建发布目录
Write-Host "准备发布目录..." -ForegroundColor Yellow
if (Test-Path $deployDir) {
    Remove-Item -Recurse -Force $deployDir
}
New-Item -ItemType Directory -Force -Path $deployDir | Out-Null

# 复制exe文件
Write-Host "复制可执行文件..." -ForegroundColor Yellow
if (Test-Path $exePath) {
    Copy-Item $exePath $deployDir
    Write-Host "✓ 已复制 pomodoro.exe" -ForegroundColor Green
} else {
    Write-Host "✗ 找不到编译好的exe文件，请先运行 xmake build" -ForegroundColor Red
    exit 1
}

# 尝试查找Qt安装路径
Write-Host "查找Qt依赖..." -ForegroundColor Yellow
$qtPaths = @(
    "C:\Qt\6.8.0\msvc2022_64\bin",
    "C:\Qt\6.7.0\msvc2022_64\bin",
    "C:\Qt\6.6.0\msvc2022_64\bin",
    "C:\Qt\6.5.0\msvc2022_64\bin",
    "C:\Qt6\6.8.0\msvc2022_64\bin",
    "C:\Qt6\6.7.0\msvc2022_64\bin"
)

$qtBinPath = $null
foreach ($path in $qtPaths) {
    if (Test-Path $path) {
        $qtBinPath = $path
        Write-Host "✓ 找到Qt: $qtBinPath" -ForegroundColor Green
        break
    }
}

if ($qtBinPath) {
    # 使用windeployqt
    $windeployqt = Join-Path $qtBinPath "windeployqt.exe"
    if (Test-Path $windeployqt) {
        Write-Host "使用windeployqt收集依赖..." -ForegroundColor Yellow
        & $windeployqt --release --no-translations --no-system-d3d-compiler --no-opengl-sw (Join-Path $deployDir "pomodoro.exe")
        Write-Host "✓ 依赖收集完成" -ForegroundColor Green
    } else {
        Write-Host "⚠ 找不到windeployqt，尝试手动复制关键DLL..." -ForegroundColor Yellow
        
        # 手动复制必要的Qt DLL
        $qtDlls = @(
            "Qt6Core.dll",
            "Qt6Gui.dll",
            "Qt6Widgets.dll"
        )
        
        foreach ($dll in $qtDlls) {
            $dllPath = Join-Path $qtBinPath $dll
            if (Test-Path $dllPath) {
                Copy-Item $dllPath $deployDir
                Write-Host "  ✓ $dll" -ForegroundColor Gray
            }
        }
        
        # 复制platforms插件
        $platformsDir = Join-Path $deployDir "platforms"
        New-Item -ItemType Directory -Force -Path $platformsDir | Out-Null
        $qwindowsDll = Join-Path (Split-Path $qtBinPath) "plugins\platforms\qwindows.dll"
        if (Test-Path $qwindowsDll) {
            Copy-Item $qwindowsDll $platformsDir
            Write-Host "  ✓ qwindows.dll" -ForegroundColor Gray
        }
    }
} else {
    Write-Host "⚠ 未找到Qt安装，程序可能需要在目标机器上安装Qt运行时" -ForegroundColor Yellow
}

# 创建README
$readmePath = Join-Path $deployDir "使用说明.txt"
@"
番茄钟计时器 v1.0
==================

使用方法：
1. 双击 pomodoro.exe 运行程序
2. 程序会以迷你窗口模式启动
3. 点击"⋮"按钮可切换模式和访问更多功能

功能说明：
• 番茄钟模式：经典25分钟工作计时
• 间隔模式：记录距上次记录的时间间隔
• 可编辑任务标题，提醒当前任务
• 记录时间功能，保存工作日志

技术支持：
如有问题，请联系开发者

"@ | Out-File -FilePath $readmePath -Encoding UTF8

Write-Host "✓ 已创建使用说明" -ForegroundColor Green

# 创建压缩包
Write-Host "创建压缩包..." -ForegroundColor Yellow
$zipPath = Join-Path $projectRoot "番茄钟_v1.0_便携版.zip"
if (Test-Path $zipPath) {
    Remove-Item $zipPath
}

Compress-Archive -Path "$deployDir\*" -DestinationPath $zipPath
Write-Host "✓ 压缩包已创建: 番茄钟_v1.0_便携版.zip" -ForegroundColor Green

# 显示文件大小
$zipSize = (Get-Item $zipPath).Length / 1MB
Write-Host ""
Write-Host "打包完成！" -ForegroundColor Green
Write-Host "文件位置: $zipPath" -ForegroundColor Cyan
Write-Host "文件大小: $([math]::Round($zipSize, 2)) MB" -ForegroundColor Cyan
Write-Host ""
Write-Host "deploy目录包含解压后的所有文件，可直接测试运行" -ForegroundColor Yellow
