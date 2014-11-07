weather
=======

Send temperature, humidity and water level from your raspberry pi and DHT22 sensor to Database.<br><br>

Created and tested for Raspbian OS.<br><br>

Written on C.<br>
Author Sergey LittleBuster Denisov.<br>
Email DenisovS21@gmail.com.<br>
License GNU GPLv3.<br>
Version: 0.4.<br><br>
<img src="http://s017.radikal.ru/i443/1411/32/23b96d5622e4.jpg" width="400" />
<br>

Dependences:
````
apt-get install libjansson4 libjansson-dev libmysqlclient18 libmysqlclient-dev
````
bcm2835 driver installation:
````
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.37.tar.gz
tar zxvf bcm2835-1.xx.tar.gz
cd tar zxvf bcm2835-1.xx
./configure
make
make install
````
To compile and install DHT driver and project print:
````
git clone https://github.com/LittleBuster/weather.git
cd weather
make
````
In folder 'weather' you may find 'weather' ELF file for works.