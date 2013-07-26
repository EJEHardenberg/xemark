from wsgiref.util import setup_testing_defaults
from wsgiref.simple_server import make_server
import os 

def callXemark(fullpath):
	return os.popen("./xemark < %s" % fullpath).read()
	
def xemarkapp(environ, start_response):
	status = '200 OK'
	headers = [('Content-Type', 'text/html')]
	file_requested = environ['PATH_INFO']

	#Stop asking me for the /favicon, I don't have it
	if file_requested == "/favicon.ico":
		start_response(status,headers)
		return [""]

	#Get path and filename
	file_info = file_requested.rsplit(os.sep,1) #[path/to/the, file]

	#Append file extension
	if not file_info[1].endswith('.xe'):
		file_info[1] = "%s%s" % (file_info[1] , ".xe")

	response_string = ""
	file_path = "%s%s" % (".","/".join(file_info))
	print file_path
	if file_info[1] in os.listdir('.' + file_info[0]): 
		response_string = callXemark(file_path)
	else:
		status = '404 NOT FOUND'

	start_response(status, headers)
	return [response_string]

make_server('', 1337, xemarkapp).serve_forever()

