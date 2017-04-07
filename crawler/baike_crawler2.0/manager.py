
class manager(object):
	def __init__(self):
		self.new_url=set()
		self.old_url=set()
	def add_url(self,url):
		if url == None:
			return 
		elif url not in self.new_url and url not in self.old_url:
			self.new_url.add(url)

	def add_urls(self,urls):
		if urls is None or len(urls) == 0:
			return
		for x in urls:
			self.add_url(x)
	def has_new_url(self):
		return len(self.new_url) != 0
	def get_url(self):
		ret=self.new_url.pop()
		self.old_url.add(ret)
		return ret

