import os, sys
print("hello world from python cgi")
print(os.environ)

if (os.getenv("CONTENT_LENGTH") is not None):
	print(sys.stdin.read(int(os.getenv("CONTENT_LENGTH"))))