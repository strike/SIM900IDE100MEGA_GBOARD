#include "inetGSM.h"
#define _GSM_CONNECTION_TOUT_ 5
#define _TCP_CONNECTION_TOUT_ 20
#define _GSM_DATA_TOUT_ 10

int InetGSM::http(int type, const char* server, int port, const char* path, char* result, int resultlength, char* useragent)
{
  boolean connected=false;
  int n_of_at=0;
  int length_write;
  char end_c[2];
  end_c[0]=0x1a;
  end_c[1]='\0';

  /*
  Status = ATTACHED.
  if(gsm.getStatus()!=GSM::ATTACHED)
    return 0;
  */
  while(n_of_at<3){
	  if(!connectTCP(server, port)){
	  	#ifdef DEBUG_ON
			Serial.println(F("DB:NOT CONN"));
		#endif	
	    	n_of_at++;
	  }
	  else{
		connected=true;
		n_of_at=3;
	}
  }

  if(!connected) return 0;

	switch (type){
    case HEAD:
      gsm.SimpleWrite(F("HEAD "));
      break;
    case PUT:
      gsm.SimpleWrite(F("PUT "));
      break;
    case DELETE:
      gsm.SimpleWrite(F("DELETE "));
      break;

    default:
      gsm.SimpleWrite(F("GET "));
  }
  gsm.SimpleWrite(path);
  gsm.SimpleWrite(F(" HTTP/1.0\nHost: "));
  // gsm.SimpleWrite(server);
  gsm.SimpleWrite(F("dcav.larkit.ru"));

  gsm.SimpleWrite(F("\n"));
  gsm.SimpleWrite(F("User-Agent: "));
  if (useragent){
    gsm.SimpleWrite(useragent);
  } else {
    gsm.SimpleWrite(F("Arduino"));
  }
  gsm.SimpleWrite(F("\n\n"));
  gsm.SimpleWrite(end_c);

  switch(gsm.WaitResp(10000, 100, "SEND")){
	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }

  	#ifdef DEBUG_ON
		Serial.println(F("DB:SENT"));
	#endif	
  int res= gsm.read(result, resultlength);

  //gsm.disconnectTCP();
  
  //int res=1;
  return res;
}

int InetGSM::httpPOST(const char* server, int port, const char* path, const char* parameters, char* result, int resultlength)
{
  boolean connected=false;
  int n_of_at=0;
  char itoaBuffer[8];
  int num_char;
  char end_c[2];
  end_c[0]=0x1a;
  end_c[1]='\0';

  while(n_of_at<3){
	  if(!connectTCP(server, port)){
	  	#ifdef DEBUG_ON
			Serial.println(F("DB:NOT CONN"));
		#endif	
	    	n_of_at++;
	  }
	  else{
		connected=true;
		n_of_at=3;
	}
  }

  if(!connected) return 0;

  gsm.SimpleWrite(F("POST "));
  gsm.SimpleWrite(path);
  gsm.SimpleWrite(F(" HTTP/1.1\nHost: "));
  gsm.SimpleWrite(server);
  gsm.SimpleWrite(F("\n"));
  gsm.SimpleWrite(F("User-Agent: Arduino\n"));
  gsm.SimpleWrite(F("Content-Length: "));
  itoa(strlen(parameters),itoaBuffer,10);  
  gsm.SimpleWrite(F("\n\n"));
  gsm.SimpleWrite(parameters);
  gsm.SimpleWrite(F("\n\n"));
  gsm.SimpleWrite(end_c);
 
  switch(gsm.WaitResp(10000, 100, "SEND OK")){
	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }
delay(5000);
	#ifdef DEBUG_ON
		Serial.println(F("DB:SENT"));
	#endif	
  int res= gsm.read(result, resultlength);

  //gsm.disconnectTCP();
  return res;
}
/*
int InetGSM::tweet(const char* token, const char* msg)
{
  //    gsm.httpPOST("arduino-tweet.appspot.com",80,"/update", "token=15514242-eWYAlMwjRQfrhnZxQiOfDXUpaYwjbSvMl1Nm5Qyg&status=Spam", buffer, 200);
  char shortbuf[200];
  strcpy(shortbuf,"token=");
  strcat(shortbuf,token);
  strcat(shortbuf,"&status=");
  strcat(shortbuf, msg);
  httpPOST("arduino-tweet.appspot.com",80,"/update",shortbuf, shortbuf, BUFFERSIZE);
}*/

