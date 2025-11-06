1. Bàn phím ma trận (Matrix-keypad):
   Khi người dùng nhấn một phím trên bàn phím ma trận, nó sẽ gửi mã ASCII tương ứng đến vi điều khiển (MCU).

2. Encoder (Núm xoay Encoder):
   Khi xoay encoder theo chiều kim đồng hồ hoặc ngược chiều kim đồng hồ, nó sẽ gửi dữ liệu tương ứng về vi điều khiển (MCU).

3. Vi điều khiển (MCU)
   Sử dụng thư viện Encoder để xử lý dữ liệu nhận được từ encoder và xử lý dữ liệu từ cả encoder và MCU. Sau đó, truyền dữ liệu đã xử lý đến máy tính/ laptop thông qua giao tiếp UART.

