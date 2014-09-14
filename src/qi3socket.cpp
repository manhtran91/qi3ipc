#include "qi3socket.h"

#include <QtX11Extras/QX11Info>

#include <kwindowsystem.h>
#include <kservicetype.h>

#include <netwm.h>

#include <i3/ipc.h>

Qi3Socket::Qi3Socket(QObject *parent)
        : QLocalSocket(parent)
{
    QString socketPath = getSocketPath();
    qDebug() << "Found socket: " << socketPath;
    connectToServer(socketPath);
}

void Qi3Socket::sendMessage(quint32 messageType, const char *payload)
{
    QByteArray message;
    message.append(I3_IPC_MAGIC);
    quint32 messageLength = strlen(payload);
    message.append((const char *) &messageLength, sizeof(messageLength));
    message.append((const char *) &messageType, sizeof(messageType));
    message.append(payload);
    this->write(message);
    this->flush();
}

QString const Qi3Socket::getSocketPath(void)
{
    // TODO: can this be done with kwindowsystem without any direct xcb_calls?
    xcb_intern_atom_reply_t *atomReply;
    char *socketPath;
    char *atomName = (char *) "I3_SOCKET_PATH";
    size_t contentMaxWords = 256;
    xcb_intern_atom_cookie_t atomCookie;
    xcb_window_t rootWindow;
    xcb_get_property_cookie_t propertyCookie;
    xcb_get_property_reply_t *propertyReply;

    xcb_connection_t *connection = QX11Info::connection();
    NETRootInfo info(connection, 0);
    rootWindow = info.rootWindow();

    atomCookie = xcb_intern_atom(connection, 0, strlen(atomName), atomName);
    atomReply = xcb_intern_atom_reply(connection, atomCookie, NULL);

    if (atomReply == NULL) {
        free(atomReply);
        qWarning() << "Socket path atom reply is NULL!";
        return Q_NULLPTR;
    }

    propertyCookie = xcb_get_property_unchecked(
            connection,                 // conn
            false,                      // _delete
            rootWindow,                 // window
            atomReply->atom,            // property
            XCB_GET_PROPERTY_TYPE_ANY,  // type
            0,                          // long_offset
            contentMaxWords             // long_length
    );

    propertyReply = xcb_get_property_reply(connection, propertyCookie, NULL);

    if (propertyReply == NULL) {
        free(atomReply);
        free(propertyReply);
        qWarning() << "Socket path property reply is NULL!";
        return Q_NULLPTR;
    }

    int propertyValueLength = xcb_get_property_value_length(propertyReply);
    socketPath = new char[propertyValueLength + 1]; // One extra for the \0

    strncpy(socketPath, (char *) xcb_get_property_value(propertyReply), propertyValueLength);
    socketPath[propertyValueLength] = '\0';

    free(atomReply);
    free(propertyReply);
    QString path(socketPath);
    delete[] socketPath;
    return path;
}

// void Qi3Socket::connected()
// {
//     qDebug() << "Connected!";
// }

// void Qi3Socket::disconnected()
// {
//     qDebug() << "Disconnected!";
// }

// void Qi3Socket::bytesWritten(qint64 bytes)
// {
//     qDebug() << bytes << " bytes written.";
// }

// QString Qi3Socket::extractPayload(payloadLength)

// void Qi3Socket::readyRead()
//     qDebug() << "Reading...";
//     uint magicLength = qstrlen(I3_IPC_MAGIC);

//     QByteArray magicHeader = m_i3Socket.read(magicLength);
//     if ((uint) magicHeader.length() != magicLength
//         || qstrncmp(magicHeader.constData(), I3_IPC_MAGIC, magicLength))
//     {
//         qWarning() << "Invalid magic header: " << magicHeader;
//         m_i3Socket.readAll(); // Flush the buffer to make the next
//         return;
//     };
//     QDataStream stream(&m_i3Socket);
//     stream.setByteOrder(QDataStream::LittleEndian);
//     qint32 replyLength;
//     stream >> replyLength;
//     qint32 replyType;
//     stream >> replyType;
//     qDebug() << "Expecting " << replyLength << " bytes of type " << replyType;

//     // TODO: Put this in a loop for getting everything of the replylenght
//     QByteArray reply = m_i3Socket.read(replyLength);
//     qDebug() << "Received " << reply.length() << " bytes with content: " << reply;
//     if (reply.length() < (int) replyLength) {
//         qWarning("Expected more from reply! Waiting for more...");
//         qWarning() << "Socket error: " << m_i3Socket.error();
//         if (m_i3Socket.waitForReadyRead(1000)) {
//             reply.append(m_i3Socket.read(replyLength - reply.length()));
//             qDebug() << "Received " << reply.length() << " bytes with content: " << reply;
//         } else {
//             qDebug() << "Timeout when waiting for more content!";
//         }
//     }

//     if (replyType == I3_IPC_EVENT_WORKSPACE) {
//         qDebug() << "Update workspace!";
//         m_i3Socket.sendMessage(I3_IPC_MESSAGE_TYPE_GET_WORKSPACES, "");
//     } else {
//         newLine(QString(reply));
//     }
//     if (m_i3Socket.bytesAvailable()) {
//         emit m_i3Socket.readyRead();
//     }
// }
