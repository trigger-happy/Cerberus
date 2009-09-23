#ifndef SERVER_NET_H
#define SERVER_NET_H
#include <QtNetwork/QtNetwork>
#include <list>
#include "patterns/singleton.h"
#include "error_defs.h"
#include "util/sql_util.h"

using std::list;

enum CONTESTANT_REQUEST {
        CR_CONTEST_STATE = 0,
        CR_AUTHENTICATE,
        CR_QDATA,
        CR_ADATA
};

/*!
\brief A class representing a connection with a contestant's client.
This class is created for each client connection that is made to the server.
*/
class ContestantConnection : public QObject
{
        Q_OBJECT;
public:
        /*!
        Constructor.
        \param parent Set the parent of this class.
        */
        ContestantConnection ( QObject* parent = 0 );

        /*!
        Set the socket that this connection will use.
        Only the ServerNetwork class should call this.
        \param socket The socket that this connectin should use.
        */
        void setSocket ( QTcpSocket* socket );

        /*!
        Set the round 1 question data that should be sent.
        \TODO: remove this later in place of a better system
        \param xml QString for the data.
        */
        void setR1QData ( const QString* xml );
public slots:
        /*!
        Called when there's a socket error.
        \param err The error
        */
        void error ( const QAbstractSocket::SocketError& err );

        /*!
        Called when there's data ready in the socket for reading.
        */
        void ready();

        /*!
        Called when the connection is disconnected by the client.
        */
        void disconnected();
signals:
        /*!
        */
        void contestantDisconnect ( ContestantConnection* cc );
private:
        //some private functions
        /*!
        Send a reply to the authentication request.
        \param res The result of the authentication.
        */
        void authenticationReply ( bool res );

        /*!
        Send a reply that an error has occurred.
        \param err The error message.
        */
        void errorReply ( ERROR_MESSAGES err );

        /*!
        Send the round 1 questin data.
        \param xml QString containing the xml data.
        */
        void sendR1QData ( const QString& xml );

        /*!
        Send a reply to the answer submission.
        \param res The result of the submission (not very relevant).
        */
        void sendR1AReply ( bool res );

        /*!
        Send a command to change the contest state.
        \param state The state (or contest round).
        */
        void sendContestState ( quint16 state );

        //private fields
        const QString* m_r1qdata;
        QTcpSocket* m_socket;
        quint16 m_blocksize;
        bool m_authenticated;
};

typedef list<ContestantConnection*> concon_list;

/*!
\brief Networking class to be used by the server.
This class handles the communication with various clients.
This includes the contestants, admin, projectors and user registration clients.
*/
class ServerNetwork : public QObject, public Singleton<ServerNetwork>
{
        Q_OBJECT;
public:
        /*!
        Constructor
        \param parent The parent of this object.
        */
        ServerNetwork ( QObject* parent = 0 );

        /*!
        Destructor
        */
        ~ServerNetwork();

        /*!
        Listen for connections at the specified port.
        \param port The port to listen to.
        */
        void listen ( quint16 port );

        /*!
        Set the Round 1 question data.
        \TODO Remove this once a better system is in place.
        \param xml The xml data for the questions in round 1.
        */
        void setR1QData ( const QString& xml );
public slots:

        /*!
        Called when there's a new pending connection.
        */
        void newConnection();

        /*!
        Called when a contestant disconnects.
        \param c A pointer to the ContestantConnection class that disconnected.
        */
        void contestantDisconnect ( ContestantConnection* c );
signals:

        /*!
        Emitted when a new connection has been made.
        */
        void onNewConnection();
protected:
        //server socket
        QTcpServer* m_server;
        //extra data, we may need a resource manager for these
        QString m_r1qdata;
        //connections
        concon_list m_contestants;
};

#endif //SERVER_NET_H
