
#include "httpClient.h"
#include "spi_fs.h"

//lfs_t lfs;
//lfs_file_t file;
//struct lfs_config cfg;
//lfs_dir_t dir;



/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t * httpc_send_buf;
uint8_t * httpc_recv_buf;
//HttpRequest request = HttpRequest_multipart_post_initializer;
HttpRequest request = HttpRequest_get_initializer;

//static int8_t httpsock = 0;
//static uint8_t dest_ip[4] = {0, };
//static uint16_t dest_port = 0;
//static uint16_t httpc_any_port = 0;

uint8_t httpc_isSockOpen = HTTPC_FALSE;
uint8_t httpc_isConnected = HTTPC_FALSE;
uint16_t httpc_isReceived = HTTPC_FALSE;

/* Private functions prototypes ----------------------------------------------*/
uint16_t get_httpc_any_port(void);


void def_delay_cb(uint32_t sec)	{};

void (*delay_ms)(uint32_t sec) = def_delay_cb;

void func_pack_http_get_download_header(uint8_t* buff, uint16_t size_buff, uint8_t* srv_ip, uint8_t * domain_name, uint16_t srv_port, char* interface);


uint8_t buf[2048];

/* Public & Private functions ------------------------------------------------*/

void reg_http_cliend_delay_cb( void(*function)(uint32_t sec) ){

	if (function ){
		delay_ms = function;
	}
}

