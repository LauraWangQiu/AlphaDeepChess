$OS = $env:OS
$IsMacOS = ($env:OSTYPE -like "*darwin*")
$IsLinux = ($env:OSTYPE -like "*linux*")

if ($OS -like "*Windows*") {
    $CUTECHESS_FILE = "cutechess-1.3.1-win64.zip"
} elseif ($IsMacOS) {
    $CUTECHESS_FILE = "Cute_Chess-1.3.1-x86_64.AppImage"
} else {
    $CUTECHESS_FILE = "cutechess-cli-1.3.1-linux64.tar.gz"
}

$CUTECHESS_URL = "https://github.com/cutechess/cutechess/releases/download/v1.3.1/$CUTECHESS_FILE"

Write-Host "Downloading CuteChess ($CUTECHESS_FILE) desde $CUTECHESS_URL..."
Invoke-WebRequest -Uri $CUTECHESS_URL -OutFile $CUTECHESS_FILE

if ($CUTECHESS_FILE -like "*.zip") {
    Expand-Archive -Path $CUTECHESS_FILE -DestinationPath "comparative/cutechess" -Force
} elseif ($CUTECHESS_FILE -like "*.tar.gz") {
    tar -xzf $CUTECHESS_FILE -C "comparative/cutechess"
} elseif ($CUTECHESS_FILE -like "*.AppImage") {
    chmod +x $CUTECHESS_FILE
    Move-Item -Path $CUTECHESS_FILE -Destination "comparative/cutechess/"
}

Remove-Item -Path $CUTECHESS_FILE -Force
Write-Host "CuteChess 1.3.1 is ready to use."

if ($OS -like "*Windows*") {
    $STOCKFISH_FILE = "stockfish-windows-x86-64.zip"
} elseif ($IsMacOS) {
    $STOCKFISH_FILE = "stockfish-macos-x86-64.tar"
} else {
    $STOCKFISH_FILE = "stockfish-ubuntu-x86-64.tar"
}

$STOCKFISH_URL = "https://github.com/official-stockfish/Stockfish/releases/download/sf_17/$STOCKFISH_FILE"

Write-Host "Downloading Stockfish 17 ($STOCKFISH_FILE) desde $STOCKFISH_URL..."
Invoke-WebRequest -Uri $STOCKFISH_URL -OutFile $STOCKFISH_FILE

if ($STOCKFISH_FILE -like "*.zip") {
    Expand-Archive -Path $STOCKFISH_FILE -DestinationPath "comparative/stockfish" -Force
} elseif ($STOCKFISH_FILE -like "*.tar") {
    tar --use-compress-program=unzstd -xf $STOCKFISH_FILE -C "comparative/stockfish"
}

Remove-Item -Path $STOCKFISH_FILE -Force
Write-Host "Stockfish 17 is ready to use."
