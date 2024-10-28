#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QWebSocketServer>

class WebSocketClient : public QObject {
    Q_OBJECT

public:
    explicit WebSocketClient(QObject *parent = nullptr);
    void subscribeToPrices(const QString &currency);

signals:
    void priceUpdated(const QString &currency, double price);

private slots:
    void onMessageReceived(const QString &message);

private:
    QWebSocket *webSocket;
};

#endif // WEBSOCKETCLIENT_H
