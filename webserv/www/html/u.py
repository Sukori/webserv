import os, sys
print("hello world from python cgi")

print("\n=[env]===============")
for k in os.environ:
	print(k, "=", os.environ[k])
print("=====================")

print("\n=[body]==============")
if (os.getenv("CONTENT_LENGTH") is not None):
	print(sys.stdin.read(int(os.getenv("CONTENT_LENGTH"))))
print("=====================")