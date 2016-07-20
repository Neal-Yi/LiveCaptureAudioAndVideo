#coding:utf-8
import sys
import time
def lineCount(filename):
	with open(filename) as fp:
		return sum(1 for i in fp)
	pass

def lineCount2(filename):
	thefile = open(filename)  
  	count = 0

  	while True:  
  		buffer = thefile.read(1<<16)  
		if not buffer:  
			break  
		count += buffer.count('\n')  
	thefile.close()
	# 最后一行没有回车符
	if count:
		count = count + 1
	 	pass 
	return count
	pass
def main():
	filename =  sys.argv[1]
	print lineCount2(filename)
	pass
main()