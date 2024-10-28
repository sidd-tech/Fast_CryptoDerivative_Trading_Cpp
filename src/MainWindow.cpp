#include "MainWindow.h"
#include <QJsonArray>
#include <QVBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    networkManager(new QNetworkAccessManager(this)),
    webSocket(new QWebSocket),
    reconnectTimer(new QTimer(this)),
    priceLabel(new QLabel("Waiting for price updates...", this)),
    instrumentNameInput(new QLineEdit(this)),
    amountInput(new QSpinBox(this)),
    orderTypeComboBox(new QComboBox(this)),
    placeOrderButton(new QPushButton("Place Order", this)),
    fetchCurrenciesButton(new QPushButton("Fetch Currencies", this)),
    apiResponseTextEdit(new QTextEdit(this)) // Initialize QTextEdit
{
    setupUI();
    wsUrl = QUrl("wss://test.deribit.com/ws/api/v2");

    // Authentication request
    authenticate();

    // Setup WebSocket connection
    connect(webSocket, &QWebSocket::connected, this, [] {
        qDebug() << "WebSocket connected!";
    });
    connect(webSocket, &QWebSocket::textMessageReceived, this, &MainWindow::handleWebSocketMessage);
    connect(reconnectTimer, &QTimer::timeout, this, &MainWindow::authenticate);
}

MainWindow::~MainWindow() {
    delete networkManager;
    delete webSocket;
}

// Function to append text to the API response area
void MainWindow::appendToApiResponseTextEdit(const QString &response) {
    apiResponseTextEdit->append(response);
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
        appendToApiResponseTextEdit("Auth Response: " + QString(QJsonDocument(response).toJson(QJsonDocument::Indented)));
    });
}

// Function to fetch currencies
void MainWindow::fetchCurrencies() {
    QNetworkRequest request(QUrl("https://test.deribit.com/api/v2/public/get_supported_index_names?type=all"));
    QNetworkReply *reply = networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, [=] {
        QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
        QJsonArray currencies = response["result"].toArray();
        
        appendToApiResponseTextEdit("Fetch Currencies Response: " + QString(QJsonDocument(response).toJson(QJsonDocument::Indented)));
        
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
    params["instrument_name"] = instrumentNameInput->text();
    params["amount"] = amountInput->value();
    params["type"] = orderTypeComboBox->currentText();

    QJsonObject body;
    body["jsonrpc"] = "2.0";
    body["id"] = 100;
    body["method"] = "private/buy";
    body["params"] = params;

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [=] {
        QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
        appendToApiResponseTextEdit("Place Order Response: " + QString(QJsonDocument(response).toJson(QJsonDocument::Indented)));
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
    instrumentNameInput->setPlaceholderText("Instrument Name");
    amountInput->setMinimum(1);
    amountInput->setMaximum(1000);
    amountInput->setPrefix("Amount: ");
    orderTypeComboBox->addItems({"market", "limit"});

    connect(placeOrderButton, &QPushButton::clicked, this, &MainWindow::placeOrder);
    connect(fetchCurrenciesButton, &QPushButton::clicked, this, &MainWindow::fetchCurrencies);

    apiResponseTextEdit->setReadOnly(true); // Make the text area read-only

    // Layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(priceLabel);
    layout->addWidget(instrumentNameInput);
    layout->addWidget(amountInput);
    layout->addWidget(orderTypeComboBox);
    layout->addWidget(placeOrderButton);
    layout->addWidget(fetchCurrenciesButton);
    layout->addWidget(apiResponseTextEdit); // Add the text area to the layout

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

void MainWindow::handleWebSocketMessage(const QString &message) {
    QJsonObject response = QJsonDocument::fromJson(message.toUtf8()).object();
    appendToApiResponseTextEdit("WebSocket Message: " + message);

    if (response.contains("params")) {
        double price = response["params"].toObject()["data"].toObject()["price"].toDouble();
        priceLabel->setText(QString("Price update: %1").arg(price));
        qDebug() << "Price update: " << price;
    }
}
