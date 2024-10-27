#include "MainWindow.h"
#include <QJsonArray>
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    networkManager(new QNetworkAccessManager(this)),
    webSocket(new QWebSocket),
    reconnectTimer(new QTimer(this))
{
    setupUI();  // Set up the UI components
    wsUrl = QUrl("wss://test.deribit.com/ws/api/v2");

    // Authentication request
    authenticate();

    // Setup WebSocket connection
    connect(webSocket, &QWebSocket::connected, this, [=] {
        qDebug() << "WebSocket connected!";
    });
    connect(webSocket, &QWebSocket::textMessageReceived, this, &MainWindow::handleWebSocketMessage);
    connect(reconnectTimer, &QTimer::timeout, this, &MainWindow::authenticate);
}

MainWindow::~MainWindow() {
    delete networkManager;
    delete webSocket;
}

// Function to authenticate and get token
void MainWindow::authenticate() {
    QNetworkRequest request(QUrl("https://test.deribit.com/api/v2/public/auth"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject params;
    params["grant_type"] = "client_credentials";
    params["client_id"] = "s4seCFEb";
    params["client_secret"] = "FFUjOLq7dO-IOeBQex4rjLSrfCMQvl5c9FmcR05aM3s";

    QJsonObject body;
    body["jsonrpc"] = "2.0";
    body["id"] = 51;
    body["method"] = "public/auth";
    body["params"] = params;

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [=] {
        QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
        authToken = response["result"].toObject()["access_token"].toString();
        reply->deleteLater();
    });
}

// Function to fetch currencies
void MainWindow::fetchCurrencies() {
    QNetworkRequest request(QUrl("https://test.deribit.com/api/v2/public/get_supported_index_names?type=all"));
    QNetworkReply *reply = networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, [=] {
        QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
        QJsonArray currencies = response["result"].toArray();
        
        for (const QJsonValue &currency : currencies) {
            qDebug() << currency.toString();
        }
        reply->deleteLater();
    });
}

// Function to place order
void MainWindow::placeOrder() {
    if (authToken.isEmpty()) authenticate();

    QNetworkRequest request(QUrl("https://test.deribit.com/api/v2/private/buy"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + authToken.toUtf8());

    QJsonObject params;
    params["instrument_name"] = "BTC-PERPETUAL";
    params["amount"] = 1;
    params["type"] = "market";

    QJsonObject body;
    body["jsonrpc"] = "2.0";
    body["id"] = 100;
    body["method"] = "private/buy";
    body["params"] = params;

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [=] {
        QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
        qDebug() << "Order placed: " << response;
        reply->deleteLater();
    });
}

// Function to start WebSocket connection
void MainWindow::subscribeToPriceStream(const QString &currency) {
    webSocket->open(wsUrl);
    QJsonObject subscribeMessage;
    subscribeMessage["jsonrpc"] = "2.0";
    subscribeMessage["id"] = 3600;
    subscribeMessage["method"] = "public/subscribe";
    QJsonObject params;
    params["channels"] = QJsonArray({ QString("deribit_price_index.%1").arg(currency) });
    subscribeMessage["params"] = params;

    webSocket->sendTextMessage(QJsonDocument(subscribeMessage).toJson(QJsonDocument::Compact));
}

void MainWindow::setupUI() {
    priceLabel = new QLabel("Waiting for price updates...", this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(priceLabel);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

void MainWindow::handleWebSocketMessage(const QString &message) {
    QJsonObject response = QJsonDocument::fromJson(message.toUtf8()).object();
    if (response.contains("params")) {
        double price = response["params"].toObject()["data"].toObject()["price"].toDouble();
        priceLabel->setText(QString("Price update: %1").arg(price));
        qDebug() << "Price update: " << price;
    }
}
