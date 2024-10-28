#include "MainWindow.h"
#include <QJsonArray>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QGroupBox>
#include <QLabel>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    networkManager(new QNetworkAccessManager(this)),
    webSocket(new QWebSocket),
    reconnectTimer(new QTimer(this))
{
    setupUI();  // Set up the UI components
    wsUrl = QUrl("wss://test.deribit.com/ws/api/v2");

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

void MainWindow::setupUI() {
    // Create main layout with tab widget
    QTabWidget *tabWidget = new QTabWidget(this);

    // Place and View Orders Tab
    QWidget *placeOrderTab = new QWidget(this);
    QVBoxLayout *placeOrderLayout = new QVBoxLayout(placeOrderTab);

    QPushButton *fetchCurrenciesButton = new QPushButton("Fetch Currencies", this);
    QComboBox *instrumentNameComboBox = new QComboBox(this);
    QSpinBox *amountInput = new QSpinBox(this);
    amountInput->setMinimum(1);
    amountInput->setMaximum(1000);

    QComboBox *orderTypeComboBox = new QComboBox(this);
    orderTypeComboBox->addItems({"Market", "Limit"});

    QPushButton *placeOrderButton = new QPushButton("Place Order", this);

    placeOrderLayout->addWidget(fetchCurrenciesButton);
    placeOrderLayout->addWidget(new QLabel("Instrument Name", this));
    placeOrderLayout->addWidget(instrumentNameComboBox);
    placeOrderLayout->addWidget(new QLabel("Amount", this));
    placeOrderLayout->addWidget(amountInput);
    placeOrderLayout->addWidget(new QLabel("Order Type", this));
    placeOrderLayout->addWidget(orderTypeComboBox);
    placeOrderLayout->addWidget(placeOrderButton);

    tabWidget->addTab(placeOrderTab, "Place and View Orders");

    // My Positions Tab
    QWidget *myPositionsTab = new QWidget(this);
    QVBoxLayout *myPositionsLayout = new QVBoxLayout(myPositionsTab);
    myPositionsLayout->addWidget(new QLabel("Positions will be shown here."));
    tabWidget->addTab(myPositionsTab, "My Positions");

    // Order Book Tab
    QWidget *orderBookTab = new QWidget(this);
    QVBoxLayout *orderBookLayout = new QVBoxLayout(orderBookTab);
    orderBookLayout->addWidget(new QLabel("Order Book will be shown here."));
    tabWidget->addTab(orderBookTab, "Order Book");

    // Subscribe Tab
    QWidget *subscribeTab = new QWidget(this);
    QVBoxLayout *subscribeLayout = new QVBoxLayout(subscribeTab);
    subscribeLayout->addWidget(new QLabel("Subscription options will be shown here."));
    tabWidget->addTab(subscribeTab, "Subscribe");

    // API Response Text Area
    QGroupBox *apiResponseGroup = new QGroupBox("API Response", this);
    QVBoxLayout *apiResponseLayout = new QVBoxLayout(apiResponseGroup);
    apiResponseTextEdit = new QTextEdit(this);
    apiResponseTextEdit->setFixedHeight(150);
    apiResponseTextEdit->setReadOnly(true);
    apiResponseLayout->addWidget(apiResponseTextEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(apiResponseGroup);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Connect Buttons
    connect(fetchCurrenciesButton, &QPushButton::clicked, this, &MainWindow::fetchCurrencies);
    connect(placeOrderButton, &QPushButton::clicked, this, [=]() {
        placeOrder(instrumentNameComboBox->currentText(), amountInput->value(), orderTypeComboBox->currentText());
    });
}

// Function to append API responses to the text area
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
        appendToApiResponseTextEdit("Authenticated: " + authToken);
        reply->deleteLater();
    });
}

// Function to fetch currencies and populate dropdown
void MainWindow::fetchCurrencies() {
    QNetworkRequest request(QUrl("https://test.deribit.com/api/v2/public/get_supported_index_names?type=all"));
    QNetworkReply *reply = networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, [=] {
        QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
        QJsonArray currencies = response["result"].toArray();
        
        QComboBox *instrumentNameComboBox = findChild<QComboBox *>("instrumentNameComboBox");
        if (instrumentNameComboBox) {
            instrumentNameComboBox->clear();
            for (const QJsonValue &currency : currencies) {
                instrumentNameComboBox->addItem(currency.toString());
            }
        }
        appendToApiResponseTextEdit("Currencies fetched: " + QJsonDocument(currencies).toJson(QJsonDocument::Compact));
        reply->deleteLater();
    });
}

// Function to place order
void MainWindow::placeOrder(const QString &instrumentName, int amount, const QString &orderType) {
    if (authToken.isEmpty()) authenticate();

    QNetworkRequest request(QUrl("https://test.deribit.com/api/v2/private/buy"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + authToken.toUtf8());

    QJsonObject params;
    params["instrument_name"] = instrumentName;
    params["amount"] = amount;
    params["type"] = orderType.toLower();

    QJsonObject body;
    body["jsonrpc"] = "2.0";
    body["id"] = 100;
    body["method"] = "private/buy";
    body["params"] = params;

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [=] {
        QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
        appendToApiResponseTextEdit("Order placed: " + QJsonDocument(response).toJson(QJsonDocument::Compact));
        reply->deleteLater();
    });
}

void MainWindow::handleWebSocketMessage(const QString &message) {
    QJsonObject response = QJsonDocument::fromJson(message.toUtf8()).object();
    if (response.contains("params")) {
        double price = response["params"].toObject()["data"].toObject()["price"].toDouble();
        appendToApiResponseTextEdit("Price update: " + QString::number(price));
    }
}

void MainWindow::subscribeToPriceStream(const QString &currency) {
    if (!webSocket->isValid()) {
        webSocket->open(wsUrl);
    }

    QJsonObject subscribeMessage;
    subscribeMessage["jsonrpc"] = "2.0";
    subscribeMessage["id"] = 3600;
    subscribeMessage["method"] = "public/subscribe";

    QJsonObject params;
    params["channels"] = QJsonArray({ QString("deribit_price_index.%1").arg(currency) });
    subscribeMessage["params"] = params;

    webSocket->sendTextMessage(QJsonDocument(subscribeMessage).toJson(QJsonDocument::Compact));
}