////////--------------------------------------second lib ---------------------------------
uint8_t func_http_get_download_file(uint8_t sockno, uint8_t * domain_name, uint8_t* srv_ip, uint16_t srv_port, char* interface, uint16_t timeout_ms, uint8_t* buff, uint16_t size_buff, char* filename, uint32_t max_file_size){
	uint16_t local_port = 60000, len = 0, cnt = 0;
	uint8_t sendok = 0, recv_start = 0;

	//uint8_t lfs_file_buf[256];
	//uint8_t filename[20];
	//static struct lfs_file_config uploadFileConfig;
	//memset(&uploadFileConfig, 0, sizeof(struct lfs_file_config));
	//uploadFileConfig.buffer = lfs_file_buf;  // use the static buffer
	//uploadFileConfig.attr_count = 0;

	uint8_t cache[2048];
	uint16_t cache_cont_len;
	uint32_t cont_len, download_cont_len;

	uint8_t debug_buff[50];
	int32_t write_status;
	//wiz_NetTimeout gWIZNETTIME = {.retry_cnt = 3,         //RCR = 3
	//                               .time_100us = 2000};     //RTR = 200

//	memcpy(filename, interface + 1, strlen(interface) -1 );

//-----------------------------------FLASH--------------------------
	int err;
//-----------------------------------FLASH--------------------------
	func_pack_http_get_download_header(buff, size_buff, srv_ip, domain_name, srv_port, interface);


	IINCHIP_WRITE(Sn_KPALVTR(sockno),0x00);//manual

	memset(cache, 0 , sizeof(cache));
	cache_cont_len = 0;

	for(;;)
	{
		switch(getSn_SR(sockno))
		{
			case SOCK_INIT:

					connect(sockno, srv_ip, srv_port);
			break;
			case SOCK_ESTABLISHED:
			{
				if(sendok == 0)
				{
					//log_net("1\r\n");

					len = strlen((char *)buff); // get len buff with HTTP headers
					send(sockno, buff, len); // send HTTP request with headers
					sendok ++;
					if(getSn_IR(sockno) & Sn_IR_CON)
					{
						setSn_IR(sockno, Sn_IR_CON); //? set interrupt while connect established (maybe)
						break;
					}
				}

				len = getSn_RX_RSR(sockno); // �������� ����� ���������� ������
				if(len > 0) // ���� ������ ����
				{
					cnt = 0; // �������� ������� �������

					memset(buff, 0, size_buff); // �������� ������
					len = recv(sockno, buff, len); //�������� ������ ������ ������� �� �������� ����
					if(recv_start == 0) //���� ������ �������� �������� ������
					{
						uint8_t res, resp_code, isstream;

						//log_net("2\r\n");

						if(cache_cont_len + len <= sizeof(cache)) //���� ����� ������ + cache_cont_len ������ ���� ����� ������� ����
						{
							//log_net("3\r\n");
							memcpy(cache+cache_cont_len, buff, len);
							cache_cont_len += len;

							memset(buff, 0, size_buff);
							len = 0;
						}
						else
						{
							uint32_t len_copy;
							//log_net("4\r\n");

							len_copy = sizeof(cache) - cache_cont_len;
							memcpy(cache+cache_cont_len, buff, len_copy);
							memcpy(buff, buff + len_copy, len - len_copy);
							memset(buff + (len - len_copy), 0, size_buff - (len - len_copy));
							len = len - len_copy;
							cache_cont_len = sizeof(cache);

						}

						res = func_analysis_http_download_header(cache, cache_cont_len, &resp_code, &isstream, &cont_len);
						if(res == 0)
						{
							if(resp_code != 200 || isstream != 1)//response code 200, must be iostream
							{
								//log_net("22\r\n");
								return 3;// response not ok
							}
							recv_start++;

							//logic -> file size should be no more than 108 Kbytes
							// TODO rewrite this condition, delete magic numbers
							if(cont_len > max_file_size)
							{
								//log_net("23\r\n");
								return 4;
							}

							//-------------------file system---------------------------
							err = 0;
							spi_fs_over_write_file(filename, 0x0,0);

							//-------------------file system---------------------------

							//if(0)
							{
								uint32_t tmp_len;
								uint8_t* pos_header;
								pos_header = (uint8_t*)strstr((char*)cache, "\r\n\r\n");
								//remove header, save file byte to cache
								tmp_len = cache_cont_len - (pos_header + 4 - cache);
								memcpy(cache, pos_header + 4, tmp_len);
								memset(cache + tmp_len, 0 , sizeof(cache) - tmp_len);
								cache_cont_len = tmp_len;// ��� ��� ���� ������ � ����, �������� ����� �������� ������
								download_cont_len = cache_cont_len;

								if(len > 0)
								{
									if(cache_cont_len + len <= sizeof(cache))
									{
										//log_net("5\r\n");
										memcpy(cache+cache_cont_len, buff, len);
										cache_cont_len += len;
										download_cont_len += len;

										memset(buff, 0, size_buff);
										len = 0;
									}
									else
									{
										uint32_t len_copy;
										//log_net("6\r\n");

										len_copy = sizeof(cache) - cache_cont_len;
										memcpy(cache+cache_cont_len, buff, len_copy);
										memcpy(buff, buff + len_copy, len - len_copy);
										memset(buff + (len - len_copy), 0, size_buff - (len - len_copy));
										len = len - len_copy;
										cache_cont_len = sizeof(cache);

//--------------------- FLASH -------------------------------------------



								//write_status = lfs_file_write(&lfs, &file, &cache, cache_cont_len);
								spi_fs_write_file_to_end(filename, &cache, cache_cont_len);


//--------------------- FLASH -------------------------------------------


										//TODO write to FLASH backup
										//func_usart2_dma_send_bytes(cache, cache_cont_len);
										memset(cache, 0, cache_cont_len);
										cache_cont_len = 0;
										memcpy(cache+cache_cont_len, buff, len);
										cache_cont_len = len;
										download_cont_len = sizeof(cache) + cache_cont_len;

									}
								}

								if(download_cont_len == cont_len)// small file download done
								{
									//log_net("7\r\n");
									//func_usart2_dma_send_bytes(cache, cache_cont_len);
									//TODO write to FLASH backup and record
/*
									int err = lfs_mount(&lfs, &cfg);

									if (err) {
										lfs_format(&lfs, &cfg);
										err = lfs_mount(&lfs, &cfg);
									}
*/
//--------------------- FLASH -------------------------------------------



									spi_fs_write_file_to_end(filename, &cache, cache_cont_len);
								//write_status = lfs_file_write(&lfs, &file, &cache, cache_cont_len);

								//log_net("Write status: ");
								//log_net(debug_buff);
								//log_net("\r\n");


								//lfs_file_close(&lfs, &file);
								//lfs_unmount(&lfs);

//--------------------- FLASH -------------------------------------------


									disconnect(sockno);
									return 0;//download file done
								}

							}
						}
					}
					else
					{
						IINCHIP_WRITE(Sn_CR(sockno),Sn_CR_SEND_KEEP);//keep-alive manual

						//file size is big, write to FALSH several times
						download_cont_len += len;
						if(cache_cont_len + len <= sizeof(cache))
						{
							//log_net("8\r\n");
							memcpy(cache+cache_cont_len, buff, len);
							cache_cont_len += len;
							memset(buff, 0, size_buff);
							len = 0;
						}
						else
						{
							uint32_t len_copy;
							//log_net("9\r\n");
							len_copy = sizeof(cache) - cache_cont_len;
							memcpy(cache+cache_cont_len, buff, len_copy);
							memcpy(buff, buff + len_copy, len - len_copy);
							memset(buff + (len - len_copy), 0, size_buff - (len - len_copy));
							len = len - len_copy;
							cache_cont_len = sizeof(cache);

							//TODO write to FLASH backup
							//func_usart2_dma_send_bytes(cache, cache_cont_len);
//--------------------- FLASH -------------------------------------------
							if(!err){

								spi_fs_write_file_to_end(filename, &cache, cache_cont_len);
								//write_status = lfs_file_write(&lfs, &file, &cache, cache_cont_len);

							}
//--------------------- FLASH -------------------------------------------


							memset(cache, 0, cache_cont_len);
							cache_cont_len = 0;
							memcpy(cache+cache_cont_len, buff, len);
							cache_cont_len = len;
						}

						//if donwload file done
						if(download_cont_len == cont_len)
						{
							//log_net("10\r\n");
							//TODO write to FLASH backup and record
							//func_usart2_dma_send_bytes(cache, cache_cont_len);
							disconnect(sockno);


//--------------------- FLASH -------------------------------------------

								spi_fs_write_file_to_end(filename, &cache, cache_cont_len);
								//write_status = lfs_file_write(&lfs, &file, &cache, cache_cont_len);
								//lfs_file_close(&lfs, &file);
								//lfs_unmount(&lfs);

//--------------------- FLASH -------------------------------------------


							return 0;//download file done
						}
					}
				}
			}
			break;
			case SOCK_CLOSE_WAIT:
					close(sockno);
					//log_net("21\r\n");

					//lfs_file_rewind(&lfs, &file);
					//lfs_file_close(&lfs, &file);
					//lfs_unmount(&lfs);

					return 2;//read timeout
			case SOCK_CLOSED:
					close(sockno);
					sendok = 0;
					recv_start = 0;
					if(local_port > 65400)
					{
						local_port = 60000;
					}
					socket(sockno, Sn_MR_TCP, local_port++, Sn_MR_ND);
			break;
		}

		cnt ++;
		if(cnt >= timeout_ms)
		{
//			log_net("20\r\n");
			//lfs_file_close(&lfs, &file);
			//lfs_unmount(&lfs);

			return 1;//connect timeout
		}
		delay_ms(10);
	}
}





