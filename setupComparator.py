import os
import platform
import subprocess
import requests
import tarfile
import zipfile
import argparse

default_comparator_dir = "enginesComparator"

os_name = platform.system()
is_windows = os_name == "Windows"
is_linux = os_name == "Linux"
is_macos = os_name == "Darwin"

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

def download_cutechess():
    if is_windows:
        cutechess_file = "cutechess-1.3.1-win64.zip"
    elif is_linux:
        cutechess_file = "cutechess-cli-1.3.1-linux64.tar.gz"
    elif is_macos:
        subprocess.run(["brew", "install", "qt"])
        os.makedirs(f"{default_comparator_dir}/cutechess", exist_ok=True)
        subprocess.run(["git", "clone", "https://github.com/cutechess/cutechess.git", f"{default_comparator_dir}/cutechess"])
        os.makedirs(f"{default_comparator_dir}/cutechess/build", exist_ok=True)
        subprocess.run(["cmake", ".."], cwd=f"{default_comparator_dir}/cutechess/build")
        subprocess.run(["make"], cwd=f"{default_comparator_dir}/cutechess/build")
    else:
        print("Unsupported OS.")
        return

    if is_windows or is_linux:
        cutechess_url = f"https://github.com/cutechess/cutechess/releases/download/v1.3.1/{cutechess_file}"
        print(f"Downloading CuteChess ({cutechess_file}) from {cutechess_url}...")
        download_file(cutechess_url, cutechess_file)

        if cutechess_file.endswith(".zip"):
            extract_zip(cutechess_file, f"{default_comparator_dir}/cutechess")
        elif cutechess_file.endswith(".tar.gz"):
            extract_tar(cutechess_file, f"{default_comparator_dir}/cutechess")

        os.remove(cutechess_file)
        print("CuteChess 1.3.1 is ready to use.")

def download_stockfish():
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
        extract_zip(stockfish_file, f"{default_comparator_dir}/stockfish")
    elif stockfish_file.endswith(".tar"):
        os.makedirs(f"{default_comparator_dir}/stockfish", exist_ok=True)
        subprocess.run(["tar", "-xf", stockfish_file, "-C", f"{default_comparator_dir}/stockfish"])
        
    os.remove(stockfish_file)
    print("Stockfish 17 is ready to use.")

def main():
    download_cutechess()

    parser = argparse.ArgumentParser(description="Setup comparator environment.")
    parser.add_argument("--download-stockfish", action="store_true", help="Download Stockfish engine")
    args = parser.parse_args()
    if args.download_stockfish:
        download_stockfish()

if __name__ == "__main__":
    main()