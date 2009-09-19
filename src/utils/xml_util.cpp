#include "xml_util.h"
#include "data_types.h"

bool XmlUtil::readR1QData(const QString& xml, R1QData& data){
	return true;
}

bool XmlUtil::writeR1QData(const R1QData& data, QString& xml){
	return true;
}

bool XmlUtil::readR1AData(const QString& xml, R1Answers& data){
	return true;
}

bool XmlUtil::writeR1AData(const R1Answers& data, QString& xml){
	return true;
}

bool XmlUtil::readClientConfig(const QString& xml, ClientConfig& conf){
	return true;
}

bool XmlUtil::readServerConfig(const QString& xml, ServerConfig& conf){
	return true;
}
