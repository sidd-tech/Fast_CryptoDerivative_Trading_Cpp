#include "ApiClient.h"
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>

ApiClient::ApiClient(QObject *parent) : QObject(parent), networkManager(new QNetworkAccessManager(this)) {}

void ApiClient::authenticate(const QString &apiKey, const QString &secret) {
    QNetworkRequest request(QUrl("https://example.com/auth"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["api_key"] = apiKey;
    json["secret"] = secret;

    QJsonDocument doc(json);
    networkManager->post(request, doc.toJson());
}

void ApiClient::fetchCurrencies() {
    QNetworkRequest request(QUrl("https://test.deribit.com/api/v2/public/get_supported_index_names?type=all"));
    networkManager->get(request);
}

void ApiClient::placeOrder(const QString &instrument, const QString &orderType, double amount) {
    QNetworkRequest request(QUrl("https://example.com/place_order"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["instrument"] = instrument;
    json["order_type"] = orderType;
    json["amount"] = amount;

    QJsonDocument doc(json);
    networkManager->post(request, doc.toJson());
}

void ApiClient::fetchOrders() {
    QNetworkRequest request(QUrl("https://example.com/fetch_orders"));
    networkManager->get(request);
}

void ApiClient::cancelOrder(const QString &orderId) {
    QNetworkRequest request(QUrl("https://example.com/cancel_order"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["order_id"] = orderId;

    QJsonDocument doc(json);
    networkManager->post(request, doc.toJson());
}
