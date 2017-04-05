import urllib2

class downloader(object):
	
	def download(self,url):
		if url == None:
			return
		ret = urllib2.urlopen(url)
		if ret.getcode != 200:
			return None
		else:
			return ret.read()