uint8_t func_analysis_http_download_header(uint8_t* buffer, uint16_t len_buf, uint8_t *resp_code, uint8_t *is_stream, uint32_t *cont_len)
{
	*resp_code = 0;
	*is_stream = 0;
	*cont_len = 0;
	if(strstr((char*)buffer, "\r\n\r\n") != NULL)//response header ok
	{
		char *p1, *p2;

		p1 = strstr((char*)buffer, "HTTP/1.1 200");
		if(p1 != NULL)// io stream
		{
			*resp_code = 200;
		}

		p1 = strstr((char*)buffer, "Accept-Ranges: bytes");
		if(p1 != NULL)// io stream
		{
			*is_stream = 1;
		}

		p1 = strstr((char*)buffer, "Content-Length: ");
		if(p1 != NULL)
		{
			p2 = strstr(p1, "\r\n");
			if(p2 != NULL)
			{
				char str_len[8] = {0,};
				memcpy(str_len, p1 + strlen("Content-Length: "), p2 - p1 - strlen("Content-Length: "));
				*cont_len = atoi(str_len);
				return 0;
			}
		}
	}
	return 1;
}
/*
void log_net(char* log)
{
#ifndef DEBUG_APP
	uint8_t logsrv_ip[] = {10, 0, 20, 250};
	uint16_t port = 2115;
	socket(4, Sn_MR_UDP, port, 0x00);
	sendto(4, (uint8_t*)log, strlen(log), logsrv_ip, port);
#endif
}
*/

void func_pack_http_get_download_header(uint8_t* buff, uint16_t size_buff, uint8_t* srv_ip, uint8_t * domain_name, uint16_t srv_port, char* interface)
{
	uint8_t tmp[64];
	memset(buff, 0, size_buff);
	//header
	memcpy(buff, "GET ", strlen("GET "));
	strcat((char *)buff, interface);
	strcat((char *)buff, " HTTP/1.1\r\n");
	memset(tmp, 0 , sizeof(tmp));

	strcat((char *)tmp, "Host: ");
	strcat((char *)tmp, domain_name);
	strcat((char *)tmp, "\r\n");
//	sprintf((char*)tmp, "Host: %d.%d.%d.%d:%d\r\n", srv_ip[0], srv_ip[1], srv_ip[2], srv_ip[3], srv_port);

	strcat((char *)buff, (char*)tmp);
	strcat((char *)buff, "Connection: Keep-Alive\r\n");
	strcat((char *)buff, "User-Agent: W5500\r\n");
	strcat((char *)buff, "Accept-Encoding: gzip,deflate\r\n");
	strcat((char *)buff, "\r\n");
}