int InetGSM::openmail(char* server, char* loginbase64, char* passbase64, char* from, char* to, char* subj)
{
	/*
	  if (!gsm.connectTCP(server, 25))
    	return 0;
    
    delay(1000);
    gsm.read(_buffer, BUFFERSIZE);    
    gsm.SimpleWrite(F("HELO\n"));
    delay(500);
    gsm.read(_buffer, BUFFERSIZE);
    gsm.SimpleWrite(F("AUTH LOGIN\n"));
    delay(500);
    gsm.read(_buffer, BUFFERSIZE);
    gsm.SimpleWrite(loginbase64);gsm.SimpleWrite(F("\n"));
    delay(500);
    gsm.read(_buffer, BUFFERSIZE);
    gsm.SimpleWrite(passbase64);gsm.SimpleWrite(F("\n"));
    delay(500);
    gsm.read(_buffer, BUFFERSIZE);
    gsm.SimpleWrite(F("MAIL FROM: ");gsm.SimpleWrite(from);gsm.SimpleWrite("\n"));
    delay(500);
    gsm.read(_buffer, BUFFERSIZE);
    gsm.SimpleWrite(F("RCPT TO: ");gsm.SimpleWrite(to);gsm.SimpleWrite("\n"));
    delay(500);
    gsm.read(_buffer, BUFFERSIZE);
    gsm.SimpleWrite(F("Subject: ");gsm.SimpleWrite(subj);gsm.SimpleWrite("\n\n"));
    return 1;
	*/
}
int InetGSM::closemail()
{
	/*
	gsm.SimpleWrite(F("\n.\n"));
	gsm.disconnectTCP();
	return 1;
	*/
}
 

int InetGSM::attachGPRS(char* domain, char* dom1, char* dom2)
{
   int i=0;
   delay(5000);
   
  //gsm._tf.setTimeout(_GSM_DATA_TOUT_);	//Timeout for expecting modem responses.
  gsm.WaitResp(50, 50);
  gsm.SimpleWriteln(F("AT+CIFSR"));
  if(gsm.WaitResp(5000, 50, "ERROR")!=RX_FINISHED_STR_RECV){
  	#ifdef DEBUG_ON
		Serial.println(F("DB:ALREADY HAVE AN IP"));
	#endif
	  gsm.SimpleWriteln(F("AT+CIPCLOSE"));
	gsm.WaitResp(5000, 50, "ERROR");
	delay(2000);
	gsm.SimpleWriteln(F("AT+CIPSERVER=0"));
	gsm.WaitResp(5000, 50, "ERROR");
	return 1;
  }
  else{

	#ifdef DEBUG_ON
		Serial.println(F("DB:STARTING NEW CONNECTION"));
	#endif
  
  gsm.SimpleWriteln(F("AT+CIPSHUT"));
  
  switch(gsm.WaitResp(500, 50, "SHUT OK")){

	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }
	#ifdef DEBUG_ON
		Serial.println(F("DB:SHUTTED OK"));
	#endif
	 delay(1000);
	 
  gsm.SimpleWrite(F("AT+CSTT=\""));
  gsm.SimpleWrite(domain);
  gsm.SimpleWrite(F("\",\""));
  gsm.SimpleWrite(dom1);
  gsm.SimpleWrite(F("\",\""));
  gsm.SimpleWrite(dom2);
  gsm.SimpleWrite(F("\"\r"));  

  
  switch(gsm.WaitResp(500, 50, "OK")){

	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }
	#ifdef DEBUG_ON
		Serial.println(F("DB:APN OK"));
	#endif
	 delay(5000);
	  
	gsm.SimpleWriteln(F("AT+CIICR"));  

  switch(gsm.WaitResp(10000, 50, "OK")){
	case RX_TMOUT_ERR: 
		return 0; 
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }
  	#ifdef DEBUG_ON
		Serial.println(F("DB:CONNECTION OK"));
	#endif

  delay(1000);


 gsm.SimpleWriteln(F("AT+CIFSR"));
 if(gsm.WaitResp(5000, 50, "ERROR")!=RX_FINISHED_STR_RECV){
	#ifdef DEBUG_ON
		Serial.println(F("DB:ASSIGNED AN IP"));
	#endif
	gsm.setStatus(gsm.ATTACHED);
	return 1;
}
	#ifdef DEBUG_ON
		Serial.println(F("DB:NO IP AFTER CONNECTION"));
	#endif
 return 0;
 }
}

