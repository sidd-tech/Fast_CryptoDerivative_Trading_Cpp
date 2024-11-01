#include "MainWindow.h"
#include <QJsonArray>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QTableWidget>
#include <QGroupBox>
#include <QLabel>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QtWebSockets/QWebSocket>
#include <QLCDNumber> 


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    networkManager(new QNetworkAccessManager(this)),
    webSocket(new QWebSocket),
    reconnectTimer(new QTimer(this))
{
    setupUI();  // Set up the UI components

    wsUrl = QUrl("wss://test.deribit.com/ws/api/v2");  // WebSocket URL
    authenticate();  // Initial authentication

    connect(webSocket, &QWebSocket::textMessageReceived, this, &MainWindow::handleWebSocketMessage);


    // Optionally connect to the error signal for debugging
    connect(webSocket, &QWebSocket::errorOccurred, this, [](QAbstractSocket::SocketError error) {
        qDebug() << "WebSocket error:" << error;
    });

    
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

    // Create a table view for displaying positions
    positionsTable = new QTableWidget(myPositionsTab);
    positionsTable->setColumnCount(6); // Adjust the number of columns based on the data
    positionsTable->setHorizontalHeaderLabels({"Instrument Name", "Size", "Mark Price", "Direction", "Kind", "Total P&L"});

    // Button to fetch positions
    QPushButton *fetchPositionsButton = new QPushButton("Fetch Positions", myPositionsTab);
    myPositionsLayout->addWidget(fetchPositionsButton);
    myPositionsLayout->addWidget(positionsTable);

    // Button to close position
    QPushButton *closePositionButton = new QPushButton("Close Selected Position", myPositionsTab);
    myPositionsLayout->addWidget(closePositionButton);

    tabWidget->addTab(myPositionsTab, "My Positions");

    // Connect button signals to the respective slots
    connect(fetchPositionsButton, &QPushButton::clicked, this, &MainWindow::fetchPositions);
    connect(closePositionButton, &QPushButton::clicked, this, &MainWindow::closeSelectedPosition);


    // Order Book Tab
    QWidget *orderBookTab = new QWidget(this);
    QVBoxLayout *orderBookLayout = new QVBoxLayout(orderBookTab);

    // Combo box for selecting an instrument
    orderBookInstrumentNameComboBox = new QComboBox(this);
    orderBookLayout->addWidget(new QLabel("Instrument Name"));
    orderBookLayout->addWidget(orderBookInstrumentNameComboBox);

    // Button to fetch instruments
    QPushButton *fetchInstrumentsButton = new QPushButton("Get Instruments", this);
    orderBookLayout->addWidget(fetchInstrumentsButton);

    // Button to get the order book
    QPushButton *getOrderBookButton = new QPushButton("Get Orderbook", this);
    orderBookLayout->addWidget(getOrderBookButton);

    // Labels for displaying order book information
    bestAskLabel = new QLabel("Best Ask: ", this);
    bestBidLabel = new QLabel("Best Bid: ", this);
    indexPriceLabel = new QLabel("Index Price: ", this);
    lastPriceLabel = new QLabel("Last Price: ", this);
    orderBookLayout->addWidget(bestAskLabel);
    orderBookLayout->addWidget(bestBidLabel);
    orderBookLayout->addWidget(indexPriceLabel);
    orderBookLayout->addWidget(lastPriceLabel);

    // Table widget for displaying the order book entries
    orderBookTable = new QTableWidget(this);
    orderBookTable->setColumnCount(2);
    orderBookTable->setHorizontalHeaderLabels(QStringList() << "Price" << "Amount");
    orderBookLayout->addWidget(orderBookTable);

    // Set the single layout for the orderBookTab
    orderBookTab->setLayout(orderBookLayout);
    tabWidget->addTab(orderBookTab, "Order Book");

    // Connect fetchInstrumentsButton to fetchCurrencies
    connect(fetchInstrumentsButton, &QPushButton::clicked, this, [=]() {
        fetchCurrencies(); // Call the fetchCurrencies function to populate the combo box
    });

    // Connect getOrderBookButton to the appropriate slot
    connect(getOrderBookButton, &QPushButton::clicked, this, [=]() {
        QString selectedInstrument = orderBookInstrumentNameComboBox->currentText();
        if (selectedInstrument.isEmpty()) {
            appendToApiResponseTextEdit("Please select an instrument.");
            return;
        }

        // Get the order book for the selected instrument
        getOrderBookForInstrument(selectedInstrument);
    });


        

    
    // Subscribe Tab
    QWidget *subscribeTab = new QWidget(this);
    QVBoxLayout *subscribeLayout = new QVBoxLayout(subscribeTab);

    // Button to fetch instruments
    QPushButton *fetchInstrumentsButton2 = new QPushButton("Get Instruments", this);
    orderBookLayout->addWidget(fetchInstrumentsButton2);

    subscribeInstrumentComboBox = new QComboBox(this);
    subscribeLayout->addWidget(new QLabel("Instrument Name"));
    subscribeLayout->addWidget(subscribeInstrumentComboBox);

    QPushButton *subscribeButton = new QPushButton("Subscribe", this);
    subscribeLayout->addWidget(subscribeButton);

    // Connect the button to the lambda
    connect(subscribeButton, &QPushButton::clicked, this, [this]() {
        QString selectedInstrument = subscribeInstrumentComboBox->currentText();
        subscribeToPriceStream(selectedInstrument);
    });

    // Initialize QLCDNumber for displaying live price
    priceDisplay = new QLCDNumber(this);
    priceDisplay->setDigitCount(10);  // Adjust based on the expected price range
    priceDisplay->setSegmentStyle(QLCDNumber::Flat);  

    subscribeLayout->addWidget(fetchInstrumentsButton2);
    subscribeLayout->addWidget(new QLabel("Instrument"));
    subscribeLayout->addWidget(subscribeInstrumentComboBox);
    subscribeLayout->addWidget(subscribeButton);
    subscribeLayout->addWidget(new QLabel("Live Price:"));
    subscribeLayout->addWidget(priceDisplay);

    tabWidget->addTab(subscribeTab, "Subscribe");

    connect(fetchInstrumentsButton2, &QPushButton::clicked, this, &MainWindow::fetchCurrencies);



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
        orderBookInstrumentNameComboBox->clear();  // Clear any existing items

        QStringList instrumentNames;  // Create a QStringList to collect instrument names

        for (const QJsonValue &instrument : instruments) {
            QString instrumentName = instrument.toString();  // Directly get the string value
            instrumentNames << instrumentName;  // Append to QStringList
        }

        // Add all instruments to both combo boxes
        instrumentNameComboBox->addItems(instrumentNames);
        orderBookInstrumentNameComboBox->addItems(instrumentNames);
        subscribeInstrumentComboBox->addItems(instrumentNames);

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
        priceDisplay->display(price);  // Update QLCDNumber with the price
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
    appendToApiResponseTextEdit("Subscribing to Instrument :"+currency);
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

void MainWindow::fetchPositions() {
    if (authToken.isEmpty()) authenticate(); // Ensure the user is authenticated

    QNetworkRequest request(QUrl("https://test.deribit.com/api/v2/private/get_positions?currency=any"));
    request.setRawHeader("Authorization", "Bearer " + authToken.toUtf8());

    QNetworkReply *reply = networkManager->get(request); // Use networkManager for consistency
    connect(reply, &QNetworkReply::finished, this, [=] {
        QByteArray responseData = reply->readAll();
        QJsonObject response = QJsonDocument::fromJson(responseData).object();



        // Check for errors in the response
        if (response.contains("error")) {
            appendToApiResponseTextEdit("Error fetching positions: " + QJsonDocument(response).toJson(QJsonDocument::Compact));
        } else {
            QJsonArray positions = response["result"].toArray();
            // Clear previous data
            positionsTable->setRowCount(0);
             // Populate the table with positions
            for (const QJsonValue &value : positions) {
                QJsonObject position = value.toObject();
                int row = positionsTable->rowCount();
                positionsTable->insertRow(row);

                // Populate the table with relevant data
                positionsTable->setItem(row, 0, new QTableWidgetItem(position["instrument_name"].toString()));
                positionsTable->setItem(row, 1, new QTableWidgetItem(QString::number(position["size"].toDouble())));
                positionsTable->setItem(row, 4, new QTableWidgetItem(QString::number(position["mark_price"].toDouble())));
                positionsTable->setItem(row, 3, new QTableWidgetItem(position["direction"].toString()));
                positionsTable->setItem(row, 2, new QTableWidgetItem(position["kind"].toString()));
                positionsTable->setItem(row, 7, new QTableWidgetItem(QString::number(position["total_profit_loss"].toDouble())));
                // Add other fields as needed
            }
            appendToApiResponseTextEdit("Positions fetched successfully."+ QJsonDocument(response).toJson(QJsonDocument::Compact));
        }
        reply->deleteLater();
    });
}

void MainWindow::closeSelectedPosition() {
    int row = positionsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Warning", "Please select a position to close.");
        return;
    }

    QString instrumentName = positionsTable->item(row, 0)->text();

    // Ensure the user is authenticated
    if (authToken.isEmpty()) authenticate(); 

    // Make the API call to close the position
    QNetworkRequest request(QUrl("https://test.deribit.com/api/v2/private/close_position"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + authToken.toUtf8());

    QJsonObject params;
    params["instrument_name"] = instrumentName;

    QJsonObject body;
    body["jsonrpc"] = "2.0";
    body["id"] = 11; // Unique request ID
    body["method"] = "private/close_position"; // Correct method
    body["params"] = params;

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [=] {
        QByteArray responseData = reply->readAll();
        QJsonObject response = QJsonDocument::fromJson(responseData).object();

        // Check for errors in the response
        if (response.contains("error")) {
            appendToApiResponseTextEdit("Error closing position: " + QJsonDocument(response).toJson(QJsonDocument::Compact));
        } else {
            appendToApiResponseTextEdit("Position closed successfully: " + QJsonDocument(response).toJson(QJsonDocument::Compact));
            fetchPositions(); // Refresh positions
        }

        reply->deleteLater();
    });
}

