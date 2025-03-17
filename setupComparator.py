import os
import platform
import subprocess
import requests
import tarfile
import zipfile

def download_file(url, dest):
    response = requests.get(url, stream=True)
    with open(dest, 'wb') as file:
        for chunk in response.iter_content(chunk_size=8192):
            file.write(chunk)

def extract_zip(file, dest):
    with zipfile.ZipFile(file, 'r') as zip_ref:
        zip_ref.extractall(dest)

def extract_tar(file, dest):
    with tarfile.open(file, 'r:gz') as tar_ref:
        tar_ref.extractall(dest)

def main():
    os_name = platform.system()
    is_windows = os_name == "Windows"
    is_linux = os_name == "Linux"
    is_macos = os_name == "Darwin"

    if is_windows:
        cutechess_file = "cutechess-1.3.1-win64.zip"
    elif is_linux:
        cutechess_file = "cutechess-cli-1.3.1-linux64.tar.gz"
    elif is_macos:
        subprocess.run(["brew", "install", "qt"])
        os.makedirs("enginesComparator/cutechess", exist_ok=True)
        subprocess.run(["git", "clone", "https://github.com/cutechess/cutechess.git", "enginesComparator/cutechess"])
        os.makedirs("enginesComparator/cutechess/build", exist_ok=True)
        subprocess.run(["cmake", ".."], cwd="enginesComparator/cutechess/build")
        subprocess.run(["make"], cwd="enginesComparator/cutechess/build")
    else:
        print("Unsupported OS.")
        return

    if is_windows or is_linux:
        cutechess_url = f"https://github.com/cutechess/cutechess/releases/download/v1.3.1/{cutechess_file}"
        print(f"Downloading CuteChess ({cutechess_file}) from {cutechess_url}...")
        download_file(cutechess_url, cutechess_file)

        if cutechess_file.endswith(".zip"):
            extract_zip(cutechess_file, "enginesComparator/cutechess")
        elif cutechess_file.endswith(".tar.gz"):
            extract_tar(cutechess_file, "enginesComparator/cutechess")

        os.remove(cutechess_file)
        print("CuteChess 1.3.1 is ready to use.")

    if is_windows:
        stockfish_file = "stockfish-windows-x86-64.zip"
    elif is_linux:
        stockfish_file = "stockfish-ubuntu-x86-64.tar"
    elif is_macos:
        stockfish_file = "stockfish-macos-x86-64.tar"

    stockfish_url = f"https://github.com/official-stockfish/Stockfish/releases/download/sf_17/{stockfish_file}"
    print(f"Downloading Stockfish 17 ({stockfish_file}) from {stockfish_url}...")
    download_file(stockfish_url, stockfish_file)

    if stockfish_file.endswith(".zip"):
        extract_zip(stockfish_file, "enginesComparator/stockfish")
    elif stockfish_file.endswith(".tar"):
        subprocess.run(["tar", "--use-compress-program=unzstd", "-xf", stockfish_file, "-C", "enginesComparator/stockfish"])

    os.remove(stockfish_file)
    print("Stockfish 17 is ready to use.")

if __name__ == "__main__":
    main()