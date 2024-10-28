#include "WebSocketClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

WebSocketClient::WebSocketClient(QObject *parent) : QObject(parent) {
    webSocket = new QWebSocket();

    connect(webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onMessageReceived);
}

void WebSocketClient::subscribeToPrices(const QString &currency) {
    QJsonObject subscribeMessage;
    subscribeMessage["jsonrpc"] = "2.0";
    subscribeMessage["id"] = 3600;
    subscribeMessage["method"] = "public/subscribe";
    subscribeMessage["params"] = QJsonObject{{"channels", QJsonArray{QString("deribit_price_index.%1").arg(currency)}}};

    webSocket->open(QUrl("wss://test.deribit.com/ws/api/v2"));
    webSocket->sendTextMessage(QJsonDocument(subscribeMessage).toJson());
}

void WebSocketClient::onMessageReceived(const QString &message) {
    QJsonDocument jsonResponse = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject responseObject = jsonResponse.object();

    if (responseObject.contains("params")) {
        QJsonObject params = responseObject["params"].toObject();
        if (params.contains("data")) {
            double price = params["data"].toObject()["price"].toDouble();
            QString currency = params["data"].toObject()["currency"].toString();
            emit priceUpdated(currency, price);
        }
    }
}
