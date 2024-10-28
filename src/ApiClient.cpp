#include "ApiClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>

ApiClient::ApiClient(QObject *parent) : QObject(parent) {
    networkManager = new QNetworkAccessManager(this);
}

void ApiClient::authenticate() {
    QJsonObject authParams{
        {"grant_type", "client_credentials"},
        {"client_id", "s4seCFEb"},
        {"client_secret", "FFUjOLq7dO-IOeBQex4rjLSrfCMQvl5c9FmcR05aM3s"}
    };

    QNetworkRequest request(QUrl("https://test.deribit.com/api/v2/public/auth"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(authParams).toJson());
    connect(reply, &QNetworkReply::finished, this, &ApiClient::onAuthenticationReply);
}

void ApiClient::placeOrder(const QString &instrument, int amount, const QString &orderType) {
    QJsonObject orderParams{
        {"instrument_name", instrument},
        {"amount", amount},
        {"type", orderType}
    };

    QNetworkRequest request(QUrl("https://test.deribit.com/api/v2/private/placeorder"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(orderParams).toJson());
    connect(reply, &QNetworkReply::finished, this, &ApiClient::onPlaceOrderReply);
}

void ApiClient::fetchOrderBook(const QString &currency) {
    QNetworkRequest request(QUrl(QString("https://test.deribit.com/api/v2/public/get_orderbook?instrument_name=%1").arg(currency)));
    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &ApiClient::onFetchOrderBookReply);
}

void ApiClient::cancelOrder(int orderId) {
    QJsonObject cancelParams{
        {"order_id", orderId}
    };

    QNetworkRequest request(QUrl("https://test.deribit.com/api/v2/private/cancelorder"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(cancelParams).toJson());
    connect(reply, &QNetworkReply::finished, this, &ApiClient::onCancelOrderReply);
}

void ApiClient::onAuthenticationReply(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        qDebug() << "Authentication Success:" << QJsonDocument::fromJson(response).toJson();
    } else {
        qDebug() << "Authentication Error:" << reply->errorString();
    }
    reply->deleteLater();
}

void ApiClient::onPlaceOrderReply(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Order Placed:" << QJsonDocument::fromJson(reply->readAll()).toJson();
    } else {
        qDebug() << "Place Order Error:" << reply->errorString();
    }
    reply->deleteLater();
}

void ApiClient::onFetchOrderBookReply(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        // Handle order book parsing and data output as needed
        qDebug() << "Order Book Fetched:" << reply->readAll();
    } else {
        qDebug() << "Fetch Order Book Error:" << reply->errorString();
    }
    reply->deleteLater();
}

void ApiClient::onCancelOrderReply(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Order Canceled:" << QJsonDocument::fromJson(reply->readAll()).toJson();
    } else {
        qDebug() << "Cancel Order Error:" << reply->errorString();
    }
    reply->deleteLater();
}
