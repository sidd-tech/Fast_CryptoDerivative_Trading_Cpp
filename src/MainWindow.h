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
    //void getOpenOrders();
    //void cancelOrder(const QString &orderId);
    void subscribeToPriceStream(const QString &currency);
    void handleWebSocketMessage(const QString &message);

private:

    QNetworkAccessManager *networkManager;
    QWebSocket *webSocket;
    QString authToken;
    QUrl wsUrl;
    QTimer *reconnectTimer;
    QLabel *priceLabel;  // To display price updates
};

#endif // MAINWINDOW_H