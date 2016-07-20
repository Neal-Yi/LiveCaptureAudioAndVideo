#coding:utf-8
import os
import os.path
import sys
# code file type set
codeFileTypes = {".cpp",
	".c",
	".h",
	".java",
	".cc",
	".py",
	".m"
	# add other file type below
}                         
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
def filetype(filename):
	return filename[filename.rfind('.'):]
	pass
def codeCount(rootdir):
	count = 0
	for parent,dirnames,filenames in os.walk(rootdir): 
			#三个参数：分别返回1.父目录 2.所有文件夹名字（不含路径） 3.所有文件名字

		for filename in filenames:                        #输出文件信息
			ft = filetype(filename)
			if ft in codeFileTypes:
				fileline = lineCount2(os.path.join(parent,filename))
				print os.path.join(parent,filename),fileline
				count += fileline
				pass
		pass
	pass
	return count
	pass
def main():
	count = 0
	for rootdir in sys.argv[1:]:
		count += codeCount(rootdir)
		pass
	print "total:",count
	pass
main()