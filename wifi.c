
/*-------------------------------------------------*/
/*                                                 */
/*          	 WIFI£¨ESP8266£©Ô´ÎÄ¼þ             */
/*                                                 */
/*-------------------------------------------------*/

// Ó²¼þÁ¬½Ó£º
// PA2 RX
// PA3 TX
// PA4 ¸´Î»

#include "stm32f10x.h"  //°üº¬ÐèÒªµÄÍ·ÎÄ¼þ
#include "wifi.h"	    //°üº¬ÐèÒªµÄÍ·ÎÄ¼þ
#include "delay.h"	    //°üº¬ÐèÒªµÄÍ·ÎÄ¼þ
#include "usart1.h"	    //°üº¬ÐèÒªµÄÍ·ÎÄ¼þ

char wifi_mode = 0;     //ÁªÍøÄ£Ê½ 0£ºSSIDºÍÃÜÂëÐ´ÔÚ³ÌÐòÀï   1£ºSmartconfig·½Ê½ÓÃAPP·¢ËÍ
	
/*-------------------------------------------------*/
/*º¯ÊýÃû£º³õÊ¼»¯WiFiµÄ¸´Î»IO                       */
/*²Î  Êý£ºÎÞ                                       */
/*·µ»ØÖµ£ºÎÞ                                       */
/*-------------------------------------------------*/
void WiFi_ResetIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                    //¶¨ÒåÒ»¸öÉèÖÃIO¶Ë¿Ú²ÎÊýµÄ½á¹¹Ìå
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); //Ê¹ÄÜPA¶Ë¿ÚÊ±ÖÓ
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;               //×¼±¸ÉèÖÃPA4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //ËÙÂÊ50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   	    //ÍÆÃâÊä³ö·½Ê½
	GPIO_Init(GPIOA, &GPIO_InitStructure);            	    //ÉèÖÃPA4
	RESET_IO(1);                                            //¸´Î»IOÀ­¸ßµçÆ½
}
/*-------------------------------------------------*/
/*º¯ÊýÃû£ºWiFi·¢ËÍÉèÖÃÖ¸Áî                          */
/*²Î  Êý£ºcmd£ºÖ¸Áî                                */
/*²Î  Êý£ºtimeout£º³¬Ê±Ê±¼ä£¨100msµÄ±¶Êý£©          */
/*·µ»ØÖµ£º0£ºÕýÈ·   ÆäËû£º´íÎó                      */
/*-------------------------------------------------*/
char WiFi_SendCmd(char *cmd, int timeout)
{
	WiFi_RxCounter = 0;                           			//WiFi½ÓÊÕÊý¾ÝÁ¿±äÁ¿ÇåÁã                        
	memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);     			//Çå¿ÕWiFi½ÓÊÕ»º³åÇø 
	WiFi_printf("%s\r\n", cmd);                  			//·¢ËÍÖ¸Áî
	while(timeout--)										//µÈ´ý³¬Ê±Ê±¼äµ½0
	{                           			
		delay_ms(100);                             		    //ÑÓÊ±100ms
		if(strstr(WiFi_RX_BUF, "OK"))              			//Èç¹û½ÓÊÕµ½OK±íÊ¾Ö¸Áî³É¹¦
			break;       									//Ö÷¶¯Ìø³öwhileÑ­»·
		u1_printf("%d ", timeout);                 			//´®¿ÚÊä³öÏÖÔÚµÄ³¬Ê±Ê±¼ä
	}			
	u1_printf("\r\n");                          			//´®¿ÚÊä³öÐÅÏ¢
	if(timeout <= 0)return 1;                    				//Èç¹ûtimeout<=0£¬ËµÃ÷³¬Ê±Ê±¼äµ½ÁË£¬Ò²Ã»ÄÜÊÕµ½OK£¬·µ»Ø1
	else return 0;		         							//·´Ö®£¬±íÊ¾ÕýÈ·£¬ËµÃ÷ÊÕµ½OK£¬Í¨¹ýbreakÖ÷¶¯Ìø³öwhile
}
/*-------------------------------------------------*/
/*º¯ÊýÃû£ºWiFi¸´Î»                                 */
/*²Î  Êý£ºtimeout£º³¬Ê±Ê±¼ä£¨100msµÄ±¶Êý£©          */
/*·µ»ØÖµ£º0£ºÕýÈ·   ÆäËû£º´íÎó                      */
/*-------------------------------------------------*/
char WiFi_Reset(int timeout)
{
	RESET_IO(0);                                    	  //¸´Î»IOÀ­µÍµçÆ½
	delay_ms(500);                                  		  //ÑÓÊ±500ms
	RESET_IO(1);                                   		  //¸´Î»IOÀ­¸ßµçÆ½	
	while(timeout--)									  //µÈ´ý³¬Ê±Ê±¼äµ½0 
	{                              		  
		delay_ms(100);                                 	  //ÑÓÊ±100ms
		if(strstr(WiFi_RX_BUF, "ready"))               	  //Èç¹û½ÓÊÕµ½ready±íÊ¾¸´Î»³É¹¦
			break;       						   		  //Ö÷¶¯Ìø³öwhileÑ­»·
		u1_printf("%d ", timeout);                     	  //´®¿ÚÊä³öÏÖÔÚµÄ³¬Ê±Ê±¼ä
	}
	u1_printf("\r\n");                              	  //´®¿ÚÊä³öÐÅÏ¢
	if(timeout <= 0)return 1;                        		  //Èç¹ûtimeout<=0£¬ËµÃ÷³¬Ê±Ê±¼äµ½ÁË£¬Ò²Ã»ÄÜÊÕµ½ready£¬·µ»Ø1
	else return 0;		         				   		  //·´Ö®£¬±íÊ¾ÕýÈ·£¬ËµÃ÷ÊÕµ½ready£¬Í¨¹ýbreakÖ÷¶¯Ìø³öwhile
}
/*-------------------------------------------------*/
/*º¯ÊýÃû£ºWiFi¼ÓÈëÂ·ÓÉÆ÷Ö¸Áî                       */
/*²Î  Êý£ºtimeout£º³¬Ê±Ê±¼ä£¨1sµÄ±¶Êý£©            */
/*·µ»ØÖµ£º0£ºÕýÈ·   ÆäËû£º´íÎó                     */
/*-------------------------------------------------*/
char WiFi_JoinAP(int timeout)
{		
	WiFi_RxCounter = 0;                                    //WiFi½ÓÊÕÊý¾ÝÁ¿±äÁ¿ÇåÁã                        
	memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);              //Çå¿ÕWiFi½ÓÊÕ»º³åÇø 
	WiFi_printf("AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASS); //·¢ËÍÖ¸Áî	
	while(timeout--)									   //µÈ´ý³¬Ê±Ê±¼äµ½0
	{                                   
		delay_ms(1000);                             		   //ÑÓÊ±1s
		if(strstr(WiFi_RX_BUF, "OK"))   //Èç¹û½ÓÊÕµ½WIFI GOT IP±íÊ¾³É¹¦
			break;       						           //Ö÷¶¯Ìø³öwhileÑ­»·
		u1_printf("%d ", timeout);                         //´®¿ÚÊä³öÏÖÔÚµÄ³¬Ê±Ê±¼ä
	}
	u1_printf("\r\n%s\r\n", WiFi_RX_BUF);
	u1_printf("\r\n");                             	       //´®¿ÚÊä³öÐÅÏ¢
	if(timeout <= 0)return 1;                              //Èç¹ûtimeout<=0£¬ËµÃ÷³¬Ê±Ê±¼äµ½ÁË£¬Ò²Ã»ÄÜÊÕµ½WIFI GOT IP£¬·µ»Ø1
	return 0;                                              //ÕýÈ·£¬·µ»Ø0
}
/*-------------------------------------------------*/
/*º¯ÊýÃû£ºÁ¬½ÓTCP·þÎñÆ÷£¬²¢½øÈëÍ¸´«Ä£Ê½            */
/*²Î  Êý£ºtimeout£º ³¬Ê±Ê±¼ä£¨100msµÄ±¶Êý£©        */
/*·µ»ØÖµ£º0£ºÕýÈ·  ÆäËû£º´íÎó                      */
/*-------------------------------------------------*/
char WiFi_Connect_Server(int timeout)
{	
	WiFi_RxCounter=0;                              	//WiFi½ÓÊÕÊý¾ÝÁ¿±äÁ¿ÇåÁã                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //Çå¿ÕWiFi½ÓÊÕ»º³åÇø   
	WiFi_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ServerIP, ServerPort);//·¢ËÍÁ¬½Ó·þÎñÆ÷Ö¸Áî
	while(timeout--)								//µÈ´ý³¬Ê±Óë·ñ
	{                           
		delay_ms(100);                             	//ÑÓÊ±100ms	
		if(strstr(WiFi_RX_BUF, "CONNECT"))          //Èç¹û½ÓÊÜµ½CONNECT±íÊ¾Á¬½Ó³É¹¦
			break;                                  //Ìø³öwhileÑ­»·
		if(strstr(WiFi_RX_BUF, "CLOSED"))           //Èç¹û½ÓÊÜµ½CLOSED±íÊ¾·þÎñÆ÷Î´¿ªÆô
			return 1;                               //·þÎñÆ÷Î´¿ªÆô·µ»Ø1
		if(strstr(WiFi_RX_BUF, "ALREADY CONNECTED"))//Èç¹û½ÓÊÜµ½ALREADY CONNECTEDÒÑ¾­½¨Á¢Á¬½Ó
			return 2;                               //ÒÑ¾­½¨Á¢Á¬½Ó·µ»Ø2
		u1_printf("%d ", timeout);                   //´®¿ÚÊä³öÏÖÔÚµÄ³¬Ê±Ê±¼ä  
	}
	u1_printf("\r\n");                              //´®¿ÚÊä³öÐÅÏ¢
	if(timeout <= 0)return 3;                       //³¬Ê±´íÎó£¬·µ»Ø3
	else                                            //Á¬½Ó³É¹¦£¬×¼±¸½øÈëÍ¸´«
	{
		u1_printf("Á¬½Ó·þÎñÆ÷³É¹¦£¬×¼±¸½øÈëÍ¸´«\r\n"); //´®¿ÚÏÔÊ¾ÐÅÏ¢
		WiFi_RxCounter = 0;                          //WiFi½ÓÊÕÊý¾ÝÁ¿±äÁ¿ÇåÁã                        
		memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);    //Çå¿ÕWiFi½ÓÊÕ»º³åÇø     
		WiFi_printf("AT+CIPSEND\r\n");               //·¢ËÍ½øÈëÍ¸´«Ö¸Áî
		while(timeout--)							 //µÈ´ý³¬Ê±Óë·ñ
		{                            
			delay_ms(100);                            //ÑÓÊ±100ms	
			if(strstr(WiFi_RX_BUF, "\r\nOK\r\n\r\n>"))//Èç¹û³ÉÁ¢±íÊ¾½øÈëÍ¸´«³É¹¦
				break;                          	 //Ìø³öwhileÑ­»·
			u1_printf("%d ", timeout);                //´®¿ÚÊä³öÏÖÔÚµÄ³¬Ê±Ê±¼ä  
		}
		if(timeout <= 0)return 4;                      //Í¸´«³¬Ê±´íÎó£¬·µ»Ø4	
	}
	return 0;	                                     //³É¹¦·µ»Ø0	
}
/*-------------------------------------------------*/
/*º¯ÊýÃû£ºWiFi_Smartconfig                         */
/*²Î  Êý£ºtimeout£º³¬Ê±Ê±¼ä£¨1sµÄ±¶Êý£©            */
/*·µ»ØÖµ£º0£ºÕýÈ·   ÆäËû£º´íÎó                     */
/*-------------------------------------------------*/
char WiFi_Smartconfig(int timeout)
{
	
	WiFi_RxCounter=0;                           		//WiFi½ÓÊÕÊý¾ÝÁ¿±äÁ¿ÇåÁã                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);     		//Çå¿ÕWiFi½ÓÊÕ»º³åÇø     
	while(timeout--)									//µÈ´ý³¬Ê±Ê±¼äµ½0
	{                           		
		delay_ms(1000);                         				//ÑÓÊ±1s
		if(strstr(WiFi_RX_BUF, "connected"))    	 		  //Èç¹û´®¿Ú½ÓÊÜµ½connected±íÊ¾³É¹¦
			break;                                  		//Ìø³öwhileÑ­»·  
		u1_printf("%d ", timeout);                 		//´®¿ÚÊä³öÏÖÔÚµÄ³¬Ê±Ê±¼ä  
	}	
	u1_printf("\r\n");                          		//´®¿ÚÊä³öÐÅÏ¢
	if(timeout <= 0)return 1;                     		//³¬Ê±´íÎó£¬·µ»Ø1
	return 0;                                   		//ÕýÈ··µ»Ø0
}
/*-------------------------------------------------*/
/*º¯ÊýÃû£ºµÈ´ý¼ÓÈëÂ·ÓÉÆ÷                           */
/*²Î  Êý£ºtimeout£º³¬Ê±Ê±¼ä£¨1sµÄ±¶Êý£©            */
/*·µ»ØÖµ£º0£ºÕýÈ·   ÆäËû£º´íÎó                     */
/*-------------------------------------------------*/
char WiFi_WaitAP(int timeout)
{		
	while(timeout--){                               //µÈ´ý³¬Ê±Ê±¼äµ½0
		delay_ms(1000);                             		//ÑÓÊ±1s
		if(strstr(WiFi_RX_BUF, "WIFI GOT IP"))         //Èç¹û½ÓÊÕµ½WIFI GOT IP±íÊ¾³É¹¦
			break;       						   								  //Ö÷¶¯Ìø³öwhileÑ­»·
		u1_printf("%d ", timeout);                     //´®¿ÚÊä³öÏÖÔÚµÄ³¬Ê±Ê±¼ä
	}
	u1_printf("\r\n");                              //´®¿ÚÊä³öÐÅÏ¢
	if(timeout <= 0)return 1;                         //Èç¹ûtimeout<=0£¬ËµÃ÷³¬Ê±Ê±¼äµ½ÁË£¬Ò²Ã»ÄÜÊÕµ½WIFI GOT IP£¬·µ»Ø1
	return 0;                                       //ÕýÈ·£¬·µ»Ø0
}
/*-------------------------------------------------*/
/*º¯ÊýÃû£ºWiFiÁ¬½Ó·þÎñÆ÷                           */
/*²Î  Êý£ºÎÞ                                       */
/*·µ»ØÖµ£º0£ºÕýÈ·   ÆäËû£º´íÎó                     */
/*-------------------------------------------------*/
char WiFi_Connect_IoTServer(void)
{	
	u1_printf("×¼±¸¸´Î»Ä£¿é\r\n");                   //´®¿ÚÌáÊ¾Êý¾Ý
	if(WiFi_Reset(50))								//¸´Î»£¬100ms³¬Ê±µ¥Î»£¬×Ü¼Æ5s³¬Ê±Ê±¼ä
	{                             
		u1_printf("¸´Î»Ê§°Ü£¬×¼±¸ÖØÆô\r\n");	      //·µ»Ø·Ç0Öµ£¬½øÈëif£¬´®¿ÚÌáÊ¾Êý¾Ý
		return 1;                                   //·µ»Ø1
	}else u1_printf("¸´Î»³É¹¦\r\n");                 //´®¿ÚÌáÊ¾Êý¾Ý
	
	u1_printf("×¼±¸ÉèÖÃSTAÄ£Ê½\r\n");                //´®¿ÚÌáÊ¾Êý¾Ý
	if(WiFi_SendCmd("AT+CWMODE=1",50))//ÉèÖÃSTAÄ£Ê½£¬100ms³¬Ê±µ¥Î»£¬×Ü¼Æ5s³¬Ê±Ê±¼ä
	{             
		u1_printf("ÉèÖÃSTAÄ£Ê½Ê§°Ü£¬×¼±¸ÖØÆô\r\n");   //·µ»Ø·Ç0Öµ£¬½øÈëif£¬´®¿ÚÌáÊ¾Êý¾Ý
		return 2;                                   //·µ»Ø2
	}else u1_printf("ÉèÖÃSTAÄ£Ê½³É¹¦\r\n");          //´®¿ÚÌáÊ¾Êý¾Ý
	
	if(wifi_mode==0) //Èç¹ûÁªÍøÄ£Ê½=0£ºSSIDºÍÃÜÂëÐ´ÔÚ³ÌÐòÀï 
	{                              
		u1_printf("×¼±¸È¡Ïû×Ô¶¯Á¬½Ó\r\n");            //´®¿ÚÌáÊ¾Êý¾Ý
		if(WiFi_SendCmd("AT+CWAUTOCONN=0",50))		 //È¡Ïû×Ô¶¯Á¬½Ó£¬100ms³¬Ê±µ¥Î»£¬×Ü¼Æ5s³¬Ê±Ê±¼ä
		{       
			u1_printf("È¡Ïû×Ô¶¯Á¬½ÓÊ§°Ü£¬×¼±¸ÖØÆô\r\n"); //·µ»Ø·Ç0Öµ£¬½øÈëif£¬´®¿ÚÌáÊ¾Êý¾Ý
			return 3;                                  //·µ»Ø3
		}else u1_printf("È¡Ïû×Ô¶¯Á¬½Ó³É¹¦\r\n");        //´®¿ÚÌáÊ¾Êý¾Ý
				
		u1_printf("×¼±¸Á¬½ÓÂ·ÓÉÆ÷\r\n");                //´®¿ÚÌáÊ¾Êý¾Ý	
		if(WiFi_JoinAP(30))//Á¬½ÓÂ·ÓÉÆ÷,1s³¬Ê±µ¥Î»£¬×Ü¼Æ30s³¬Ê±Ê±¼ä
		{                          
			u1_printf("Á¬½ÓÂ·ÓÉÆ÷Ê§°Ü£¬×¼±¸ÖØÆô\r\n");  //·µ»Ø·Ç0Öµ£¬½øÈëif£¬´®¿ÚÌáÊ¾Êý¾Ý
			return 4;                                 //·µ»Ø4	
		}else u1_printf("Á¬½ÓÂ·ÓÉÆ÷³É¹¦\r\n");         //´®¿ÚÌáÊ¾Êý¾Ý			
	}
	
	u1_printf("×¼±¸ÉèÖÃÍ¸´«\r\n");                    //´®¿ÚÌáÊ¾Êý¾Ý
	if(WiFi_SendCmd("AT+CIPMODE=1",50)) 			 //ÉèÖÃÍ¸´«£¬100ms³¬Ê±µ¥Î»£¬×Ü¼Æ5s³¬Ê±Ê±¼ä
	{           
		u1_printf("ÉèÖÃÍ¸´«Ê§°Ü£¬×¼±¸ÖØÆô\r\n");       //·µ»Ø·Ç0Öµ£¬½øÈëif£¬´®¿ÚÌáÊ¾Êý¾Ý
		return 8;                                    //·µ»Ø8
	}else u1_printf("ÉèÖÃÍ¸´«³É¹¦\r\n");              //´®¿ÚÌáÊ¾Êý¾Ý
	
	u1_printf("×¼±¸¹Ø±Õ¶àÂ·Á¬½Ó\r\n");                //´®¿ÚÌáÊ¾Êý¾Ý
	if(WiFi_SendCmd("AT+CIPMUX=0",50)) 				 //¹Ø±Õ¶àÂ·Á¬½Ó£¬100ms³¬Ê±µ¥Î»£¬×Ü¼Æ5s³¬Ê±Ê±¼ä
	{            
		u1_printf("¹Ø±Õ¶àÂ·Á¬½ÓÊ§°Ü£¬×¼±¸ÖØÆô\r\n");   //·µ»Ø·Ç0Öµ£¬½øÈëif£¬´®¿ÚÌáÊ¾Êý¾Ý
		return 9;                                    //·µ»Ø9
	}else u1_printf("¹Ø±Õ¶àÂ·Á¬½Ó³É¹¦\r\n");          //´®¿ÚÌáÊ¾Êý¾Ý
	 
	u1_printf("×¼±¸Á¬½Ó·þÎñÆ÷\r\n");                  //´®¿ÚÌáÊ¾Êý¾Ý
	if(WiFi_Connect_Server(100))      				 //Á¬½Ó·þÎñÆ÷£¬100ms³¬Ê±µ¥Î»£¬×Ü¼Æ10s³¬Ê±Ê±¼ä
	{            
		u1_printf("Á¬½Ó·þÎñÆ÷Ê§°Ü£¬×¼±¸ÖØÆô\r\n");     //·µ»Ø·Ç0Öµ£¬½øÈëif£¬´®¿ÚÌáÊ¾Êý¾Ý
		return 10;                                   //·µ»Ø10
	}else u1_printf("Á¬½Ó·þÎñÆ÷³É¹¦\r\n");            //´®¿ÚÌáÊ¾Êý¾Ý	
	return 0;                                        //ÕýÈ··µ»Ø0
}
	