int InetGSM::dettachGPRS()
{
  if (gsm.getStatus()==gsm.IDLE) return 0;
   
  //gsm._tf.setTimeout(_GSM_CONNECTION_TOUT_);

  //Serial2.flush();

  //GPRS dettachment.
  gsm.SimpleWriteln(F("AT+CGATT=0"));
  if(gsm.WaitResp(5000, 50, "OK")!=RX_FINISHED_STR_NOT_RECV)
  {
    gsm.setStatus(gsm.ERROR);
    return 0;
  }
  delay(500);
  
  // Commented in initial trial code!!
  //Stop IP stack.
  //Serial2 << "AT+WIPCFG=0" <<  _DEC(cr) << endl;
  //	if(!gsm._tf.find("OK")) return 0;
  //Close GPRS bearer.
  //Serial2 << "AT+WIPBR=0,6" <<  _DEC(cr) << endl;

  gsm.setStatus(gsm.READY);
  return 1;
}

int InetGSM::connectTCP(const char* server, int port)
{
  //gsm._tf.setTimeout(_TCP_CONNECTION_TOUT_);

  //Status = ATTACHED.
  //if (getStatus()!=ATTACHED)
    //return 0;

  //Serial2.flush();
  
  //Visit the remote TCP server.
   gsm.SimpleWrite(F("AT+CIPSTART=\"TCP\",\""));
   gsm.SimpleWrite(server);
   gsm.SimpleWrite(F("\","));
   gsm.SimpleWriteln(port);
  
  switch(gsm.WaitResp(1000, 200, "OK")){
	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }
  #ifdef DEBUG_ON
	Serial.println(F("DB:RECVD CMD"));
  #endif	

  switch(gsm.WaitResp(15000, 200, "OK")){
	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }

  #ifdef DEBUG_ON
	Serial.println(F("DB:OK TCP"));
  #endif

  delay(3000);
  gsm.SimpleWriteln(F("AT+CIPSEND"));
  switch(gsm.WaitResp(5000, 200, ">")){
	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }

  #ifdef DEBUG_ON
	Serial.println(F("DB:>"));
  #endif
  delay(4000);
  return 1;
}

int InetGSM::disconnectTCP()
{
  //Status = TCPCONNECTEDCLIENT or TCPCONNECTEDSERVER.
  /*
  if ((getStatus()!=TCPCONNECTEDCLIENT)&&(getStatus()!=TCPCONNECTEDSERVER))
     return 0;
  */
  //gsm._tf.setTimeout(_GSM_CONNECTION_TOUT_);


  //Serial2.flush();

  //Switch to AT mode.
  //Serial2 << "+++" << endl;
  
  //delay(200);
  
  //Close TCP client and deact.
  gsm.SimpleWriteln(F("AT+CIPCLOSE"));

  //If remote server close connection AT+QICLOSE generate ERROR
  /*if(gsm._tf.find("OK"))
  {
    if(getStatus()==TCPCONNECTEDCLIENT)
      gsm.setStatus(ATTACHED);
    else
      gsm.setStatus(TCPSERVERWAIT);
    return 1;
  }
  gsm.setStatus(ERROR);
  
  return 0;    */
  if(gsm.getStatus()==gsm.TCPCONNECTEDCLIENT)
      	gsm.setStatus(gsm.ATTACHED);
   elsehttp://www.facebook.com/photo.php?fbid=2486533357849&set=a.2316317142550.2138694.1088100793&type=1&ref=nf
        gsm.setStatus(gsm.TCPSERVERWAIT);   
    return 1;
}

int InetGSM::connectTCPServer(int port)
{
/*
  if (getStatus()!=ATTACHED)
     return 0;
*/
  //gsm._tf.setTimeout(_GSM_CONNECTION_TOUT_);

  //Serial2.flush();

  // Set port
  
  gsm.SimpleWrite(F("AT+CIPSERVER=1,"));
  gsm.SimpleWriteln(port);
/*
  switch(gsm.WaitResp(5000, 50, "OK")){
	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }

  switch(gsm.WaitResp(5000, 50, "SERVER")){ //Try SERVER OK
	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }
*/
  //delay(200);  

  return 1;

}

boolean InetGSM::connectedClient()
{
  /*
  if (getStatus()!=TCPSERVERWAIT)
     return 0;
  */
  
   gsm.SimpleWriteln(F("AT+CIPSTATUS"));
  // Alternative: AT+QISTAT, although it may be necessary to call an AT 
  // command every second,which is not wise
  /*
  switch(gsm.WaitResp(1000, 200, "OK")){
	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }*/
  //gsm._tf.setTimeout(1);
  if(gsm.WaitResp(5000, 50, "CONNECT OK")!=RX_FINISHED_STR_RECV)
  {
    gsm.setStatus(gsm.TCPCONNECTEDSERVER);
    return true;
  }
  else
    return false;
 }

