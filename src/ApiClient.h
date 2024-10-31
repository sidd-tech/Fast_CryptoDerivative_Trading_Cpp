#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>

class ApiClient : public QObject {
    Q_OBJECT

public:
    explicit ApiClient(QObject *parent = nullptr);

    void authenticate(const QString &apiKey, const QString &secret);
    void fetchCurrencies();
    void placeOrder(const QString &instrument, const QString &orderType, double amount);
    void fetchOrders();
    void cancelOrder(const QString &orderId);

signals:
    void ordersFetched(const QString &orders);
    void orderPlaced(const QString &status);

private:
    QNetworkAccessManager *networkManager;
};

#endif // APICLIENT_H
