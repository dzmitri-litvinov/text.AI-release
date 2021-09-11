all:
	g++ -O3 -s script.cpp -o script.cgi /usr/lib/x86_64-linux-gnu/libcgicc.a -I/usr/include/mysql -lmysqlclient
	g++ -O3 -s text.cpp -o text.cgi /usr/lib/x86_64-linux-gnu/libcgicc.a -I/usr/include/mysql -lmysqlclient
clean:
	rm -f script.cgi
	rm -f text.cgi
