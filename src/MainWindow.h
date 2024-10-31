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


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setupUI();
    void authenticate();
    void fetchCurrencies();
    void placeOrder(const QString &instrumentName, int amount, const QString &orderType,double price); // With price
    void subscribeToPriceStream(const QString &currency);
    void handleWebSocketMessage(const QString &message);
    void fetchOrders();  // Declaration for fetching orders
    void cancelSelectedOrder();  // Declaration for canceling selected order
    

private:
    void appendToApiResponseTextEdit(const QString &response); // Add this line
    QNetworkAccessManager *networkManager;
    QWebSocket *webSocket;
    QString authToken;
    QUrl wsUrl;
    QTimer *reconnectTimer;
    QComboBox *instrumentNameComboBox;
    QLabel *priceLabel;
    QLineEdit *instrumentNameInput;
    QSpinBox *amountInput;
    QComboBox *orderTypeComboBox;
    QPushButton *placeOrderButton;
    QPushButton *fetchCurrenciesButton;
    QComboBox *orderSelectionComboBox;  // Define it here
    QTextEdit *apiResponseTextEdit; // Text area for API responses
    QTableWidget *orderTable;
    QPushButton *cancelOrderButton;


};

#endif // MAINWINDOW_H
