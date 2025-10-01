# Keyboard_Shortcut
Mục đích: Phát triển một bàn phím thông minh để thực thi các tác vụ và phím tắt một cách nhanh chóng, giúp tối ưu hóa trải nghiệm và nâng cao năng suất làm việc.
Thành phần chính:
- Vi điều khiển trung tâm: ATMEGA 328P.
- Thiết bị nhập liệu: Bàn phím ma trận 4x4 và 2 bộ mã hóa vòng xoay (encoder).
- Nguyên lý hoạt động:
  + Vi điều khiển (MCU) đọc dữ liệu đầu vào từ bàn phím và encoder.
  + Dữ liệu được truyền đến PC/Laptop thông qua giao tiếp UART.
  + Chương trình Python trên máy tính sẽ nhận dữ liệu này và thực hiện các hành động tương ứng như nhấn tổ hợp phím, điều khiển âm lượng, hoặc mở ứng dụng.
