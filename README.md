# Crptocurrency Derivative Trading Desktop Application (C++)

## This is a desktop application made for trading cryptocurrencies and it's derivative

This is App is made using Qt Framework. This app utilises **"test.deribit.com"** to get APIs to implement different functionlity.
The API calls are of 2 types public and private. For pirvate requests, account needs to be created in Deribit platform and similarly OAuth2.0 is setup in backend for getting session key which is used during making the subsequent requests.

These are the primary functionality using HTTP Request :

1. Place and cancel Order
2. Fetch Currencies
3. Buy/sell
4. View Orders
5. Get Positions
6. Fetch Orderbook

Additionally, websockets are implemented to fetch live price upon subscribing to any currencies. Which sends live price updates for the selected currency.

All the code logics are written in 'src' folder meanwhile the 'build' folder contains the executable file.


### Place Order Tab 
<img width="1280" alt="Screenshot 2024-12-03 at 9 05 19 PM" src="https://github.com/user-attachments/assets/f1c194fc-879e-4aed-b6f1-928987865cb6">

### Positions Tab
<img width="1280" alt="Screenshot 2024-12-03 at 9 05 39 PM" src="https://github.com/user-attachments/assets/edf7363f-acfa-4518-8e36-54c18d7a60bc">

### OrderBook Tab
<img width="1280" alt="Screenshot 2024-12-03 at 9 06 04 PM" src="https://github.com/user-attachments/assets/9708eb2a-85ab-43a2-a595-55519e7c1b86">

### Subscription Tab
<img width="1280" alt="Screenshot 2024-12-03 at 9 06 19 PM" src="https://github.com/user-attachments/assets/4cbb42b2-6294-4d1f-b37b-3c8622bb5d24">
