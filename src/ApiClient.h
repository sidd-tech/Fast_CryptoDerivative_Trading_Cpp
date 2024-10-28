#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QVector>

// Define data structures for order and position
struct OrderData {
    double price;
    double amount;
};

struct PositionData {
    QString instrument_name;
    double size;
    double mark_price;
    QString direction;
    QString kind;
    double total_profit_loss;
};

class ApiClient : public QObject {
    Q_OBJECT

public:
    explicit ApiClient(QObject *parent = nullptr);
    
    void authenticate();
    void placeOrder(const QString &instrument, int amount, const QString &orderType);
    void fetchOrderBook(const QString &currency);
    void cancelOrder(int orderId);

private slots:
    void onAuthenticationReply(QNetworkReply *reply);
    void onPlaceOrderReply(QNetworkReply *reply);
    void onFetchOrderBookReply(QNetworkReply *reply);
    void onCancelOrderReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
};

#endif // APICLIENT_H
