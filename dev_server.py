from http.server import test, SimpleHTTPRequestHandler
import os

from generate import SITE_FOLDER


def main():
    pwd = os.getcwd()
    try:
        os.chdir(SITE_FOLDER)  # or any path you like
        test(SimpleHTTPRequestHandler)
    finally:
        os.chdir(pwd)


if __name__ == "__main__":
    main()
