#ifndef QI3SOCKET_H
#define QI3SOCKET_H

#include <QtNetwork/QLocalSocket>
#include <i3/ipc.h>

namespace I3IPC
{
    enum MessageType {
        Command = I3_IPC_MESSAGE_TYPE_COMMAND,
        Workspaces = I3_IPC_MESSAGE_TYPE_GET_WORKSPACES,
        Subscribe = I3_IPC_MESSAGE_TYPE_SUBSCRIBE,
        Outputs = I3_IPC_MESSAGE_TYPE_GET_OUTPUTS,
        Tree = I3_IPC_MESSAGE_TYPE_GET_TREE,
        Marks = I3_IPC_MESSAGE_TYPE_GET_MARKS,
        BarConfig = I3_IPC_MESSAGE_TYPE_GET_BAR_CONFIG,
        GetVersion = I3_IPC_MESSAGE_TYPE_GET_VERSION,
        WorkspaceEvent = I3_IPC_EVENT_WORKSPACE,
        OutputEvent = I3_IPC_EVENT_OUTPUT,
        ModeEvent = I3_IPC_EVENT_MODE,
        WindowEvent = I3_IPC_EVENT_WINDOW,
        BarConfigUpdate = I3_IPC_EVENT_BARCONFIG_UPDATE
    };

    class Qi3Socket : public QLocalSocket
    {
        Q_OBJECT

    public:
        Qi3Socket(QObject *parent);

        void sendMessage(MessageType messageType, const char *payload);

        public slots:
            void connected();
            void disconnected();
            void bytesWritten(qint64 bytes);

    private:
            QString const getSocketPath(void);
    };
}

#endif /* QI3SOCKET_H */
