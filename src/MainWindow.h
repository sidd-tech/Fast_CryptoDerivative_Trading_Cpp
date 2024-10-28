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
#include <QTextEdit> // Include for QTextEdit

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setupUI();
    void authenticate();
    void fetchCurrencies();
    void placeOrder();
    void subscribeToPriceStream(const QString &currency);
    void handleWebSocketMessage(const QString &message);

private:
    void appendToApiResponseTextEdit(const QString &response); // Add this line
    QNetworkAccessManager *networkManager;
    QWebSocket *webSocket;
    QString authToken;
    QUrl wsUrl;
    QTimer *reconnectTimer;

    QLabel *priceLabel;
    QLineEdit *instrumentNameInput;
    QSpinBox *amountInput;
    QComboBox *orderTypeComboBox;
    QPushButton *placeOrderButton;
    QPushButton *fetchCurrenciesButton;
    QTextEdit *apiResponseTextEdit; // Text area for API responses
};

#endif // MAINWINDOW_H
