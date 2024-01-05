/*
 * web_pages.h
 *
 *  Created on: 26 янв. 2023 г.
 *      Author: kir
 */

#ifndef DEFINITIONS_DATA_TYPES_WEB_PAGES_H_
#define DEFINITIONS_DATA_TYPES_WEB_PAGES_H_

/*************************************************************************************
 * HTML Pages (web pages)/*"<link rel=\"stylesheet\" href=\"styles.css\">\r\n"\
 *************************************************************************************/

#define index_page			"<!DOCTYPE html>\r\n"\
							"<html lang=\"ua\">\r\n"\
							"<meta charset=\"windows-1251\">\r\n"\
							"<head>\r\n"\
							"	<style>\r\n"\
							"		h1 {text-align: center;}\r\n"\
							"		h2 {text-align: center;}\r\n"\
							"		h3 {text-align: center;}\r\n"\
							"		p {text-align: center;}\r\n"\
							"		div {text-align: center;}\r\n"\
							"		a {text-align: center;}\r\n"\
							"	</style>\r\n"\
							"</head>\r\n"\
							"<body>\r\n"\
							"	<h2>Головна сторінка</h2>\r\n"\
							"	<h3>Модуль SHY-DI3-DO3</h3>\r\n"\
							"	<!-- <div><a href=\"settings_network\">Налаштування мережі</a></div> -->\r\n"\
							"	<div><a href=\"mqtt.html\">Налаштування MQTT</a></div>\r\n"\
							"	<div><a href=\"settings_MODBUS_TCP.html\">Налаштування ModbusTCP</a></div>\r\n"\
							"	<div><a href=\"settings_MQTT\">Інформація про модуль</a></div>\r\n"\
							"	<div><a href=\"restart.cgi\" target=\"_blank\">Перезавантаження</a></div>\r\n"\
							"	</body>\r\n"\
							"</html>\r\n"

#define conf_page_mqtt		"<!DOCTYPE html>\r\n"\
							"<html lang=\"ua\">\r\n"\
							"<head>\r\n"\
							"<meta charset=\"windows-1251\">\r\n"\
							"	<style>\r\n"\
							"		h2 {text-align: center;}\r\n"\
							"		form {text-align: center;}\r\n"\
							"	</style>\r\n"\
							"</head>\r\n"\
							"<body>\r\n"\
							"	<h2>Сторінка налаштувань функції MQTT</h2>\r\n"\
							"	<form action=\"settings_MQTT.cgi\" method=\"post\" target=\"_blank\">\r\n"\
							"	<fieldset>\r\n"\
							"		<legend>Налаштування:</legend>\r\n"\
							"			<div style=\"display:flex;flex-direction:row;justify-content:center\">\r\n"\
							"				<div style=\"display:flex;flex-direction:column;align-items:end;justify-content:space-between\">\r\n"\
							"					<label for=\"Enable\">Увімкнути функцію MQTT:</label>\r\n"\
							"					<br>\r\n"\
							"					<label for=\"login\">Логін:</label>\r\n"\
							"					<label for=\"pass\">Пароль:</label>\r\n"\
							"					<label for=\"ip_addr\">IP Адреса серверу:</label>\r\n"\
							"					<label for=\"port\">Порт  серверу :</label>\r\n"\
							"				</div>\r\n"\
							"				<div style=\"display:flex;flex-direction:column;margin-left:20px;width:200px\">\r\n"\
							"					<div style=\"display:flex;flex-direction:row\">\r\n"\
							"						<input type=\"radio\" name=\"MQTT\" value=\"on\" checked> Так \r\n"\
							"						<input type=\"radio\" name=\"MQTT\" value=\"off\"> Ні \r\n"\
							"					</div>\r\n"\
							"					<br>\r\n"\
							"					<input type=\"text\" id=\"login\" name=\"login\" maxlength=\"30\">\r\n"\
							"					<input type=\"password\" id=\"pass\" name=\"pass\" maxlength=\"30\">\r\n"\
							"					<input type=\"text\" id=\"ip_addr\" name=\"ip_addr\" minlength=\"7\" maxlength=\"15\" size=\"15\" pattern=\"^((\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.){3}(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])$\">\r\n"\
							"					<input type=\"number\" id=\"port\" name=\"port\" min=\"1\" max=\"65535\" placeholder = \"за замовченням 1883\" >\r\n"\
							"				</div>\r\n"\
							"			</div>\r\n"\
							"			<br>\r\n"\
							"			<input type=\"submit\" value=\"Застосувати\" >\r\n"\
							"	</fieldset>\r\n"\
							"	</form>\r\n"\
							"</body>\r\n"\
							"</html>\r\n"

