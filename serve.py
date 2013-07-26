from wsgiref.util import setup_testing_defaults
from wsgiref.simple_server import make_server
import os 

def callXemark():
   	#p = subprocess.Popen('./xemark < example.xe', shell=True, stdout=subprocess.PIPE)
	return os.popen("./xemark < example.xe").read()
    	return p.stdout.readlines() 

def xemarkapp(environ, start_response):
    status = '200 OK'
    headers = [('Content-Type', 'text/html')]
    start_response(status, headers)
    return [callXemark()]

make_server('', 1337, xemarkapp).serve_forever()
