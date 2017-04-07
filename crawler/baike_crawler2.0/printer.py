
class printer(object):

	def __init__(self):
		self.datas = []
	def collect_data(self,data):
		if data is None:
			return 
		self.datas.append(data)
	
	def output_html(self):
		fout=open("crawbaike.html",'w')

		fout.write("<html>")
		
		fout.write("<body>")
		fout.write("<table>")

		for x in self.datas:
			fout.write("<tr>")

			fout.write("<td>%s</td>" % x['url'].encode('utf-8'))
			fout.write("<td>%s</td>" % x['title'].encode('utf-8'))
			fout.write("<td>%s</td>" % x['summ'].encode('utf-8'))

			fout.write("</tr>")

		fout.write("</table>")
		fout.write("</body>")
		fout.write("</html>")

		fout.close()