#define setting_MQTT_page	"<!DOCTYPE html>\r\n"\
							"<html lang=\"ua\">\r\n"\
							"<head>\r\n"\
							"<meta charset=\"windows-1251\">\r\n"\
							"	<style>\r\n"\
							"		h2 {text-align: center;}\r\n"\
							"		form {text-align: center;}\r\n"\
							"	</style>\r\n"\
							"</head>\r\n"\
							"<body>\r\n"\
							"	<h2>Сторінка налаштувань функції MQTT</h2>\r\n"\
							"	<form action=\"settings_MQTT.cgi\" method=\"post\" target=\"_blank\">\r\n"\
							"	<fieldset>\r\n"\
							"		<legend>Налаштування:</legend>\r\n"\
							"			<div style=\"display:flex;flex-direction:row;justify-content:center\">\r\n"\
							"				<div style=\"display:flex;flex-direction:column;align-items:end;justify-content:space-between\">\r\n"\
							"					<label for=\"Enable\">Увімкнути функцію MQTT:</label>\r\n"\
							"					<br>\r\n"\
							"					<label for=\"login\">Логін:</label>\r\n"\
							"					<label for=\"pass\">Пароль:</label>\r\n"\
							"					<label for=\"ip_addr\">IP Адреса серверу:</label>\r\n"\
							"					<label for=\"port\">Порт  серверу :</label>\r\n"\
							"				</div>\r\n"\
							"				<div style=\"display:flex;flex-direction:column;margin-left:20px;width:200px\">\r\n"\
							"					<div style=\"display:flex;flex-direction:row\">\r\n"\
							"						<input type=\"radio\" name=\"MQTT\" value=\"on\" checked> Так \r\n"\
							"						<input type=\"radio\" name=\"MQTT\" value=\"off\"> Ні \r\n"\
							"					</div>\r\n"\
							"					<br>\r\n"\
							"					<input type=\"text\" id=\"login\" name=\"login\" maxlength=\"30\">\r\n"\
							"					<input type=\"password\" id=\"pass\" name=\"pass\" maxlength=\"30\">\r\n"\
							"					<input type=\"text\" id=\"ip_addr\" name=\"ip_addr\" minlength=\"7\" maxlength=\"15\" size=\"15\" pattern=\"^((\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.){3}(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])$\">\r\n"\
							"					<input type=\"number\" id=\"port\" name=\"port\" min=\"1\" max=\"65535\" placeholder = \"за замовченням 1883\" >\r\n"\
							"				</div>\r\n"\
							"			</div>\r\n"\
							"			<br>\r\n"\
							"			<input type=\"submit\" value=\"Застосувати\" >\r\n"\
							"	</fieldset>\r\n"\
							"	</form>\r\n"\
							"</body>\r\n"\
							"</html>\r\n"

#define settings_ModbusTCP_page "<!DOCTYPE html>\r\n"\
								"<html lang=\"ua\">\r\n"\
								"<meta charset=\"windows-1251\">\r\n"\
								"<head>\r\n"\
								"	<style>\r\n"\
								"		h2 {text-align: center;}\r\n"\
								"		form {text-align: center;}\r\n"\
								"	</style>\r\n"\
								"</head>\r\n"\
								"<body>\r\n"\
								"	<h2>Сторінка налаштувань функції ModbusTCP</h2>\r\n"\
								"	<form action=\"settings_ModbusTCP.cgi\" method=\"post\" target=\"_blank\">\r\n"\
								"	<fieldset>\r\n"\
								"		<legend>Налаштування:</legend>\r\n"\
								"			<label for=\"Enable\">Увімкнути функцію ModbusTCP:</label>\r\n"\
								"			<input type=\"radio\" name=\"ModbusTCP\" value=\"on\" checked> Так\r\n"\
								"			<input type=\"radio\" name=\"ModbusTCP\" value=\"off\"> Ні<br><br>\r\n"\
								"			<input type=\"submit\" value=\"Застосувати\" >\r\n"\
								"	</fieldset>\r\n"\
								"	</form>\r\n"\
								"</body>\r\n"\
								"</html>\r\n"

#define download_file_page		"<!DOCTYPE html>\r\n"\
								"	<html>\r\n"\
								"	  <head>\r\n"\
								"		<meta charset=\"utf-8\" />\r\n"\
								"		<title>Form</title>\r\n"\
								"	  </head>\r\n"\
								"	  <body>\r\n"\
								"		<form action=\"/action.cgi\" method=\"post\" enctype=\"multipart/form-data\">\r\n"\
								"		  <input type=\"text\" name=\"name\" placeholder=\"Name\" />\r\n"\
								"		  <input type=\"file\" name=\"profilePicture\" />\r\n"\
								"		  <input type=\"submit\" value=\"Submit\" />\r\n"\
								"		</form>\r\n"\
								"	  </body>\r\n"\
								"	</html>\r\n"\

#endif /* DEFINITIONS_DATA_TYPES_WEB_PAGES_H_ */
