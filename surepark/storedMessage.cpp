#include "stdafx.h"
#include "storedMessage.h"


storedMessage::storedMessage(int addr, storedMessage * obj)
{
	address = addr;
	length = 0;
	preMsg = obj;
}


storedMessage::~storedMessage()
{
}


void storedMessage::setMessage(String * str)
{
	unsigned char * ch;
	int i = 0 ;
	length = str->length();
	ch = (unsigned char*)malloc(length);
	str->getBytes(ch, length);
	for (; ch != nullptr; ch++, i++) EEPROM.write(address+i,*ch);
}


String storedMessage::getMessage()
{
	int i = 0;
	String str= "";
	for (; i < length; i++) str += EEPROM.read(address + i);
	return String();
}
