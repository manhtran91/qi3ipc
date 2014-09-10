#ifndef QI3SOCKET_H
#define QI3SOCKET_H

#include <QtNetwork/QLocalSocket>

class Qi3Socket : public QLocalSocket
{
Q_OBJECT

public:
    Qi3Socket(QObject *parent);

    void sendMessage(quint32 messageType, const char *payload);

private:
    QString const getSocketPath(void);
};

#endif /* QI3SOCKET_H */