void MainWindow::getOrderBookForInstrument(const QString &instrumentName) {
    QNetworkRequest request(QUrl("https://test.deribit.com/api/v2/public/get_order_book?instrument_name=" + instrumentName.toUpper()));
    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        // Parse the JSON response
        QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
        QJsonObject result = response["result"].toObject();

        // Update the labels with best ask, best bid, index price, and last price
        double bestAskPrice = result["best_ask_price"].toDouble();
        double bestBidPrice = result["best_bid_price"].toDouble();
        double indexPrice = result["index_price"].toDouble();
        double lastPrice = result["last_price"].toDouble();

        bestAskLabel->setText("Best Ask: " + QString::number(bestAskPrice));
        bestBidLabel->setText("Best Bid: " + QString::number(bestBidPrice));
        indexPriceLabel->setText("Index Price: " + QString::number(indexPrice));
        lastPriceLabel->setText("Last Price: " + QString::number(lastPrice));

        // Clear any existing rows in the table
        orderBookTable->clearContents();
        orderBookTable->setRowCount(0);

        // Populate the order book table with asks and bids
        QJsonArray asks = result["asks"].toArray();
        QJsonArray bids = result["bids"].toArray();

        // Set the row count for asks and bids
        int rowCount = asks.size() + bids.size();
        orderBookTable->setRowCount(rowCount);

        // Fill the table with ask orders
        int row = 0;
        for (const QJsonValue &ask : asks) {
            QJsonArray askData = ask.toArray();
            double price = askData[0].toDouble();
            double amount = askData[1].toDouble();

            orderBookTable->setItem(row, 0, new QTableWidgetItem(QString::number(price)));
            orderBookTable->setItem(row, 1, new QTableWidgetItem(QString::number(amount)));
            row++;
        }

        // Fill the table with bid orders
        for (const QJsonValue &bid : bids) {
            QJsonArray bidData = bid.toArray();
            double price = bidData[0].toDouble();
            double amount = bidData[1].toDouble();

            orderBookTable->setItem(row, 0, new QTableWidgetItem(QString::number(price)));
            orderBookTable->setItem(row, 1, new QTableWidgetItem(QString::number(amount)));
            row++;
        }

        // Log the response for debugging
        appendToApiResponseTextEdit(QJsonDocument(response).toJson(QJsonDocument::Indented));
        reply->deleteLater();
    });
}

void MainWindow::onWebSocketMessageReceived(QString message) {
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject obj = doc.object();

    if (obj.contains("params") && obj["params"].toObject().contains("data")) {
        double price = obj["params"].toObject()["data"].toObject()["price"].toDouble();
        priceLabel->setText(QString("Live Price: %1").arg(price));
    }
}
