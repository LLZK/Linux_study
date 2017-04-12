# -*- coding: utf-8 -*-

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: http://doc.scrapy.org/en/latest/topics/item-pipeline.html

import json

class MycrawPipeline(object):
	def __init__(self):
		self.file = open('mycraw.txt',mode='wb')

    def process_item(self, item, spider):
		self.file.write(item['news_title'].encode('GBK'))
		self.file.write("\n")
		self.file.write(item['news_date'].encode('GBK'))
		self.file.write("\n")
		self.file.write(item['news_url'].encode('GBK'))
		self.file.write("\n")
        return item
