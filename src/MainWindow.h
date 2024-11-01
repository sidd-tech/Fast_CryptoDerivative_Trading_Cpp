#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWebSocket>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QtWebSockets/QWebSocket>
#include <QSslConfiguration>  // For SSL configuration with WebSocket
#include <QSslSocket>         // For disabling SSL verification if needed
#include <QLCDNumber>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setupUI();
    void authenticate();
    void fetchCurrencies();
    void placeOrder(const QString &instrumentName, int amount, const QString &orderType, double price);
    void subscribeToPriceStream(const QString &currency);  // WebSocket subscription function
    void handleWebSocketMessage(const QString &message);
    void fetchOrders();
    void cancelSelectedOrder();
    void fetchPositions();
    void closeSelectedPosition();
    void getOrderBookForInstrument(const QString &instrumentName);
    void onWebSocketMessageReceived(QString message);

private:
    void appendToApiResponseTextEdit(const QString &response);
    QNetworkAccessManager *networkManager;
    QWebSocket *webSocket;
    QString authToken;
    QUrl wsUrl;
    QTimer *reconnectTimer;
    QComboBox *instrumentNameComboBox;
    QComboBox *orderBookInstrumentNameComboBox;
    QComboBox *subscribeInstrumentComboBox;  // Add combo box for Subscribe tab
    QLabel *priceLabel;
    QLineEdit *instrumentNameInput;
    QSpinBox *amountInput;
    QComboBox *orderTypeComboBox;
    QPushButton *placeOrderButton;
    QPushButton *fetchCurrenciesButton;
    QComboBox *orderSelectionComboBox;
    QTextEdit *apiResponseTextEdit;
    QTableWidget *orderTable;
    QPushButton *cancelOrderButton;
    QTableWidget *positionsTable;
    QLabel *bestAskLabel;
    QLabel *bestBidLabel;
    QLabel *indexPriceLabel;
    QLabel *lastPriceLabel;
    QTableWidget *orderBookTable;
    QLCDNumber *priceDisplay;
};

#endif // MAINWINDOW_H
