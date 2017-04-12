
import scrapy
from mycraw.items import MycrawItem
import logging

class mycrawSpider(scrapy.Spider):
	name="mycraw"
	allowed_domains = ["sust.edu.cn"]
	start_urls = ["http://news.www.sust.edu.cn/index/zhyw/2.htm"]

	def parse(self,response):
		page_num=195
		page_news_num = 58;

		if response.status == 200:
			for i in range(3,page_num+1):
				for j in range(1,page_news_num+1):
					item = MycrawItem()
					item['news_url'],item['news_title'],item['news_date'] = response.xpath("//div[@class='lanmu-main']/div[@class='lanmu-left fl']/ul[@class='lanmu-list']/li["str(j)"]/div/a/@href" "|//div[@class='lanmu-main']/div[@class='lanmu-left fl']/ul[@class='lanmu-list']/li["str(j)"]/div/a/text()" "|//div[@class='lanmu-main']/div[@class='lanmu-left fl']/ul[@class='lanmu-list']/li["str(j)"]/span/text()").extract()

					yield item

			next_page_url = "http://news.www.sust.edu.cn/index/zhyw/"+str(i)+".htm"
			yield scrapy.Request(next_page_url,callback=self.parse_news)

	def parse_news(self,response):
		page_news_num = 58;
		if response.status == 200:
			for j in range(1,page_news_num+1):
				item = MycrawItem()
				item['news_url'],item['news_title'],item['news_date'] = response.xpath("//div[@class='lanmu-main']/div[@class='lanmu-left fl']/ul[@class='lanmu-list']/li["str(j)"]/div/a/@href" "|//div[@class='lanmu-main']/div[@class='lanmu-left fl']/ul[@class='lanmu-list']/li["str(j)"]/div/a/text()" "|//div[@class='lanmu-main']/div[@class='lanmu-left fl']/ul[@class='lanmu-list']/li["str(j)"]/span/text()").extract()
				yield item


