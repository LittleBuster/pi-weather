// weather ap is used for reading temperature and humidity from dht22 sensor
// on raspberry pi B version with Raspbian OS and add to database.
//
// Copyright © 20012-2014 by Sergey Denisov aka 'LittleBuster'
// E-Mail: DenisovS21 at gmail dor com (DenisovS21@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>

#ifndef LOG_LOCAL_H
#define LOG_LOCAL_H

#define LOG_WARNING 1
#define LOG_ERROR 2


void log( const char *message, int type );

#endif