import urllib2

class downloader(object):
	def download(self,url):
		if url == None:
			return None
		ret = urllib2.urlopen(url.encode("utf-8"))
		if ret.getcode() != 200:
			return None
		print ret.getcode()

		return ret.read()

