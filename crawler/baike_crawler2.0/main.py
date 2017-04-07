import downloader
import htmlParser
import manager
import printer

class SpiderMain(object):
	def __init__(self):
		self.urls = manager.manager()
		self.downloader = downloader.downloader()
		self.parser=htmlParser.htmlParser()
		self.printer = printer.printer()

	def craw(self,root_url):
		print "start..."
		self.urls.add_url(root_url)
		print "add root url ..."
		new_urls=set()
		url_data={}
		count = 0
		while self.urls.has_new_url() and count < 5:
			new_url=self.urls.get_url()
			print new_url
			html_cont=self.downloader.download(new_url)
			count = count+1
			print count,new_url
			new_urls,url_data = self.parser.parse(new_url,html_cont)
			self.urls.add_urls(new_urls)
			self.printer.collect_data(url_data)
			print "done ...",count

		self.printer.output_html()
		


      
if __name__=="__main__":
	root_url="http://baike.baidu.com/link?url=N3Spy0UyxYYxODZ8kWfs1ZVgVWuBRKWa_hpEEhjP5NslkjSe0yHqH6bZ-AaTEWu-cz7NJoXfBsYvQIktzAECUct1AY0M8SWwhiQfPPfiVtXFm8gyB-W0Laupe7UUUMAKOflNO4XnNHBF60D41_Iora"
	
	obj_spider = SpiderMain()
	obj_spider.craw(root_url)
	
