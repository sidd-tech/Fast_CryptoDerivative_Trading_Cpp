#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>

class WebSocketClient : public QObject {
    Q_OBJECT

public:
    explicit WebSocketClient(QObject *parent = nullptr);
    void connectToWebSocket();

signals:
    void dataReceived(const QString &data); // Signal for receiving data

private:
    // Add any necessary private members here
};

#endif // WEBSOCKETCLIENT_H
