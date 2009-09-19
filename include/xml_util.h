#ifndef XML_UTIL_H
#define XML_UTIL_H
#include <QtXml/QtXml>
#include "data_types.h"

class XmlUtil{
public:
	/*!
	Parse the round 1 question data.
	\param xml The xml data in a QString.
	\param data A reference to the R1QData struct to be filled up.
	\return true on success, false on failure
	*/
	bool readR1QData(const QString& xml, R1QData& data);
	
	/*!
	Serialize the round 1 question data into an xml format.
	\param data R1QData struct to serialize.
	\param xml A reference to a QString to contain the xml data.
	\return true on success, false on failure
	*/
	bool writeR1QData(const R1QData& data, QString& xml);
	
	/*!
	Parse the round 1 answer data.
	\param xml The xml data in a QString.
	\param data The R1Answers struct to fill up.
	\return true on success, false on failure.
	*/
	bool readR1AData(const QString& xml, R1Answers& data);
	
	/*!
	Serialize the round 1 answer data into an xml format.
	\param data R1Answers struct to serialize.
	\param xml Reference to QString to contain the xml data.
	\return true on success, false on failure.
	*/
	bool writeR1AData(const R1Answers& data, QString& xml);
	
	/*!
	Read the client config from xml.
	\param xml Xml data in a QString.
	\param conf Reference to a ClientConfig struct to fill up.
	\return true on succes, false on failure.
	*/
	bool readClientConfig(const QString& xml, ClientConfig& conf);
	
	/*!
	Read the server config from xml.
	\param xml Xml data in a QString.
	\param conf Reference to a ServerConfig struct to fill up.
	\return true on success, false on failure.
	*/
	bool readServerConfig(const QString& xml, ServerConfig& conf);
protected:
};

#endif //XML_UTIL_H