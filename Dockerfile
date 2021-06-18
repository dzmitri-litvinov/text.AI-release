FROM ubuntu:18.04
RUN apt update -y
RUN apt install apache2 -y
RUN apt install g++ -y
RUN apt install libcgicc3 -y
RUN apt install libcgicc-dev -y
RUN apt install make -y

copy index.html /var/www/html/
copy conf/apache2.conf /etc/apache2/apache2.conf
copy conf/ports.conf /etc/apache2/ports.conf
copy conf/cgid.conf /etc/apache2/mods-enabled/
copy conf/cgid.load /etc/apache2/mods-enabled/
copy conf/cgi.load /etc/apache2/mods-enabled/
copy src/serverRequest.js /var/www/html/src/serverRequest.js
copy src/index.css /var/www/html/src/index.css

copy favicon.ico /var/www/html/favicon.ico
copy src/particles.js /var/www/html/src/particles.js
copy src/app.js /var/www/html/src/app.js 
copy src/select.js /var/www/html/src/select.js

copy src/arrow-bottom.jpg /var/www/html/src/arrow-bottom.jpg
copy src/arrow-top.jpg /var/www/html/src/arrow-top.jpg
copy src/body.png /var/www/html/src/body.png
copy src/facebook.png /var/www/html/src/facebook.png
copy src/github.png /var/www/html/src/github.png
copy src/logo.png /var/www/html/src/logo.png
copy src/main.webp /var/www/html/src/main.webp
copy src/misprint.png /var/www/html/src/misprint.png
copy src/repetition.png /var/www/html/src/repetition.png
copy src/uniqueness.png /var/www/html/src/uniqueness.png
copy src/validation.png /var/www/html/src/validation.png
copy src/vk.png /var/www/html/src/vk.png

RUN mkdir /var/www/cgi-bin
copy Makefile /var/www/cgi-bin/
copy script.cpp /var/www/cgi-bin/
copy db.txt /var/www/cgi-bin/db.txt
WORKDIR /var/www/cgi-bin
RUN make
RUN chmod 755 script.cgi
RUN rm Makefile script.cpp
CMD apache2ctl -D FOREGROUND