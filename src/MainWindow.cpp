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
#include <QTableWidget>
#include <QDoubleSpinBox>


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

    // Place Order Tab
    QWidget *placeOrderTab = new QWidget(this);
    QVBoxLayout *placeOrderLayout = new QVBoxLayout(placeOrderTab);
    QPushButton *fetchCurrenciesButton = new QPushButton("Fetch Currencies", this);
    instrumentNameComboBox = new QComboBox(this);
    QSpinBox *amountInput = new QSpinBox(this);
    amountInput->setMinimum(1);
    amountInput->setMaximum(1000);
    QComboBox *orderTypeComboBox = new QComboBox(this);
    orderTypeComboBox->addItems({"Market", "Limit"});
    QPushButton *placeOrderButton = new QPushButton("Place Order", this);
    QDoubleSpinBox *priceInput = new QDoubleSpinBox(this);
    priceInput->setRange(0.0, 100000.0); 
    priceInput->setDecimals(2); 
    priceInput->setPrefix("$"); // Optional: add a dollar prefix

    placeOrderLayout->addWidget(fetchCurrenciesButton);
    placeOrderLayout->addWidget(new QLabel("Instrument Name"));
    placeOrderLayout->addWidget(instrumentNameComboBox);
    placeOrderLayout->addWidget(new QLabel("Amount"));
    placeOrderLayout->addWidget(amountInput);
    placeOrderLayout->addWidget(new QLabel("Order Type"));
    placeOrderLayout->addWidget(orderTypeComboBox);
    placeOrderLayout->addWidget(new QLabel("Price (for Limit orders only)"));
    placeOrderLayout->addWidget(priceInput);
    placeOrderLayout->addWidget(placeOrderButton);
    tabWidget->addTab(placeOrderTab, "Place Order");

    // View Orders Tab
    QWidget *viewOrdersTab = new QWidget(this);
    QVBoxLayout *viewOrdersLayout = new QVBoxLayout(viewOrdersTab);
    QPushButton *fetchOrdersButton = new QPushButton("Fetch Orders", this);
    orderSelectionComboBox = new QComboBox(this);
    QPushButton *cancelOrderButton = new QPushButton("Cancel Selected Order", this);

    

    viewOrdersLayout->addWidget(fetchOrdersButton);
    viewOrdersLayout->addWidget(orderSelectionComboBox);
    viewOrdersLayout->addWidget(cancelOrderButton);
    tabWidget->addTab(viewOrdersTab, "View Orders");

    // Connect button to fetchOrders function
    connect(fetchOrdersButton, &QPushButton::clicked, this, &MainWindow::fetchOrders);

    tabWidget->addTab(viewOrdersTab, "View Orders");

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
        placeOrder(
            instrumentNameComboBox->currentText(),
            amountInput->value(),
            orderTypeComboBox->currentText(),
            priceInput->value()  // Pass the price value
        );
    });
    connect(fetchOrdersButton, &QPushButton::clicked, this, &MainWindow::fetchOrders);
    connect(cancelOrderButton, &QPushButton::clicked, this, &MainWindow::cancelSelectedOrder);
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
    params["client_id"] = "MP8rDpCD";
    params["client_secret"] = "W8xoRwBBwOVY2yzemy4Dq7ck21NYMmKp2MOMC5fi-LY";

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
        // Parse the JSON response
        QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
        QJsonArray instruments = response["result"].toArray();

        instrumentNameComboBox->clear();  // Clear any existing items

        for (const QJsonValue &instrument : instruments) {
            QString instrumentName = instrument.toString();  // Directly get the string value
            instrumentNameComboBox->addItem(instrumentName);  // Add to dropdown
        }

        // Log the response for debugging
        appendToApiResponseTextEdit(QJsonDocument(response).toJson(QJsonDocument::Indented));
        reply->deleteLater();
    });
}



// Function to place an order
void MainWindow::placeOrder(const QString &instrumentName, int amount, const QString &orderType, double price) {
    if (authToken.isEmpty()) authenticate();

    QNetworkRequest request(QUrl("https://test.deribit.com/api/v2/private/buy"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + authToken.toUtf8());

    QJsonObject params;
    params["instrument_name"] = instrumentName.toUpper(); // Correctly formatted instrument name
    params["amount"] = amount; // Integer amount
    params["type"] = orderType.toLower(); // Order type in lowercase  
    params["otoco_config"] = QJsonArray{QJsonObject()}; // Empty object array as per Python code

    if (orderType.toLower() == "limit") {
        params["price"] = price; // Include price if necessary
    }

    QJsonObject body;
    body["jsonrpc"] = "2.0";
    body["id"] = 100; // Unique request ID
    body["method"] = "private/buy"; // Correct method according to Python code
    body["params"] = params;

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [=] {
        QByteArray responseData = reply->readAll();
        QJsonObject response = QJsonDocument::fromJson(responseData).object();

        // Check for errors in the response
        if (response.contains("error")) {
            appendToApiResponseTextEdit("Error placing order: Instrument = "+instrumentName + QJsonDocument(response).toJson(QJsonDocument::Compact));
        } else {
            appendToApiResponseTextEdit("Order placed: " + QJsonDocument(response).toJson(QJsonDocument::Compact));
        }

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

void MainWindow::fetchOrders() {
    QNetworkRequest request(QUrl("https://test.deribit.com/api/v2/private/get_open_orders"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + authToken.toUtf8());

    QJsonObject params;
    params["currency"] = "BTC"; // or the specific currency you are targeting

    QJsonObject body;
    body["jsonrpc"] = "2.0";
    body["id"] = 101;
    body["method"] = "private/get_open_orders";
    body["params"] = params;

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [=] {
        QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
        QJsonArray orders = response["result"].toArray();

        orderSelectionComboBox->clear();  // Clear previous entries
        for (const QJsonValue &order : orders) {
            QString orderId = order.toObject()["order_id"].toString();
            QString label = QString("Order %1").arg(orderId);
            orderSelectionComboBox->addItem(label, orderId);
        }

        appendToApiResponseTextEdit(QJsonDocument(response).toJson(QJsonDocument::Indented));
        reply->deleteLater();
    });
}

void MainWindow::cancelSelectedOrder() {
    QString selectedOrderId = orderSelectionComboBox->currentData().toString();
    if (selectedOrderId.isEmpty()) {
        appendToApiResponseTextEdit("No order selected to cancel.");
        return;
    }

    QNetworkRequest request(QUrl("https://test.deribit.com/api/v2/private/cancel"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + authToken.toUtf8());

    QJsonObject params;
    params["order_id"] = selectedOrderId;

    QJsonObject body;
    body["jsonrpc"] = "2.0";
    body["id"] = 102;
    body["method"] = "private/cancel";
    body["params"] = params;

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [=] {
        QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
        appendToApiResponseTextEdit("Order cancellation response: " + QJsonDocument(response).toJson(QJsonDocument::Compact));
        reply->deleteLater();
    });
}
